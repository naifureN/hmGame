#include"Header.h"
using namespace sf;
using namespace std;
//Funs

void Game::initWindow() {
	
	this->window.create(sf::VideoMode(1280, 720), "Czarodziej Bartjociechus", Style::Titlebar | Style::Close);
	this->window.setFramerateLimit(60);
	sf::Image icon;
	if (icon.loadFromFile("gfx/player.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}

void Game::initVars() {
	//inicjalizacja zmiennych, tekstur oraz overlaye
	this->startGame = true;
	this->endGame = false;
	this->showControls = false;
	this->shootDelay = 0.5f;
	this->bulletTexture.loadFromFile("gfx/bullet.png");
	this->mouseLeftPressedLastFrame = false;
	this->backgroundTexture.loadFromFile("gfx/background.png");
	this->backgroundSprite.setTexture(backgroundTexture);
	this->potionTexture.loadFromFile("gfx/healPot.png");
	this->controlsOverlay.setSize(Vector2f(1280.f, 720.f));
	this->controlsOverlay.setFillColor(Color::Black);
	this->inUpgrade = false;
	this->upgradeOverlay.setSize(Vector2f(1280.f, 720.f));
	this->upgradeOverlay.setFillColor(Color(0, 0, 0, 200));

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	initObstacles();
}

void Game::initFonts() {
	try {
		if (!this->font.loadFromFile("Fonts/Symtext.ttf")) {
			throw std::runtime_error("Nie udalo sie zaladowac czcionki: Fonts/Symtext.ttf");
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Blad ladowania czcionki: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}


void Game::initText() {
	// --- HORDE MODE ---
	this->startText.setFont(this->font);
	this->startText.setCharacterSize(80);
	this->startText.setFillColor(Color::Red);
	this->startText.setString("HORDE MODE");
	
	//Srodkowanie starttext
	FloatRect bounds = startText.getLocalBounds();
	startText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	startText.setPosition(1280.f / 2.f, 150.f);

	// --- GAME OVER ---
	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(80);
	this->endGameText.setFillColor(Color::Red);
	this->endGameText.setString("GAME OVER");

	//Srodkowanie endGameText
	FloatRect endBounds = endGameText.getLocalBounds();
	this->endGameText.setOrigin(endBounds.left + endBounds.width / 2.f, endBounds.top + endBounds.height / 2.f);
	this->endGameText.setPosition(1280.f / 2.f, 200.f);

	// --- CONTROLS ---
	this->controlsText.setFont(this->font);
	this->controlsText.setCharacterSize(40);
	this->controlsText.setFillColor(Color::Red);
	this->controlsText.setString("W - Move forward\nS - Move backwards\nA - Move left\nD - Move right\nLMB - Shoot\nG - Give up");

	//Srodkowanie controls
	FloatRect controlBounds = controlsText.getLocalBounds();
	this->controlsText.setOrigin(controlBounds.left + controlBounds.width / 2.f, controlBounds.top + controlBounds.height / 2.f);
	this->controlsText.setPosition(1280.f / 2.f, 250.f);

	// --- CHOOSE UPGRADE ---
	this->upgradeText.setFont(this->font);
	this->upgradeText.setCharacterSize(60);
	this->upgradeText.setFillColor(Color::Red);
	this->upgradeText.setString("CHOOSE YOUR UPGRADE");

	//Srodkowanie upgradeText
	FloatRect upgradeBounds = upgradeText.getLocalBounds();
	this->upgradeText.setOrigin(upgradeBounds.left + upgradeBounds.width / 2.f, upgradeBounds.top + upgradeBounds.height / 2.f);
	this->upgradeText.setPosition(1280.f / 2.f, 120.f);

}


void Game::initButtons(bool startMode)
{
	buttons.clear();

	float centerX = 1280.f / 2.f;
	float menuY = 280.f;

	//TWORZENIE PRZYCISKOW DO MENU ITD
	
	if (showControls) {
		// BACK TO MENU
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 260.f, 0.f, 0.f, "BACK TO MENU"));
	}
	else if (startMode) {
		// START + EXIT
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 30.f, 0.f, 0.f, "START"));
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 200.f, 0.f, 0.f, "EXIT"));
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 120.f, 0.f, 0.f, "CONTROLS"));
	}
	else {
		// RESTART + EXIT
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 30.f, 0.f, 0.f, "RESTART"));
		buttons.emplace_back(std::make_unique<Button>(centerX, menuY + 130.f, 0.f, 0.f, "EXIT"));
	}
}

void Game::initUpgradeButtons() {
	upgradeButtons.clear();

	//TWORZENIE PRZYCISKOW Z UPGRADAMI
	float centerX = 1280.f / 2.f;
	float startY = 300.f;
	float spacing = 100.f;

	upgradeButtons.emplace_back(std::make_unique<Button>(centerX, startY, 0.f, 0.f, "MORE HP"));
	upgradeButtons.emplace_back(std::make_unique<Button>(centerX, startY + spacing, 0.f, 0.f, "FASTER BULLETS"));
	upgradeButtons.emplace_back(std::make_unique<Button>(centerX, startY + 2 * spacing, 0.f, 0.f, "MORE DAMAGE"));
}




