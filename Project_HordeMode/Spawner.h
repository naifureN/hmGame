#pragma once
#include "includes.h"
#include "Enemy.h"
using namespace sf;
class Spawner {
	Texture EnemyTexture = Texture();
	Texture StandardTexture = Texture();
	Texture RangeTexture = Texture();
	std::vector<std::unique_ptr<Enemy>> enemies;
	int waveNumber = 0;
	int enemiesToSpawn = 0;
	int enemiesSpawned = 0;
	int killedThisTurn = 0;
	sf::Clock spawnClock = Clock();
	float spawnInterval = 1.0f;
	static Font font;
	Text waveText;
	RectangleShape waveBarBackground;
	RectangleShape waveBarFill;
public:
	Spawner();
	~Spawner();
	void spawn();
	void updateEnemies(Vector2f playerpos, Sprite playerSprite, Player& player);
	void renderEnemies(RenderWindow* window); 
	std::vector<std::unique_ptr<Enemy>>& getEnemies();
	void startNextWave();
	bool isWaveCleared() const;
	void initWaveBar();
	void updateWaveBar();
	void addKilled();
};

