﻿#include "global.h"
#include "game.h"

int main() {
	cout << "hello world" << endl;

	//  Manual, Standard, Greedy, MonteCarlo, Learning
	string playerType[3] = { "Standard", "Greedy", "Greedy" };

	// humanPlayer = false, automatically play

	skatGame(playerType);

	system("pause");

	return 0;
}