void Game::resetGame() {
	//USTAWIANIE WSZYSTKIEGO NA DOMYSLNA WARTOSC / USUWANIE WSZYSTKICH UTWORZONYCH OBIEKTOW
	
	endGame = false;
	bullets.clear();
	spawner.getEnemies().clear();      
	player.setPos(Vector2f(640, 360));
	obstacles.clear();
	potions.clear();
	initObstacles();
	player.setMaxHp(100);
	player.setHp(100);
	spawner.setWaveNumber(0);
	spawner.startNextWave();


}


const bool Game::running() const {
	//GETTER DO GLOWNEGO WARUNKU TRWANIA GRY
	return this->runningbool;
}

void Game::initObstacles() {
	//TWORZENIE PRZESZKOD
	obstacles.clear();

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int obstacle_count = 5 + (std::rand() % 6); // 2-6 przeszkód

	for (int i = 0; i < obstacle_count; ++i) {
		auto obstacle = std::make_unique<sf::RectangleShape>();

		// Ustawienia przeszkody
		obstacle->setSize(sf::Vector2f(40 + (std::rand() % 150),  30 + (std::rand() % 70)));

		//LOSOWY ROZRZUT 
		float xpos =  std::rand() % window.getSize().x;
		float ypos =  std::rand() % window.getSize().y;

		while(((xpos < 790 && xpos>490) || (ypos < 510 && ypos>210))) {
			xpos = std::rand() % window.getSize().x;
			ypos = std::rand() % window.getSize().y ;

		}
			obstacle->setPosition(xpos, ypos);
			obstacle->setRotation(std::rand() % 360);
			obstacle->setFillColor(sf::Color(50 + (std::rand() % 150), 50 + (std::rand() % 150), 50 + (std::rand() % 150)));
	
			obstacles.push_back(std::move(obstacle));
	}
}


