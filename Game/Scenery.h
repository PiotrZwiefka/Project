#ifndef SCENERY_H
#define SCENERY_H
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "gameobject.h"
class Scenery : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Scenery(int windowWidth, int windowHeight) {
        texture.loadFromFile("Tilemap_Flat.png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(320, 0, 192, 192));
        float scaleX = static_cast<float>(windowWidth) / 192;
        float scaleY = static_cast<float>(windowHeight) / 192;
        sprite.setScale(scaleX, scaleY);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }
};
#endif // SCENERY_H
