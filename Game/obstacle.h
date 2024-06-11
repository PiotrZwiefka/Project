#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "gameobject.h"
class Obstacle : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Obstacle(float x, float y) {
        texture.loadFromFile("Tilemap_Flat.png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(192, 192, 64, 64));
        sprite.setPosition(x, y);
        sprite.setScale(4, 4);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }
};
#endif // OBSTACLE_H
