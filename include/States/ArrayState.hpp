#pragma once
#include "State.hpp"
#include <imgui.h>

class ArrayState : public State {
public:
    StateID handleEvents(sf::RenderWindow& window, const sf::Event& event) override { return StateID::None; }
    StateID update(float dt) override {
        StateID next_state = StateID::None;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300.f, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
        ImGui::Begin("Arreglo (WIP)", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        
        if (ImGui::Button("<- Volver al Menu", ImVec2(-1, 0))) next_state = StateID::MainMenu;
        ImGui::Separator();
        ImGui::TextWrapped("Esta estructura esta actualmente en construccion. Proximamente estara integrada al sistema de animaciones.");
        
        ImGui::End();
        return next_state;
    }
    void render(sf::RenderWindow& window) override {}
};
