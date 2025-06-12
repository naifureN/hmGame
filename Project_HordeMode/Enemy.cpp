#include "Enemy.h"
using namespace sf;

//Renderowanie przeciwnikow
void Enemy::render(sf::RenderTarget* target) {
	target->draw(this->sprite);
	target->draw(this->hpBarBackground);
	target->draw(this->hpBarFill);
	renderEffect(target);
}

//Tworzenie przeciwnika wraz z losowaniem z ktorej strony sie pojawi
Enemy::Enemy(Texture* tex, float m): hp(100), maxHp(100), modifier(m) {
	this->sprite.setTexture(*tex);
	int dir = rand() % 4;
	int rx, ry;
	switch (dir) {
	case 0:
		rx = -(rand() % 40 + 40);
		ry = rand() % 640 + 40;
		this->sprite.setPosition(Vector2f(rx, ry));
		break;
	case 1:
		rx = rand() % 40 + 40 + 1280;
		ry = rand() % 640 + 40;
		this->sprite.setPosition(Vector2f(rx, ry));
		break;
	case 2:
		rx = rand() % 1200 + 40;
		ry = -(rand() % 40 + 40);
		this->sprite.setPosition(Vector2f(rx, ry));
		break;
	case 3:
		rx = rand() % 1200 + 40;
		ry = rand() % 40 + 40 + 720;
		this->sprite.setPosition(Vector2f(rx, ry));
		break;
	}

	hpBarBackground.setPosition(sprite.getPosition());
	hpBarFill.setPosition(sprite.getPosition()); // x te same a y-10 jest ideolo

	hpBarBackground.setSize(Vector2f(32.f, 5.f));
	hpBarBackground.setFillColor(Color(50, 50, 50));

	hpBarFill.setSize(Vector2f(32.f, 5.f));
	hpBarFill.setFillColor(Color(0, 255, 0));

	updateHpBar();
}

Enemy::~Enemy(){}

//Zwraca czy przeciwnik zyje
bool Enemy::isDead() const {
	return hp <= 0;
}

//Funkcja zwracajaca polozenie sprite'a oraz jego rozmiar
const FloatRect Enemy::getBounds() const {
	return sprite.getGlobalBounds();
}

//Zadawanie obrazen przeciwnikowi
void Enemy::takeDamage(int damage) {
	hp -= damage;
	if (hp < 0) {
		hp=0;
	}
	updateHpBar();
}

//Aktualizacja paska zycia na podstawie obecnej ilosci punktow zycia
void Enemy::updateHpBar() {
	if (maxHp <= 0) {
		return;
	}

	float hpPercent = static_cast<float>(hp) / maxHp;
	hpPercent = std::clamp(hpPercent, 0.f, 1.f);
	hpBarFill.setSize(Vector2f(32 * hpPercent, 5));

	if (hpPercent < 0.3) {
		hpBarFill.setFillColor(Color(255, 0, 0));
	}
	else if (hpPercent < 0.6) {
		hpBarFill.setFillColor(Color(255, 165, 0));
	}
}

//ruch Sprite'a przeciwnika
void Enemy::update(Vector2f playerpos, Sprite playerSprite, Player& player) {
	if (inverted == true) {
		sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
		sprite.setScale(-1.f, 1.f);
	}
	else if (inverted == false) {
		sprite.setOrigin(0.f, 0.f);
		sprite.setScale(1.f, 1.f);
	}

	if (checkCollision(playerSprite)) {
		collided(player);
	}
	else {
		moveEnemy(playerpos);
	}
	hpBarBackground.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y-10);
	hpBarFill.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 10);
}

//ustawia AttackSpeed przeciwnika
void Enemy::setAttackSpeed(float x) {
	attackSpeed = x;
}

//zwracaAttackSpeed przeciwnika
float Enemy::getAttackSpeed() {
	return attackSpeed;
}

//ustawia punkty zycia przeciwnika
void Enemy::setHP(int x) {
	hp = x;
	maxHp = x;
}

//zwraca pozycje sprite'a
Vector2f Enemy::getSpritePos() {
	return sprite.getPosition();
}

//ustawia predkosc ruchu przeciwnika
void Enemy::setMovespeed(float x) {
	movespeed = x;
}

//porusza sprite przeciwnika
void Enemy::moveSprite(Vector2f movement) {
	sprite.move(movement);
}

//zwraca predkosc ruchu przeciwnika
float Enemy::getMovespeed() {
	return movespeed;
}

//sprawdza czy sprite przeciwnika koliduje z innym sprite'em
bool Enemy::checkCollision(const sf::Sprite& otherSprite) {
	return sprite.getGlobalBounds().intersects(otherSprite.getGlobalBounds());
}

//zwraca czas od ostatniego ataku
float Enemy::getAttackTime() {
	return attackTimer.getElapsedTime().asMilliseconds() / 1000;
}

