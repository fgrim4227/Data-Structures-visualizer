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