void Game::pollEvents() {
	//WYLAPYWANIE WYDARZEN MOUSE CLICK I KEYBOARD CLICK
	while (this->window.pollEvent(this->evnt))
	{
		switch (this->evnt.type)
		{
		case sf::Event::KeyPressed:
			if (this->evnt.key.code == sf::Keyboard::Escape) {
				runningbool = false;
				this->window.close();
			}
			if (this->evnt.key.code == sf::Keyboard::G) {
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
	//GETTER DO ENDGAME
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
	//WYKRYWANIE KLIKNIECIA
	//ZABEZPIECZENIE PRZED PRZYTRZYMYWANIEM PRZYCISKU

	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	bool mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	for (auto& button : buttons) {
		button->update(mousePos, mouseLeftPressedLastFrame);
	}

	bool currentLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (!mouseLeftPressedLastFrame && currentLeftPressed) {
		for (auto& button : buttons) {
			if (button->isClicked()) {
				std::cout << "Kliknieto przycisk: " << button->getText() << std::endl;

				// DZIALANIE KONKRETNYCH PRZYCISKOW
				if (button->getText() == "START") {
					startGame = false;
					initButtons(false);
					break;
				}
				else if (button->getText() == "EXIT") {
					runningbool = false;
					window.close();
					break;
				}
				else if (button->getText() == "RESTART") {
					resetGame();
					break;
				}
				else if (button->getText() == "CONTROLS") {
					this->showControls = true;
					initButtons(false);
					break;
				}
				else if (button->getText() == "BACK TO MENU") {
					showControls = false;
					initButtons(true);
					break;
				}
			}
		}
	}

	mouseLeftPressedLastFrame = currentLeftPressed;
}



void Game::update() {
	//UPDATE WSZYSTKIEGO PO KOLEI
	//UPDATE ODPOWIEDNICH OBJEKTOW GDY SPELNIONY JEST ODPOWIEDNI WARUNEK
	this->pollEvents();
	this->isPlayerDead();
	if (this->startGame == true) {
		updateButtons();

		return;
	}
	if (inUpgrade) {
		updateUpgradeButtons();
		return; 
	}


	if (this->getEndGame() == false) {
		this->player.update(&this->window, Mouse::getPosition(window));
		updateBullets();
		this->spawner.updateEnemies(player.getPos(), player.getSprite(), player);
		this->spawner.spawn();
		if (this->spawner.isWaveCleared()) {
			this->spawner.startNextWave();
			if (this->spawner.getWaveNumber() > 1) {
				this->inUpgrade = true;
				this->initUpgradeButtons();
			}
		}
		for (auto& enemy : spawner.getEnemies()) {
			if (checkCollisionWithObstacles(enemy->getBounds())) {
				Vector2f dir = enemy->getEnemyPosition() - player.getPos();

				float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
				if (len != 0)
					dir /= len;

				// Cofamy po kroku az wyjdzie z kolizji
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
		this->updatePotions();
	}
	if (this->getEndGame()) {
		updateButtons();
	}

	if (checkCollisionWithObstacles(player.getPlayerBounds())) {
		player.PushBack();
	}

	
}
	


void Game::renderButtons()
{
	//WYSWIETLANIE NA EKRANIE PRZYCISKOW Z WEKTORA
	for (auto& button : buttons) {
		button->render(&window);
	}

}

	
/// Obsluga strzelania przeciwnikow
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

void Game::isPlayerDead() {
	//Sprawdzanie czy gracz umarl, wyswietlenie konca gry 
	if (player.getHp() == 0) {
		endGame = true;
	}
}

void Game::render() {
	//WYSWIETLANIE NA EKRANIE KONKRETNYCH OBJEKTOW WRAZ Z WARUNKAMI
	this->window.clear();
	if (this->startGame) {
		//WYSWIETLANIE EKRANU POCZATKOWEGO
		window.draw(this->startText);

		if (this->showControls) {
			window.draw(controlsOverlay);
			window.draw(this->controlsText);
		}

		renderButtons();  
	}


	else {
		if (inUpgrade) {
			window.draw(upgradeOverlay);
			window.draw(this->upgradeText);
			//wYSWIETLANIE UPGRADOW TYLKO GDY JEST TO POTRZEBNE
			for (auto& btn : upgradeButtons)
				btn->render(&window);
			window.display();
			return; 
		}
		else {
			//RYSOWANIE TLA, WROGOW POCISKOW I POTIONOW
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

			for (auto& p : potions) this->window.draw(*p);
		}
	}
	if (this->endGame==true){
		//WYSWIETLANIE EKRANU KONCOWEGO
		sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(overlay);

		window.draw(endGameText);
		renderButtons();    
	}
	this->window.display();
}

bool Game::checkCollisionWithObstacles(const sf::FloatRect& bounds) const {
	//SPRAWDZANIE KOLIZJI Z PRZESZKODAMI
	for (const auto& obstacle : obstacles) {
		if (checkRotatedCollision(obstacle, bounds)) {
			return true;
		}
	}
	return false;
}

bool Game::checkRotatedCollision(const std::unique_ptr<sf::RectangleShape>& rect, const sf::FloatRect& bounds) const {
	//Sprawdzanie kolizji z obroconymi przeszkodami 
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
	//Tworzenie przyciskow lecacych w strone kursora, restart cooldownu do strzelania 
	if (shootClock.getElapsedTime().asSeconds() >= shootDelay) {
		Vector2i mousepos = Mouse::getPosition(window);
		bullets.emplace_back(std::make_unique<Bullet>(bulletTexture, player.getPos(), Vector2f(mousepos), int(float(25)*damageModifier)));
		shootClock.restart();
	}
}

void Game::updateBullets() {
	//Aktualizacja pociskow
	for (size_t i = 0; i < bullets.size();) {
		bullets[i]->update();

		bool bulletHit = false;
		auto& enemies = spawner.getEnemies();

		for (size_t j = 0; j < enemies.size();) {
			if (bullets[i]->getGlobalBounds().intersects(enemies[j]->getBounds())) {
				enemies[j]->takeDamage(bullets[i]->getDamage());
				bulletHit = true;

				if (enemies[j]->isDead()) {
					int r = rand() % 100;
					if (r < 10) {
						createPotion(enemies[j]->getEnemyPosition());
					}
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

void Game::updatePotions() {
	//Aktualizacja potionow
	const Sprite& playerSprite = player.getSprite();
	for (size_t i = 0; i < potions.size(); ) {
		if (potions[i]->getGlobalBounds().intersects(playerSprite.getGlobalBounds()) and player.getHp() < player.getMaxHp()) {
			player.setHp(min(player.getHp() + 10, player.getMaxHp()));
			potions.erase(potions.begin() + i);
		}
		else {
			++i;
		}
	}
}

void Game::updateUpgradeButtons() {
	//AKTUALIZOWANIE STANU PRZYCISKOW DO UPGRADOW
	Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	bool mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	for (auto& button : upgradeButtons)
		button->update(mousePos, mouseLeftPressedLastFrame);

	if (!mouseLeftPressedLastFrame && mouseLeftPressed) {
		for (auto& button : upgradeButtons) {
			if (button->isClicked()) {
				std::string text = button->getText();
				//DZIALANIE KONKRETNYCH ULEPSZEN
				if (text == "MORE HP") {
					player.setMaxHp(player.getMaxHp() + 20);
				}
				else if (text == "FASTER BULLETS") {
					shootDelay = std::max(shootDelay - 0.1f, 0.05f);
				}
				else if (text == "MORE DAMAGE") {
					damageModifier += 0.325f;
				}

				inUpgrade = false; 
				break;
			}
		}
	}

	mouseLeftPressedLastFrame = mouseLeftPressed;
}


void Game::createPotion(Vector2f position) {
	//TWORZENIE OBJEKTOW POTION
	potions.emplace_back(std::make_unique<Sprite>());
	potions.back()->setTexture(potionTexture);
	potions.back()->setPosition(position);
}

bool Game::isBulletOut(const Bullet& bullet) const {
	//SPRAWDZANIE CZY POCISK WYLECIAL POZA EKRAN
	auto bounds = bullet.getGlobalBounds();
	return (bounds.left + bounds.width<0 || bounds.left>window.getSize().x || bounds.top + bounds.height<0 || bounds.top>window.getSize().y);
}