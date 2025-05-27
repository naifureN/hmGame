#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include"Player.h"

class Game {
	sf::RenderWindow* window;
	sf::Event evnt;
	bool endGame;

	Player player;

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