//ustawia obrazenia zadawane przez przeciwnika
void Enemy::setDamage(int x) {
	damage = x;
}

//zwraca obrazenia zadawane przez przeciwnika
int Enemy::getDamage() const {
	return damage;
}

//resetuje timer na podstawie ktorego przeciwnik atakuje
void Enemy::resetAttackTimer() {
	attackTimer.restart();
}

//porusza sprite przeciwnika do tylu
void Enemy::EnemyPushBack(const Vector2f& pushVec) {
	sprite.move(pushVec);
}

//zwraca pozycje sprite'a przeciwnika
const Vector2f& Enemy::getEnemyPosition() const{
	return sprite.getPosition();
}

//ustawia modyfikator trudnosci przeciwnika
void Enemy::setModifier(float x) {
	modifier = x;
}

//zwraca modyfikator trudnosci przeciwnika
float Enemy::getModifier() {
	return modifier;
}


//ustawienie podstawowych zmiennych przeciwnika rodzaju Standard
void StandardEnemy::initVars() {
	setAttackSpeed(2.5f / modifier);
	setHP(int(float(100)*modifier));
	setMovespeed(1.5f / modifier);
	setDamage(int(float(10)*modifier));
	if (rand() % 2 == 1) {
		AngleDir = true;
	}
	else {
		AngleDir = false;
	}
}

//konstruktor przeciwnika rodzaju Standard
StandardEnemy::StandardEnemy(Texture* tex, float m) : Enemy(tex, m){
	initVars();
	slashTexture.loadFromFile("gfx/slash.png");
	slashSprite.setTexture(slashTexture);
	slashSprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
}

//porusza przeciwnika rodzaju Standard
void StandardEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float spread = 50.f;
	float RandDegr = (rand() % 10) + 1;
	if (AngleDir == 1) {
		RandDegr *= -1;
	}
	float rad = RandDegr * 3.14 / 180;
	float x = direction.x * cos(rad) - direction.y * sin(rad);
	float y = direction.x * sin(rad) + direction.y * cos(rad);
	direction = Vector2f(x, y);
	if (noiseTimer.getElapsedTime().asSeconds() > 1.f) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-spread, spread);

	directionNoise = { dist(gen), dist(gen) };
	noiseTimer.restart();
	}
	
	direction += directionNoise;
	if (direction.x > 0) {
		inverted = true;
	}
	else if (direction.x < 0) {
		inverted = false;
	}
	float dirLen = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		direction /= dirLen;
		moveSprite(direction * getMovespeed());
	}
}

//obsluguje co sie dzieje gdy przeciwnik rodzaju Standard zderzy sie z graczem
void StandardEnemy::collided(Player& player) {
	if (getAttackTime() > getAttackSpeed()) {
		player.takeDamage(getDamage());
		resetAttackTimer();

		showEffect = true;
		effectTimer.restart();
		slashSprite.setPosition(getSpritePos());
		if (inverted == true) {
			slashSprite.setOrigin(sprite.getLocalBounds().width, 0.f);
			slashSprite.setScale(-1.f, 1.f);
		}
		else if (inverted == false) {
			slashSprite.setOrigin(0.f, 0.f);
			slashSprite.setScale(1.f, 1.f);
		}
	}
}

//renderowanie ataku przeciwnika rodzaju Standard
void StandardEnemy::renderEffect(RenderTarget* target) {
	if (showEffect) {
		if (effectTimer.getElapsedTime().asSeconds() < 0.3f) {
			target->draw(slashSprite);
		}
		else {
			showEffect = false;
		}
	}
}


//ustawienie podstawowych zmiennych przeciwnika rodzaju Tank
void TankEnemy::initVars() {
	setAttackSpeed(4.0f / modifier);
	setHP(int(float(200) * modifier));
	setMovespeed(1.f / modifier);
	setDamage(int(float(30) * modifier));
	if (rand() % 2 == 1) {
		AngleDir = true;
	}
	else {
		AngleDir = false;
	}
}

//konstruktor przeciwnika rodzaju Tank
TankEnemy::TankEnemy(Texture* tex, float m) : Enemy(tex, m) {
	initVars();
	slashTexture.loadFromFile("gfx/slash.png");
	slashSprite.setTexture(slashTexture);
	sf::Vector2u texSize = slashTexture.getSize();
	slashSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
}

//porusza przeciwnika rodzaju Tank
void TankEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float spread = 50.f;
	float RandDegr = (rand() % 10) + 1;
	if (AngleDir == 1) {
		RandDegr *= -1;
	}
	float rad = RandDegr * 3.14 / 180;
	float x = direction.x * cos(rad) - direction.y * sin(rad);
	float y = direction.x * sin(rad) + direction.y * cos(rad);
	direction = Vector2f(x, y);
	if (noiseTimer.getElapsedTime().asSeconds() > 1.f) {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-spread, spread);

		directionNoise = { dist(gen), dist(gen) };
		noiseTimer.restart();
	}

	direction += directionNoise;
	if (direction.x > 0) {
		inverted = true;
	}
	else if (direction.x < 0) {
		inverted = false;
	}
	float dirLen = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		direction = direction / dirLen;
		moveSprite(direction * getMovespeed());
	}
}

