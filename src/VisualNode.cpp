#include <VisualNode.hpp>

VisualNode::VisualNode(const std::string& value, const sf::Font& font, float radius, sf::Color color) 
    : label(font, value, 20) 
{
    // Usamos el parámetro radius y color
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    sf::FloatRect textBounds = label.getLocalBounds();
    
    label.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f, 
        textBounds.position.y + textBounds.size.y / 2.0f
    });
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
}
void VisualNode::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    states.transform *= getTransform();
    target.draw(shape, states);
    target.draw(label, states);
}