#include "Bullet.h"

Bullet::Bullet() {
	this->initVars();
	this->initShape();
}

Bullet::~Bullet() {

}

void Bullet::initVars() {
	this->speed = 10.f;
}

void Bullet::initShape() {
	this->texture.loadFromFile("gfx/bullet.png");
	this->sprite.setTexture(this->texture);
}

void Bullet::render(RenderTarget* target) {
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