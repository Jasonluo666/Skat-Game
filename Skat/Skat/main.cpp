#include "global.h"
#include "game.h"
#include "MCTS/mctsTest.h"

using namespace mcts_skat;

int main() {
	cout << "hello world" << endl;

	//  [0]:Declarer [1]:Opponent -> Manual, Standard, Greedy, MonteCarlo, Learning
	string playerType[2] = { "Greedy", "MonteCarlo" };

	int declarerWinCount = 0, totalCount = 20;
	float result = 0;
	for (int matchCounter = 0; matchCounter < totalCount; matchCounter++)
		declarerWinCount += skatGame(playerType);
	
	result = (float)declarerWinCount / totalCount;
	cout << "Result: " << declarerWinCount << "/" << totalCount << " = " << result << endl;
	// msa::LoopTimer::test(10000);

	system("pause");

	return 0;
}