//obsluguje co sie dzieje gdy przeciwnik rodzaju Tank zderzy sie z graczem
void TankEnemy::collided(Player& player) {
	if (getAttackTime() > getAttackSpeed()) {
		player.takeDamage(getDamage());
		resetAttackTimer();

		showEffect = true;
		effectTimer.restart();
		slashSprite.setPosition(getSpritePos());

		if (inverted == true) {
			slashSprite.setOrigin(sprite.getLocalBounds().width, 0.f);
			slashSprite.setScale(-1.f, 1.f);
		}
		else if (inverted == false) {
			slashSprite.setOrigin(0.f, 0.f);
			slashSprite.setScale(1.f, 1.f);
		}
	}
}

//renderowanie ataku przeciwnika rodzaju Tank
void TankEnemy::renderEffect(RenderTarget* target) {
	if (showEffect) {
		if (effectTimer.getElapsedTime().asSeconds() < 0.3f) {
			target->draw(slashSprite);
		}
		else {
			showEffect = false;
		}
	}
}


//ustawienie podstawowych zmiennych przeciwnika rodzaju Range
void RangeEnemy::initVars() {
	setAttackSpeed(2.0f / modifier);
	setHP(int(float(75) * modifier));
	setMovespeed(2.75f / modifier);
	int r = rand() % 26;
	closest = 300.0f + r;
	setDamage(int(float(5) * modifier));
	setshootCooldown(2.f / modifier);
	if (rand() % 2 == 1) {
		AngleDir = true;
	}
	else {
		AngleDir = false;
	}
}

//konstruktor przeciwnika rodzaju Range
RangeEnemy::RangeEnemy(Texture* tex, float m) : Enemy(tex, m) {
	initVars();
	initBulletTexture(&bulletTexture);
}

//porusza przeciwnika rodzaju Range
void RangeEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float spread = 50.f;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-spread, spread);
	float RandDegr = (rand() % 10)+1;
	if (AngleDir == 1) {
		RandDegr *= -1;
	}
	float rad = RandDegr* 3.14 / 180;
	float x = direction.x * cos(rad) - direction.y * sin(rad);
	float y = direction.x * sin(rad) + direction.y * cos(rad);
	direction = Vector2f(x, y);
	Vector2f directionRand = direction;
	if (noiseTimer.getElapsedTime().asSeconds() > 1.f) {

	directionRand.x += dist(gen);
	directionRand.y += dist(gen);
	}

	if (direction.x > 0) {
		inverted = true;
	}
	else if (direction.x < 0) {
		inverted = false;
	}
	float dirLen = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		 direction /= dirLen;
		 directionRand /= dirLen;

		if (dirLen > closest) {
			moveSprite(directionRand * getMovespeed()); 
		}
		else if (dirLen < 220.0f) {
			moveSprite(direction * -getMovespeed() * 1.42f);
		}
		else {
			if (shootTimer.getElapsedTime().asSeconds() >= shootCooldown) {
				Bullet newBullet(bulletTexture, getSpritePos(), playerpos);
				bullets.push_back(newBullet);
				shootTimer.restart();
			}
		}
	}
}

void RangeEnemy::collided(Player& player) {}

//wczytanie tekstury pociskow przeciwnika rodzaju Range
void RangeEnemy::initBulletTexture(Texture* tex) {
	bulletTexture.loadFromFile("gfx/EnemyBullet.png");
}

//ruch pociskow przeciwnika rodzaju Range
void RangeEnemy::updateBullets() {
	for(size_t i = 0; i < bullets.size(); ) {
		bullets[i].update();

		if (bullets[i].getPos().x < 0 || bullets[i].getPos().x > 1280 ||
			bullets[i].getPos().y < 0 || bullets[i].getPos().y > 720) {
			bullets.erase(bullets.begin() + i);
		}
		else {
			i++;
		}
	}
}

//rysowanie pociskow przeciwnika rodzaju Range na ekran
void RangeEnemy::renderBullets(RenderTarget* target) {
	for (auto& bullet : bullets) {
		bullet.render(target);
	}
}

//zwraca pociski przeciwnika rodzaju Range
std::vector<Bullet>& RangeEnemy::getBullets() {
	return bullets;
}

//ustawia predkosc ataku przeciwnika rodzaju range
void RangeEnemy::setshootCooldown(float x) {
	shootCooldown = x;
}

void RangeEnemy::renderEffect(RenderTarget* target) {}