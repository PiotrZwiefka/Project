#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 2400;
const int WINDOW_HEIGHT = 1800;
const float PLAYER_SPEED = 500.0f;
const float COLLISION_SPEED_FACTOR = 0.4f;
const int OBSTACLE_SIZE = 256;
const int BORDER_MARGIN = 200;
const float ENEMY_SPEED = 100.0f;
const int ATTACK_DAMAGE = 80;
const float ATTACK_COOLDOWN = 1.5f;
const float ENEMY_SPAWN_TIME = 3.0f;
class GameObject {
protected:
    int x, y;
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
};

class MainCharacter : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speedFactor;
public:
    MainCharacter(float x, float y) : speedFactor(1.0f) {
        texture.loadFromFile("Warrior_Purple.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(63, 45, 78, 91));
    }

    void moveKey(char direction, float dt) {
        float offsetX = 0.0f, offsetY = 0.0f;
        float speed = PLAYER_SPEED * speedFactor;
        switch (direction) {
        case 'L':
            offsetX = -speed * dt;
            break;
        case 'R':
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
};

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

class PowerUpCoin : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    PowerUpCoin(float x, float y) {
        texture.loadFromFile("G_Idle.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
    }
    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }

    // void grantAdvantage() {} Granding Advantages maybe later
};

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

class Enemy : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int health;
public:
    Enemy(float x, float y) : health(100) {
        texture.loadFromFile("Torch_Red.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(53, 54, 74, 79));
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
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
};

class Game {
private:
    sf::RenderWindow window;
    MainCharacter player;
    Scenery background;
    std::vector<GameObject*> objects;
    int highScore;
    sf::Clock attackCooldownClock;
    sf::Clock enemySpawnClock;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void handleInput(float dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.moveKey('L', dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.moveKey('R', dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.moveKey('U', dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.moveKey('D', dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            if (attackCooldownClock.getElapsedTime().asSeconds() >= ATTACK_COOLDOWN) {
                for (size_t i = 1; i < objects.size(); ++i) {
                    Enemy* enemy = dynamic_cast<Enemy*>(objects[i]);
                    if (enemy && enemy->getBounds().intersects(player.getBounds())) {
                        enemy->decreaseHealth(ATTACK_DAMAGE);
                        if (enemy->getHealth() <=0) {
                            objects.erase(objects.begin() + i);
                            delete enemy;
                            i--;
                        }
                    }
                }
                attackCooldownClock.restart();
            }
            else{
                std::cout<< "cooldown:" << ATTACK_COOLDOWN - attackCooldownClock.getElapsedTime().asSeconds();
            }
        }
    }

    void update(float dt) {
        handleInput(dt);
        checkCollisions();

        sf::FloatRect playerBounds = player.getBounds();
        if (playerBounds.left < 0)
            player.move(-playerBounds.left, 0);
        if (playerBounds.top < 0)
            player.move(0, -playerBounds.top);
        if (playerBounds.left + playerBounds.width > WINDOW_WIDTH)
            player.move(WINDOW_WIDTH - playerBounds.left - playerBounds.width, 0);
        if (playerBounds.top + playerBounds.height > WINDOW_HEIGHT)
            player.move(0, WINDOW_HEIGHT - playerBounds.top - playerBounds.height);
        if (enemySpawnClock.getElapsedTime().asSeconds() >= ENEMY_SPAWN_TIME) {
            spawnEnemy();
            enemySpawnClock.restart();
        }

        for (size_t i = 1; i < objects.size(); ++i) {
            Enemy* enemy = dynamic_cast<Enemy*>(objects[i]);
            if (enemy) {
                enemy->moveTowards(player, dt);
            }
        }
    }

    void checkCollisions() {
        bool isColliding = false;

        for (size_t i = 1; i < objects.size(); ++i) {
            if (objects[i]->getBounds().intersects(player.getBounds())) {
                PowerUpCoin* coin = dynamic_cast<PowerUpCoin*>(objects[i]);
                if (coin) {
                    highScore++;
                    objects.erase(objects.begin() + i);
                    delete coin;
                    placeNewPowerUpCoin();
                    i--;
                } else {
                    isColliding = true;
                }
            }
        }

        if (isColliding) {
            player.setSpeedFactor(COLLISION_SPEED_FACTOR);
        } else {
            player.setSpeedFactor(1.0f);
        }
    }

    void render() {
        window.clear();

        // Draw scenery first
        background.draw(window);

        // Draw player before obstacles
        player.draw(window);

        // Draw obstacles, enemies, and coins last to overlap the player
        for (size_t i = 1; i < objects.size(); ++i) {
            objects[i]->draw(window);
        }

        window.display();
    }

    bool isOverlapping(sf::FloatRect rect1, sf::FloatRect rect2) {
        return rect1.intersects(rect2);
    }

    void placeObstacles() {
        for (int i = 1; i <= 10; ++i) {
            bool placed = false;
            while (!placed) {
                float x = BORDER_MARGIN + rand() % (WINDOW_WIDTH - 2 * BORDER_MARGIN - OBSTACLE_SIZE);
                float y = BORDER_MARGIN + rand() % (WINDOW_HEIGHT - 2 * BORDER_MARGIN - OBSTACLE_SIZE);
                Obstacle* newObstacle = new Obstacle(x, y);
                sf::FloatRect newBounds = newObstacle->getBounds();
                bool overlapping = false;

                for (size_t j = 1; j < objects.size(); ++j) {
                    if (isOverlapping(newBounds, objects[j]->getBounds())) {
                        overlapping = true;
                        break;
                    }
                }

                if (!overlapping) {
                    objects.push_back(newObstacle);
                    placed = true;
                } else {
                    delete newObstacle;
                }
            }
        }
    }

    void placePowerUpCoins() {
        for (int i = 1; i <= 5; ++i) {
            placeNewPowerUpCoin();
        }
    }

    void placeNewPowerUpCoin() {
        bool placed = false;
        while (!placed) {
            float x = BORDER_MARGIN + rand() % (WINDOW_WIDTH - 2 * BORDER_MARGIN - OBSTACLE_SIZE);
            float y = BORDER_MARGIN + rand() % (WINDOW_HEIGHT - 2 * BORDER_MARGIN - OBSTACLE_SIZE);
            PowerUpCoin* newCoin = new PowerUpCoin(x, y);
            sf::FloatRect newBounds = newCoin->getBounds();
            bool overlapping = false;

            for (size_t j = 1; j < objects.size(); ++j) {
                if (isOverlapping(newBounds, objects[j]->getBounds())) {
                    overlapping = true;
                    break;
                }
            }

            if (!overlapping) {
                objects.push_back(newCoin);
                placed = true;
            } else {
                delete newCoin;
            }
        }
    }

    void spawnEnemy() {
        int side = rand() % 4;
        float x = 0, y = 0;

        switch (side) {
        case 0:
            x = rand() % WINDOW_WIDTH;
            y = 0;
            break;
        case 1:
            x = rand() % WINDOW_WIDTH;
            y = WINDOW_HEIGHT;
            break;
        case 2:
            x = 0;
            y = rand() % WINDOW_HEIGHT;
            break;
        case 3:
            x = WINDOW_WIDTH;
            y = rand() % WINDOW_HEIGHT;
            break;
        }

        Enemy* newEnemy = new Enemy(x, y);
        objects.push_back(newEnemy);

    }
public:
    Game() :
        window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Knight"),
        player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
        background(WINDOW_WIDTH, WINDOW_HEIGHT),
        highScore(0)
    {
        objects.push_back(&background);
        placeObstacles();
        placePowerUpCoins();
    }

    ~Game() {
        for (size_t i = 1; i < objects.size(); ++i) {
            delete objects[i];
        }
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            float dt = deltaTime.asSeconds();
            processEvents();
            update(dt);
            render();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
