#include <iostream>
#pragma once
#include "State.hpp"
#include "BST.hpp"
#include <imgui.h>
#include <queue>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>

template <typename T>
class BSTState : public State {
private:
    BST<T> tree;
    const sf::Font& font;
    int valueInput = 0;
    char filenameBuffer[128] = "secuencia.txt";
    
    // ¡La famosa cola de comandos!
    std::queue<std::unique_ptr<VisualCommand>> commandQueue;

    // View (Cámara) independiente para el BST
    sf::View view;
    bool is_dragging = false;
    sf::Vector2f last_mouse_pos;

public:
    BSTState(const sf::Font& f) : font(f), tree(f) {
        tree.setCommandQueue(&commandQueue); // Inyectamos la cola al árbol
        view.setSize({800.f, 600.f});
        view.setCenter({400.f, 300.f});
    }

    StateID handleEvents(sf::RenderWindow& window, const sf::Event& event) override {
        if (ImGui::GetIO().WantCaptureMouse) return StateID::None;

        if (const auto* mouse_wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (mouse_wheel->wheel == sf::Mouse::Wheel::Vertical) {
                float zoom_factor = (mouse_wheel->delta > 0) ? 0.9f : 1.1f;
                view.zoom(zoom_factor);
            }
        }
        if (const auto* mouse_button = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse_button->button == sf::Mouse::Button::Right) {
                is_dragging = true;
                // Importante mapear con la vista actual
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

        // MENÚ DE IMGUI DEL BST
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(220.f, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
        ImGui::Begin("Controles BST", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button("<- Volver al Menu", ImVec2(-1, 0))) next_state = StateID::MainMenu;
        ImGui::Separator();
        
        ImGui::InputInt("Valor", &valueInput);        
        if (ImGui::Button("Insertar", ImVec2(-1, 0))) tree.insert(static_cast<T>(valueInput));
        if (ImGui::Button("Borrar (Sucesor)", ImVec2(-1, 0))) tree.remove_successor(static_cast<T>(valueInput));
        if (ImGui::Button("Borrar (Predecesor)", ImVec2(-1, 0))) tree.remove_predecessor(static_cast<T>(valueInput));
                if (ImGui::Button("Limpiar", ImVec2(-1, 0))) tree.clear();
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
                    if (!item.empty()) tree.insert(static_cast<T>(std::stoi(item)));
                }
            }
        }
        ImGui::End();

        tree.update(dt);
        if (!commandQueue.empty()) {
            bool is_finished = commandQueue.front()->execute(dt);
            if (is_finished) {
                commandQueue.pop();
            }
        }

        return next_state;
    }

    void render(sf::RenderWindow& window) override {
        window.setView(view);
        tree.draw(window); // Pronto arreglaremos las coordenadas de esto
        window.setView(window.getDefaultView());
    }
};


