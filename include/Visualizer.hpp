#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <functional>
#include <memory>
#include "State.hpp"
#include "States/MainMenuState.hpp"
#include "States/BSTState.hpp"
#include "States/AVLState.hpp"
#include "States/TreapState.hpp"
#include "States/DoubleLinkedListState.hpp"
#include "States/CircularDLLState.hpp"
#include "States/SingleLinkedListState.hpp"
#include "States/ArrayState.hpp"
#include "States/StackState.hpp"
#include "States/QueueState.hpp"
#include "States/BSTAState.hpp"

template <typename T>
class Visualizer {
private:
    std::unique_ptr<State> current_state;
    std::function<bool(T,T)> cmp;

    void changeState(StateID id, const sf::Font& font) {
        switch (id) {
            case StateID::MainMenu:
                current_state = std::make_unique<MainMenuState>();
                break;
            case StateID::BSTVisualizer:
                current_state = std::make_unique<BSTState<T>>(font);
                break;
            case StateID::AVLVisualizer:
                current_state = std::make_unique<AVLState<T>>(font);
                break;
            case StateID::TreapVisualizer:
                current_state = std::make_unique<TreapState<T>>(font);
                break;
            case StateID::DoubleLinkedListVisualizer:
                current_state = std::make_unique<DoubleLinkedListState<T>>(font);
                break;
            case StateID::CircularDLLVisualizer:
                current_state = std::make_unique<CircularDLLState<T>>(font);
                break;
            case StateID::SingleLinkedListVisualizer:
                current_state = std::make_unique<SingleLinkedListState<T>>(font);
                break;
            case StateID::ArrayVisualizer:
                current_state = std::make_unique<ArrayState>();
                break;
            case StateID::StackVisualizer:
                current_state = std::make_unique<StackState>();
                break;
            case StateID::QueueVisualizer:
                current_state = std::make_unique<QueueState>();
                break;
            case StateID::BSTArrayVisualizer:
                current_state = std::make_unique<BSTAState>();
                break;
            default: break;
        }
    }

public:
    Visualizer(std::function<bool(T,T)> CMP) : cmp(CMP), current_state(nullptr) {}
    ~Visualizer() = default;

    void handle_events(sf::RenderWindow& window, const sf::Event& event) {
        if (current_state) {
            StateID next = current_state->handleEvents(window, event);
            if (next == StateID::Exit) window.close();
        }
    }

    void update_and_run(sf::RenderWindow& window, sf::Clock& delta_clock, const sf::Font& font) {
        ImGui::SFML::Update(window, delta_clock.restart());
        
        if (!current_state) {
            changeState(StateID::MainMenu, font);
        }

        float dt = 1.0f / 60.0f; 
        StateID next = current_state->update(dt);

        if (next != StateID::None) {
            if (next == StateID::Exit) window.close();
            else changeState(next, font);
        }

        current_state->render(window);
        ImGui::SFML::Render(window);
    }
};



