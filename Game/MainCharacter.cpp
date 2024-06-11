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

    void move(float offsetX, float offsetY) {
        sprite.move(offsetX, offsetY);
    }

    void setSpeedFactor(float factor) {
        speedFactor = factor;
    }

    float getSpeedFactor() const {
        return speedFactor;
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void decreaseHealth(int amount) {
        health -= amount;
        sprite.setTextureRect(sf::IntRect(0,19,13,19));
    }

    int getHealth() const {
        return health;
    }
    void hit()
    {
        sprite.setTextureRect(sf::IntRect(13,19,13,19));
    }
};
