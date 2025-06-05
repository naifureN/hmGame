#include"Header.h"
using namespace sf;
using namespace std;
//Funs

void Game::initWindow() {
	this->window.create(sf::VideoMode(1280, 720), "Horda Kurwiu", Style::Titlebar | Style::Close);
	this->window.setFramerateLimit(60);

}

void Game::initVars() {
	this->endGame = false;
	this->shootDelay = 0.001f;
	this->bulletTexture.loadFromFile("gfx/bullet.png");
}

void Game::initFonts(){
	this->font.loadFromFile("Fonts/Allura-Regular.otf");
}

void Game::initText(){
	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(30);
	this->endGameText.setFillColor(Color::Red);
	this->endGameText.setPosition(Vector2f(200.f, 200.f));
	this->endGameText.setString("GAME OVER");

}

const bool Game::running() const {
	return this->runningbool;
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
			if (this->evnt.key.code == sf::Keyboard::K) {
				this->endGame=true;
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

const bool& Game::getEndGame() const
{
	return this->endGame;
}

//Cons and Destrs
Game::Game() {
	this->initWindow();
	this->initVars();
	this->initFonts();
	this->initText();
}

Game::~Game() {
	bullets.clear();
	this->window.close();
}


void Game::update() {
	if (this->getEndGame() == false) {
		this->pollEvents();
		this->player.update(&this->window);
		updateBullets();
		this->spawner.updateEnemies(player.getPos());
	}
	this->pollEvents();
}

void Game::render() {
	this->window.clear();
	this->player.render(&this->window);
	this->spawner.renderEnemies(&this->window);
	for (auto& b : bullets) b->render(&window);
	if (this->getEndGame() == true) {
		this->window.draw(endGameText);
		this->window.display();
		std::this_thread::sleep_for(5000ms);
		this->window.close();
		this->runningbool = false;
	}
	this->window.display();

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

		if (bulletHit || isBulletOut(*bullets[i])) {
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