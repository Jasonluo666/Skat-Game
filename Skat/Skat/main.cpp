#include "global.h"
#include "game.h"
#include "MCTS/mctsTest.h"

using namespace mcts_skat;

int main() {
	cout << "hello world" << endl;

	//  Manual, Standard, Greedy, MonteCarlo, Learning
	string playerType[3] = { "Standard", "MonteCarlo", "Greedy" };

	skatGame(playerType);
	
	// msa::LoopTimer::test(10000);

	system("pause");

	return 0;
}