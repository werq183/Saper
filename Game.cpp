#include "Game.hpp"
#include <iostream>

#include "Application.hpp"
using namespace std::chrono;

Game::Game(Application& app) : app(app), board(this) {
	auto menuButtonCallback = [](Application* app){
		app->setState(AppState::Menu);
		app->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		std::cout << "New state: MENU\n";
	};

	menuButtons.emplace_back("Menu", app.getFont(), 10, 10, 90, 50, menuButtonCallback);

	auto color2 = sf::Color(230, 230, 230);
	text.setFont(app.getFont());
	text.setString("GAME");
	text.setCharacterSize(24);
	text.setFillColor(color2);
	text.setPosition(90 + 10 * 3, 15);
}

void Game::reset(int boardSizeX, int boardSizeY, std::vector<int> mineIndices = {}) {
	state = GameState::Ready;
	std::cout << "Nowa gra. rozmiar=(" << boardSizeX << ", " << boardSizeY << ")\n";
	if (mineIndices.empty()) {
		const int bombCount = boardSizeX * boardSizeY * 0.25;
		board.reset(boardSizeX, boardSizeY, bombCount, mineIndices);
	} else {
		board.reset(boardSizeX, boardSizeY, mineIndices.size(), mineIndices);
	}
	board.setPosition(0, 100);
	auto boardSize = board.getSize();
	int tutorialSpace = 0; // dodatkowa przestrzeń na poradnik

	if (app.getState() == AppState::Tutorial) {
		app.tutorial.setPosition(0, boardSize.y + 100);
		tutorialSpace = 200;
	}

	app.setWindowSize(boardSize.x, boardSize.y + 100 + tutorialSpace);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(board, states);
	target.draw(text, states);
	for (auto&& button : menuButtons) {
		target.draw(button, states);
	}
}

bool Game::checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const {
	return true;
}

void Game::parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) {
	if (state == GameState::Ready || state == GameState::Playing) {
		board.parseClick(mouseButtonEvent, application);
	}
	for (auto&& button : menuButtons) {
		button.parseClick(mouseButtonEvent, application);
	}
}

void Game::finishGame(bool win) {
	if (win) {
		std::cout << "Wygrana!!\n";
		setState(GameState::Won);
	} else {
		std::cout << "KABOOM!! Saper myli sie tylko raz.\n";
		setState(GameState::Lost);
		board.showEverything();
	}
}

void Game::setState(const GameState newState) {
	if (state == GameState::Ready && newState == GameState::Playing) {
		// rozpoczęcie gry
		startTime = high_resolution_clock::now();
	}
	state = newState;
	update();
}

GameState Game::getState() const {
	return state;
}

sf::String Game::getGameTimeString() const {
	sf::String str;
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	auto seconds = milliseconds / 1000;
	milliseconds = milliseconds % 1000;
	auto minutes = seconds / 60;
	seconds = seconds % 60;

	if (minutes.count() < 10) str += '0';
	str += std::to_string(minutes.count()) + ":";
	if (seconds.count() < 10) str += '0';
	str += std::to_string(seconds.count()) + ".";
	if (milliseconds.count() < 100) str += '0';
	if (milliseconds.count() < 10) str += '0';
	str += std::to_string(milliseconds.count());

	return str;
}

void Game::update() {
	sf::String str;
	switch (state) {
		case GameState::Ready:
			str = L"Kliknij dowolne pole\nby zacząć grę";
			break;
		case GameState::Playing: {
			endTime = high_resolution_clock::now();
			str = "Czas: " + getGameTimeString();
			break;
		}
		case GameState::Won: {
			str = "Czas: " + getGameTimeString() + "\nWygrana!!";
			break;
		}
		case GameState::Lost: {
			str = "Czas: " + getGameTimeString() + L"\nSaper myli się tylko raz.";
			break;
		}

	}
	text.setString(str);
}
