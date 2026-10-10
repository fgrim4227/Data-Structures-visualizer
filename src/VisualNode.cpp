#include <cmath>
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
}void VisualNode::setColor(sf::Color color) {
    shape.setFillColor(color);
}
sf::Color VisualNode::getColor() const {
    return shape.getFillColor();
}
void VisualNode::setOpacity(std::uint8_t alpha) {
    sf::Color sc = shape.getFillColor();
    sc.a = alpha;
    shape.setFillColor(sc);
    
    sf::Color oc = shape.getOutlineColor();
    oc.a = alpha;
    shape.setOutlineColor(oc);
    
    sf::Color tc = label.getFillColor();
    tc.a = alpha;
    label.setFillColor(tc);
}
std::uint8_t VisualNode::getOpacity() const {
    return shape.getFillColor().a;
}

void VisualNode::setString(const std::string& str) {
    label.setString(str);
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f, 
        textBounds.position.y + textBounds.size.y / 2.0f
    });
}

void VisualNode::setTargetPosition(sf::Vector2f target) {
    targetPosition = target;
}

void VisualNode::update(float dt) {
    sf::Vector2f current = getPosition();
    sf::Vector2f diff = targetPosition - current;
    float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);
    if (dist > 0.5f) {
        move(diff * 10.0f * dt); // Velocidad de interpolacion (Lerp)
    } else {
        setPosition(targetPosition);
    }
}

