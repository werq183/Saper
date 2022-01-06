#include "Board.hpp"

#include <iostream>
#include <random>

Board::Board(Game* game) : game(game) {
	rectangle.setFillColor(sf::Color(128, 128, 128));
}

void Board::reset(int boardSizeX, int boardSizeY, int bombCount, std::vector<int> mineIndices) {
	sizeX = boardSizeX;
	sizeY = boardSizeY;
	this->bombCount = bombCount;
	this->emptyFieldsLeft = sizeX * sizeY - bombCount;

	rectangle.setSize(sf::Vector2f(sizeX * stepSize - fieldsSpacing + 2 * fieldsOffset,
	                               sizeY * stepSize - fieldsSpacing + 2 * fieldsOffset));

	fields.clear();

	for (int i = 0; i < sizeX * sizeY; i++) {
		int x = i % sizeX;
		int y = i / sizeX;
		auto& field = fields.emplace_back(*this, x, y);
		field.setPosition(fieldsOffset + x * stepSize,
		                  fieldsOffset + y * stepSize);
	}

	if (mineIndices.empty()) {
		plantRandomMines(bombCount);
	} else {
		plantMines(mineIndices);
	}
}

size_t Board::getFieldCount() const {
	return fields.size();
}

void Board::plantMines(std::vector<int> mineIndices) {
	for (auto& index : mineIndices) {
		fields[index].plantMine();
	}
}

void Board::plantRandomMines(int count) {
	// Generowanie liczb losowych: https://stackoverflow.com/a/13445752
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> generator(0, getFieldCount() - 1);

	int bombsToPlant = count;
	while (bombsToPlant > 0) {
		const int index = generator(rng);
		//std::cout << "na polu" << index << " jest mina\n";
		if (fields[index].isMined()) continue;
		bombsToPlant--;
		fields[index].plantMine();
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(rectangle, states);

	for (auto&& field : fields) {
		target.draw(field, states);
	}
}

bool Board::checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const {
	auto&& x = mouseButtonEvent.x;
	auto&& y = mouseButtonEvent.y;
	auto pos = this->getPosition();
	sf::FloatRect boardBounds(pos.x, pos.y, rectangle.getSize().x, rectangle.getSize().y);

	if (boardBounds.contains(x, y)) {
		return true;
	}
	return false;
}

Field& Board::getField(const sf::Vector2i& fieldCoords) {
	return fields[fieldCoords.x + fieldCoords.y * sizeX];
}

void Board::parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) {
	if (!checkClick(mouseButtonEvent)) {
		return;
	}

	switch (mouseButtonEvent.button) {
		case sf::Mouse::XButton1: {
			std::cout << "CLICK: XButton1\n";
			for (auto& f : fields) {
				if (!f.isMined() && !f.isDiscovered()) {
					f.discover();
				}
			}
			return;
		}
		case sf::Mouse::XButton2: {
			std::cout << "CLICK: XButton2\n";
			for (auto& f : fields) {
				if (f.isMined()) {
					f.toggleMark();
				}
			}
			return;
		}
		default: {}
	}

	auto&& pos = this->getPosition();
	auto&& x = mouseButtonEvent.x - pos.x;
	auto&& y = mouseButtonEvent.y - pos.y;

	auto getFieldByCoords = [&](int x, int y){
		int x1 = x - fieldsOffset;
		int y1 = y - fieldsOffset;

		int x2 = x1 / stepSize;
		int y2 = y1 / stepSize;

		if (x1 < 0 || x1 % stepSize > fieldSize
			|| y1 < 0 || y1 % stepSize > fieldSize
			|| x2 >= sizeX || y2 >= sizeY) {
			return sf::Vector2i(-1, -1);
		}

		return sf::Vector2i(x2, y2);
	};

	auto fieldCoords = getFieldByCoords(x, y);
	if (fieldCoords.x < 0 || fieldCoords.y < 0) return;

	// std::cout << "Board: field clicked! " << fieldCoords.x << ", " << fieldCoords.y << "\n";
	auto& field = getField(fieldCoords);
	field.parseClick(mouseButtonEvent, application);
}

sf::Vector2f Board::getSize() const {
	return rectangle.getSize();
}

uint64_t Board::getSizeX() const {
	return sizeX;
}

uint64_t Board::getSizeY() const {
	return sizeY;
}

void Board::showEverything() {
	for (auto& f : fields) {
		if (f.isMined() && !f.isDiscovered()) {
			f.refreshTexture();
		}
	}
}

void Board::setHighlight(const sf::Vector2i& fieldCoords, bool highlightState) {
	getField(fieldCoords).setHighlight(highlightState);
}
