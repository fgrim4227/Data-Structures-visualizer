#pragma once
#include "Data_Structure.hpp"
#include "VisualNode.hpp"
#include <memory>

template <typename T>
struct DLLNode {
    T data;
    std::shared_ptr<DLLNode<T>> next = nullptr;
    std::shared_ptr<DLLNode<T>> prev = nullptr;
    VisualNode visual;

    DLLNode(T val, const sf::Font& font) 
        : data(val), visual(std::to_string(val), font) {}
};

template <typename T>
class DoubleLinkedList : public DataStructure<T> {
private:
    std::shared_ptr<DLLNode<T>> centinel;
    const sf::Font& font;

    void update_positions(std::shared_ptr<DLLNode<T>> node, float x, float y, int count) {
        if (!node || node == centinel) return;
        node->visual.setTargetPosition(sf::Vector2f(x, y));
        update_positions(node->next, x + 120.f, y, count + 1);
    }

    void internal_draw(sf::RenderWindow& window, std::shared_ptr<DLLNode<T>> node) {
        if (!node || node == centinel) return;
        if (node->next && node->next != centinel) {
            sf::Color cNext = sf::Color::White; cNext.a = node->next->visual.getOpacity();
            sf::Color cCurr = sf::Color::Black; cCurr.a = node->visual.getOpacity();
            // Dos lineas para simular doble enlace (arriba y abajo)
            sf::Vector2f offset(0, 10);
            sf::Vertex line1[] = { sf::Vertex(node->visual.getPosition() - offset, cCurr), sf::Vertex(node->next->visual.getPosition() - offset, cNext) };
            sf::Vertex line2[] = { sf::Vertex(node->visual.getPosition() + offset, cCurr), sf::Vertex(node->next->visual.getPosition() + offset, cNext) };
            window.draw(line1, 2, sf::PrimitiveType::Lines);
            window.draw(line2, 2, sf::PrimitiveType::Lines);
        } else if (node->next == centinel) {
            // Tierra (NULL) al final
            sf::Text nullText(font, "NULL", 15);
            nullText.setFillColor(sf::Color(150, 150, 150));
            nullText.setPosition(node->visual.getPosition() + sf::Vector2f(100.f, -10.f));
            sf::Color cCurr = sf::Color::Black; cCurr.a = node->visual.getOpacity();
            sf::Vertex line[] = { sf::Vertex(node->visual.getPosition(), cCurr), sf::Vertex(nullText.getPosition() + sf::Vector2f(0.f, 10.f), sf::Color(150, 150, 150)) };
            window.draw(line, 2, sf::PrimitiveType::Lines);
            window.draw(nullText);
        }
        internal_draw(window, node->next);
        window.draw(node->visual);
    }

public:
    DoubleLinkedList(const sf::Font& f) : font(f) {
        centinel = std::make_shared<DLLNode<T>>(T(), font);
        centinel->visual.setOpacity(0); // Centinela invisible
        centinel->next = centinel;
        centinel->prev = centinel;
    }
    ~DoubleLinkedList() override { clear(); }

    void insert(T value) override {
        auto newNode = std::make_shared<DLLNode<T>>(value, font);
        newNode->visual.setOpacity(0);
        newNode->visual.setPosition(sf::Vector2f(50.f, 300.f));

        auto last_node = centinel->prev;
        
        // Animacion de recorrido (opcional para D_Linked_list, pero recorremos desde el inicio)
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

    void update_animations(std::shared_ptr<DLLNode<T>> node, float dt) {
        if (!node || node == centinel) return;
        node->visual.update(dt);
        update_animations(node->next, dt);
    }
    
    void update(float dt) override { update_animations(centinel->next, dt); }
    
    void draw(sf::RenderWindow& window) override {
        update_positions(centinel->next, 150.f, 300.f, 0);
        internal_draw(window, centinel->next);
    }
};

