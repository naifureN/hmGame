#include "Spawner.h"
using namespace sf;
Spawner::Spawner() {
	EnemyTexture.loadFromFile("gfx/enemy.png");
}
Spawner::~Spawner(){}

void Spawner::spawn() {
    enemies.emplace_back(&EnemyTexture);
}

//void Spawner::updateEnemies(RenderWindow* window) {
//    for (auto& e : enemies)
//        e.update(window);
//}

void Spawner::renderEnemies(RenderWindow* window) {
    for (auto& e : enemies)
        e.render(window);
}