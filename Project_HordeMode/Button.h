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
	

	bool isPressed;
	bool isHovered;

public:
	Button(float x, float y, float width, float height, const string& buttonText = "");
	virtual ~Button();
	
	//Init
	void initFonts();
	void initText(const string& buttonText = "");
	virtual void setText(const string& buttonText = "");


	//Functions
	void update(Vector2f& mousePos);
	
	void render(sf::RenderTarget* target);
	bool isClicked() const;
	const string& getText() const;
};

class RestartButton : public Button{
//public:
//	RestartButton(float x, float y, float width, float height);
//	
//	void setText();

};

