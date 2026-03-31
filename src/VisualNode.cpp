#include <VisualNode.hpp>

VisualNode::VisualNode(const std::string& value, const sf::Font& font) 
    : label(font, value, 20) 
{
    // Configuración del círculo
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    // En SFML 3, getLocalBounds() devuelve un sf::FloatRect
    // que tiene 'position' (con x, y) y 'size' (con x, y)
    sf::FloatRect textBounds = label.getLocalBounds();
    
    // Centrar el origen del texto usando .size
    label.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f, 
        textBounds.position.y + textBounds.size.y / 2.0f
    });
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    //label.setScale(sf::Vector2f((shape.getRadius() / textBounds.size.x) + 0.3,shape.getRadius() / textBounds.size.y));
}

void VisualNode::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    states.transform *= getTransform();
    target.draw(shape, states);
    target.draw(label, states);
}