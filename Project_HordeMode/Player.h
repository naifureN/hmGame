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

	void initVars();
	void initShape();
public:
	Player(float x = 960.f, float y = 540.f);
	~Player();

	void updateInput();
	void updateWindowBoundsCollision(sf::RenderTarget* target);
	void update(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
	Vector2f normalize_vector(float vecx, float vecy);
};