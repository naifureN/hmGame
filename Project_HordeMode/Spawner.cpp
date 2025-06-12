#include "Spawner.h"
using namespace sf;
Font Spawner::font;

//konstruktor spawnera przeciwnikow
Spawner::Spawner() {
    try {
        if (!this->font.loadFromFile("Fonts/Symtext.ttf")) {
            throw std::runtime_error("Nie udalo sie zaladowac czcionki: Fonts/Symtext.ttf");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Blad ladowania czcionki: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
	EnemyTexture.loadFromFile("gfx/tank.png");
    StandardTexture.loadFromFile("gfx/standard.png");
    RangeTexture.loadFromFile("gfx/range.png");
    initWaveBar();
}
Spawner::~Spawner(){}

//wczytanie podstawowych wartosci paska progressu fali
void Spawner::initWaveBar() {
    waveBarBackground.setSize(Vector2f(320.f, 10.f));
    waveBarBackground.setFillColor(Color(50, 50, 50));
    waveBarBackground.setOutlineThickness(1.f);
    waveBarBackground.setOutlineColor(Color::White);
    waveBarBackground.setPosition(Vector2f(1280/2-160, 30));

    waveBarFill.setSize(Vector2f(320.f, 10.f));
    waveBarFill.setFillColor(Color::Red);
    waveBarFill.setPosition(Vector2f(1280 / 2 - 160, 30));

    waveText.setFont(font);
    waveText.setCharacterSize(18);
    waveText.setString("Wave " + std::to_string(waveNumber));
    waveText.setPosition(Vector2f(1280 / 2 - waveText.getGlobalBounds().width/2, 5));
}

//aktualizowanie paska progressu fali przeciwnikow
void Spawner::updateWaveBar() {
    float waveState = 1;
    
    if (enemiesToSpawn != 0) {
        waveState = float(killedThisTurn) / float(enemiesToSpawn);
    }
    waveBarFill.setSize(Vector2f(320.f * waveState, 10.f));
    waveText.setString("Wave " + std::to_string(waveNumber));
    waveText.setPosition(Vector2f(1280 / 2 - waveText.getGlobalBounds().width / 2, 3));
}

//rozpoczecie kolejnej fali
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
    if (waveNumber % 3 == 0) {
        modifier += 0.1f;
    }
}

//tworzenie przeciwnikow na podstawie obecnej fali
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
            enemies.emplace_back(std::make_unique<StandardEnemy>(&StandardTexture, this->modifier));
            break;
        case 1:
            enemies.emplace_back(std::make_unique<RangeEnemy>(&RangeTexture, this->modifier));
            break;
        case 2:
            enemies.emplace_back(std::make_unique<TankEnemy>(&EnemyTexture, this->modifier));
            break;
        }
        enemiesSpawned++;
        spawnClock.restart();
    }
}

//wywolanie aktualizacji dla kazdego przeciwnika
void Spawner::updateEnemies(Vector2f playerpos, Sprite playerSprite, Player& player) {
    updateWaveBar();
    for (auto& e : enemies) {
        e->update(playerpos, playerSprite, player);
    }
}

//rysowanie wszystkich przeciwnikow oraz informacji o fali na ekran 
void Spawner::renderEnemies(RenderWindow* window) {
    for (auto& e : enemies)
        e->render(window);
    window->draw(waveText);
    window->draw(waveBarBackground);
    window->draw(waveBarFill);
}

//zwraca pointer do wektora zawierajacego przeciwnikow
std::vector<std::unique_ptr<Enemy>>& Spawner::getEnemies() {
    return enemies;
}

//zwraca czy obecna fala sie zakonczyla
bool Spawner::isWaveCleared() const {
    return enemiesToSpawn == enemiesSpawned && enemies.empty();
}

//zwieksza licznik przeciwnikow zabitych w tej fali
void Spawner::addKilled() {
    killedThisTurn++;
}

//ustawia numer obecnej fali
void Spawner::setWaveNumber(int x) {
    waveNumber = x;
}

//zwraca numer obecnej fali
int Spawner::getWaveNumber() {
    return waveNumber;
}
