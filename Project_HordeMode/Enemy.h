#pragma once
#include"Header.h"
#include"Player.h"
class Enemy
{
	sf::RectangleShape shape;

	float movespeed;

	void initVars();
	void initShape();
public:
	Enemy();
	~Enemy();

	void update(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
};

