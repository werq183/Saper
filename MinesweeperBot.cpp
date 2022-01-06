#include "MinesweeperBot.hpp"

#include <iostream>
#include <random>

#include "Application.hpp"

using namespace std::chrono;
using namespace std::literals;

MinesweeperBot::MinesweeperBot(Application* app): app(app), stepTime(1500ms), lastStep(0) { }

void MinesweeperBot::start() {
	startTime = high_resolution_clock::now();
	this->running = true;
}

void MinesweeperBot::update() {
	if (app->game.getState() == GameState::Lost
		|| app->game.getState() == GameState::Won) {
		return;
	}

	auto endTime = high_resolution_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	int currentStep = milliseconds.count() / stepTime.count();
	if (currentStep <= lastStep) {
		return;
	}

	this->lastStep++;
	//std::cout << "lastStep: " << lastStep << "; " << milliseconds << "; " << stepTime << "\n";

	makeMove();

}

void MinesweeperBot::makeMove() {
	int N = app->game.board.sizeX;
	int M = app->game.board.sizeY;

	auto&& fields = app->game.board.fields;
	std::vector<Field*> unknownFields;
	for (auto& field : fields) {
		if (!field.isDiscovered()) {
			unknownFields.push_back(&field);
		}
	}

	if (unknownFields.size() == fields.size()) {
		std::cout << "Plansza jest nowa, klikam cokolwiek\n";
		unknownFields[0]->discover();
	}

	// czy pole o takich wspó³rzêdnych nale¿y do planszy
	auto isValid = [&](int x, int y){
		return (x >= 0 && y >= 0 && x < N && y < M);
	};

	// TODO: tu mo¿na napisaæ algorytm znajduj¹cy najbezpieczniejsze pola do klikniêcia

	// gdy ju¿ nie ma ¿adnych bezpiecznych pól, to losujemy
	if (!unknownFields.empty()) {
		std::cout << "Nie wiem co robic, klikam cokolwiek\n";

		// Generowanie liczb losowych: https://stackoverflow.com/a/13445752
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> generator(0, unknownFields.size() - 1);

		int index = generator(rng);
		unknownFields[index]->discover();
	}

}
