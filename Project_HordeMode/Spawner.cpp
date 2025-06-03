#include "Spawner.h"
using namespace sf;
Spawner::Spawner() {
	EnemyTexture.loadFromFile("gfx/enemy.png");
}
Spawner::~Spawner(){}

void Spawner::spawn() {
    int r = rand() % 2;
    switch (r) {
    case 0:
        enemies.emplace_back(std::make_unique<StandardEnemy>(&EnemyTexture));
        break;
    case 1:
        enemies.emplace_back(std::make_unique<TankEnemy>(&EnemyTexture));
        break;
    }
    
}

void Spawner::updateEnemies() {
    for (auto& e : enemies)
        e->update();
}


void Spawner::renderEnemies(RenderWindow* window) {
    for (auto& e : enemies)
        e->render(window);
}

std::vector<std::unique_ptr<Enemy>>& Spawner::getEnemies() {
    return enemies;
}
