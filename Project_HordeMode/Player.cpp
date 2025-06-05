#include "Player.h"
#include <iostream>
Font Player::font;

void Player::initVars() {
	this->movespeed = 5.f;
	this->direction = Vector2f(0, 0);
}

void Player::initShape() {
	this->texture.loadFromFile("gfx/player.png");
	this->shape.setTexture(this->texture);
}

void Player::initHpBar() {

	playerhpBarBackground.setSize(sf::Vector2f(200.f, 20.f));
	playerhpBarBackground.setFillColor(sf::Color(50, 50, 50));
	playerhpBarBackground.setOutlineThickness(2.f);
	playerhpBarBackground.setOutlineColor(sf::Color::White);

	playerhpBarFill.setSize(sf::Vector2f(200.f, 20.f));
	playerhpBarFill.setFillColor(sf::Color::Green);

	font.loadFromFile("gfx/Symtext.ttf");

	hpText.setFont(font);
	hpText.setCharacterSize(18);
	hpText.setString("Piwo");
}


Player::Player(float x, float y) : hp(100), maxHp(100) {
	this->shape.setPosition(x, y);
	this->initVars();
	this->initShape();
	this->initHpBar();
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

void Player::update(sf::RenderTarget* target) {
	this->updateInput();
	this->updateWindowBoundsCollision(target);
	this->updateHpbar();
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

void Player::updateHpbar() {
	float hpPercent = static_cast<float>(hp) / maxHp;
	hpPercent = std::clamp(hpPercent, 0.f, 1.f);


	playerhpBarFill.setSize(sf::Vector2f(200.f * hpPercent, 20.f));
	hpText.setString(std::to_string(hp) + "/" + std::to_string(maxHp));

	if (hpPercent < 0.25f) {
		playerhpBarFill.setFillColor(sf::Color::Red);
	}
	else if (hpPercent < 0.6f) {
		playerhpBarFill.setFillColor(sf::Color::Yellow);
	}
	else {
		playerhpBarFill.setFillColor(sf::Color::Green);
	}
}

void Player::renderHpBar(sf::RenderTarget* target) {

	sf::Vector2u windowSize = target->getSize();
	float posX = windowSize.x - playerhpBarBackground.getSize().x - 20.f;
	float posY = windowSize.y - playerhpBarBackground.getSize().y - 32.f;

	playerhpBarBackground.setPosition(posX, posY);
	playerhpBarFill.setPosition(posX, posY);

	float posXtext = playerhpBarBackground.getPosition().x + playerhpBarBackground.getSize().x / 2 - hpText.getLocalBounds().width / 2;
	float posYtext = playerhpBarBackground.getPosition().y + 22;

	hpText.setPosition(posXtext, posYtext);

	target->draw(playerhpBarBackground);
	target->draw(playerhpBarFill);
	target->draw(this->hpText);
}

void Player::takeDamage(int damage) {
	hp -= damage;
	hp = std::max(0, hp); // Zabezpieczenie przed ujemnym HP
	updateHpbar(); // Aktualizuj wyglÄ…d paska
}

const Sprite& Player::getSprite() const {
	return shape;
}