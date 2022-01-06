#pragma once
#include <SFML/Window/Event.hpp>

class Application;

class Clickable {
public:
	virtual bool checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const = 0;
	virtual void parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) = 0;
};
