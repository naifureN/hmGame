#include "Spawner.h"
using namespace sf;
Spawner::Spawner() {
	EnemyTexture.loadFromFile("gfx/enemy.png");
}
Spawner::~Spawner(){}

void Spawner::startNextWave() {
    waveNumber++;
    int r = 0;
    if (waveNumber > 1) {
        r = rand() % (waveNumber / 2);
    }
    enemiesToSpawn = 3 + waveNumber + r;
    enemiesSpawned = 0;
    killedThisTurn = 0;
    spawnInterval = std::max(0.2f, 1.5f - waveNumber * 0.1f);
    spawnClock.restart();
}

void Spawner::spawn() {
    if (enemiesSpawned < enemiesToSpawn && float(spawnClock.getElapsedTime().asMilliseconds())/1000 >= spawnInterval) {
        int numTypes = 1;
        if (waveNumber >= 4) {
            numTypes = 2;
        }
        if (waveNumber >= 8) {
            numTypes = 3;
        }

        int type = rand() % numTypes;
        switch (type) {
        case 0:
            enemies.emplace_back(std::make_unique<StandardEnemy>(&EnemyTexture));
            break;
        case 1:
            enemies.emplace_back(std::make_unique<RangeEnemy>(&EnemyTexture));
            break;
        case 2:
            enemies.emplace_back(std::make_unique<TankEnemy>(&EnemyTexture));
            break;
        }
        enemiesSpawned++;
        spawnClock.restart();
    }
}

void Spawner::updateEnemies(Vector2f playerpos, Sprite playerSprite, Player& player) {
    for (auto& e : enemies)
        e->update(playerpos, playerSprite, player);
}

void Spawner::renderEnemies(RenderWindow* window) {
    for (auto& e : enemies)
        e->render(window);
}

std::vector<std::unique_ptr<Enemy>>& Spawner::getEnemies() {
    return enemies;
}

bool Spawner::isWaveCleared() const {
    return enemiesToSpawn == enemiesSpawned && enemies.empty();
}