#include "global.h"
#include "game.h"

using namespace mcts_skat;

int game(string playerType[2], int totalCount, bool is_generate_data, int turn_start_at) {
	// prepare to load Keras (Python function)
	if (playerType[0] == "Learning" || playerType[1] == "Learning") {
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('C:/Users/ljsPC/Desktop/NNW_Player/')");
	}

	int declarerWinCount = 0;
	

	History NNWdata[10];	// training data
	fstream Training_Data[10];

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
			bool isDeclarerWin = skatGame(playerType, NNWdata, turn_start_at);
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

		// msa::LoopTimer::test(10000);

		// close Keras (Python function)
		if (playerType[0] == "Learning" || playerType[1] == "Learning")
			Py_Finalize();

		for (int i = 0; i < 10; i++)
			Training_Data[i].close();

		// update the log
		fstream log_update("C:/Users/ljsPC/Desktop/NNW_Training/train_log.txt");	// read
		int count = 0;
		if (log_update.is_open()) {
			log_update >> count;
			count += declarerWinCount;

			log_update.close();
		}
		log_update.open("C:/Users/ljsPC/Desktop/NNW_Training/train_log.txt");	// write
		if (log_update.is_open()) {
			log_update << count;

			log_update.close();
		}
	}
	else {
		for (int matchCounter = 0; matchCounter < totalCount; matchCounter++) {
			bool isDeclarerWin = skatGame(playerType, NNWdata, turn_start_at);
			// record the Declarer data
			if (isDeclarerWin)
				declarerWinCount++;
		}

	}

	return declarerWinCount;
}

int main() {
	//  [0]:Declarer [1]:Opponent -> Manual, Standard, Random, Greedy, MonteCarlo, Learning
	string playerType[2];
	int totalCount = 50;
	int turn_start_at = 7;
	// generate training data
	bool is_generate_data = false;
	int declarer_win_count = 0, opponent_win_count = 0;

	playerType[0] = "Learning", playerType[1] = "MonteCarlo";
	declarer_win_count = game(playerType, totalCount, is_generate_data, turn_start_at);
	playerType[0] = "MonteCarlo", playerType[1] = "Learning";
	opponent_win_count = totalCount - game(playerType, totalCount, is_generate_data, turn_start_at);

	std::cout << "declarer_win_count Result: " << declarer_win_count << "/" << totalCount << endl;
	std::cout << "opponent_win_count Result: " << opponent_win_count << "/" << totalCount << endl;

	// update the log
	fstream data_update("C:/Users/ljsPC/Desktop/data.txt");	// read
	int count_declarer = 0, count_opponent = 0, count_turn = 0;
	if (data_update.is_open()) {
		data_update >> count_turn;
		data_update >> count_declarer;
		data_update >> count_opponent;
		count_turn += totalCount;
		count_declarer += declarer_win_count;
		count_opponent += opponent_win_count;

		data_update.close();
	}
	data_update.open("C:/Users/ljsPC/Desktop/data.txt");	// write
	if (data_update.is_open()) {
		data_update << count_turn << " ";
		data_update << count_declarer << " ";
		data_update << count_opponent;

		data_update.close();
	}

	// system("pause");
	return 0;
}