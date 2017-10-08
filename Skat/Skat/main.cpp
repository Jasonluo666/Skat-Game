#include "global.h"
#include "game.h"

int main() {
	cout << "hello world" << endl;
	// humanPlayer = false, automatically play
	bool humanPlayer = false;
	int playerNo = 1;

	skatGame(humanPlayer, playerNo);

	system("pause");

	return 0;
}