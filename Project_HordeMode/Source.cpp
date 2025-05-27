#include"Header.h"
//Funs

void Game::initWindow() {
	sf::VideoMode fullscreenMode = sf::VideoMode::getFullscreenModes()[0];
	this->videoMode = fullscreenMode;
	this->window = new sf::RenderWindow(this->videoMode, "Project", sf::Style::Fullscreen);
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
				break;
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
	this->window->display();
}
