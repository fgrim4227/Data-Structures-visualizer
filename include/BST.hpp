/*#pragma once
#include <functional>

template<typename T, typename Compare = std::less<T>>
struct BSTNode
{
    T key; // Se utiliza 'key' para mantener consistencia con el AVL
    BSTNode* left;
    BSTNode* right;
    Compare cmp;

    BSTNode(T k, Compare c = Compare()) : key(k), left(nullptr), right(nullptr), cmp(c) {}
};

template<typename T, typename Compare = std::less<T>>
class BST
{
    using Node = BSTNode<T, Compare>;
    private:
        Node* root;

        void insert(Node*& node, T key)
        {
            if (!node)
            {
                node = new Node(key);
                return;
            }
            if (node->cmp(key, node->key))
            {
                insert(node->left, key);
            }
            else if (node->cmp(node->key, key))
            {
                insert(node->right, key);
            }
            // Si son iguales, se omite (asumiendo que no hay duplicados)
        }

        Node* search(Node* node, T key)
        {
            if (!node) return nullptr;
            
            if (node->cmp(key, node->key))
            {
                return search(node->left, key);
            }
            else if (node->cmp(node->key, key))
            {
                return search(node->right, key);
            }
            return node;
        }

        void delete_node(Node*& node, T key)
        {
            if (!node) return;

            if (node->cmp(key, node->key))
            {
                delete_node(node->left, key);
            }
            else if (node->cmp(node->key, key))
            {
                delete_node(node->right, key);
            }
            else
            {
                // Nodo a eliminar encontrado
                // Caso 1: Es un nodo hoja o tiene solo 1 hijo
                if (!node->left || !node->right)
                {
                    Node* temp = node->left ? node->left : node->right;
                    delete node;
                    node = temp;
                }
                // Caso 2: Tiene 2 hijos
                else
                {
                    // Buscamos el sucesor in-order (el menor del subárbol derecho)
                    Node* temp = node->right;
                    while (temp->left)
                    {
                        temp = temp->left;
                    }
                    node->key = temp->key;
                    delete_node(node->right, temp->key);
                }
            }
        }

        void delete_tree(Node* node)
        {
            if (!node) return;
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }

    public:
    auto get_root() const { return root; }
        BST() : root(nullptr) {}
        ~BST() { delete_tree(root); }

        void clean_tree()
        {
            delete_tree(root);
            root = nullptr;
        }

        Node* get_root() const { return root; }
        void set_root(Node* s_root) { root = s_root; }

        void insert(T key)
        {
            insert(root, key);
        }

        Node* find(T key)
        {
            return search(root, key);
        }

        void erase(T key)
        {
            delete_node(root, key);
        }
};
*/
#pragma once
#include "Data_Structure.hpp"
#include "GenBinaryNode.hpp"

template <typename T>
class BST : public DataStructure<T> {
private:
    BinaryNode<T>* root = nullptr;
    const sf::Font& font;

    BinaryNode<T>* internal_insert(BinaryNode<T>* node, T value) {
        if (!node) {
            auto newNode = new BinaryNode<T>(value, font);
            newNode->visual.setOpacity(0);
            // ¡Comando activado!
            this->enqueueCommand(std::make_unique<FadeInCommand>(&newNode->visual, 0.5f));
            return newNode;
        }
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color::Red, 0.3f));
        if (value < node->data) node->left = internal_insert(node->left, value);
        else if (value > node->data) node->right = internal_insert(node->right, value);
        return node;
    }

    BinaryNode<T>* remove_successor_logic(BinaryNode<T>* node, T key) {
        if (!node) return nullptr;
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color(255, 165, 0), 0.3f));
        if (key < node->data) node->left = remove_successor_logic(node->left, key);
        else if (key > node->data) node->right = remove_successor_logic(node->right, key);
        else {
            if (!node->left || !node->right) {
                BinaryNode<T>* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            BinaryNode<T>* temp = node->right;
            while (temp->left) temp = temp->left;
            node->data = temp->data;
            node->visual.setString(std::to_string(node->data));
            node->right = remove_successor_logic(node->right, temp->data);
        }
        return node;
    }

    BinaryNode<T>* remove_predecessor_logic(BinaryNode<T>* node, T key) {
        if (!node) return nullptr;
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color(255, 165, 0), 0.3f));
        if (key < node->data) node->left = remove_predecessor_logic(node->left, key);
        else if (key > node->data) node->right = remove_predecessor_logic(node->right, key);
        else {
            if (!node->left || !node->right) {
                BinaryNode<T>* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            BinaryNode<T>* temp = node->left;
            while (temp->right) temp = temp->right;
            node->data = temp->data;
            node->visual.setString(std::to_string(node->data));
            node->left = remove_predecessor_logic(node->left, temp->data);
        }
        return node;
    }

    BinaryNode<T>* search_node(BinaryNode<T>* node, T key) {
        if (!node) return nullptr;
        this->enqueueCommand(std::make_unique<HighlightNodeCommand>(&node->visual, sf::Color::Cyan, 0.3f));
        if (key < node->data) return search_node(node->left, key);
        if (key > node->data) return search_node(node->right, key);
        return node;
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
    BST(const sf::Font& f) : font(f) {}
    ~BST() override { clear(); }

    void insert(T value) override { root = internal_insert(root, value); }

    void remove(T value) override { remove_successor(value); }

    void remove_successor(T value) { root = remove_successor_logic(root, value); }
    void remove_predecessor(T value) { root = remove_predecessor_logic(root, value); }

    void search(T value) override { search_node(root, value); }
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







