#ifndef ENEMY_H
#define ENEMY_H
#include "MainCharacter.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "gameobject.h"
#include <cmath>
class Enemy : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int health;
    sf::Clock attackClock;
    int currentFrame;
    float frameTime;
    sf::Clock animationClock;
public:
    Enemy(float x, float y) : health(100), currentFrame(0), frameTime(0.1f) {
        texture.loadFromFile("slime.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(29, 9, 14, 15));
        sprite.setScale(5, 5);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    void updateAnimation(float dt) {
        if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
            currentFrame = (currentFrame + 1) % 6;
            int column = currentFrame % 3 + 1;
            int row = currentFrame / 3;
            int left = column * 24 + 5;
            int top = row * 24 +9;
            sprite.setTextureRect(sf::IntRect(left, top, 14, 15));
            animationClock.restart();
        }
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }

    void moveTowards(MainCharacter& player, float dt) {
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2f enemyPos = sprite.getPosition();
        sf::Vector2f direction = playerPos - enemyPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
        sprite.move(direction * ENEMY_SPEED * dt);
    }

    void decreaseHealth(int amount) {
        health -= amount;
    }

    int getHealth() const {
        return health;
    }

    sf::Clock& getAttackClock() {
        return attackClock;
    }
};
#endif // ENEMY_H
