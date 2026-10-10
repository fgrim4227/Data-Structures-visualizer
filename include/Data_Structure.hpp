#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <memory>
#include "Command.hpp"

template <typename T>
class DataStructure {
protected:
    std::queue<std::unique_ptr<VisualCommand>>* commandQueue = nullptr;

    void enqueueCommand(std::unique_ptr<VisualCommand> cmd) {
        if (commandQueue) commandQueue->push(std::move(cmd));
    }

public:
    virtual ~DataStructure() = default;

    void setCommandQueue(std::queue<std::unique_ptr<VisualCommand>>* queue) {
        commandQueue = queue;
    }

    virtual void insert(T value) = 0;
    virtual void remove(T value) = 0; 
    virtual void search(T value) = 0;
    virtual void clear() = 0;

    virtual void update(float dt) = 0; 
    virtual void draw(sf::RenderWindow& window) = 0;
};