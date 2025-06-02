#include "Enemy.h"
using namespace sf;

void Enemy::render(sf::RenderTarget* target) {
	target->draw(this->sprite);
	target->draw(this->hpBarBackground);
	target->draw(this->hpBarFill);
}
Enemy::Enemy(Texture* tex): hp(100), maxHp(100) {
	this->sprite.setTexture(*tex);
	this->sprite.setPosition(100, 200);
	hpBarBackground.setPosition(100, 190);
	hpBarFill.setPosition(100,190); // x te same a y-10 jest ideolo

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
