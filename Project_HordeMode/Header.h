#pragma once
#include "includes.h"
#include"Player.h"
#include"Spawner.h"
#include"Bullet.h"
#include "Button.h"
#include"Enemy.h"


class Game {
private:
	sf::RenderWindow window;
	sf::Event evnt;
	Texture backgroundTexture;
	Sprite backgroundSprite;
	bool endGame;
	bool startGame;
	bool showControls;
	std::vector<std::unique_ptr<Bullet>> bullets;
	vector<std::unique_ptr<Button>> buttons;
	Texture bulletTexture;
	Player player;
	Spawner spawner;
	Clock shootClock;
	Clock controlsClock;
	Font font;
	Text endGameText;
	Text startText;
	Text controlsText;
	float shootDelay;
	bool runningbool = true;
	bool mouseLeftPressedLastFrame;

	void initWindow();
	void initVars();
	void initFonts();
	void initText();
	void initButtons(bool startMode=false);

	std::vector<std::unique_ptr<RectangleShape>> obstacles;
	void initObstacles();

public:
	
	const bool& getEndGame() const;
	Game();
	Game(const Game&) = delete;            // Blokada kopiowania
	Game& operator=(const Game&) = delete; // Blokada przypisania
	~Game();

	void resetGame();
	const bool running() const;
	void pollEvents();

	void updateButtons();
	void update();

	void renderButtons();
	void render();

	bool checkCollisionWithObstacles(const sf::FloatRect& bounds) const;

	bool checkRotatedCollision(const std::unique_ptr<sf::RectangleShape>& rect, const sf::FloatRect& bounds) const;


	void shoot();
	void updateBullets();
	bool isBulletOut(const Bullet& bullet) const;

	void Enemyshoot();
};