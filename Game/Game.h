#ifndef GAME_H
#define GAME_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <map>
#include <constants.h>
#include <gameobject.h>
#include <MainCharacter.h>
#include <obstacle.h>
#include <Powerupcoin.h>
#include <Scenery.h>
#include <Enemy.h>
class Game {
private:
    sf::RenderWindow window;
    MainCharacter player;
    Scenery background;
    std::vector<GameObject*> objects;
    int highScore; //Coins collected
    sf::Clock attackCooldownClock;
    sf::Clock enemySpawnClock;
    bool gameOver;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text highScoreText;
    sf::Texture bannerTexture;
    sf::Sprite bannerSprite;
    sf::Texture cornerBannerTexture;
    sf::Sprite cornerBannerSprite;
    sf::Texture healthTexture;
    sf::Sprite healthSprite;
    sf::Texture healthbarTexture;
    sf::Sprite healthbarSprite;
    std::map<Enemy*, sf::Clock> enemyCollisionClocks;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
//Function that takes keybord inputs
    void handleInput(float dt) {
        if (gameOver) return;

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
                player.hit();
                for (size_t i = 1; i < objects.size(); ++i) {
                    Enemy* enemy = dynamic_cast<Enemy*>(objects[i]);
                    if (enemy && enemy->getBounds().intersects(player.getBounds())) {
                        enemy->decreaseHealth(PL_ATTACK_DAMAGE);
                        if (enemy->getHealth() <= 0) {
                            objects.erase(objects.begin() + i);
                            delete enemy;
                            i--;
                            //delates enemy when it has less or equal to 0hp
                        }
                    }
                }
                attackCooldownClock.restart();
            }
        }
    }
    void update(float dt) {
        if (gameOver) return;

        handleInput(dt);
        checkCollisions();
        healthSprite.setTextureRect(sf::IntRect(0, 0, player.getHealth() * 10, 48)); //healthbar update
        if (player.getHealth() <= 0) {
            gameOver = true; // game over checking
            displayGameOver();
            return;
        }
//makes player stay within window borders
        sf::FloatRect playerBounds = player.getBounds();
        if (playerBounds.left < 0)
            player.move(-playerBounds.left, 0);
        if (playerBounds.top < 0)
            player.move(0, -playerBounds.top);
        if (playerBounds.left + playerBounds.width > WINDOW_WIDTH)
            player.move(WINDOW_WIDTH - playerBounds.left - playerBounds.width, 0);
        if (playerBounds.top + playerBounds.height > WINDOW_HEIGHT)
            player.move(0, WINDOW_HEIGHT - playerBounds.top - playerBounds.height);
        //times enemies spawn so they dont spawn all at one time
        if (enemySpawnClock.getElapsedTime().asSeconds() >= ENEMY_SPAWN_TIME) {
            spawnEnemy();
            enemySpawnClock.restart();
        }

        for (size_t i = 1; i < objects.size(); ++i) {
            Enemy* enemy = dynamic_cast<Enemy*>(objects[i]);
            if (enemy) {
                enemy->moveTowards(player, dt);
                enemy->updateAnimation(dt);
//makes enemies attack player
                if (enemy->getBounds().intersects(player.getBounds())) {
                    auto it = enemyCollisionClocks.find(enemy);
                    if (it == enemyCollisionClocks.end()) {
                        enemyCollisionClocks[enemy].restart();
                    } else if (it->second.getElapsedTime().asSeconds() >= ENEMY_ATTACK_COOLDOWN) {
                        player.decreaseHealth(BOT_ATTACK_DAMAGE);
                        it->second.restart();
                    }
                } else {
                    enemyCollisionClocks.erase(enemy);
                }
            } else {
                PowerUpCoin* coin = dynamic_cast<PowerUpCoin*>(objects[i]);
                if (coin) {
                    coin->update(dt);
                }
            }
        }
    }

