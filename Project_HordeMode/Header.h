#pragma once
#include "includes.h"
#include"Player.h"
#include"Spawner.h"
#include"Bullet.h"
#include"Enemy.h"


class Game {
	sf::RenderWindow window;
	sf::Event evnt;
	bool endGame;
	std::vector<std::unique_ptr<Bullet>> bullets;
	Texture bulletTexture;
	Player player;
	Spawner spawner;
	Clock shootClock;
	float shootDelay;
	bool runningbool = true;
	std::vector<std::unique_ptr<RectangleShape>> obstacles;

	void initWindow();
	void initVars();
	void initObstacles();

public:
	Game();
	Game(const Game&) = delete;            // Blokada kopiowania
	Game& operator=(const Game&) = delete; // Blokada przypisania
	~Game();

	const bool running() const;
	void pollEvents();

	void update();
	void render();

	bool checkCollisionWithObstacles(const sf::FloatRect& bounds) const;

	bool checkRotatedCollision(const std::unique_ptr<sf::RectangleShape>& rect, const sf::FloatRect& bounds) const;


	void shoot();
	void updateBullets();
	bool isBulletOut(const Bullet& bullet) const;

	void Enemyshoot();
};