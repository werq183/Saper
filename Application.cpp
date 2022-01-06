#include "Application.hpp"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "Button.hpp"

// 😻💣
Application::Application()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"Neko Saper"), game(*this), minesweeperBot(this),
	  tutorial(this) {
	window.setFramerateLimit(60);

	if (!this->font.loadFromFile("RobotoMono-VariableFont_wght.ttf")) {
		std::cout << "Blad ladowania fonta.\n";
	}

	auto easyButtonCallback = [](Application* app){
		app->setState(AppState::Game);
		std::cout << "New state: GAME\n";
		app->game.reset(10, 10, {});
	};

	auto mediumButtonCallback = [](Application* app) {
		app->setState(AppState::Game);
		std::cout << "New state: GAME\n";
		app->game.reset(16, 16, {});
	};

	auto hardButtonCallback = [](Application* app) {
		app->setState(AppState::Game);
		std::cout << "New state: GAME\n";
		app->game.reset(32, 16, {});
	};

	auto customButtonCallback = [](Application* app) {
		app->setState(AppState::Game);
		std::cout << "New state: GAME\n";
		app->game.reset(10, 15, {});
	};

	auto tutorialButtonCallback = [](Application* app){
		app->setState(AppState::Tutorial);
		std::cout << "New state: TUTORIAL\n";
		app->game.reset(10, 15, {11, 24, 44});
		app->tutorial.start();
		//app->minesweeperBot.start();
	};

	std::vector<Button> menuButtons;
	menuButtons.emplace_back(L"Początkujący", this->font, (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 200, BUTTON_WIDTH, 50,
	                         easyButtonCallback);
	menuButtons.emplace_back("Zaawansowany", this->font, (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 300, BUTTON_WIDTH, 50,
							mediumButtonCallback);
	menuButtons.emplace_back("Ekspert", this->font, (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 400, BUTTON_WIDTH, 50,
							hardButtonCallback);
	menuButtons.emplace_back("Niestandardowy", this->font, (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 500, BUTTON_WIDTH, 50,
							customButtonCallback);
	menuButtons.emplace_back("Samouczek", this->font, (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 600, BUTTON_WIDTH, 50,
	                         tutorialButtonCallback);

	//testy start
	//startButtonCallback(this);
	//testy koniec

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			} else if (event.type == sf::Event::MouseButtonReleased) {
				switch (state) {
					case AppState::Menu: {
						for (auto&& button : menuButtons) {
							button.parseClick(event.mouseButton, this);
						}
						break;
					}
					case AppState::Tutorial:
					case AppState::Game: {
						game.parseClick(event.mouseButton, this);
						break;
					}
					default: {}
				}
			}
		}
		game.update();
		if (state == AppState::Tutorial) {
			tutorial.update();
			//minesweeperBot.update();
		}

		window.clear();
		switch (state) {
			case AppState::Menu: {
				for (auto&& button : menuButtons) {
					window.draw(button);
				}
				break;
			}
			case AppState::Tutorial:
				window.draw(tutorial);
			case AppState::Game: {
				window.draw(game);
				break;
			}
		}
		window.display();
	}
}

void Application::setState(const AppState newState) {
	this->state = newState;
}

void Application::setWindowSize(const uint32_t width, const uint32_t height) {
	window.setSize(sf::Vector2u(width, height));
	const sf::FloatRect visibleArea(0, 0, width, height);
	window.setView(sf::View(visibleArea));
}

AppState Application::getState() const {
	return state;
}

sf::Font& Application::getFont() {
	return font;
}
