#pragma once
#include "includes.h"
#include <math.h>

class Player
{
private:
	Texture texture;
	float movespeed;
	Vector2f direction;
	int hp;
	int maxHp;
	RectangleShape playerhpBarBackground;
	RectangleShape playerhpBarFill;
	static Font font;
	Text hpText;

	void initVars();
	void initShape();
	void initHpBar();
	void updateHpbar();
protected:
	bool inverted = false;
	Sprite shape;
public:
	Player(float x = 640.f, float y = 320.f);
	~Player();

	void updateInput();
	void updateWindowBoundsCollision(sf::RenderTarget* target);
	void update(sf::RenderTarget* target, Vector2i mousepos);
	void render(sf::RenderTarget* target);
	Vector2f normalize_vector(float vecx, float vecy);
	const Vector2f& getPos() const;
	void renderHpBar(sf::RenderTarget* target);
	void takeDamage(int damage);
	const Sprite& getSprite() const;

	FloatRect getPlayerBounds() const;
	float getSpeed() const;
	Vector2f getDirection() const;

	void PushBack();
};