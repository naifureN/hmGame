#include "Button.h"

Button::Button(float x, float y, float width, float height, const string& buttonText) {
    initFonts();

    
    shape.setSize(sf::Vector2f(width, height));

    shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);
    shape.setPosition(x, y);        

    shape.setFillColor(Color::Transparent);

    initText(buttonText);           
}


Button::~Button() {}

void Button::initFonts() {
	this->font.loadFromFile("Fonts/Symtext.ttf");
}

void Button::initText(const string& buttonText) {
	this->text.setFont(this->font);
	this->text.setCharacterSize(60);
	this->text.setFillColor(Color::Red);
	this->setText(buttonText);
}

void Button::setText(const string& buttonText) {
	this->text.setString(buttonText);
	auto bounds = text.getLocalBounds();

	float paddingX = 20.f;
	float paddingY = 10.f;

	
	shape.setSize({ bounds.width + paddingX * 2, bounds.height + paddingY * 2 });

	
	shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);

	
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	text.setPosition(shape.getPosition());   
}



void Button::update(Vector2f& mousePos, bool mouseLeftPressedLastFrame) {
    this->isHovered = this->shape.getGlobalBounds().contains(mousePos);
  

    if (isHovered) {
        shape.setFillColor(Color(102, 99, 99));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            shape.setFillColor(Color(158, 157, 157));
      
        }
    }
    else {
        shape.setFillColor(Color::Transparent);
    }

	this->prevFramePressed = mouseLeftPressedLastFrame;
}

void Button::render(sf::RenderTarget* target) {
	target->draw(shape);
	target->draw(text);
}

bool Button::isClicked() const {
    return  isHovered && !prevFramePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left); 
}


string Button::getText() const {
	return this->text.getString().toAnsiString();//Ma zostac toANsi bo sie zjebie inaczej
}
