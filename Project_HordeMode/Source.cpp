#include"Header.h"
using namespace sf;
using namespace std;
//Funs

void Game::initWindow() {
	this->window.create(sf::VideoMode(1280, 720), "Horda Kurwiu", Style::Titlebar | Style::Close);
	this->window.setFramerateLimit(60);

}

void Game::initVars() {
	this->startGame = true;
	this->endGame = false;
	this->shootDelay = 0.001f;
	this->bulletTexture.loadFromFile("gfx/bullet.png");
	this->mouseLeftPressedLastFrame = false;
}

void Game::initFonts(){
	this->font.loadFromFile("Fonts/Symtext.ttf");
}

void Game::initText(){
	this->startText.setFont(this->font);
	this->startText.setCharacterSize(60);
	this->startText.setFillColor(Color::Red);
	this->startText.setString("HORDE MODE");
	this->startText.setOrigin(startText.getGlobalBounds().width / 2, startText.getGlobalBounds().height / 2);
	this->startText.setPosition(640.f, 120.f);
	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(60);
	this->endGameText.setFillColor(Color::Red);
	this->endGameText.setString("GAME OVER");
	this->endGameText.setOrigin(endGameText.getGlobalBounds().width / 2, endGameText.getGlobalBounds().height / 2);
	this->endGameText.setPosition(640.f, 200.f);
	

}

void Game::initButtons(bool startMode)
{
	buttons.clear();

	float centerX = 1280.f / 2.f;
	float menuY = 280.f;

	buttons.emplace_back(
		std::make_unique<Button>(
			centerX,
			menuY + 30.f,
			100.f, 100.f,
			startMode ? "START" : "RESTART"));

	buttons.emplace_back(
		std::make_unique<Button>(
			centerX,                
			menuY + 130.f,     
			100.f, 100.f,               
			"EXIT"));

	

	
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
				/*cout << "ENDGAME USTAWIONE NA TRUE" << endl;*/
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
	this->initButtons(true);
}

Game::~Game() {
	bullets.clear();
	this->window.clear();
	this->window.close();
}


void Game::updateButtons()
{
	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	bool mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	for (auto& button : buttons) {
		
		button->update(mousePos, this->mouseLeftPressedLastFrame);

		if (button->isClicked()) {
			std::cout << "Kliknieto przycisk: " << button->getText() << std::endl;
			if (button->getText() == "START") {
				startGame = false;
				initButtons(false);
				return;
			}
			else if (button->getText() == "EXIT") {
				this->runningbool = false;
				this->window.close();
				return;
			}
			else if (button->getText() == "RESTART") {

			}
		
		}
	}

	
	mouseLeftPressedLastFrame = mouseLeftPressed;
}



void Game::update() {
	this->pollEvents();
	if (this->startGame==true) {             
		updateButtons();          
		return;                   
	}

	if (this->getEndGame() == false) {
		this->player.update(&this->window);
		updateBullets();
		this->spawner.updateEnemies(player.getPos());
	}
	if (this->getEndGame()) {
		/*std::cout << "GAME OVER — updateButtons() wywo³ane" << std::endl;*/
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
	window.clear();

	if (startGame==true)
	{
		this->window.draw(startText);   
		renderButtons();          
	}
	else
	{
		
		this->player.render(&window);
		this->spawner.renderEnemies(&window);
		for (auto& bullet : bullets) bullet->render(&window);

		if (endGame==true) {            
			sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
			overlay.setFillColor(sf::Color(0, 0, 0, 150));
			this->window.draw(overlay);

			this->window.draw(endGameText);
			renderButtons();      
		}
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