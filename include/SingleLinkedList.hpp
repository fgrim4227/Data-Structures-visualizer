#pragma once
#include "Data_Structure.hpp"
#include "VisualNode.hpp"

template <typename T>
struct SLLNode {
    T data;
    SLLNode* next = nullptr;
    VisualNode visual;

    SLLNode(T val, const sf::Font& font) 
        : data(val), visual(std::to_string(val), font) {}
};

template <typename T>
class SingleLinkedList : public DataStructure<T> {
private:
    SLLNode<T>* head = nullptr;
    const sf::Font& font;

    void update_positions(SLLNode<T>* node, float x, float y) {
        if (!node) return;
        node->visual.setTargetPosition(sf::Vector2f(x, y));
        update_positions(node->next, x + 120.f, y);
    }

    void internal_draw(sf::RenderWindow& window, SLLNode<T>* node) {
        if (!node) return;
        if (node->next) {
            sf::Color cNext = sf::Color::White; cNext.a = node->next->visual.getOpacity();
            sf::Color cCurr = sf::Color::Black; cCurr.a = node->visual.getOpacity();
            sf::Vertex line[] = { sf::Vertex(node->visual.getPosition(), cCurr), sf::Vertex(node->next->visual.getPosition(), cNext) };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        } else {
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

    void delete_list(SLLNode<T>* node) {
        if (!node) return;
        delete_list(node->next);
        delete node;
    }

public:
    SingleLinkedList(const sf::Font& f) : font(f) {}
    ~SingleLinkedList() override { clear(); }

    void insert(T value) override {
        auto newNode = new SLLNode<T>(value, font);
        newNode->visual.setOpacity(0);
        newNode->visual.setPosition(sf::Vector2f(50.f, 300.f)); 
        
        if (!head) {
            head = newNode;
            this->enqueueCommand(std::make_unique<FadeInCommand>(&newNode->visual, 0.5f));
            return;
        }

        SLLNode<T>* current = head;
        while (current->next) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current->visual, sf::Color::Red, 0.3f));
            current = current->next;
        }
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current->visual, sf::Color::Red, 0.3f));
        
        current->next = newNode;
        this->enqueueCommand(std::make_unique<FadeInCommand>(&newNode->visual, 0.5f));
    }

    void remove(T value) override {
        if (!head) return;
        
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&head->visual, sf::Color(255, 165, 0), 0.3f));
        if (head->data == value) {
            SLLNode<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        SLLNode<T>* current = head;
        while (current->next && current->next->data != value) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current->next->visual, sf::Color(255, 165, 0), 0.3f));
            current = current->next;
        }

        if (current->next) {
            SLLNode<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    void search(T value) override {
        SLLNode<T>* current = head;
        while (current) {
            this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&current->visual, sf::Color::Cyan, 0.3f));
            if (current->data == value) return;
            current = current->next;
        }
    }

    void clear() override {
        delete_list(head);
        head = nullptr;
    }

    void update_animations(SLLNode<T>* node, float dt) {
        if (!node) return;
        node->visual.update(dt);
        update_animations(node->next, dt);
    }
    
    void update(float dt) override { update_animations(head, dt); }
    
    void draw(sf::RenderWindow& window) override {
        update_positions(head, 150.f, 300.f); 
        internal_draw(window, head);
    }
};
