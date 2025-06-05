#pragma once
#include "includes.h"
#include "Player.h"
using namespace sf;
class Enemy {
	Sprite sprite;
	float movespeed;

	int hp;
	int maxHp;
	RectangleShape hpBarBackground;
	RectangleShape hpBarFill;

private:
	float attackSpeed;
	Clock attackTimer;

public:
	Enemy(Texture* tex);
	virtual ~Enemy(); //nie przesuwaæ do virtualek bo coœ siê zjebie


	void update(Vector2f playerpos, Sprite playerSprite, Player player);
	void render(sf::RenderTarget* target);
	void takeDamage(int damage);
	bool isDead() const;
	void updateHpBar();
	void moveSprite(Vector2f movement);
	bool checkCollision(const sf::Sprite& otherSprite);
	void resetAttackTimer();

	// virtualki
	virtual void moveEnemy(Vector2f playerpos) = 0;
	virtual void initVars() = 0;
	virtual void collided(Player player) = 0;

	//gettery i settery
	void setHP(int x);
	void setMovespeed(float x);
	float getMovespeed();
	const FloatRect getBounds() const;
	void setAttackSpeed(float x);
	float getAttackSpeed();
	void setSpriteColor(Color c);
	Vector2f getSpritePos();
	float getAttackTime();
};

class StandardEnemy : public Enemy {
public:
	StandardEnemy(Texture* tex);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player player);
};

class TankEnemy : public Enemy {
public:
	TankEnemy(Texture* tex);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player player);
};

class RangeEnemy : public Enemy {
	float closest;
public:
	RangeEnemy(Texture* tex);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player player);
};

