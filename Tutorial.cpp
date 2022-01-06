#include "Tutorial.hpp"
#include "Application.hpp"

Tutorial::Tutorial(Application* app) : app(app), tutorialStep(0) {

	auto color2 = sf::Color(230, 230, 230);
	text.setFont(app->getFont());
	text.setString("GAME");
	text.setCharacterSize(20);
	text.setFillColor(color2);
	text.setPosition(10, 10);
	text.setString("test 1");
}

void Tutorial::start() {
	tutorialStep = 0;
}

void Tutorial::update() {
	switch (tutorialStep) {
		case 0:
			text.setString(L"Na start kliknij dowolne pole,\nnp. to podœwietlone.");
			app->game.board.setHighlight({7, 1}, true); // puste pole
			if (app->game.board.getField({7, 1}).isDiscovered()) {
				tutorialStep++;
			}
			break;
		case 1:
			app->game.board.setHighlight({7, 1}, false);

			text.setString(
				L"Jeœli widzisz jedynkê która po skosie\nma nieodkryte pole (jedno!), to znaczy,\n¿e jest tam bomba. Oznacz to pole PPM.");
			app->game.board.setHighlight({5, 1}, true); // jedynka
			app->game.board.setHighlight({4, 2}, true); // bomba
			if (app->game.board.getField({4, 2}).isMarked()) {
				tutorialStep++;
			}

			break;
		case 2:
			app->game.board.setHighlight({5, 1}, false); // jedynka
			app->game.board.setHighlight({4, 2}, false); // bomba
			text.setString(L"Powodzenia!");

			break;
	}
}

void Tutorial::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(text, states);
}
