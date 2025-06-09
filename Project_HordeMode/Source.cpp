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
	this->backgroundTexture.loadFromFile("gfx/background.png");
	this->backgroundSprite.setTexture(backgroundTexture);
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	initObstacles();
}

void Game::initFonts(){
	this->font.loadFromFile("Fonts/Symtext.ttf");
}

void Game::initText() {
	// --- HORDE MODE ---
	this->startText.setFont(this->font);
	this->startText.setCharacterSize(60);
	this->startText.setFillColor(Color::Red);
	this->startText.setString("HORDE MODE");
	
	//Srodkowanie starttext
	FloatRect bounds = startText.getLocalBounds();
	startText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	startText.setPosition(1280.f / 2.f, 120.f);

	// --- GAME OVER ---
	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(60);
	this->endGameText.setFillColor(Color::Red);
	this->endGameText.setString("GAME OVER");

	//Srodkowanie endGameText
	FloatRect endBounds = endGameText.getLocalBounds();
	this->endGameText.setOrigin(endBounds.left + endBounds.width / 2.f, endBounds.top + endBounds.height / 2.f);
	this->endGameText.setPosition(1280.f / 2.f, 200.f);
}


void Game::initButtons(bool startMode)
{
	buttons.clear();

	float centerX = 1280.f / 2.f;
	float menuY = 280.f;

	if (startMode) {
		// START + EXIT
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 30.f, 0.f, 0.f, "START"));
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 130.f, 0.f, 0.f, "EXIT"));
	}
	else {
		// RESTART + EXIT
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 30.f, 0.f, 0.f, "RESTART"));
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 130.f, 0.f, 0.f, "EXIT"));
	}
}



void Game::resetGame() {
	
	endGame = false;
	bullets.clear();
	spawner.getEnemies().clear();      
	/*playersetPosition(Vector2f(640, 600));  */

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
	this->initButtons(true);
}

Game::~Game() {
	bullets.clear();
	this->window.clear();
	obstacles.clear();
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
				resetGame();
				return;
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
		this->spawner.updateEnemies(player.getPos(), player.getSprite(), player);
		this->spawner.spawn();
		if (this->spawner.isWaveCleared()) {
			this->spawner.startNextWave();
		}
		for (auto& enemy : spawner.getEnemies()) {
			if (checkCollisionWithObstacles(enemy->getBounds())) {
				Vector2f dir = enemy->getEnemyPosition() - player.getPos();

				float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
				if (len != 0)
					dir /= len;

				// Cofamy po kroku a¿ wyjdzie z kolizji
				const float maxPush = 10.f;
				const float step = 0.5f;

				for (float moved = 0.f; moved < maxPush; moved += step) {
					enemy->EnemyPushBack(dir * step); // teraz przekazujemy gotowy krok
					if (!checkCollisionWithObstacles(enemy->getBounds())) {
						break;
					}
				}
			}
		}
		this->Enemyshoot();
	}
	if (this->getEndGame()) {
		updateButtons();
	}

	if (checkCollisionWithObstacles(player.getPlayerBounds())) {
		std::cout << "Collision" << std::endl;
		player.PushBack();
	}

	
}
	

void Game::renderButtons()
{
	for (auto& button : buttons) {
		button->render(&window);
	}

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
				else if(checkCollisionWithObstacles(bullets[i].getGlobalBounds())){
					bullets.erase(bullets.begin() + i);
				}
				else {
					i++;
				}
			}
		}
	}
}

void Game::render() {
	this->window.clear();
	if (this->startGame) {
		renderButtons();        // START / EXIT
		window.draw(this->startText);
	}
	else {
		this->window.draw(this->backgroundSprite);
		for (size_t i = 0; i < obstacles.size(); ++i) {
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
		
	}
	if (this->endGame==true){
		sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(overlay);

		window.draw(endGameText);
		renderButtons();    // RESTART / EXIT
	}
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
	const sf::Transform& transform = rect->getTransform();
	sf::Transform inverseTransform = transform.getInverse();

	// Przekszta³cenie wszystkich 4 rogów prostok¹ta `bounds` do lokalnego uk³adu wspó³rzêdnych `rect`
	sf::Vector2f points[4] = {
		inverseTransform.transformPoint({bounds.left, bounds.top}),
		inverseTransform.transformPoint({bounds.left + bounds.width, bounds.top}),
		inverseTransform.transformPoint({bounds.left, bounds.top + bounds.height}),
		inverseTransform.transformPoint({bounds.left + bounds.width, bounds.top + bounds.height})
	};

	// Wyznaczenie lokalnego bounding boxa z tych punktów
	float minX = points[0].x, maxX = points[0].x;
	float minY = points[0].y, maxY = points[0].y;
	for (int i = 1; i < 4; ++i) {
		if (points[i].x < minX) minX = points[i].x;
		if (points[i].x > maxX) maxX = points[i].x;
		if (points[i].y < minY) minY = points[i].y;
		if (points[i].y > maxY) maxY = points[i].y;
	}

	sf::FloatRect rectLocal(minX, minY, maxX - minX, maxY - minY);

	return rect->getLocalBounds().intersects(rectLocal);
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
					spawner.addKilled();
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