#pragma once
#include "Data_Structure.hpp"
#include "VisualNode.hpp"
#include <memory>
#include <cmath>

template <typename T>
class CircularDoubleLinkedList : public DataStructure<T> {
private:
    std::shared_ptr<DLLNode<T>> centinel;
    const sf::Font& font;

    int count_nodes() {
        int n = 1;
        auto curr = centinel->next;
        while (curr != centinel && curr != nullptr) {
            n++;
            curr = curr->next;
        }
        return n;
    }

    void update_positions_circular(std::shared_ptr<DLLNode<T>> node, float cx, float cy, float R, int N, int i) {
        if (!node || (i > 0 && node == centinel)) return;
        float theta = -1.57079632f - i * (6.2831853f / N);
        node->visual.setTargetPosition(sf::Vector2f(cx + R * std::cos(theta), cy + R * std::sin(theta)));
        update_positions_circular(node->next, cx, cy, R, N, i + 1);
    }

    void internal_draw_circular(sf::RenderWindow& window, std::shared_ptr<DLLNode<T>> node, int i) {
        if (!node || (i > 0 && node == centinel)) return;
        if (node->next) {
            sf::Color cNext = sf::Color::White; cNext.a = node->next->visual.getOpacity();
            sf::Color cCurr = sf::Color::Black; cCurr.a = node->visual.getOpacity();
            
            sf::Vector2f p1 = node->visual.getPosition();
            sf::Vector2f p2 = node->next->visual.getPosition();
            sf::Vector2f dir = p2 - p1;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            
            if (len > 0.1f) {
                sf::Vector2f perp(-dir.y / len, dir.x / len);
                sf::Vector2f offset = perp * 8.f; 
                sf::Vertex line1[] = { sf::Vertex(p1 - offset, cCurr), sf::Vertex(p2 - offset, cNext) };
                sf::Vertex line2[] = { sf::Vertex(p1 + offset, cCurr), sf::Vertex(p2 + offset, cNext) };
                window.draw(line1, 2, sf::PrimitiveType::Lines);
                window.draw(line2, 2, sf::PrimitiveType::Lines);
            }
        }
        internal_draw_circular(window, node->next, i + 1);
        window.draw(node->visual);
    }

public:
    CircularDoubleLinkedList(const sf::Font& f) : font(f) {
        centinel = std::make_shared<DLLNode<T>>(T(), font);
        centinel->visual.setOpacity(255);
        centinel->visual.setString("CENT");
        centinel->visual.setPosition(sf::Vector2f(400.f, 300.f));
        centinel->next = centinel;
        centinel->prev = centinel;
    }
    ~CircularDoubleLinkedList() override { clear(); }

    void insert(T value) override {
        auto newNode = std::make_shared<DLLNode<T>>(value, font);
        newNode->visual.setOpacity(0);
        newNode->visual.setPosition(centinel->visual.getPosition()); 

        auto last_node = centinel->prev;
        
        auto current = centinel->next;
        while (current != centinel) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current->visual, sf::Color::Red, 0.2f));
            current = current->next;
        }

        last_node->next = newNode;
        newNode->prev = last_node;
        newNode->next = centinel;
        centinel->prev = newNode;

        this->enqueueCommand(std::make_unique<FadeInCommand>(&newNode->visual, 0.5f));
    }

    void remove(T value) override {
        auto current_node = centinel->next;
        while(current_node != centinel) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current_node->visual, sf::Color(255, 165, 0), 0.3f));
            if(current_node->data == value) {
                auto prev_node = current_node->prev;
                auto next_node = current_node->next;
                prev_node->next = next_node;
                next_node->prev = prev_node;
                return;
            }
            current_node = current_node->next;
        }
    }

    void search(T value) override {
        auto current_node = centinel->next;
        while(current_node != centinel) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current_node->visual, sf::Color::Cyan, 0.3f));
            if(current_node->data == value) return;
            current_node = current_node->next;
        }
    }

    void clear() override {
        centinel->next = centinel;
        centinel->prev = centinel;
    }

    void update_animations_circular(std::shared_ptr<DLLNode<T>> node, float dt, int i) {
        if (!node || (i > 0 && node == centinel)) return;
        node->visual.update(dt);
        update_animations_circular(node->next, dt, i + 1);
    }
    
    void update(float dt) override { update_animations_circular(centinel, dt, 0); }
    
    void draw(sf::RenderWindow& window) override {
        int N = count_nodes();
        float R = std::min(250.f, 50.f + N * 15.f); // Radio dinámico basado en N
        update_positions_circular(centinel, 400.f, 300.f, R, N, 0);
        internal_draw_circular(window, centinel, 0);
    }
};
