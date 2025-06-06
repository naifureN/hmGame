#include "Button.h"

Button::Button(float x, float y, float width, float height, const string& buttonText)
{
	initFonts();
	

	this->shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(Color::White);
	
	initText(buttonText);
}

Button::~Button()
{
}

void Button::initFonts()
{
	this->font.loadFromFile("Fonts/Symtext.ttf");
}

void Button::initText(const string& buttonText)
{
	this->text.setFont(this->font);
	this->text.setCharacterSize(60);
	this->text.setFillColor(Color::Red);
	
	this->setText(buttonText);
}

void Button::setText(const string& buttonText)
{
	this->text.setString(buttonText);
	this->text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	this->text.setPosition(
		shape.getPosition().x + shape.getSize().x / 2.0f,
		shape.getPosition().y + shape.getSize().y / 2.0f);

}

void Button::update(Vector2f& mousePos)
{
	this->isHovered = this->shape.getGlobalBounds().contains(mousePos);
	this->isPressed = false;

	if (isHovered) {
		shape.setFillColor(Color::Yellow);
		if (Mouse::isButtonPressed(Mouse::Left)) {
			shape.setFillColor(Color::Green);
			this->isPressed = true;
		}
	}
	else {
		shape.setFillColor(Color::White);
	}
}

void Button::render(sf::RenderTarget* target)
{
	target->draw(shape);
	target->draw(text);
}

bool Button::isClicked() const
{
	return this->isPressed && this->isHovered;
}

const string& Button::getText() const
{
	return this->text.getString();
}


//RestartButton::RestartButton(float x, float y, float width, float height):Button(x,y,width,height)
//{
//	setText();
//}
//
//void RestartButton::setText()
//{
//	this->text.setString("RESTART");
//	this->text.setPosition(Vector2f(400.f, 260.f));
//}
