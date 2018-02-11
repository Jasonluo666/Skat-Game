#include "global.h"
#include "game.h"

using namespace mcts_skat;

int main() {
	//  [0]:Declarer [1]:Opponent -> Manual, Standard, Greedy, MonteCarlo, Learning
	string playerType[2] = { "Learning", "Standard" };

	// prepare to load Keras (Python function)
	if (playerType[0] == "Learning" || playerType[1] == "Learning") {
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('C:/Users/ljsPC/Desktop/NNW_Player/')");
	}

	int declarerWinCount = 0, totalCount = 20;
	float result = 0;

	History NNWdata[10];	// training data
	fstream Training_Data;
	Training_Data.open("C://Users/ljsPC/Desktop/training_data.txt", ios::app);

	if (!Training_Data.is_open()) {
		std::cout << "cannot open training data file.." << endl;

		std::system("pause");
		return 0;
	}

	for (int matchCounter = 0; matchCounter < totalCount; matchCounter++) {
		bool isDeclarerWin = skatGame(playerType, NNWdata);
		// record the Declarer data
		if (isDeclarerWin) {
			declarerWinCount++;

			std::cout << "generate training data.." << endl;
			// generate training data
			for (int turn = 0; turn < 10; turn++) {
				// cards agent holds
				for (int suit = 0; suit < 4; suit++) {
					for (int card = 0; card < 8; card++) {
						Training_Data << NNWdata[turn].cards[suit][card] << " ";
					}
				}

				// cards other two players hold
				for (int suit = 0; suit < 4; suit++) {
					for (int card = 0; card < 8; card++) {
						Training_Data << !NNWdata[turn].gameState[suit][card] << " ";
					}
				}

				// cards that have been played in this turn
				for (int suit = 0; suit < 4; suit++) {
					for (int card = 0; card < 8; card++) {
						if ((suit == NNWdata[turn].currentState[0] / 10 && card == NNWdata[turn].currentState[0] % 10)
							|| (suit == NNWdata[turn].currentState[1] / 10 && card == NNWdata[turn].currentState[1] % 10))
							Training_Data << 1 << " ";
						else
							Training_Data << 0 << " ";
					}
				}

				// action agent takes
				for (int suit = 0; suit < 4; suit++) {
					for (int card = 0; card < 8; card++) {
						if (suit == NNWdata[turn].action / 10 && card == NNWdata[turn].action % 10)
							Training_Data << 1 << " ";
						else
							Training_Data << 0 << " ";
					}
				}

				Training_Data << endl;
			}
			std::cout << "generate training data.. finish" << endl;
		}
	}
	
	result = (float)declarerWinCount / totalCount;
	std::cout << "Result: " << declarerWinCount << "/" << totalCount << " = " << result << endl;
	// msa::LoopTimer::test(10000);

	// close Keras (Python function)
	if (playerType[0] == "Learning" || playerType[1] == "Learning")
		Py_Finalize();

	Training_Data.close();
	std::system("pause");
	return 0;
}