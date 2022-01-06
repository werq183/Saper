#include "Button.hpp"

Button::Button(sf::String string, sf::Font& font, double x, double y, double width, double height,
               std::function<void(Application*)> onClick)
	: onClick(onClick), button_x(x), button_y(y), button_width(width), button_height(height), font(font) {
	auto color1 = sf::Color(30, 30, 30);
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setOutlineColor(color1);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(x, y);
	rectangle.setFillColor(sf::Color(70, 70, 70));

	auto color2 = sf::Color(230, 230, 230);
	text.setFont(font);
	text.setString(string);
	text.setCharacterSize(24);
	text.setFillColor(color2);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	text.setPosition(x + 10, y + 10);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rectangle, states);
	target.draw(text, states);
}

bool Button::checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const {
	auto&& x = mouseButtonEvent.x;
	auto&& y = mouseButtonEvent.y;
	if (x > button_x && x < button_x + button_width && y > button_y && y < button_y + button_height) {
		return true;
	}
	return false;
}

void Button::parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) {
	if (mouseButtonEvent.button != sf::Mouse::Left) return;

	if (checkClick(mouseButtonEvent)) {
		onClick(application);
	}
}
