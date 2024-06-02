#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class GameObject {
protected:
    int x,y;
public:
    virtual void update() = 0;
};
class MainCharacter : public GameObject {
private:
    int lives;
    int shootingRange;
    int shootingRate;
public:
    void move(char direction);
    void attack();
    void selectWeapen(int weapon);
};
class Enemy : public GameObject {
protected:
    int health;
public:
    virtual void move() = 0;
    virtual void attack() = 0;
};
class ExplodingEnemy : public Enemy {
public:
    void move();
    void explode();
};
class ShootingEnemy : public Enemy {
public:
    void move();
    void shoot();
};
class Obstacle : public GameObject {
public:
    void blockMovement();
};
class PowerUpCoin : public GameObject {
public:
    void grantAdvantage();
};
class Scenery : public GameObject {
public:
    void renderFoggyForest();
};
class Game {
private:
    MainCharacter player;
    std::vector<Enemy*> enemies;
    std::vector<Obstacle*> obstacles;
    std::vector<PowerUpCoin*> coins;
    Scenery scenery;
    int wave;
    int highscore;
public:
    void start();
    void end();
    void update();
    void spawnEnemies();
    void checkCollisions();
    void collectCoins();
    void increaseDifficulty();
};

int main()
{

    return 0;
}
