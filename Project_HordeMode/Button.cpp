#include "Button.h"

Button::Button(float x, float y, float width, float height, const string& buttonText)
{
    initFonts();

    // chwilowy rozmiar, zaraz i tak go dopasujemy w setText()
    shape.setSize(sf::Vector2f(width, height));

    // ORIGIN = œrodek
    shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);
    shape.setPosition(x, y);        // <- x,y to TERAZ ŒRODEK

    shape.setFillColor(Color::Transparent);

    initText(buttonText);           // tu dopasujemy finalny rozmiar
}


Button::~Button() {}

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
	auto bounds = text.getLocalBounds();

	float paddingX = 20.f;
	float paddingY = 10.f;

	// dopasuj rozmiar hitboxu
	shape.setSize({ bounds.width + paddingX * 2, bounds.height + paddingY * 2 });

	// uaktualnij origin na œrodek nowego kszta³tu
	shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);

	// wycentruj tekst wewn¹trz prostok¹ta
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	text.setPosition(shape.getPosition());   // bo shape.pos to ju¿ œrodek
}



void Button::update(Vector2f& mousePos, bool mouseLeftPressedLastFrame)
{
	this->isHovered = this->shape.getGlobalBounds().contains(mousePos);
	this->isPressed = false;

	if (isHovered) {
		shape.setFillColor(Color(102, 99, 99));
		if (Mouse::isButtonPressed(Mouse::Left)) {
			shape.setFillColor(Color(158, 157, 157));
			this->isPressed = true;
		}
	}
	else {
		shape.setFillColor(Color::Black);
	}

	
}

void Button::render(sf::RenderTarget* target)
{
	target->draw(shape);
	target->draw(text);
}

bool Button::isClicked() const
{
	return this->isHovered && !wasPressedLastFrame && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

string Button::getText() const
{
	return this->text.getString().toAnsiString();
}
