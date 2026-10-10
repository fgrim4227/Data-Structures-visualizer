#pragma once
#include "Data_Structure.hpp"
#include "GenBinaryNode.hpp"
#include <algorithm>

template <typename T>
class AVL : public DataStructure<T> {
private:
    BinaryNode<T>* root = nullptr;
    const sf::Font& font;

    int get_height(BinaryNode<T>* node) { return node ? node->height : 0; }
    int get_balance(BinaryNode<T>* node) { return node ? get_height(node->right) - get_height(node->left) : 0; }
    void update_height(BinaryNode<T>* node) { if (node) node->height = 1 + std::max(get_height(node->left), get_height(node->right)); }

    void rotate_left(BinaryNode<T>*& node) {
        BinaryNode<T>* R = node->right;
        node->right = R->left;
        R->left = node;
        update_height(node);
        update_height(R);
        node = R;
    }

    void rotate_right(BinaryNode<T>*& node) {
        BinaryNode<T>* L = node->left;
        node->left = L->right;
        L->right = node;
        update_height(node);
        update_height(L);
        node = L;
    }

    void balance_sub_tree(BinaryNode<T>*& node) {
        update_height(node);
        int balance = get_balance(node);
        if (balance < -1) {
            if (get_balance(node->left) > 0) rotate_left(node->left);
            rotate_right(node);
        } else if (balance > 1) {
            if (get_balance(node->right) < 0) rotate_right(node->right);
            rotate_left(node);
        }
    }

    void internal_insert(BinaryNode<T>*& node, T key) {
        if (!node) {
            node = new BinaryNode<T>(key, font);
            node->visual.setOpacity(0);
            // ¡Comando activado!
            this->enqueueCommand(std::make_unique<FadeInCommand>(&node->visual, 0.5f));
            return;
        }
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color::Red, 0.3f));
        if (key < node->data) internal_insert(node->left, key);
        else if (key > node->data) internal_insert(node->right, key);
        else return;

        balance_sub_tree(node);
    }

    void internal_delete(BinaryNode<T>*& node, T key) {
        if (!node) return;
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color(255, 165, 0), 0.3f));
        if (key < node->data) internal_delete(node->left, key);
        else if (key > node->data) internal_delete(node->right, key);
        else {
            if (!node->left || !node->right) {
                BinaryNode<T>* temp = node->left ? node->left : node->right;
                delete node;
                node = temp;
            } else {
                BinaryNode<T>* temp = node->right;
                while (temp->left) temp = temp->left;
                node->data = temp->data;
                node->visual.setString(std::to_string(node->data));
                internal_delete(node->right, temp->data);
            }
        }
        if (node) balance_sub_tree(node);
    }

    void delete_tree(BinaryNode<T>* node) {
        if (!node) return;
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }

        void update_positions(BinaryNode<T>* node, float x, float y, float hOffset) {
        if (!node) return;
        node->visual.setTargetPosition(sf::Vector2f(x, y));
        update_positions(node->left, x - hOffset, y + 80.f, hOffset / 2.2f);
        update_positions(node->right, x + hOffset, y + 80.f, hOffset / 2.2f);
    }

    void internal_draw(sf::RenderWindow& window, BinaryNode<T>* node) {
        if (!node) return;
        if (node->left) {
                        sf::Color cLeft = sf::Color::White; cLeft.a = node->left->visual.getOpacity();
            sf::Color cRootL = sf::Color::Black; cRootL.a = cLeft.a;
            sf::Vertex line[] = { sf::Vertex(node->visual.getPosition(), cRootL), sf::Vertex(node->left->visual.getPosition(), cLeft) };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
        if (node->right) {
                        sf::Color cRight = sf::Color::White; cRight.a = node->right->visual.getOpacity();
            sf::Color cRootR = sf::Color::Black; cRootR.a = cRight.a;
            sf::Vertex line[] = { sf::Vertex(node->visual.getPosition(), cRootR), sf::Vertex(node->right->visual.getPosition(), cRight) };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
        internal_draw(window, node->left);
        internal_draw(window, node->right);
        window.draw(node->visual); 
    }

public:
    auto get_root() const { return root; }
    AVL(const sf::Font& f) : font(f) {}
    ~AVL() override { clear(); }

    void insert(T value) override { internal_insert(root, value); }
    void remove(T value) override { internal_delete(root, value); }
    BinaryNode<T>* search_node(BinaryNode<T>* node, T key) { if (!node) return nullptr; this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color::Cyan, 0.3f));
        if (key < node->data) return search_node(node->left, key); if (key > node->data) return search_node(node->right, key); return node; } void search(T value) override { search_node(root, value); }
    void clear() override { delete_tree(root); root = nullptr; }
        void update_animations(BinaryNode<T>* node, float dt) {
        if (!node) return;
        node->visual.update(dt);
        update_animations(node->left, dt);
        update_animations(node->right, dt);
    }
    void update(float dt) override { update_animations(root, dt); }
    void draw(sf::RenderWindow& window) override { update_positions(root, 400.f, 50.f, 200.f); internal_draw(window, root); }
};








