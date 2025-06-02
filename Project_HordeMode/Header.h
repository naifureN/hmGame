#pragma once
#include "includes.h"
#include"Player.h"
#include"Spawner.h"
#include"Bullet.h"


class Game {
	sf::RenderWindow* window;
	sf::Event evnt;
	bool endGame;
	Texture bulletTexture;
	Player player;
	Spawner spawner;
	std::vector<Bullet>bullets;
	Clock shootClock;
	float shootDelay;

	void initWindow();
	void initVars();

public:
	Game();
	~Game();

	const bool running() const;
	void pollEvents();

	void update();
	void render();

	void shoot();
	void updateBullets();
	bool isBulletOut(const Bullet& bullet) const;
};