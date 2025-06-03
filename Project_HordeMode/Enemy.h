#pragma once
#include "includes.h"
using namespace sf;
class Enemy {
	Sprite sprite;
	float movespeed;
	int hp;
	int maxHp;
	RectangleShape hpBarBackground;
	RectangleShape hpBarFill;
	float attackSpeed;
	Clock attackTimer;

public:
	Enemy(Texture* tex);
	virtual ~Enemy();
	void update();
	void render(sf::RenderTarget* target);
	void takeDamage(int damage);
	bool isDead() const;
	virtual void initVars() = 0;
	const FloatRect getBounds() const;
	void setAttackSpeed(float x);
	float getAttackSpeed();
	void updateHpBar();
	void setHP(int x);
	void setSpriteColor();
};

class StandardEnemy : public Enemy {
public:
	StandardEnemy(Texture* tex);
	void initVars();
};

class TankEnemy : public Enemy {
public:
	TankEnemy(Texture* tex);
	void initVars();
};
