#ifndef POWERUPCOIN_H
#define POWERUPCOIN_H
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "gameobject.h"
class PowerUpCoin : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int currentFrame;
    float frameTime;
    sf::Clock animationClock;
public:
    PowerUpCoin(float x, float y) : currentFrame(0), frameTime(0.1f) {
        texture.loadFromFile("coin.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        sprite.setScale(6,6);
    }

    void update(float dt) {
        if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
            currentFrame = (currentFrame + 1) % 12;
            int left = currentFrame * 16;
            sprite.setTextureRect(sf::IntRect(left, 0, 16, 16));
            animationClock.restart();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }
};
#endif // POWERUPCOIN_H
