#include"Header.h"
using namespace sf;
//Funs

void Game::initWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Horda Kurwiu", Style::Titlebar|Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initVars() {
	this->endGame = false;
	this->shootDelay = 0.001f;
	std::cout<<this->bulletTexture.loadFromFile("bullet.png");
}

const bool Game::running() const {
	return this->window->isOpen();
}

void Game::pollEvents() {
	while (this->window->pollEvent(this->evnt))
	{
		switch (this->evnt.type)
		{
		case sf::Event::KeyPressed:
			if (this->evnt.key.code == sf::Keyboard::Escape) {
				this->window->close();
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
			this->window->close();
			break;
		}
		
	}
}

//Cons and Destrs
Game::Game() {
	this->initWindow();
	this->initVars();
}

Game::~Game(){
	delete this->window;
}

void Game::update() {
	this->pollEvents();
	this->player.update(this->window);
	updateBullets();

}

void Game::render() {
	this->window->clear();
	this->player.render(this->window);
	this->spawner.renderEnemies(this->window);
	for (auto& bullet : bullets) {
		bullet.render(window);
	}
	this->window->display();

}

void Game::shoot() {
	if (shootClock.getElapsedTime().asSeconds() >= shootDelay) {
		Vector2i mousepos = Mouse::getPosition(*window);
		bullets.emplace_back(&bulletTexture, player.getPos(), Vector2f(mousepos));
		shootClock.restart();
	}
}

void Game::updateBullets() {
	for (auto& bullet : bullets) {
		bullet.update();
	}
}