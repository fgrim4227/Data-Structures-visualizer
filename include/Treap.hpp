#include<random>
template<typename T, typename Compare = std::less<T>>
//Con min heap
struct TreapNode
{
    T key;
    int priority;
    TreapNode* left;
    TreapNode* right;
    Compare cmp;
    TreapNode(T K, Compare cmp = Compare()): key(K), left(nullptr), right(nullptr), cmp(cmp)
    {
        static std::random_device rng;
        static std::mt19937 gen(rng());
        static std::uniform_int_distribution<int> dist(1, 1e9);
        priority = dist(gen);
    }
    ~TreapNode() = default;
};
template<typename T, typename Compare = std::less<T>>
class Treap
{
    using TrNode = TreapNode<T, Compare>;
    private:
        TrNode* root;
        void rotate_left(TrNode*& node)
        {
            TrNode* R = node->right;
            node->right = R->left;
            R->left = node;
            node = R;
        }
        void rotate_right(TrNode*& node)
        {
            TrNode* L = node->left;
            node->left = L->right;
            L->right = node;
            node = L;
        }
        void insert(TrNode*& node, T key)
        {
            if(!node)
            {
                node = new TrNode(key);
                return;
            }
            if(node->cmp(key, node->key))
            {
                insert(node->left, key);
                if((node->left->priority < node->priority))
                {
                    rotate_right(node);
                }
            }
            else if(node->cmp(node->key, key))
            {
                insert(node->right,key);
                if(node->right->priority < node->priority)
                {
                    rotate_left(node);
                }
            }
            //If it already exists, then nothing must be done
        }
        TrNode* delete_node(TrNode* node, T key)
        {
            if(!node) return nullptr;
            if(node->cmp(key, node->key))
            {
                node->left = delete_node(node->left, key);
                return node;
            }
            else if(node->cmp(node->key, key))
            {
                node->right = delete_node(node->right, key);
                return node;
            }
            else
            {
                //Caso 1: Leaf Node
                if(!node->left && ! node->right)
                {
                    delete node;
                    return nullptr;
                }
                //Case 2: One child
                else if(!node->left)
                {
                    TrNode* temp = node->right;
                    delete node;
                    return temp;
                }
                else if(!node->right)
                {
                    TrNode* temp = node->left;
                    delete node;
                    return temp;
                }
                //Case 3: two Children
                else
                {
                    if(node->left->priority < node->right->priority)
                    {
                        rotate_right(node);
                        node->right = delete_node(node->right, key);
                    }
                    else
                    {
                        rotate_left(node);
                        node->left = delete_node(node->left, key);
                    }
                    return node;
                }
            }
        }
        void inorder_traversal(TrNode* node, int depth = 0)
        {
            if(!node) return;
            inorder_traversal(node->left, depth + 1);
            std::cout<< node->key << " ";
            inorder_traversal(node->right, depth + 1);
        }
        TrNode* search(TrNode* node, T key)
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
        void split_treap(TrNode* root, TrNode*& L, TrNode*& R, T key)
        {
            if(!root)
            {
                L = R = nullptr;
                return;
            }
            else if(root->cmp(key, root->key))
            {
                R = root;
                split_treap(root->left, L, R->left, key);
            }
            else
            {
                L = root;
                split_treap(root->right, L->right, R, key);
            }
        }
        TrNode* merge_treaps(TrNode* L, TrNode* R)
        {
            if(!L) return R;
            if(!R) return L;
            if(L->priority < R->priority)
            {
                L->right = merge_treaps(L->right, R);
                return L;
            }
            else
            {
                R->left = merge_treaps(L, R->left);
                return R;
            }
        }
        TrNode* delete_with_merge(TrNode* node, T key)
        {
            TrNode* new_node = new TrNode(key);
            TrNode* L = nullptr;
            TrNode* R = nullptr;
            split_treap(node, L, R, key);
            TrNode** walker = &L;
            while(*walker && (*walker)->right)
            {
                walker = &((*walker)->right);
            }
            if(*walker && (*walker)->key == key)
            {
                TrNode* to_delete = *walker;
                *walker = (*walker)->left;
                delete to_delete;
            }
            return merge_treaps(L, R);    
        }
        void destroy_treap(TrNode* node)
        {
            if(!node) return;
            destroy_treap(node->left);
            destroy_treap(node->right);
            delete node;
        }

    public:
        Treap(): root(nullptr){}
        ~Treap()
        {
            destroy_treap(root);
        }
        void clean_tree()
       {
            destroy_treap(root);
            root = nullptr;
       }
        TrNode* get_root() const { return root; }
        void set_root(TrNode* s_root) 
        {
            root = s_root;
        };
        void insert(T key)
        {
            insert(root, key);
        }
        void delete_node(T key)
        {
            root = delete_node(root, key);
        }
        void delete_with_merge(T key)
        {
            root = delete_with_merge(root, key);
        }
        void inorder_traversal()
        {
            inorder_traversal(root);
            std::cout<<std::endl;
        }
        TrNode* search(T key)
        {
            return search(root, key);
        }
        
};