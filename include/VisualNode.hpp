#ifndef VISUAL_NODE_HPP
#define VISUAL_NODE_HPP

#include <SFML/Graphics.hpp>
#include <string>

class VisualNode : public sf::Drawable, public sf::Transformable 
{
    public:
        VisualNode(const std::string& value, const sf::Font& font);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    private:
        sf::CircleShape shape;
        sf::Text label;
};

#endif