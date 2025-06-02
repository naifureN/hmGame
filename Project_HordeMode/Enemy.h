#pragma once
#include "includes.h"
using namespace sf;
class Enemy
{
	Sprite sprite;
	float movespeed;
	int hp;
	int maxHp;
	RectangleShape hpBarBackground;
	RectangleShape hpBarFill;

	//virtual void initVars() = 0;

public:
	Enemy(Texture* tex);
	~Enemy();
	//virtual void update(sf::RenderTarget* target) = 0;
	void render(sf::RenderTarget* target);

	void takeDamage(int damage);
	bool isDead() const;
	const FloatRect getBounds() const;

	void updateHpBar();
	

};

