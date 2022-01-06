#pragma once
#include <SFML/Graphics/Font.hpp>

#include "Game.hpp"
#include "MinesweeperBot.hpp"
#include "Tutorial.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int BUTTON_WIDTH = 250;

enum class AppState {
	Menu,
	Game,
	Tutorial
};

class Application {
	sf::Font font;
	AppState state = AppState::Menu;
	sf::RenderWindow window;
public:
	Application();
	void setState(AppState newState);
	void setWindowSize(uint32_t width, uint32_t height);
	AppState getState() const;
	Game game;
	MinesweeperBot minesweeperBot;
	Tutorial tutorial;
	sf::Font& getFont();
};
