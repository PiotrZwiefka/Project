#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
class GameObject {
protected:
    int x, y;
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
};

#endif // GAMEOBJECT_H
