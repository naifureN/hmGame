#include "Enemy.h"
using namespace sf;

void Enemy::render(sf::RenderTarget* target) {
	target->draw(this->sprite);
}
Enemy::Enemy(Texture* tex) {
	this->sprite.setTexture(*tex);
	this->sprite.setPosition(100, 200);
}
Enemy::~Enemy(){}