#pragma once
#include "includes.h"
using namespace sf;
class Enemy
{
	Sprite sprite;
	float movespeed;

	//virtual void initVars() = 0;
	//virtual void initShape() = 0;
public:
	Enemy(Texture* tex);
	~Enemy();
	//virtual void update(sf::RenderTarget* target) = 0;
	void render(sf::RenderTarget* target);
};

