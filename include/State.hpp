#pragma once
#include <SFML/Graphics.hpp>

enum class StateID {
    None,
    MainMenu,
    BSTVisualizer,
    AVLVisualizer,
    TreapVisualizer,
    DoubleLinkedListVisualizer,
    CircularDLLVisualizer,
    SingleLinkedListVisualizer,
    ArrayVisualizer,
    StackVisualizer,
    QueueVisualizer,
    BSTArrayVisualizer,
    Exit
};

class State {
public:
    virtual ~State() = default;
    virtual StateID handleEvents(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual StateID update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

