#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "Clickable.hpp"

class Board;
const int fieldSize = 40;

class Field : public Clickable, public sf::Drawable, public sf::Transformable {
	Board& board;
	bool hasMine = false;
	bool isFieldMarked = false;
	bool isFieldDiscovered = false;
	bool highlight = false;
	int neighbors = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static sf::Texture fieldTexture;
	static sf::Texture fieldTextureMarked;
	static sf::Texture fieldTextureMine;
	static std::array<sf::Texture, 9> fieldTexturesUncovered;
	static bool texturesLoaded;
	sf::RectangleShape rectangle;
	sf::Vector2i coordinates;
public:
	Field(Board& board, int x, int y);
	bool loadTextures();
	void plantMine();
	void refreshTexture();
	void toggleMark();
	std::vector<sf::Vector2i> getAllNeighborCoords(bool diagonal) const;
	void discover();
	void setNeighbors(int n);
	bool checkClick(const sf::Event::MouseButtonEvent& mouseButtonEvent) const override;
	void parseClick(const sf::Event::MouseButtonEvent& mouseButtonEvent, Application* application) override;
	bool isMined() const;
	bool isDiscovered() const;
	bool isMarked() const;
	void setHighlight(bool highlightState);

	friend class MinesweeperBot;
};
