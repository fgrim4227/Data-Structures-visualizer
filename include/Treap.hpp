#pragma once
#include "Data_Structure.hpp"
#include <cstdlib>

template <typename T>
struct TreapNode {
    T data;
    int priority;
    TreapNode* left = nullptr;
    TreapNode* right = nullptr;
    VisualNode visual;

    TreapNode(T val, const sf::Font& font) 
        : data(val), priority(std::rand()), visual(std::to_string(val), font) {}
};

template <typename T>
class Treap : public DataStructure<T> {
private:
    TreapNode<T>* root = nullptr;
    const sf::Font& font;

    void split_treap(TreapNode<T>* node, TreapNode<T>*& L, TreapNode<T>*& R, T key) {
        if (!node) { L = R = nullptr; return; }
        if (key > node->data) {
            L = node;
            split_treap(node->right, L->right, R, key);
        } else {
            R = node;
            split_treap(node->left, L, R->left, key);
        }
    }

    TreapNode<T>* merge_treaps(TreapNode<T>* L, TreapNode<T>* R) {
        if (!L) return R;
        if (!R) return L;
        if (L->priority > R->priority) { 
            L->right = merge_treaps(L->right, R);
            return L;
        } else {
            R->left = merge_treaps(L, R->left);
            return R;
        }
    }

    void internal_insert(TreapNode<T>*& node, TreapNode<T>* newNode) {
        if (!node) {
            node = newNode;
            return;
        }
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color::Red, 0.3f));
        if (newNode->priority > node->priority) {
            split_treap(node, newNode->left, newNode->right, newNode->data);
            node = newNode;
        } else {
            if (newNode->data < node->data) internal_insert(node->left, newNode);
            else internal_insert(node->right, newNode);
        }
    }

    TreapNode<T>* delete_node(TreapNode<T>* node, T key) {
        if (!node) return nullptr;
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color(255, 165, 0), 0.3f));
        if (key < node->data) node->left = delete_node(node->left, key);
        else if (key > node->data) node->right = delete_node(node->right, key);
        else {
            TreapNode<T>* temp = merge_treaps(node->left, node->right);
            delete node;
            return temp;
        }
        return node;
    }

    TreapNode<T>* delete_with_merge_split_logic(TreapNode<T>* node, T key) {
        TreapNode<T> *L = nullptr, *R = nullptr, *M = nullptr;
        split_treap(node, L, R, key);
        split_treap(R, M, R, key + 1); 
        if (M) delete M;
        return merge_treaps(L, R);
    }

    void delete_tree(TreapNode<T>* node) {
        if (!node) return;
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }

        void update_positions(TreapNode<T>* node, float x, float y, float hOffset) {
        if (!node) return;
        node->visual.setTargetPosition(sf::Vector2f(x, y));
        update_positions(node->left, x - hOffset, y + 80.f, hOffset / 2.2f);
        update_positions(node->right, x + hOffset, y + 80.f, hOffset / 2.2f);
    }

    void internal_draw(sf::RenderWindow& window, TreapNode<T>* node) {
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
    Treap(const sf::Font& f) : font(f) {}
    ~Treap() override { clear(); }

    void insert(T value) override {
        auto newNode = new TreapNode<T>(value, font);
        newNode->visual.setOpacity(0);
        internal_insert(root, newNode);
        this->enqueueCommand(std::make_unique<FadeInCommand>(&newNode->visual, 0.5f));
    }

    void remove(T value) override { remove_standard(value); }

    void remove_standard(T value) { root = delete_node(root, value); }
    void remove_merge_split(T value) { root = delete_with_merge_split_logic(root, value); }

    void search(T value) override { }
    void clear() override { delete_tree(root); root = nullptr; }
        void update_animations(TreapNode<T>* node, float dt) {
        if (!node) return;
        node->visual.update(dt);
        update_animations(node->left, dt);
        update_animations(node->right, dt);
    }
    void update(float dt) override { update_animations(root, dt); }
    void draw(sf::RenderWindow& window) override { update_positions(root, 400.f, 50.f, 200.f); internal_draw(window, root); }
};






