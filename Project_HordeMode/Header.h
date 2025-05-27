#pragma once
#include "includes.h"
#include"Player.h"
#include "Enemy.h"
#include "Spawner.h"

class Game {
	sf::RenderWindow* window;
	sf::Event evnt;
	bool endGame;

	Player player;
	Spawner spawner;

	void initWindow();
	void initVars();

public:
	Game();
	~Game();

	const bool running() const;
	void pollEvents();

	void update();
	void render();
};