//checks colisions
    void checkCollisions() {
        bool isColliding = false;

        for (size_t i = 1; i < objects.size(); ++i) {
            if (isOverlapping(player.getBounds(), objects[i]->getBounds())) {
                PowerUpCoin* coin = dynamic_cast<PowerUpCoin*>(objects[i]);
                //if object that is coliding with player is coin it adds on to high score and deletes coin and places new
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
        //slows player when they are coliding with bushes and slimes also with coins but they are imidiently deleted so its unnoticable
        if (isColliding) {
            player.setSpeedFactor(COLLISION_SPEED_FACTOR);
        } else {
            player.setSpeedFactor(1.0f);
        }
    }
//renders all objects and text
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

        // Draw the banner at the bottom right corner
        window.draw(cornerBannerSprite);

        // Draw the health at the top left corner
        window.draw(healthSprite);
        window.draw(healthbarSprite);
        if (gameOver) {
            window.draw(bannerSprite);
            window.draw(gameOverText);
            window.draw(highScoreText);
        }

        window.display();
    }

    bool isOverlapping(sf::FloatRect rect1, sf::FloatRect rect2) {
        return rect1.intersects(rect2);
    }
//places obstacles in way that they are not placed coiding and are far flom screen border
    void placeObstacles() {
        for (int i = 1; i <= 10; ++i) {
            bool placed = false;
            while (!placed) {
                float x = 200 + rand() % (WINDOW_WIDTH - 2 * 200 - 256);
                float y = 200 + rand() % (WINDOW_HEIGHT - 2 * 200 - 256);
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
//places starting coins
    void placePowerUpCoins() {
        for (int i = 1; i <= 5; i++) {
            placeNewPowerUpCoin();
        }
    }
//places new coin the same way bushes spawn at start
    void placeNewPowerUpCoin() {
        bool placed = false;
        while (!placed) {
            float x = 200 + rand() % (WINDOW_WIDTH - 2 * 200 - 256);
            float y = 200 + rand() % (WINDOW_HEIGHT - 2 * 200 - 256);
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
//spawns enemies at screen border so its like them going from outside of it
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
//displays game over data so the text banner and highscore
    void displayGameOver() {
        if (!font.loadFromFile("PixelOperator8.ttf")) {
            std::cerr << "Error loading font\n";
            return;
        }
        gameOverText.setFont(font);
        gameOverText.setString("Game Over");
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - 100);

        highScoreText.setFont(font);
        highScoreText.setString("High Score: " + std::to_string(highScore));
        highScoreText.setCharacterSize(20);
        highScoreText.setFillColor(sf::Color::Black);
        highScoreText.setPosition(WINDOW_WIDTH / 2 - highScoreText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2);

        if (!bannerTexture.loadFromFile("Gameover.png")) {
            std::cerr << "Error loading banner texture\n";
            return;
        }
        bannerSprite.setTexture(bannerTexture);
        bannerSprite.setPosition(WINDOW_WIDTH / 2 - bannerSprite.getLocalBounds().width * 2, WINDOW_HEIGHT / 2 - bannerSprite.getLocalBounds().height * 2);
        bannerSprite.setScale(4, 4);
    }

public:
    Game() :
        window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Knight"),
        player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
        background(WINDOW_WIDTH, WINDOW_HEIGHT),
        highScore(0),
        gameOver(false)
    {
        objects.push_back(&background);
        placeObstacles();
        placePowerUpCoins();
//creates a baner that show controls
        if (!cornerBannerTexture.loadFromFile("Controls.png")) {
            std::cerr << "Error loading corner banner texture\n";
        }
        cornerBannerSprite.setTexture(cornerBannerTexture);
        cornerBannerSprite.setPosition(WINDOW_WIDTH - cornerBannerTexture.getSize().x *2, WINDOW_HEIGHT - cornerBannerTexture.getSize().y*2);
        cornerBannerSprite.setScale(2, 2);
//creates the health indicator in healthbar
        if (!healthTexture.loadFromFile("health.png")) {
            std::cerr << "Error loading corner health texture\n";
        }
        healthSprite.setTexture(healthTexture);
        healthSprite.setPosition(51, 0);
        healthSprite.setScale(1, 1);
//creates the overlay of the healthbar
        if (!healthbarTexture.loadFromFile("healthbar.png")) {
            std::cerr << "Error loading corner healthbar texture\n";
        }
        healthbarSprite.setTexture(healthbarTexture);
        healthbarSprite.setPosition(0, 0);
        healthbarSprite.setScale(1, 1);
    }
    ~Game() {
        for (size_t i = 1; i < objects.size(); ++i) {
            delete objects[i];
        }
    }
//runs the game
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
#endif // GAME_H
