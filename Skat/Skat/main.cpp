#include "global.h"
#include "game.h"

using namespace mcts_skat;

int main() {
	//  [0]:Declarer [1]:Opponent -> Manual, Standard, Random, Greedy, MonteCarlo, Learning
	string playerType[2] = { "MonteCarlo", "MonteCarlo" };

	// prepare to load Keras (Python function)
	if (playerType[0] == "Learning" || playerType[1] == "Learning") {
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('C:/Users/ljsPC/Desktop/NNW_Player/')");
	}

	int declarerWinCount = 0, totalCount = 1000;
	float result = 0;

	History NNWdata[10];	// training data
	fstream Training_Data[10];

	// generate training data
	bool is_generate_data = true;
	if (is_generate_data) {
		string dir = "C://Users/ljsPC/Desktop/NNW_Training/training_data";

		for (int i = 0; i < 10; i++) {
			Training_Data[i].open(dir + char(i + '0') + ".txt", ios::app);

			if (!Training_Data[i].is_open()) {
				std::cout << "cannot open training data file.." << endl;

				std::system("pause");
				return 0;
			}
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
							Training_Data[turn] << NNWdata[turn].cards[suit][card] << " ";
						}
					}

					// cards other two players hold
					for (int suit = 0; suit < 4; suit++) {
						for (int card = 0; card < 8; card++) {
							Training_Data[turn] << !NNWdata[turn].gameState[suit][card] << " ";
						}
					}

					// cards that have been played in this turn
					for (int suit = 0; suit < 4; suit++) {
						for (int card = 0; card < 8; card++) {
							if ((suit == NNWdata[turn].currentState[0] / 10 && card == NNWdata[turn].currentState[0] % 10)
								|| (suit == NNWdata[turn].currentState[1] / 10 && card == NNWdata[turn].currentState[1] % 10))
								Training_Data[turn] << 1 << " ";
							else
								Training_Data[turn] << 0 << " ";
						}
					}

					// action agent takes
					for (int suit = 0; suit < 4; suit++) {
						for (int card = 0; card < 8; card++) {
							if (suit == NNWdata[turn].action / 10 && card == NNWdata[turn].action % 10)
								Training_Data[turn] << 1 << " ";
							else
								Training_Data[turn] << 0 << " ";
						}
					}

					Training_Data[turn] << endl;
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

		for (int i = 0; i < 10; i++)
			Training_Data[i].close();
	}
	else {
		for (int matchCounter = 0; matchCounter < totalCount; matchCounter++) {
			bool isDeclarerWin = skatGame(playerType, NNWdata);
			// record the Declarer data
			if (isDeclarerWin)
				declarerWinCount++;
		}

		result = (float)declarerWinCount / totalCount;
		std::cout << "Result: " << declarerWinCount << "/" << totalCount << " = " << result << endl;
	}

	std::system("pause");
	return 0;
}