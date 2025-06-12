#pragma once
#include "includes.h"
#include "Player.h"
#include"Bullet.h"
using namespace sf;
class Enemy {
	float movespeed;

	int hp;
	int maxHp;
	RectangleShape hpBarBackground;
	RectangleShape hpBarFill;

	int damage;

	float attackSpeed;
	Clock attackTimer;
protected:
	Sprite sprite;
	bool inverted = false;
	bool AngleDir;
	float modifier = 1.f;

public:
	Enemy(Texture* tex, float m);
	virtual ~Enemy();

	Clock noiseTimer;
	Vector2f directionNoise;

	void update(Vector2f playerpos, Sprite playerSprite, Player& player);
	void render(sf::RenderTarget* target);
	void takeDamage(int damage);
	bool isDead() const;
	void updateHpBar();
	void moveSprite(Vector2f movement);
	bool checkCollision(const sf::Sprite& otherSprite);
	void resetAttackTimer();
	void EnemyPushBack(const Vector2f& pushVec);
	const Vector2f& getEnemyPosition() const;


	// virtualki
	virtual void moveEnemy(Vector2f playerpos) = 0;
	virtual void initVars() = 0;
	virtual void collided(Player& player) = 0;
	virtual void renderEffect(RenderTarget* target) = 0;

	//gettery i settery
	void setHP(int x);
	void setMovespeed(float x);
	float getMovespeed();
	const FloatRect getBounds() const;
	void setAttackSpeed(float x);
	float getAttackSpeed();
	Vector2f getSpritePos();
	float getAttackTime();
	void setDamage(int x);
	int getDamage() const;
	void setModifier(float x);
	float getModifier();
};

class StandardEnemy : public Enemy {
	Texture slashTexture;
	Sprite slashSprite;
	Clock effectTimer;
	bool showEffect = false;
public:
	StandardEnemy(Texture* tex, float m);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player& player);
	void renderEffect(RenderTarget* target);
};

class TankEnemy : public Enemy {
	Texture slashTexture;
	Sprite slashSprite;
	Clock effectTimer;
	bool showEffect = false;
public:
	TankEnemy(Texture* tex, float m);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player& player);
	void renderEffect(RenderTarget* target);
};

class RangeEnemy : public Enemy {
	float closest;
	Clock shootTimer;
	float shootCooldown;
	std::vector<Bullet>bullets;
	Texture bulletTexture;

	void initBulletTexture(Texture* tex);
public:
	RangeEnemy(Texture* tex, float m);
	void initVars();
	void moveEnemy(Vector2f playerpos);
	void collided(Player& player);

	void updateBullets();
	void renderBullets(RenderTarget* target);

	void setshootCooldown(float x);

	std::vector<Bullet>& getBullets();
	void renderEffect(RenderTarget* target);
};

