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

void Enemy::update() {
	//this->sprite.move(Vector2f(3, 0));
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

void Enemy::setSpriteColor() {
	sprite.setColor(Color::Red);
}

void StandardEnemy::initVars() {
	setAttackSpeed(2.1);
	setHP(100);
}

StandardEnemy::StandardEnemy(Texture* tex) : Enemy(tex){
	initVars();
}

void TankEnemy::initVars() {
	setAttackSpeed(4.0);
	setHP(200);
}

TankEnemy::TankEnemy(Texture* tex) : Enemy(tex) {
	initVars();
	setSpriteColor();
}
