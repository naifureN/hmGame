#include "Bullet.h"

Bullet::Bullet(const Texture& bulletTexture, Vector2f playerPos, Vector2f mousepos): damage(25){
	this->sprite.setTexture(bulletTexture);
	this->initVars();
	this->setPosition(playerPos);
	this->setTarget(playerPos, mousepos);
}

Bullet::~Bullet() {
	if (sprite.getTexture() == nullptr) {
		std::cout << "Destruktor Bullet: sprite bez tekstury" << std::endl;
	}
}

void Bullet::initVars() {
	this->speed = 10.f;
}


void Bullet::render(RenderTarget* target) {
	if (sprite.getTexture() == nullptr) {
		std::cout << "WARNING: sprite ma nullptr tekstury w destrukcji!" << std::endl;
		return;
	}
	target->draw(this->sprite);
}

void Bullet::update() {
	sprite.move(velocity);

}

void Bullet::setTarget(const Vector2f& playerpos, const Vector2f& mousepos) {
	Vector2f direction = mousepos - playerpos;
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		direction /= length;
	}

	velocity = direction * speed;

}

void Bullet::setPosition(const Vector2f& pos) {
	sprite.setPosition(pos);
}

const Vector2f& Bullet::getPos() const {
	return sprite.getPosition();
}

FloatRect Bullet::getGlobalBounds() const {
	return sprite.getGlobalBounds();
}

int Bullet::getDamage() const {
	return damage;
}