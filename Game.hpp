#pragma once
#include <chrono>

#include "Board.hpp"
#include <SFML/Graphics.hpp>

#include "Button.hpp"
#include "Clickable.hpp"

enum class GameState {
	Ready,
	Playing,
	Won,
	Lost
};

class Game : public Clickable, public sf::Drawable {
	Application& app;
	Board board;
	GameState state = GameState::Ready;
	std::vector<Button> menuButtons;
	sf::Text text;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> endTime;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	explicit Game(Application& app);
	void reset(int boardSizeX, int boardSizeY, std::vector<int> mineIndices);
	bool checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const override;
	void parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) override;
	void finishGame(bool win);
	void setState(GameState newState);
	GameState getState() const;

	sf::String getGameTimeString() const;
	void update();
	friend class MinesweeperBot;
	friend class Tutorial;
};
