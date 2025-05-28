#pragma once
#include"includes.h"
class Bullet {
	Texture* texture;
	Sprite sprite;
	float speed;
	Vector2f velocity;

	void initVars();

public:
	Bullet(Texture* bulletTexture, Vector2f playerPos, Vector2f mousepos);
	~Bullet();

	void setPosition(const Vector2f& pos);
	void setTarget(const Vector2f& playerpos, const Vector2f& mousepos);
	const Vector2f& getPos() const;
	FloatRect getGlobalBounds() const;

	void update();
	void render(RenderTarget* target);

};

