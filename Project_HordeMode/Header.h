#pragma once
#include "includes.h"
#include"Player.h"
#include"Spawner.h"
#include"Bullet.h"
#include "Button.h"


class Game {
private:
	sf::RenderWindow window;
	sf::Event evnt;
	bool endGame;
	std::vector<std::unique_ptr<Bullet>> bullets;
	vector<std::unique_ptr<Button>> buttons;
	Texture bulletTexture;
	Player player;
	Spawner spawner;
	Clock shootClock;
	Clock endGameClock;
	Font font;
	Text endGameText;
	RestartButton* restartButton;
	float shootDelay;
	bool runningbool = true;

	void initWindow();
	void initVars();
	void initFonts();
	void initText();
	void initButtons();


public:
	
	const bool& getEndGame() const;
	Game();
	~Game();

	const bool running() const;
	void pollEvents();

	void updateButtons();
	void update();

	void renderButtons();
	void render();

	void shoot();
	void updateBullets();
	bool isBulletOut(const Bullet& bullet) const;
};