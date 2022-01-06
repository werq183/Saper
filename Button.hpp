#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>

#include "Clickable.hpp"

class Button : public Clickable, public sf::Drawable {
public:
	Button(sf::String, sf::Font& font, double x, double y, double width, double height,
	       std::function<void(Application*)> onClick);
	bool checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const override;
	void parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) override;
	std::function<void(Application*)> onClick;
private:
	double button_x, button_y, button_width, button_height;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::RectangleShape rectangle;
	sf::Text text;
	sf::Font font;
};
