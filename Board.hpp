#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Clickable.hpp"
#include "Field.hpp"

class Game;

class Board : public Clickable, public sf::Drawable, public sf::Transformable {
	Game* game;
	uint64_t sizeX, sizeY;
	std::vector<Field> fields;
	uint64_t bombCount;
	uint64_t emptyFieldsLeft;
	sf::RectangleShape rectangle;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Board(Game* game);
	size_t getFieldCount() const;
	void plantMines(std::vector<int> mineIndices);
	void plantRandomMines(int count);
	void reset(int boardSizeX, int boardSizeY, int bombCount, std::vector<int> mineIndices);
	bool checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const override;
	Field& getField(const sf::Vector2i& fieldCoords);
	void parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) override;
	sf::Vector2f getSize() const; // rozmiar w pikselach
	uint64_t getSizeX() const; // kolumny
	uint64_t getSizeY() const; // wiersze
	void showEverything();
	void setHighlight(const sf::Vector2i&, bool highlightState);

	static constexpr int fieldsOffset = 20;
	static constexpr int fieldsSpacing = 5;
	static constexpr int stepSize = fieldSize + fieldsSpacing;
	friend class Field;
	friend class MinesweeperBot;
	friend class Tutorial;
};
