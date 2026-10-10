#pragma once
#include <SFML/Graphics.hpp>
#include "VisualNode.hpp"
#include <cstdint>

class VisualCommand {
public:
    virtual ~VisualCommand() = default;
    virtual bool execute(float dt) = 0; 
};

class HighlightNodeCommand : public VisualCommand {
private:
    VisualNode* node;
    sf::Color targetColor;
    sf::Color originalColor;
    float duration;
    float elapsedTime;
    bool started = false;

public:
    HighlightNodeCommand(VisualNode* n, sf::Color color, float time) 
        : node(n), targetColor(color), duration(time), elapsedTime(0.0f) {}

    bool execute(float dt) override {
        if (!node) return true;
        if (!started) {
            originalColor = node->getColor();
            started = true;
        } 

        elapsedTime += dt;
        if (elapsedTime >= duration) {
            node->setColor(originalColor);
            return true; 
        }

        node->setColor(targetColor);
        return false; 
    }
};

class FadeInCommand : public VisualCommand {
private:
    VisualNode* node;
    float duration;
    float elapsedTime;
    bool started = false;

public:
    FadeInCommand(VisualNode* n, float time) 
        : node(n), duration(time), elapsedTime(0.0f) {}

    bool execute(float dt) override {
        if (!node) return true;
        if (!started) {
            node->setOpacity(0);
            started = true;
        }

        elapsedTime += dt;
        float progress = elapsedTime / duration;
        
        if (progress >= 1.0f) {
            node->setOpacity(255);
            return true;
        }

        node->setOpacity(static_cast<std::uint8_t>(255 * progress));
        return false;
    }
};
