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
	this->font.loadFromFile("Fonts/Symtext.ttf");
}

void Game::initText(){
	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(60);
	this->endGameText.setFillColor(Color::Red);
	this->endGameText.setString("GAME OVER");
	this->endGameText.setOrigin(endGameText.getGlobalBounds().width / 2, endGameText.getGlobalBounds().height / 2);
	this->endGameText.setPosition(640.f, 280.f);
	

}

void Game::initButtons()
{
	buttons.clear();
	buttons.emplace_back(make_unique<Button>(400.f, 300.f, 200.f, 80.f, "EXIT")); //TU DODAJ KOLEJNE JAK BEDA
	
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
				cout << "ENDGAME USTAWIONE NA TRUE" << endl;
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
	this->initButtons();
}

Game::~Game() {
	bullets.clear();
	this->window.clear();
	this->window.close();
}


void Game::updateButtons()
{
	std::cout << "[DEBUG] updateButtons wywo³ane" << std::endl;

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	bool mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	for (auto& button : buttons) {
		button->update(mousePos, mouseLeftPressed);

		if (button->isClicked()) {
			std::cout << "Kliknieto przycisk: " << button->getText() << std::endl;

			if (button->getText() == "EXIT") {
				cout << "ZAMKNIJ GRE KURWA" << endl;
				this->runningbool = false;
				this->window.close();
				return;
			}
		}
	}
}


void Game::update() {
	this->pollEvents();
	if (this->getEndGame() == false) {
		this->player.update(&this->window);
		updateBullets();
		this->spawner.updateEnemies(player.getPos());
	}
	if (this->getEndGame()) {
		std::cout << "GAME OVER — updateButtons() wywo³ane" << std::endl;
		updateButtons();
	}

	
	
}

void Game::renderButtons()
{
	for (auto& button : buttons) {
		button->render(&window);
	}

}

void Game::render() {
	this->window.clear();

	this->player.render(&this->window);
	this->spawner.renderEnemies(&this->window);
	for (auto& b : bullets) b->render(&window);


	if (this->getEndGame()) {
		sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		this->window.draw(overlay);

		this->window.draw(endGameText);
		this->renderButtons();
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