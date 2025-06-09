#pragma once
#include "includes.h"
using namespace sf;
using namespace std;

class Button
{
protected:
	RectangleShape shape;
	Font font;
	Text text;

	bool isHovered;
	bool prevFramePressed{ false };

public:
	Button(float x, float y, float width, float height, const string& buttonText = "");
	virtual ~Button();

	void initFonts();
	void initText(const string& buttonText = "");
	virtual void setText(const string& buttonText = "");

	void update(Vector2f& mousePos, bool mouseLeftPressedLastFrame);
	void render(sf::RenderTarget* target);

	bool isClicked() const;
	string getText() const;
};
