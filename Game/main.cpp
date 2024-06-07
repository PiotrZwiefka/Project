#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

const int WINDOW_WIDTH = 2400;
const int WINDOW_HEIGHT = 1800;
const float PLAYER_SPEED = 500.0f;
const float COLLISION_SPEED_FACTOR = 0.2f;
const int OBSTACLE_SIZE = 256;
const int BORDER_MARGIN = 200;

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

    void grantAdvantage() {
        // Implement logic to grant advantage to the player
    }
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

class Game {
private:
    sf::RenderWindow window;
    MainCharacter player;
    Scenery background;
    std::vector<GameObject*> objects;
    int highScore;

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
    }

    void checkCollisions() {
        bool isColliding = false;

        for (size_t i = 1; i < objects.size(); ++i) {
            if (objects[i]->getBounds().intersects(player.getBounds())) {
                // Check if the object is a PowerUpCoin
                PowerUpCoin* coin = dynamic_cast<PowerUpCoin*>(objects[i]);
                if (coin) {
                    highScore++;
                    objects.erase(objects.begin() + i);
                    delete coin;
                    placeNewPowerUpCoin();
                    i--;  // Adjust the index to account for the erased element
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

        // Draw obstacles and coins last to overlap the player
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

public:
    Game() :
        window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Labyrinth Game"),
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
