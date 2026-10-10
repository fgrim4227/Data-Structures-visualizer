#pragma once
#include "State.hpp"
#include <imgui.h>

class MainMenuState : public State {
public:
    StateID handleEvents(sf::RenderWindow& window, const sf::Event& event) override { return StateID::None; }

    StateID update(float dt) override {
        StateID next_state = StateID::None;
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Trees")) {
                if (ImGui::MenuItem("Binary Search Tree")) next_state = StateID::BSTVisualizer;
                if (ImGui::MenuItem("AVL Tree")) next_state = StateID::AVLVisualizer;
                if (ImGui::MenuItem("Treap")) next_state = StateID::TreapVisualizer;
                if (ImGui::MenuItem("BST Array")) next_state = StateID::BSTArrayVisualizer;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Linear Structures")) {
                if (ImGui::MenuItem("Single Linked List")) next_state = StateID::SingleLinkedListVisualizer;
                if (ImGui::MenuItem("Double Linked List")) next_state = StateID::DoubleLinkedListVisualizer;
                if (ImGui::MenuItem("Circular Double Linked List")) next_state = StateID::CircularDLLVisualizer;
                if (ImGui::MenuItem("Array")) next_state = StateID::ArrayVisualizer;
                if (ImGui::MenuItem("Stack")) next_state = StateID::StackVisualizer;
                if (ImGui::MenuItem("Queue")) next_state = StateID::QueueVisualizer;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("App")) {
                if (ImGui::MenuItem("Exit")) next_state = StateID::Exit;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        return next_state;
    }
    void render(sf::RenderWindow& window) override {}
};

