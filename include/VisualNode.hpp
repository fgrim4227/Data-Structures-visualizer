#ifndef VISUAL_NODE_HPP
#define VISUAL_NODE_HPP

#include <SFML/Graphics.hpp>
#include <string>

class VisualNode : public sf::Drawable, public sf::Transformable 
{
    public:
        VisualNode(const std::string& value, const sf::Font& font, float radius = 35.f, sf::Color color = sf::Color::Blue);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
        // Nuevos métodos para animaciones
        void setColor(sf::Color color);
        sf::Color getColor() const;

        void setOpacity(std::uint8_t alpha); // 0 = invisible, 255 = opaco
        std::uint8_t getOpacity() const;
        void setTargetPosition(sf::Vector2f target);
        void update(float dt);
        void setString(const std::string& str);
        
    private:
        sf::CircleShape shape;
        sf::Text label;
        sf::Color currentColor;
        sf::Vector2f targetPosition; // Útil para guardar el color base si necesitas leerlo rápido
};

#endif


