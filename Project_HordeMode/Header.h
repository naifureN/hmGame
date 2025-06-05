#pragma once
#include "includes.h"
#include"Player.h"
#include"Spawner.h"
#include"Bullet.h"


class Game {
	sf::RenderWindow window;
	sf::Event evnt;
	bool endGame;
	std::vector<std::unique_ptr<Bullet>> bullets;
	Texture bulletTexture;
	Player player;
	Spawner spawner;
	Clock shootClock;
	Clock endGameClock;
	Font font;
	Text endGameText;
	float shootDelay;
	bool runningbool = true;

	void initWindow();
	void initVars();
	void initFonts();
	void initText();


public:
	const bool& getEndGame() const;
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