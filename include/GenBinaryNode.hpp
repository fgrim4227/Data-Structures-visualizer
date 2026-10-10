#pragma once
#include "VisualNode.hpp"

template <typename T>
struct BinaryNode {
    T data;
    int height = 1;
    BinaryNode* left = nullptr;
    BinaryNode* right = nullptr;
    VisualNode visual;

    BinaryNode(T val, const sf::Font& font) 
        : data(val), visual(std::to_string(val), font) {}
};