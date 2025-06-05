#include "Enemy.h"
using namespace sf;


void Enemy::render(sf::RenderTarget* target) {
	target->draw(this->sprite);
	target->draw(this->hpBarBackground);
	target->draw(this->hpBarFill);
}
Enemy::Enemy(Texture* tex): hp(100), maxHp(100) {
	this->sprite.setTexture(*tex);
	int rx = rand() % 500 + 40;
	this->sprite.setPosition(Vector2f(rx, 100));
	hpBarBackground.setPosition(sprite.getPosition());
	hpBarFill.setPosition(sprite.getPosition()); // x te same a y-10 jest ideolo

	hpBarBackground.setSize(Vector2f(32.f, 5.f));
	hpBarBackground.setFillColor(Color(50, 50, 50));

	hpBarFill.setSize(Vector2f(32.f, 5.f));
	hpBarFill.setFillColor(Color(0, 255, 0));

	updateHpBar();
}
Enemy::~Enemy(){}

bool Enemy::isDead() const {
	return hp <= 0;
}

const FloatRect Enemy::getBounds() const {
	return sprite.getGlobalBounds();
}

void Enemy::takeDamage(int damage) {
	hp -= damage;
	if (hp < 0) {
		hp=0;
	}

	updateHpBar();
}

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

void Enemy::update(Vector2f playerpos, Sprite playerSprite, Player player) {
	if (checkCollision(playerSprite)) {
		collided(player);
	}
	else {
		moveEnemy(playerpos);
	}
	hpBarBackground.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y-10);
	hpBarFill.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 10);
}
void Enemy::setAttackSpeed(float x) {
	attackSpeed = x;
}
float Enemy::getAttackSpeed() {
	return attackSpeed;
}

void Enemy::setHP(int x) {
	hp = x;
	maxHp = x;
}

void Enemy::setSpriteColor(Color c) {
	sprite.setColor(c);
}

Vector2f Enemy::getSpritePos() {
	return sprite.getPosition();
}

void Enemy::setMovespeed(float x) {
	movespeed = x;
}

void Enemy::moveSprite(Vector2f movement) {
	sprite.move(movement);
}

float Enemy::getMovespeed() {
	return movespeed;
}

bool Enemy::checkCollision(const sf::Sprite& otherSprite) {
	return sprite.getGlobalBounds().intersects(otherSprite.getGlobalBounds());
}

float Enemy::getAttackTime() {
	return attackTimer.getElapsedTime().asMilliseconds() / 1000;
}

void Enemy::resetAttackTimer() {
	attackTimer.restart();
}

void StandardEnemy::initVars() {
	setAttackSpeed(2.5f);
	setHP(100);
	setMovespeed(3.0f);
}

StandardEnemy::StandardEnemy(Texture* tex) : Enemy(tex){
	initVars();
}

void StandardEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float dirLen = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		direction = direction / dirLen;
		moveSprite(direction * getMovespeed());
	}
}

void StandardEnemy::collided(Player player) {
	if (getAttackTime() > getAttackSpeed()) {
		//player.dealDamage()
		std::cout << "hit! " << std::endl;
		resetAttackTimer();
	}
}

void TankEnemy::initVars() {
	setAttackSpeed(4.0f);
	setHP(200);
	setMovespeed(1.5f);
}

TankEnemy::TankEnemy(Texture* tex) : Enemy(tex) {
	initVars();
	setSpriteColor(Color::Red);
}

void TankEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float dirLen = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		direction = direction / dirLen;
		moveSprite(direction * getMovespeed());
	}
}

void TankEnemy::collided(Player player) {
	if (getAttackTime() > getAttackSpeed()) {
		//player.dealDamage()
		std::cout << "hit! " << std::endl;
		resetAttackTimer();
	}
}

void RangeEnemy::initVars() {
	setAttackSpeed(2.0f);
	setHP(75);
	setMovespeed(2.75f);
	int r = rand() % 26;
	closest = 300.0f + r;
}

RangeEnemy::RangeEnemy(Texture* tex) : Enemy(tex) {
	initVars();
	setSpriteColor(Color::Yellow);
}

void RangeEnemy::moveEnemy(Vector2f playerpos) {
	Vector2f direction = playerpos - getSpritePos();
	float dirLen = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (dirLen > 0.0f) {
		direction = direction / dirLen;
		
		if (dirLen > closest) {
			moveSprite(direction * getMovespeed());
		}
		else if (dirLen < 220.0f) {
			moveSprite(direction * -getMovespeed() * 1.42f);
		}
	}
	
}

void RangeEnemy::collided(Player player) {}
