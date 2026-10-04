#pragma once
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
/*
#Antogua implementacion
#include<VisualNode.hpp>
#include<functional>
#include<memory>
template<typename T>
class BST : public std::enable_shared_from_this<BST<T>>
{
    using BSTNode = std::shared_ptr<BST<T>>;
    public:
        BST(T value, std::function<bool(T,T)> cmp, BSTNode l_child, BSTNode r_child): data(value), CMP(cmp), left(l_child), right(r_child) {}
        BST(T value, std::function<bool(T,T)> cmp): data(value), CMP(cmp), left(nullptr), right(nullptr) {}
        ~BST() = default;
        BSTNode search(T value);
        bool insert(T value);
        BSTNode delete_node(T value);
        T getData() const { return data; }
        std::shared_ptr<BST<T>> getLeft() const { return left; }
        std::shared_ptr<BST<T>> getRight() const { return right; }
        BSTNode get_inorder_succesor_start();
        BSTNode get_inorder_predecessor_start();
    private:
        T data;
        BSTNode left;
        BSTNode right;
        std::function<bool(T,T)> CMP;
        BSTNode get_inorder_successor(BSTNode subtree)
        {
            if(!subtree)
            {
                return nullptr;
            }
            return subtree->left ? get_inorder_successor(subtree->left) : subtree;
        }
        BSTNode get_inorder_predecessor(BSTNode subtree)
        {
            if(!subtree)
            {
                return nullptr;
            }
            return (subtree->right) ? get_inorder_predecessor(subtree->right) : subtree;
        }

};
template<typename T>
bool BST<T>::insert(T value)
{
    if(CMP(data, value))
    {
        if(!left)
        {
            left = std::make_shared<BST<T>>(value, this->CMP);
            return true;
        }
        return left->insert(value);
    }
    else if(CMP(value, data))
    {
        if(!right)
        {
            right = std::make_shared<BST<T>>(value, this->CMP);
            return true;
        }
        return right->insert(value);
    }
    return false;
}
template<typename T>
std::shared_ptr<BST<T>> BST<T>::search(T value)
{
    if(!this)
    {
        return nullptr;
    }
    if(CMP(data, value))
    {
        return left->search(value);
    }
    else if(CMP(value, data))
    {
        return right->search(value);
    }
    return this->shared_from_this();
}
template <typename T>
std::shared_ptr<BST<T>> BST<T>::get_inorder_succesor_start()
{
    return this->get_inorder_successor(right);
}
template <typename T>
std::shared_ptr<BST<T>> BST<T>::get_inorder_predecessor_start()
{

    return this->get_inorder_predecessor(left);
}
template<typename T>
std::shared_ptr<BST<T>> BST<T>::delete_node(T value)
{
    if(!this)
    {
        return nullptr;
    }
    if(CMP(data, value))
    {
        if(left) left = left->delete_node(value);
    }
    else if(CMP(value, data))
    {
        if(right) right = right->delete_node(value);
    }
    else
    {
        if(!left) return right;
        if(!right) return left;
        BSTNode replacement = this->get_inorder_succesor_start();
        this->data = replacement->data;
        right = right->delete_node(replacement->data);
    }
    return this->shared_from_this();
}
*/