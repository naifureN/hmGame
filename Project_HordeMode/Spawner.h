#pragma once
#include "includes.h"
#include "Enemy.h"
using namespace sf;
class Spawner {
	Texture EnemyTexture;
	std::vector<std::unique_ptr<Enemy>> enemies;
public:
	Spawner();
	~Spawner();
	void spawn();
	void updateEnemies(Vector2f playerpos);
	void renderEnemies(RenderWindow* window); 
	std::vector<std::unique_ptr<Enemy>>& getEnemies();
};

