#include<stdio.h>
#include<memory>
template<typename T>
class D_linked_Node : public std::enable_shared_from_this<D_linked_Node<T>>
{
    using DL_ptr = std::shared_ptr<D_linked_Node<T>>;
    public:
        D_linked_Node(T value): data(value), next(nullptr), prev(nullptr) {}
        T getData() const { return data; }
        DL_ptr getNext() const { return next; }
        DL_ptr getPrev() const { return prev; }
        void setPrev(DL_ptr prev_node) { prev = prev_node; }
        void setNext(DL_ptr next_node) { next = next_node; }
    private:
        T data;
        DL_ptr next;
        DL_ptr prev;
};
template<typename T>        
class D_Linked_list : public std::enable_shared_from_this<D_Linked_list<T>>
{
    using DL_ptr = std::shared_ptr<D_linked_Node<T>>;
    public:
        D_Linked_list() : centinel(std::make_shared<D_linked_Node<T>>(T()))
        { 
            centinel->setNext(centinel); 
            centinel->setPrev(centinel);
        }
        D_Linked_list(T value) : centinel(std::make_shared<D_linked_Node<T>>(value)) 
        { 
            centinel->setNext(centinel); 
            centinel->setPrev(centinel);
        }
        ~D_Linked_list() = default;
        void push_back(T value);
        void push_front(T value);
        DL_ptr search(T value);
        DL_ptr pop_first();
        DL_ptr pop_last();
        DL_ptr get_centinel() const { return centinel; }
        DL_ptr delete_node(T value);
    private:
        DL_ptr centinel;
};
template<typename T>
void D_Linked_list<T>::push_back(T value)
{
    if(!centinel->getPrev())
    {
        auto insert_node = std::make_shared<D_linked_Node<T>>(value);
        centinel->setPrev(insert_node);
        centinel->setNext(insert_node);
        insert_node->setNext(centinel);
        insert_node->setPrev(centinel);
        return;
    }
    auto new_node = std::make_shared<D_linked_Node<T>>(value);
    
    // El nodo que actualmente está al final
    auto last_node = centinel->getPrev(); 
    
    // Reajustamos enlaces para meter el nuevo nodo entre el antiguo último y el centinela
    last_node->setNext(new_node);
    new_node->setPrev(last_node);
    
    new_node->setNext(centinel);
    centinel->setPrev(new_node);
} 
template<typename T>
void D_Linked_list<T>::push_front(T value)
{
    if(!centinel->getNext())
    {
        auto insert_node = std::make_shared<D_linked_Node<T>>(value);
         centinel->setPrev(insert_node);
        centinel->setNext(insert_node);
        insert_node->setNext(centinel);
        insert_node->setPrev(centinel);
        return;
    }
    auto first_node = centinel->getNext();
    auto insert_node = std::make_shared<D_linked_Node<T>>(value);
    first_node->setPrev(insert_node);
    insert_node->setNext(first_node);
    insert_node->setPrev(centinel);
    centinel->setNext(insert_node);
}
template<typename T>
std::shared_ptr<D_linked_Node<T>> D_Linked_list<T>::search(T value)
{
    auto current_node = centinel->getNext();
    while(current_node != centinel)
    {
        if(current_node->getData() == value)
            return current_node;
        current_node = current_node->getNext();
    }
    return nullptr;
}
template<typename T>
std::shared_ptr<D_linked_Node<T>> D_Linked_list<T>::pop_first()
{
    if(!centinel->getNext())
    {
        return nullptr;
    }
    auto first_node = centinel->getNext();
    auto second_node = first_node->getNext();
    if(second_node == centinel)
    {
        centinel->setNext(centinel);
        centinel->setPrev(centinel);
        return first_node;
    }
    centinel->setNext(second_node);
    second_node->setPrev(centinel);
    first_node->setNext(nullptr);
    first_node->setPrev(nullptr);
    return first_node;
}
template<typename T>
std::shared_ptr<D_linked_Node<T>> D_Linked_list<T>::pop_last()
{
    if(!centinel->getPrev())
    {
        return nullptr;
    }
    auto last_node = centinel->getPrev();
    auto second_to_last_node = last_node->getPrev();
    if(second_to_last_node == centinel)
    {
        centinel->setNext(centinel);
        centinel->setPrev(centinel);
        return last_node;
    }
    centinel->setPrev(second_to_last_node);
    second_to_last_node->setNext(centinel);
    last_node->setPrev(nullptr);
    last_node->setNext(nullptr);
    return last_node;
}
template <typename T>
std::shared_ptr<D_linked_Node<T>> D_Linked_list<T>::delete_node(T value)
{
    auto current_node = centinel->getNext();
    
    while(current_node != centinel && current_node != nullptr)
    {
        if(current_node->getData() == value)
        {
            auto prev_node = current_node->getPrev();
            auto next_node = current_node->getNext();
            
            prev_node->setNext(next_node);
            next_node->setPrev(prev_node);
            
            // Aislar el nodo borrado de forma segura antes de retornarlo
            current_node->setNext(nullptr);
            current_node->setPrev(nullptr);
            return current_node;
        }
        current_node = current_node->getNext();
    }
    return nullptr; // No se encontró
}



