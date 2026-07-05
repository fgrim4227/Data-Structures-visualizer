#include<iostream>
#include<algorithm>
#include<functional>
template<typename T, typename Compare = std::less<T>>
struct AVLNode
{
    T key;
    int height;
    AVLNode* left;
    AVLNode* right;
    Compare cmp;
    AVLNode(T k, Compare c = Compare()) : key(k), height(1), left(nullptr), right(nullptr), cmp(c) {};
};

template<typename T, typename Compare = std::less<T>>
class AVLTree
{
    using AVLnode = AVLNode<T, Compare>;
    private:
        AVLnode* root;
        int get_height(AVLnode* node)
        {
            return (node) ? node->height : 0;
        }
        int get_balance(AVLnode* node)
        {
            return(node) ? get_height(node->right) - get_height(node->left) : 0;
        }
        void update_height(AVLnode* node)
        {
            if(node)
            {
                node->height = 1 + std::max(get_height(node->left), get_height(node->right));
            }
        }
        void rotate_left(AVLnode*& node)
        {
            AVLnode* R = node->right;
            node->right = R->left;
            R->left = node;
            //Update height of node due to his new 2 sons
            update_height(node);
            //With the correct height of his new son, we update the height of the new father
            update_height(R);
            //We readjust the content of the memory of node so the grandfather points to R
            node = R;
        }
        void rotate_right(AVLnode*& node)
        {
            AVLnode* L = node->left;
            node->left = L->right;
            L->right = node;
            //Update height of node with new sons
            update_height(node);
            //With that correct height we now update the height of the new father
            update_height(L);
            //Now we change the memory of node
            node = L;
        }
        void balance_sub_tree(AVLnode*& node)
        {
            update_height(node);

            int balance = get_balance(node);

            if(balance < -1)
            {
                if(get_balance(node->left) > 0)
                {
                    rotate_left(node->left);
                }
                rotate_right(node);
            }
            else if(balance > 1)
            {
                if(get_balance(node->right) < 0)
                {
                    rotate_right(node->right);
                }
                rotate_left(node);
            }
            //Not neccesary, the rotation functions already update the height of the nodes, but it is a good practice to keep the height updated after balancing
            //update_height(node);
        }
        void insert(AVLnode*& node, T key)
        {
            if(!node)
            {
                node = new AVLnode(key);
                return;
            }
            else if(node->cmp(key, node->key))
            {
                insert(node->left, key);
            }
            else if(node->cmp(node->key, key))
            {
                insert(node->right, key);
            }
            else
            {
                return;
            }
            balance_sub_tree(node);
        }
        /*
        bool insert(AVLNode*& node, T key)
        {
            bool result = false;
            if(!node)
            {
                node = new AVLNode(key);
                return true;
            }    
            else if(node->cmp(key, node->key))
            {
                result = insert(node->left, key);
            }
            else if(node->cmp(node->key,key))
            {
                result = insert(node->right, key);
            }
            else
            {
                return false;
            }
            balance_sub_tree(node);
            return result;
        }
        */
       AVLnode* search(AVLnode* node, T key)
       {
            if(!node) return nullptr;
            if(node->cmp(key, node->key))
            {
                return search(node->left, key);
            }
            else if(node->cmp(node->key, key))
            {
                return search(node->right, key);            
            }
            else
            {
                return node;
            }
       }
       void delete_node(AVLnode*& node, T key)
       {
            if(!node) return;
            if(node->cmp(key, node->key))
            {
                delete_node(node->left, key);
            }
            else if(node->cmp(node->key, key))
            {
                delete_node(node->right, key);
            }
            else
            {
                //Node to delete found!!!
                //Case 1: LeafNode or just 1 child
                if(!node->left || !node->right)
                {
                    AVLnode* temp = node->left ? node->left : node->right;
                    //delete erases the AVLNode that node points to in the heap
                    delete node;
                    //Then the memory that is node is equaled by its only child
                    //That way the node's parent is now pointing to it's grandchild
                    node = temp;
                }
                //Case 2: Node has 2 children
                else
                {
                    //Traditional erasure
                    AVLnode* temp = node->right;
                    while(temp->left)
                    {
                        temp = temp->left;
                    }
                    //Just the key because the dif height and ptrs are already correct
                    node->key = temp->key;
                    delete_node(node->right, temp->key);
                }
            }
            if(node)
            {
                balance_sub_tree(node);
            }
       }
       /*
       //This one probably doesn't work
       AVLNode* traditional_delete(AVLNode*& node, T key)
       {
            if(!node) return nullptr;
            AVLNode* deleted = nullptr;
            if(node->cmp(key, node->key))
            {
                deleted = traditional_delete(node->left, key);
            }
            else if(node->cmp(node->key, key))
            {
                deleted = traditional_delete(node->right, key);
            }
            else
            {
                deleted = node;
                if(!node->left || !node->right)
                {
                    //Theoritecally don't need delete because i have to return the node
                    node = node->left ? node->left : node->right;                    
                }
                AVLNode* successor = node->right;
                while(successor->left)
                {
                    successor = succesor->left;
                }
                node->key = succesor->key;
                traditional_delete(node->right, successor->key);
            }
            if(node)
            {
                balance_subtree(node);
            }
            return deleted;
       }
       */
      void delete_tree(AVLnode* node)
      {
        if(!node) return;
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
      }
    public:
       AVLTree(): root(nullptr) {};
       ~AVLTree() {delete_tree(root);}
       void clean_tree()
       {
            delete_tree(root);
            root = nullptr;
       }
       AVLnode* get_root() const { return root; }
       void set_root(AVLnode* s_root)
       {
            root = s_root;
       };
       void insert(T key)
       {
            insert(root, key);
       }
       AVLnode* find(T key)
       {
            return search(root, key);
       }
       void erase(T key)
       {
            delete_node(root, key);
       }
};