#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

//graphics: https://pixelfrog-assets.itch.io/tiny-swords
const int WINDOW_WIDTH = 2400;
const int WINDOW_HEIGHT = 1800;
const float PLAYER_SPEED = 500;
class GameObject {
protected:
    int x,y;
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
};
class MainCharacter : public GameObject {
private:
    // int lives;
    // int shootingRange;
    // int shootingRate;
    sf::Texture texture;

    sf::Sprite sprite;
public:
    // void move(char direction);
    // void attack();
    // void selectWeapen(int weapon);
    MainCharacter(float x, float y) {
        texture.loadFromFile("Warrior_Purple.png");
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(63, 45,78 , 91));
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
};
// class Enemy : public GameObject {
// protected:
//     int health;
// public:
//     virtual void move() = 0;
//     virtual void attack() = 0;
// };
// class ExplodingEnemy : public Enemy {
// public:
//     void move();
//     void explode();
// };
// class ShootingEnemy : public Enemy {
// public:
//     void move();
//     void shoot();
// };




class Obstacle : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Obstacle(float x, float y) {
        texture.loadFromFile("Tree.png");
        sprite.setTexture(texture);

        sprite.setTextureRect(sf::IntRect(43, 4, 154, 178));
        sprite.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const override {
        return sprite.getGlobalBounds();
    }
};




// class PowerUpCoin : public GameObject {
// public:
//     void grantAdvantage();
// };




class Scenery : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Scenery(int windowWidth, int windowHeight) {
        texture.loadFromFile("Tilemap_Flat.png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 192, 192));
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




// class Game {
// private:
//     MainCharacter player;
//     std::vector<Enemy*> enemies;
//     std::vector<Obstacle*> obstacles;
//     std::vector<PowerUpCoin*> coins;
//     Scenery scenery;
//     int wave;
//     int highscore;
// public:
//     void start();
//     void end();
//     void update();
//     void spawnEnemies();
//     void checkCollisions();
//     void collectCoins();
//     void increaseDifficulty();
// };

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Labyrinth Game");

    Scenery background(WINDOW_WIDTH, WINDOW_HEIGHT);
    std::vector<GameObject*> objects;
    objects.push_back(&background);

        for (int i = 1; i <= 20; i++)
        {
        objects.push_back(new Obstacle(rand() % 2401, rand() % 1800));
        }
    MainCharacter player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dx = 0.0f, dy = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            dx -= PLAYER_SPEED * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            dx += PLAYER_SPEED * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            dy -= PLAYER_SPEED * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            dy += PLAYER_SPEED * dt;

        player.move(dx, dy);

        sf::FloatRect playerBounds = player.getBounds();
        if ((playerBounds.left) < 0)
            player.move(-(playerBounds.left), 0);
        if ((playerBounds.top) < 0)
            player.move(0, -(playerBounds.top));
        if (playerBounds.left+ 78 > WINDOW_WIDTH)
            player.move(WINDOW_WIDTH - playerBounds.left- 78, 0);
        if (playerBounds.top+ 91 > WINDOW_HEIGHT)
            player.move(0, WINDOW_HEIGHT - playerBounds.top- 91);

        for (size_t i = 1; i < objects.size(); ++i) {
            if (objects[i]->getBounds().intersects(player.getBounds())) {
                player.move(-dx, -dy);
            }
        }

        window.clear();

        for (auto& obj : objects) {
            obj->draw(window);
        }
        player.draw(window);

        window.display();
    }



    return 0;
}
