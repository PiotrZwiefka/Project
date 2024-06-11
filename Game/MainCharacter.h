#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "constants.h"
#include "gameobject.h"
class MainCharacter : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speedFactor;
    int health;
public:
    MainCharacter(float x, float y) : speedFactor(1.0f),  health(100){
        texture.loadFromFile("knight.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(0,0,13,19));
        sprite.setScale(5,5);
    }
    //handles inputs provided by update function from game
    //and converts them into movement variables
    void moveKey(char direction, float dt) {
        float offsetX = 0.0f, offsetY = 0.0f;
        float speed = PLAYER_SPEED * speedFactor;
        switch (direction) {
        case 'L':
            sprite.setTextureRect(sf::IntRect(13,0,13,19));
            offsetX = -speed * dt;
            break;
        case 'R':
            sprite.setTextureRect(sf::IntRect(0,0,13,19));
            offsetX = speed * dt;
            break;
        case 'U':
            offsetY = -speed * dt;
            break;
        case 'D':
            offsetY = speed * dt;
            break;
        default:
            break;
        }
        sprite.move(offsetX, offsetY);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }
//moves player by provided data
    void move(float offsetX, float offsetY) {
        sprite.move(offsetX, offsetY);
    }
//changes speed factor
    void setSpeedFactor(float factor) {
        speedFactor = factor;
    }
//outputs current speedfactor
    float getSpeedFactor() const {
        return speedFactor;
    }
//outputs position in vector
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
//lowers hp of player
    void decreaseHealth(int amount) {
        health -= amount;
        sprite.setTextureRect(sf::IntRect(0,19,13,19));
    }
//outputs player health
    int getHealth() const {
        return health;
    }
//changes player texture when he attacks and the cooldown ended
    void hit()
    {
        sprite.setTextureRect(sf::IntRect(13,19,13,19));
    }
};
#endif // MAINCHARACTER_H
