#pragma once
#include "includes.h"
#include <math.h>

class Player
{
private:
	Texture texture;
	Sprite shape;
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
public:
	Player(float x = 960.f, float y = 540.f);
	~Player();

	void updateInput();
	void updateWindowBoundsCollision(sf::RenderTarget* target);
	void update(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
	Vector2f normalize_vector(float vecx, float vecy);
	const Vector2f& getPos() const;
	void renderHpBar(sf::RenderTarget* target);
	void takeDamage(int damage);
	const Sprite& getSprite() const;
};