#include"Header.h"
using namespace sf;
//Funs

void Game::initWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(1280,720), "Horda Kurwiu");
	this->window->setFramerateLimit(60);
}

void Game::initVars() {
	this->endGame = false;
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
			case Event::Closed:
				this->window->close();
		}
		
	}
}

//Cons and Destrs
Game::Game() {
	this->initWindow();
}

Game::~Game(){
	delete this->window;
}

void Game::update() {
	this->pollEvents();
	this->player.update(this->window);

}

void Game::render() {
	this->window->clear();
	this->player.render(this->window);
	this->spawner.renderEnemies(this->window);
	this->window->display();
}
