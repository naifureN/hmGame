#include "Player.h"
#include <iostream>
void Player::initVars(){
	this->movespeed = 5.f;
	this->direction = Vector2f(0, 0);
}

void Player::initShape() {
	this->texture.loadFromFile("gfx/player.png");
	this->shape.setTexture(this->texture);
}

Player::Player(float x, float y) {
	this->shape.setPosition(x, y);
	this->initVars();
	this->initShape();
}

Player::~Player() {

}

void Player::updateInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) and not (sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
		this->direction.x = -1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) and not (sf::Keyboard::isKeyPressed(sf::Keyboard::A))) {
		this->direction.x = 1.f;
	}
	else {
		this->direction.x = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) and not (sf::Keyboard::isKeyPressed(sf::Keyboard::S))) {
		this->direction.y = -1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and not (sf::Keyboard::isKeyPressed(sf::Keyboard::W))) {
		this->direction.y = 1.f;
	}
	else {
		this->direction.y = 0.f;

	}
	this->direction = normalize_vector(this->direction.x, this->direction.y);
	shape.setPosition(operator+(shape.getPosition(), operator*(this->direction, this->movespeed)));
}

void Player::updateWindowBoundsCollision(sf::RenderTarget* target) {

	sf::FloatRect playerBounds = this->shape.getGlobalBounds();

	if (playerBounds.left <= 1.f) {
		this->shape.setPosition(0.f, this->shape.getPosition().y);
	}

	if (playerBounds.left + playerBounds.width >= target->getSize().x) {
		this->shape.setPosition(target->getSize().x - playerBounds.width, this->shape.getPosition().y);
	}

	if (playerBounds.top <= 0.f) {
		this->shape.setPosition(this->shape.getPosition().x, 0.f);
	}

	if (playerBounds.top + playerBounds.height >= target->getSize().y) {
		this->shape.setPosition(this->shape.getPosition().x, target->getSize().y - playerBounds.height);
	}


}
//losowy komentarz ¿eby by³y zmiany
void Player::update(sf::RenderTarget* target) {
	this->updateInput();
	this->updateWindowBoundsCollision(target);
}

void Player::render(sf::RenderTarget* target) {
	target->draw(this->shape);
}

Vector2f Player::normalize_vector(float vecx, float vecy) {
	float len = sqrtf(vecx * vecx + vecy * vecy);
	if (len > 0) {
		vecx = vecx / len;
		vecy = vecy / len;
	}
	return Vector2f(vecx, vecy);
}

const Vector2f& Player::getPos() const {
	return shape.getPosition();
}
const Sprite& Player::getSprite() const {
	return shape;
}