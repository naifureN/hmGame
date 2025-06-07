#include"Header.h"
using namespace sf;
//Funs

void Game::initWindow() {
	this->window.create(sf::VideoMode(1280, 720), "Horda Kurwiu", Style::Titlebar | Style::Close);
	this->window.setFramerateLimit(60);

}

void Game::initVars() {
	this->endGame = false;
	this->shootDelay = 0.001f;
	this->bulletTexture.loadFromFile("gfx/bullet.png");
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	initObstacles();
}

const bool Game::running() const {
	return this->runningbool;
}

void Game::initObstacles() {
	obstacles.clear();

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int obstacle_count = 2 + (std::rand() % 5); // 2-6 przeszkód

	for (int i = 0; i < obstacle_count; ++i) {
		auto obstacle = std::make_unique<sf::RectangleShape>();

		// Ustawienia przeszkody
		obstacle->setSize(sf::Vector2f(
			50 + (std::rand() % 150),  // szerokoœæ 50-200
			30 + (std::rand() % 70)     // wysokoœæ 30-100
		));

		obstacle->setPosition(
			100 + (std::rand() % (window.getSize().x - 200)),
			100 + (std::rand() % (window.getSize().y - 200))
		);

		obstacle->setRotation(std::rand() % 360);
		obstacle->setFillColor(sf::Color(
			50 + (std::rand() % 150),
			50 + (std::rand() % 150),
			50 + (std::rand() % 150)
		));

		obstacles.push_back(std::move(obstacle));
	}
}


void Game::pollEvents() {
	while (this->window.pollEvent(this->evnt))
	{
		switch (this->evnt.type)
		{
		case sf::Event::KeyPressed:
			if (this->evnt.key.code == sf::Keyboard::Escape) {
				runningbool = false;
				this->window.close();
			}
			if (this->evnt.key.code == sf::Keyboard::Space) {
				this->spawner.spawn();
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (evnt.mouseButton.button == Mouse::Left)
			{
				shoot();
			}
			break;
		case Event::Closed:
			runningbool = false;
			this->window.close();
			break;
		}
		
	}
}

//Cons and Destrs
Game::Game() {
	this->initWindow();
	this->initVars();
}

Game::~Game() {
	bullets.clear();
	obstacles.clear();
	this->window.close();
}


void Game::update() {
	this->pollEvents();
	this->player.update(&this->window);
	if (checkCollisionWithObstacles(player.getPlayerBounds())) {

	}
	updateBullets();
	this->spawner.updateEnemies(player.getPos(), player.getSprite(), player);
	this->Enemyshoot();

}

void Game::Enemyshoot() {
	for (auto& enemy : spawner.getEnemies()) {
		enemy->update(player.getPos(), player.getSprite(), player);
		if (auto ranged = dynamic_cast<RangeEnemy*>(enemy.get())) {
			ranged->updateBullets();
			auto& bullets = ranged->getBullets();
			for (size_t i = 0; i < bullets.size();) {
				if (bullets[i].getGlobalBounds().intersects(player.getPlayerBounds())) {
					player.takeDamage(ranged->getDamage());
					bullets.erase(bullets.begin() + i);
				}
				else {
					++i;
				}
			}
		}
	}
}

void Game::render() {
	this->window.clear();
	for (size_t i = 0; i < obstacles.size();++i) {
		window.draw(*obstacles[i]);
	}
	this->player.render(&this->window);
	this->spawner.renderEnemies(&this->window);
	for (auto& enemy : spawner.getEnemies()) {
		if (auto rangedEnemy = dynamic_cast<RangeEnemy*>(enemy.get())) {
			rangedEnemy->renderBullets(&this->window);
		}
	}
	for (auto& b : bullets) b->render(&window);
	this->player.renderHpBar(&this->window);
	this->window.display();

}

bool Game::checkCollisionWithObstacles(const sf::FloatRect& bounds) const {
	for (const auto& obstacle : obstacles) {
		if (checkRotatedCollision(obstacle, bounds)) {
			return true;
		}
	}
	return false;
}

bool Game::checkRotatedCollision(const std::unique_ptr<sf::RectangleShape>& rect, const sf::FloatRect& bounds) const {
	const sf::Transform& transform = rect->getTransform(); // UWAGA: rect-> zamiast rect.
	sf::Transform inverseTransform = transform.getInverse();

	sf::FloatRect rectLocal;
	rectLocal.left = inverseTransform.transformPoint(bounds.left, bounds.top).x;
	rectLocal.top = inverseTransform.transformPoint(bounds.left, bounds.top).y;
	rectLocal.width = bounds.width;
	rectLocal.height = bounds.height;

	return rect->getLocalBounds().intersects(rectLocal); // rect-> zamiast rect.
}

void Game::shoot() {
	if (shootClock.getElapsedTime().asSeconds() >= shootDelay) {
		Vector2i mousepos = Mouse::getPosition(window);
		bullets.emplace_back(std::make_unique<Bullet>(bulletTexture, player.getPos(), Vector2f(mousepos)));
		shootClock.restart();
	}
}

void Game::updateBullets() {
	for (size_t i = 0; i < bullets.size();) {
		bullets[i]->update();

		bool bulletHit = false;
		auto& enemies = spawner.getEnemies();

		for (size_t j = 0; j < enemies.size();) {
			if (bullets[i]->getGlobalBounds().intersects(enemies[j]->getBounds())) {
				enemies[j]->takeDamage(bullets[i]->getDamage());
				bulletHit = true;

				if (enemies[j]->isDead()) {
					enemies.erase(enemies.begin() + j);
					continue;
				}
			}
			j++;
		}

		if (bulletHit || isBulletOut(*bullets[i]) || checkCollisionWithObstacles(bullets[i]->getGlobalBounds())) {
			bullets.erase(bullets.begin() + i);
		}
		else {
			i++;
		}
	}
}


bool Game::isBulletOut(const Bullet& bullet) const {
	auto bounds = bullet.getGlobalBounds();
	return (bounds.left + bounds.width<0 || bounds.left>window.getSize().x || bounds.top + bounds.height<0 || bounds.top>window.getSize().y);
}