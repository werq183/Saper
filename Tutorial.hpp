#pragma once
#include <iostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

class Application;

class Tutorial : public sf::Drawable, public sf::Transformable {
	Application* app;
	int tutorialStep;

	sf::Text text;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	explicit Tutorial(Application* app);

	void start();
	void update();

};
