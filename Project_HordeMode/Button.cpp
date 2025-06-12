#include "Button.h"

// --- KONSTRUKTOR I DESTRUKTOR ---
Button::Button(float x, float y, float width, float height, const string& buttonText) {
    initFonts();

    
    shape.setSize(sf::Vector2f(width, height));

    shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);
    shape.setPosition(x, y);        

    shape.setFillColor(Color::Transparent);

    initText(buttonText);           
}


Button::~Button() {}

// --- INICJALIZACJA FONTÓW I TEKSTU ---
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
	// Zmienia tekst i dostosowuje rozmiar przycisku do jego treœci
	this->text.setString(buttonText);
	auto bounds = text.getLocalBounds();

	float paddingX = 20.f;
	float paddingY = 10.f;

	// Aktualizacja rozmiaru i pozycji kszta³tu oraz tekstu
	
	shape.setSize({ bounds.width + paddingX * 2, bounds.height + paddingY * 2 });

	
	shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);

	
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	text.setPosition(shape.getPosition());   
}

// --- AKTUALIZACJA STANU PRZYCISKU ---
// Sprawdza, czy kursor znajduje siê nad przyciskiem i czy zosta³ klikniêty

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
// --- RYSOWANIE PRZYCISKU --
void Button::render(sf::RenderTarget* target) {
	target->draw(shape);
	target->draw(text);
}
// --- SPRAWDZENIE KLIKNIÊCIA ---
// Zwraca true, jeœli przycisk zosta³ klikniêty w tej klatce
bool Button::isClicked() const {
    return  isHovered && !prevFramePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left); 
}


string Button::getText() const {
	// Zwraca aktualny tekst przycisku jako zwyk³y string
	return this->text.getString().toAnsiString();//Ma zostac toANsi bo sie zepsuje
}
