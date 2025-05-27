#pragma once
#include "includes.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
using namespace sf;
class Spawner {
	Texture EnemyTexture;
	std::vector<Enemy> enemies;
public:
	Spawner();
	~Spawner();

	void spawn();
	//void updateEnemies(RenderWindow* window);
	void renderEnemies(RenderWindow* window); 
};

