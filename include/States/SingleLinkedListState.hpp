#pragma once
#include "State.hpp"
#include "SingleLinkedList.hpp"
#include <imgui.h>
#include <queue>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

template <typename T>
class SingleLinkedListState : public State {
private:
    SingleLinkedList<T> list;
    const sf::Font& font;
    int valueInput = 0;
    char filenameBuffer[128] = "secuencia.txt";
    std::queue<std::unique_ptr<VisualCommand>> commandQueue;
    sf::View view;
    bool is_dragging = false;
    sf::Vector2f last_mouse_pos;

public:
    SingleLinkedListState(const sf::Font& f) : font(f), list(f) {
        list.setCommandQueue(&commandQueue);
        view.setSize({800.f, 600.f});
        view.setCenter({400.f, 300.f});
    }

    StateID handleEvents(sf::RenderWindow& window, const sf::Event& event) override {
        if (ImGui::GetIO().WantCaptureMouse) return StateID::None;
        if (const auto* mouse_wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (mouse_wheel->wheel == sf::Mouse::Wheel::Vertical) {
                view.zoom((mouse_wheel->delta > 0) ? 0.9f : 1.1f);
            }
        }
        if (const auto* mouse_button = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse_button->button == sf::Mouse::Button::Right) {
                is_dragging = true;
                last_mouse_pos = window.mapPixelToCoords(mouse_button->position, view);
            }
        }
        if (const auto* mouse_button = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouse_button->button == sf::Mouse::Button::Right) is_dragging = false;
        }
        if (const auto* mouse_move = event.getIf<sf::Event::MouseMoved>()) {
            if (is_dragging) {
                sf::Vector2f current_mouse_pos = window.mapPixelToCoords(mouse_move->position, view);
                view.move(last_mouse_pos - current_mouse_pos);
                last_mouse_pos = window.mapPixelToCoords(mouse_move->position, view);
            }
        }
        return StateID::None;
    }

    StateID update(float dt) override {
        StateID next_state = StateID::None;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(220.f, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
        ImGui::Begin("Controles Lista Simple", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button("<- Volver al Menu", ImVec2(-1, 0))) next_state = StateID::MainMenu;
        ImGui::Separator();
        ImGui::InputInt("Valor", &valueInput);        
        if (ImGui::Button("Insertar", ImVec2(-1, 0))) list.insert(static_cast<T>(valueInput));
        if (ImGui::Button("Borrar", ImVec2(-1, 0))) list.remove(static_cast<T>(valueInput));
        if (ImGui::Button("Buscar", ImVec2(-1, 0))) list.search(static_cast<T>(valueInput));
        if (ImGui::Button("Limpiar", ImVec2(-1, 0))) list.clear();
        ImGui::Separator();
        ImGui::InputText("TXT", filenameBuffer, sizeof(filenameBuffer));
        if (ImGui::Button("Cargar desde TXT", ImVec2(-1, 0))) {
            std::ifstream file(filenameBuffer);
            if (!file.is_open()) { std::cout << "ERROR: No se encontro el archivo: " << filenameBuffer << std::endl; }
            std::string line, item;
            while (std::getline(file, line)) {
                for (char& c : line) if (c == ';' || c == '\n' || c == '\r') c = ',';
                std::stringstream ss(line);
                while (std::getline(ss, item, ',')) {
                    item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
                    if (!item.empty()) list.insert(static_cast<T>(std::stoi(item)));
                }
            }
        }
        ImGui::End();

        list.update(dt);
        if (!commandQueue.empty()) {
            if (commandQueue.front()->execute(dt)) commandQueue.pop();
        }
        return next_state;
    }

    void render(sf::RenderWindow& window) override {
        window.setView(view);
        list.draw(window);
        window.setView(window.getDefaultView());
    }
};
