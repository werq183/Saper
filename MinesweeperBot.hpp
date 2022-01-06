#pragma once
#include <chrono>

class Application;

class MinesweeperBot {
	Application* app;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::milliseconds stepTime;
	int lastStep;
	bool running = false;
public:
	explicit MinesweeperBot(Application* app);
	void start();
	void makeMove();
	void update();
};
