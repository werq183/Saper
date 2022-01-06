#include "Field.hpp"
#include <iostream>
#include <stack>

#include "Board.hpp"
#include "Game.hpp"

bool Field::texturesLoaded = false;
sf::Texture Field::fieldTexture;
sf::Texture Field::fieldTextureMarked;
sf::Texture Field::fieldTextureMine;
std::array<sf::Texture, 9> Field::fieldTexturesUncovered;


Field::Field(Board& board, int x, int y) : board(board), coordinates(x, y) {
	if (!texturesLoaded) {
		texturesLoaded = loadTextures();
		std::cout << "ladowanie tekstur: " << texturesLoaded << "\n";
	}
	rectangle.setSize(sf::Vector2f(fieldSize, fieldSize));
	refreshTexture();
}

bool Field::loadTextures() {
	using namespace std::string_literals;
	if (!fieldTexture.loadFromFile("textures/square.png"))
		return false;

	if (!fieldTextureMarked.loadFromFile("textures/flag.png"))
		return false;

	if (!fieldTextureMine.loadFromFile("textures/bomb.png"))
		return false;

	for (int i = 0; i < 9; i++) {
		if (!fieldTexturesUncovered[i].loadFromFile("textures/"s + std::to_string(i) + ".png"s))
			return false;
	}

	return true;
}

void Field::plantMine() {
	this->hasMine = true;
	const auto neighborFields = getAllNeighborCoords(true);
	for (auto& coords : neighborFields) {
		auto& neigh = board.getField(coords);
		neigh.setNeighbors(neigh.neighbors + 1);
	}
}

void Field::refreshTexture() {
	if (highlight) {
		rectangle.setOutlineColor(sf::Color(255, 255, 0));
		rectangle.setOutlineThickness(2);
	} else {
		rectangle.setOutlineThickness(0);
	}

	if (isFieldDiscovered) {
		if (hasMine) {
			rectangle.setFillColor(sf::Color(255, 0, 0));
			rectangle.setTexture(&fieldTextureMine, true);
		} else {
			rectangle.setTexture(&fieldTexturesUncovered[neighbors], true);
			//rectangle.setScale(10, 10);
		}
	} else {
		//rectangle.setFillColor(sf::Color(255, 255, 255));
		if (board.game->getState() == GameState::Lost && hasMine) {
			rectangle.setTexture(&fieldTextureMine, true);
		} else if (isFieldMarked) {
			rectangle.setTexture(&fieldTextureMarked, true);
		} else {
			rectangle.setTexture(&fieldTexture, true);
		}
	}
}

void Field::toggleMark() {
	if (board.game->getState() == GameState::Ready) {
		board.game->setState(GameState::Playing);
	}

	isFieldMarked = !isFieldMarked;
	refreshTexture();
}

std::vector<sf::Vector2i> Field::getAllNeighborCoords(bool diagonal = false) const {
	std::vector<sf::Vector2i> neighborFields;
	auto up = coordinates;
	auto down = coordinates;
	auto left = coordinates;
	auto right = coordinates;
	if (--up.y >= 0) { neighborFields.emplace_back(up); }
	if (++down.y < board.getSizeY()) { neighborFields.emplace_back(down); }
	if (--left.x >= 0) { neighborFields.emplace_back(left); }
	if (++right.x < board.getSizeX()) { neighborFields.emplace_back(right); }

	if (diagonal) {
		auto up_left = coordinates;
		auto up_right = coordinates;
		auto down_left = coordinates;
		auto down_right = coordinates;
		if (--up_left.y >= 0 && --up_left.x >= 0) { neighborFields.emplace_back(up_left); }
		if (--up_right.y >= 0 && ++up_right.x < board.getSizeX()) { neighborFields.emplace_back(up_right); }
		if (++down_left.y < board.getSizeY() && --down_left.x >= 0) { neighborFields.emplace_back(down_left); }
		if (++down_right.y < board.getSizeY() && ++down_right.x < board.getSizeX()) {
			neighborFields.emplace_back(down_right);
		}
	}

	return neighborFields;
}

void Field::discover() {
	if (board.game->getState() == GameState::Ready) {
		board.game->setState(GameState::Playing);
	}

	if (hasMine) {
		board.game->finishGame(false);
		isFieldDiscovered = true;
		refreshTexture();
		return;
	}

	sf::String str;
	// algorytm flood fill
	std::stack<sf::Vector2i> stack;
	stack.push(coordinates);

	while (!stack.empty()) {
		// bierzemy kolejne pole ze stosu
		sf::Vector2i c = stack.top();
		stack.pop();
		auto& f = board.getField(c);

		// jeśli nie było jeszcze odkryte, to się nim zajmujemy...
		if (!f.isFieldDiscovered) {
			// odkrywamy je
			f.isFieldDiscovered = true;
			f.refreshTexture();
			board.emptyFieldsLeft--;
			//std::cout << "left: " << board.emptyFieldsLeft << "\n";

			// jeśli też ma 0 min w sąsiedztwie, to dodajemy sąsiadów na stos
			if (f.neighbors == 0) {
				const auto neighborFields = f.getAllNeighborCoords(true);
				for (auto& coords : neighborFields) {
					stack.push(coords);
				}
			}
		}
	}

	if (board.emptyFieldsLeft == 0) {
		board.game->finishGame(true);
	}
}

void Field::setNeighbors(int n) {
	neighbors = n;
	refreshTexture();
}

void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(rectangle, states);
}

bool Field::checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const {
	return true;
}

void Field::parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) {
	// nie sprawdzamy czy kliknięcie jest na tym polu, bo zostało to już wyliczone w Board::parseClick
	if (isFieldDiscovered) {
		std::cout << "Pole bylo juz odkryte\n";
		return;
	}

	switch (mouseButtonEvent.button) {
		case sf::Mouse::Left: {
			if (isFieldMarked) {
				std::cout << "To pole jest oznaczone i nie mozna go odkryc\n";
				return;
			}
			discover();
			break;
		}
		case sf::Mouse::Right: {
			toggleMark();
			break;
		}
		default: {}
	}
}

bool Field::isMined() const {
	return hasMine;
}

bool Field::isDiscovered() const {
	return isFieldDiscovered;
}

bool Field::isMarked() const {
	return isFieldMarked;
}

void Field::setHighlight(bool highlightState) {
	highlight = highlightState;
	refreshTexture();
}
