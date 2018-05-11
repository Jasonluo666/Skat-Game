#include "player.h"

Player::Player(int playerNo) {
	this->playerNo = playerNo;
	numberAll = number[0] = number[1] = number[2] = number[3] = 0;
	winValue[0] = winValue[1] = winValue[2] = 0;
	minGameValue = 18;
	winCardsNumber = 0;
	Manual = Standard = Random = Greedy = MonteCarlo = Learning = false;
	pModule = NULL;
	pFunc = NULL;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++) {
			cards[i][j] = false;
			gameState[i][j] = false;
		}

	for (int i = 0; i < 32; i++)
		winCards[i] = Empty;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			player_not_have[i][j] = false;
}
// find the Matadors
int Player::findGameLevel() {
	int gameLevel = 1;

	if (cards[0][0] == true) {	// with Jack of Club
		if (cards[1][0] == true) {
			gameLevel++;
			if (cards[2][0] == true) {
				gameLevel++;
				if (cards[3][0] == true) {
					gameLevel++;
				}
			}
		}
	}
	else {					// without Jack of Club
		if (cards[1][0] == false) {
			gameLevel++;
			if (cards[2][0] == false) {
				gameLevel++;
				if (cards[3][0] == false) {
					gameLevel++;
				}
			}
		}
	}

	return gameLevel;
}

//sort the card by value
void Player::sortCard() {
	int gameLevel = findGameLevel();

	// largest value one could bid
	if (number[0] > 0)
		maxGameValue = Base_Value_Club * (gameLevel + 1);	// with gameLevel, play gameLevel + 1
	else if (number[1] > 0)
		maxGameValue = Base_Value_Spade * (gameLevel + 1);
	else if (number[2] > 0)
		maxGameValue = Base_Value_Heart * (gameLevel + 1);
	else if (number[3] > 0)
		maxGameValue = Base_Value_Diamond * (gameLevel + 1);

	cout << maxGameValue << endl;
}
// collect the cards
void Player::getCard(int card) {
	int suit = card / 10;
	int value = card % 10;
	// get the card
	cards[suit][value] = true;
	number[suit]++;
	numberAll++;
	// delete the card one has
	gameState[suit][value] = true;

	cout << "player " << playerNo << " got " << card << endl;

	if (numberAll == 10 || numberAll == 12)
		this->sortCard();
}
// human player
void Player::setPlayerType(string playerType, int turn_start_at) {
	this->turn_start_at = turn_start_at;
	if (playerType == "Manual")
		Manual = true;
	else if (playerType == "Standard")
		Standard = true;
	else if (playerType == "Random")
		Random = true;
	else if (playerType == "Greedy")
		Greedy = true;
	else if (playerType == "MonteCarlo")
		MonteCarlo = true;
	else if (playerType == "Learning")
		Learning = true;
	else
		cout << "error player type" << endl;
}

void Player::manualPlayer(int* currentState, int playSequence) {
	int card = Empty;
	cout << "------------------" << "human player" << playerNo << "------------------" << endl;
	for (int priorPlayer = 0; priorPlayer < playerNo; priorPlayer++)
		cout << "Player " << priorPlayer << " plays " << identifyCard(currentState[priorPlayer]).c_str() << endl;
	cout << "------------------your cards------------------" << endl;
	for (int suit = 0; suit < 4; suit++) {
		cout << identifySuit(suit).c_str() << ": ";
		for (int num = 0; num < 8; num++)
			if(cards[suit][num] = true)
			cout << identifyCard(suit * 10 + num).c_str() << " ";
		cout << endl;
	}
	cout << "------------------your turn------------------" << endl;
	cout << "please input an two-degit number ([Club->0, Spade->1, Heart->2, Diamond->3][J->7 : 0->7]):" << endl;
	for (bool isValid = false; !isValid; ) {
		cin >> card;
		if (cards[card / 10][card % 10] == false)
			cout << "you do not have this card." << endl;
		else if (currentState[0] != Empty) {
			int suit;
			if (currentState[0] % 10 == 0)
				suit = trump;
			else
				suit = currentState[0] / 10;

			if (card % 10 == 0 || card / 10 == trump || card / 10 == suit || number[suit] == 0) {
				cout << "You play " << identifySuit(card / 10).c_str() << identifyValue(cards[card / 10][card % 10]).c_str() << endl;
				isValid = true;
			}
			else
				cout << "you cannot play " << identifySuit(card / 10).c_str() << identifyValue(cards[card / 10][card % 10]).c_str() << " because of the wrong suit." << endl;
		}
		else {
			cout << "You play " << identifySuit(card / 10).c_str() << identifyValue(cards[card / 10][card % 10]).c_str() << endl;
			isValid = true;
		}
	}
	cout << "------------------------------------------" << endl;
	currentState[playSequence] = play(card / 10, card % 10);
}

void Player::standardPlayer(int* currentState, int playSequence) {
	int suit;
	currentState[playSequence] = Empty;

	switch (playSequence) {
		case 0: {
			// forehand (F)
			// first player plays the largest card
			for (int suit = 0; suit < 4; suit++)
				if (cards[suit][0] == true) {
					currentState[playSequence] = play(suit, 0);
					break;
				}

			if (currentState[playSequence] == Empty)		// not Jack found
				if (number[trump] > 0) {
					for (int num = 0; num < 8; num++)
						if (cards[trump][num] == true) {
							currentState[playSequence] = play(trump, num);
							break;
						}
				}
				else {
					int position, maxValue = INF;
					for (int suit = 0; suit < 4; suit++) {
						if (number[suit] == 0)
							continue;
						for (int num = 0; num < 8; num++) {
							if (cards[suit][num] == true) {
								if (maxValue > num) {
									maxValue = num;
									position = suit;
								}
								break;
							}
						}

					}
					currentState[playSequence] = play(position, maxValue);
				}
			break;
		}
		case 1: {		// middlehand (M)
			int lowestCard;
			//if (playerNo == whosDeclarer || whosDeclarer == 0) {		// play the largest card if could win, otherwise play the smallest one
			if (currentState[0] % 10 == 0)		// Jack belongs to trump
				suit = trump;
			else
				suit = currentState[0] / 10;


			if (currentState[0] % 10 == 0) {	// player 1 played Jack
				for (int i = 0; i < currentState[0] / 10; i++)
					if (cards[i][0] == true) {
						currentState[playSequence] = play(i, 0);
						return;
					}
			}
			else if (number[suit] > 0 || number[trump] > 0) {		// player 1 didn't play Jack
				for (int suit = 0; suit < 4; suit++)		// play Jack
					if (cards[suit][0] == true) {
						currentState[playSequence] = play(suit, 0);
						return;
					}

				int findmax1 = 0, findmax2 = 0;
				for ( ; findmax1 < 8; findmax1++) {
					if (cards[suit][findmax1] == true)
						break;
				}
				for (; findmax2 < 8; findmax2++) {
					if (cards[trump][findmax2] == true)
						break;
				}

				if (suit == trump && suit * 10 + findmax1 < currentState[0]) {		// play trump
					for (int num = 0; num < 8; num++)
						if (cards[suit][num] == true) {
							currentState[playSequence] = play(suit, num);
							return;
						}
				}
				else if (suit != trump) {
					if (number[trump] > 0) {
						currentState[playSequence] = play(trump, findmax2);
						return;
					}
					else if (number[suit] > 0 && suit * 10 + findmax1< currentState[0]) {
						currentState[playSequence] = play(suit, findmax1);
						return;
					}
				}
			}
			// couldn't win this turn
			if (currentState[0] % 10 == 0)
				suit = trump;
			else
				suit = currentState[0] / 10;

			if (number[suit] > 0) {		// have current suit
				if (suit == trump)
					for (int num = 3; num >= 0; num--)
						if (cards[num][0] == true) {
							lowestCard = num * 10;
							break;
						}

				for (int num = 7; num >= 0; num--)
					if (cards[suit][num] == true) {
						lowestCard = suit * 10 + num;
						break;
					}
				currentState[playSequence] = play(lowestCard / 10, lowestCard % 10);
			}
			else {		// don't have current suit
				int minValue = Empty, position, findMin;
				for (int i = 0; i < 4; i++) {
					if (i == trump || number[i] == 0)
						continue;
					for (findMin = 0; findMin < 8; findMin++) {
						if (cards[i][findMin] == true && minValue < findMin) {
							minValue = findMin;
							position = i;
						}
					}
				}

				if (minValue != Empty)		// have non-trump cards
					currentState[playSequence] = play(position, minValue);
				else						// need to play trump
					for (findMin = 0; findMin < 8; findMin++)
						if (cards[trump][findMin] == true) {
							currentState[playSequence] = play(trump, findMin);
							break;
						}
			}
			//}
			//else {
			//
			//}
			break;
		}
		case 2: {	// rearhand (R)
					//if (playerNo == whosDeclarer) {		// play the largest card
			int largestCard, lowestCard;
			if (currentState[0] / 10 == currentState[1] / 10) 		// same suit
				largestCard = (currentState[0] < currentState[0]) ? currentState[0] : currentState[0];
			else if (currentState[0] / 10 == trump || currentState[0] % 10 == 0
				|| currentState[1] / 10 == trump || currentState[1] % 10 == 0) {		// player 1 uses trump
				if (currentState[0] / 10 == trump) {
					if (currentState[1] % 10 == 0 && currentState[0] % 10 != 0)
						largestCard = currentState[1];
					else
						largestCard = currentState[0];
				}
				else if (currentState[1] / 10 == trump) {
					if (currentState[0] % 10 == 0 && currentState[0] < currentState[1])
						largestCard = currentState[0];
					else
						largestCard = currentState[1];
				}
				else if (currentState[0] % 10 == 0) {
					if (currentState[1] % 10 == 0 && currentState[1] < currentState[0])
						largestCard = currentState[1];
					else
						largestCard = currentState[0];
				}
				else if (currentState[1] % 10 == 0) {
					if (currentState[0] % 10 == 0 && currentState[0] < currentState[1])
						largestCard = currentState[0];
					else
						largestCard = currentState[1];
				}
			}
			else {			// player 1 doesn't have cards
				largestCard = currentState[0];
			}
			if (largestCard % 10 == 0)
				suit = trump;
			else
				suit = largestCard / 10;


			if (largestCard % 10 == 0) {	// player 1 // 2 played Jack
				for (int i = 0; i < largestCard / 10; i++)
					if (cards[i][0] == true) {
						currentState[playSequence] = play(i, 0);
						return;
					}
			}
			else if (number[suit] > 0 || number[trump] > 0) {		// player 1 // 2 didn't play Jack
				for (int suit = 0; suit < 4; suit++)		// play Jack
					if (cards[suit][0] == true) {
						currentState[playSequence] = play(suit, 0);
						return;
					}

				int findmax1 = 0, findmax2 = 0;
				for (; findmax1 < 8; findmax1++) {
					if (cards[suit][findmax1] == true)
						break;
				}
				for (; findmax2 < 8; findmax2++) {
					if (cards[trump][findmax2] == true)
						break;
				}

				if (suit == trump && suit * findmax1 < largestCard) {		// play trump
					currentState[playSequence] = play(suit, findmax1);
					return;
				}
				else if (suit != trump) {
					if (number[trump] > 0) {
						currentState[playSequence] = play(trump, findmax2);
						return;
					}
					else if (number[suit] > 0 && suit * 10 + findmax1 < largestCard) {
						currentState[playSequence] = play(suit, findmax1);
						return;
					}
				}
			}
			// couldn't win this turn
			if (currentState[0] % 10 == 0)
				suit = trump;
			else
				suit = currentState[0] / 10;

			if (number[suit] > 0) {		// have current suit
				if (suit == trump)
					for (int num = 3; num >= 0; num--)
						if (cards[num][0] == true) {
							lowestCard = num * 10;
							break;
						}

				for (int num = 7; num >= 0; num--)
					if (cards[suit][num] == true) {
						lowestCard = suit * 10 + num;
						break;
					}
				currentState[playSequence] = play(lowestCard / 10, lowestCard % 10);
			}
			else {		// don't have current suit
				int minValue = Empty, position, findMin;
				for (int i = 0; i < 4; i++) {
					if (i == trump || number[i] == 0)
						continue;
					for (findMin = 0; findMin < 8; findMin++) {
						if (cards[i][findMin] == true && minValue < findMin) {
							minValue = findMin;
							position = i;
						}
					}
				}

				if (minValue != Empty)		// have non-trump cards
					currentState[playSequence] = play(position, minValue);
				else						// need to play trump
					for (findMin = 0; findMin < 8; findMin++)
						if (cards[trump][findMin] == true) {
							currentState[playSequence] = play(trump, findMin);
							break;
						}
			}
			//}
			//else {
			//
			//}
			break;
		}
	}
}

// Player with random actions (no bias)
void Player::randomPlayer(int* currentState, int playSequence) {
	int suitable_actions[10] = { Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty };
	int action_count = 0;

	// first player
	if (playSequence == 0) {
		for (int suit = 0; suit < 4; suit++) {
			for (int card = 0; card < 8; card++) {
				if (cards[suit][card])
					suitable_actions[action_count++] = suit * 10 + card;
			}
		}
	}
	// need to follow the suit
	else {
		int current_suit = currentState[0] / 10;

		for (int suit = 0; suit < 4; suit++) {
			for (int card = 0; card < 8; card++) {
				if (cards[suit][card] && suit == current_suit)
					suitable_actions[action_count++] = suit * 10 + card;
			}
		}

		// no suitable card to play
		if (action_count == 0) {
			for (int suit = 0; suit < 4; suit++) {
				for (int card = 0; card < 8; card++) {
					if (cards[suit][card])
						suitable_actions[action_count++] = suit * 10 + card;
				}
			}
		}
	}
	
	std::random_device rd;
	int random_action = suitable_actions[rd() % action_count];
	currentState[playSequence] = play(random_action / 10, random_action % 10);
}

// Greedy optimisation-based Approach
void Player::greedyPlayer(int* currentState, int playSequence) {
	int playCard = Empty;
	int counter = 0;

	switch (playSequence) {
	case 0:
		// Ec = [Vc + Average(Vc2) + Average(Vc3)] * Average[P(c)]	

		for (int suit = 0; suit < 4; suit++) {
			float winPossibility = 0;
			float averageVc = 0;
			for (int card = 0, possibleCardCount = 0, possibleStateCount = 0; card < 8; card++) {
				if (cards[suit][card] == true) {
					// calculate Average[P(c)] & Average(Vc) -> Average(Vc2)//Average(Vc3)
					for (int p2Suit = 0; p2Suit < 4; p2Suit++)
						for (int p2Card = 0; p2Card < 8; p2Card++)
							// Player2's card
							if (gameState[p2Card][p2Card] == false) {
								for (int p3Suit = 0; p3Suit < 4; p3Suit++)
									for (int p3Card = 0; p3Card < 8; p3Card++) {
										if (p3Suit == p2Suit && p3Card == p2Card)
											continue;
										// Player3's card
										if (gameState[p3Card][p3Card] == false) {
											if (isBigger(cardToInt(suit, card), cardToInt(p2Suit, p2Card)) && isBigger(cardToInt(suit, card), cardToInt(p3Suit, p3Card)))
												winPossibility++;

											possibleStateCount++;
										}
									}

								averageVc = averageVc + cardValue(cardToInt(p2Card, p2Card));
								possibleCardCount++;
							}
					averageVc = averageVc / possibleCardCount;
					winPossibility = winPossibility / possibleStateCount;


					greedyEstimation[counter][0] = (float)cardToInt(suit, card);
					greedyEstimation[counter++][1] = winPossibility * (cardValue(cardToInt(suit, card)) + 2 * averageVc);
				}
			}
		}

		break;
	case 1:
		// Ec = [Vc1 + Vc + Average(Vc3)] * Average[P(c)]	

		for (int suit = 0; suit < 4; suit++) {
			float winPossibility = 0, winPossibility1 = 0, winPossibility2 = 0;
			float averageVc3 = 0;
			for (int card = 0, possibleStateCount = 0; card < 8; card++) {
				if (cards[suit][card] == true) {
					// calculate Average[P(c)] & Average(Vc3)
					winPossibility1 = isBigger(currentState[0], cardToInt(suit, card)) ? 0 : 1;
					for (int p3Suit = 0; p3Suit < 4; p3Suit++)
						for (int p3Card = 0; p3Card < 8; p3Card++)
							if (gameState[p3Suit][p3Card] == false) {
								if (isBigger(cardToInt(suit, card), cardToInt(p3Suit, p3Card)))
									winPossibility2++;
								possibleStateCount++;
								averageVc3 = averageVc3 + cardValue(cardToInt(suit, card));
							}
					winPossibility2 = winPossibility2 / possibleStateCount;
					winPossibility = winPossibility1 * winPossibility2;
					averageVc3 = averageVc3 / possibleStateCount;

					greedyEstimation[counter][0] = (float) cardToInt(suit, card);
					greedyEstimation[counter++][1] = winPossibility * (cardValue(currentState[0]) + cardValue(cardToInt(suit, card)) + averageVc3);
				}
			}
		}

		break;
	case 2:
		// Ec = (Vc1 + Vc2 + Vc) * P(c)

		for (int suit = 0; suit < 4; suit++) {
			float winPossibility = 0;
			for (int card = 0; card < 8; card++) {
				if (cards[suit][card] == true) {
					// calculate the possibility to win -> P(c)
					if (isBigger(currentState[0], currentState[1]) && !isBigger(currentState[0], cardToInt(suit, card)))
						winPossibility = 1;
					else if (!isBigger(currentState[0], currentState[1]) && !isBigger(currentState[1], cardToInt(suit, card)))
						winPossibility = 1;
					else
						winPossibility = 0;

					greedyEstimation[counter][0] = (float) cardToInt(suit, card);
					greedyEstimation[counter++][1] = winPossibility * (cardValue(currentState[0]) + cardValue(currentState[1]) + cardValue(cardToInt(suit, card)));
				}
			}
		}

		break;
	}

	float maxEstimation = -1;
	for (int num = 0; num < counter; num++) {
		if (maxEstimation < greedyEstimation[num][1]) {
			maxEstimation = greedyEstimation[num][1];
			playCard = (int)greedyEstimation[num][0];
		}
	}
	// impossible to win, choose the right suit
	if (maxEstimation == 0 && playSequence != 0) {
		int suit, lowestCard;
		if (currentState[0] % 10 == 0)
			suit = trump;
		else
			suit = currentState[0] / 10;

		if (number[suit] > 0) {		// have current suit
			if (suit == trump)
				for (int num = 3; num >= 0; num--)
					if (cards[num][0] == true) {
						lowestCard = num * 10;
						break;
					}

			for (int num = 7; num >= 0; num--)
				if (cards[suit][num] == true) {
					lowestCard = suit * 10 + num;
					break;
				}
			currentState[playSequence] = play(lowestCard / 10, lowestCard % 10);
		}
		else {		// don't have current suit
			int minValue = Empty, position, findMin;
			for (int i = 0; i < 4; i++) {
				if (i == trump || number[i] == 0)
					continue;
				for (findMin = 0; findMin < 8; findMin++) {
					if (cards[i][findMin] == true && minValue < findMin) {
						minValue = findMin;
						position = i;
					}
				}
			}

			if (minValue != Empty)		// have non-trump cards
				currentState[playSequence] = play(position, minValue);
			else						// need to play trump
				for (findMin = 0; findMin < 8; findMin++)
					if (cards[trump][findMin] == true) {
						currentState[playSequence] = play(trump, findMin);
						break;
					}
		}
	}
	else
		currentState[playSequence] = play(playCard / 10, playCard % 10);
}

// Monte-Carlo Tree Search
void Player::MCTSPlayer(int* currentState, int playSequence) {
	using namespace mcts_skat;
	struct input_state_info input_data;

	// initialization
	for (int suit = 0; suit < 4; suit++)
		for (int card = 0; card < 8; card++) {
			input_data.cards[suit][card] = cards[suit][card];
			input_data.gameState[suit][card] = !(gameState[suit][card]);
		}

	if (playSequence != 0) {
		for (int i = 0; i < playSequence; i++)
			input_data.gameState[currentState[i] / 10][currentState[i] % 10] = false;

		if (currentState[0] / 10 == trump || currentState[0] % 10 == 0)
			input_data.current_suit = trump;
		else
			input_data.current_suit = currentState[0] / 10;
	}

	input_data.current_player = playerNo;
	input_data.player_id = playerNo;
	input_data.declarer_id = whosDeclarer;
	input_data.play_sequence[0] = currentState[0];
	input_data.play_sequence[1] = currentState[1];
	input_data.play_sequence[2] = currentState[2];
	input_data.trump_suit = trump;
	input_data.turn_count = playSequence;
	input_data.win_value[0] = winValue[0];
	input_data.win_value[1] = winValue[1];
	input_data.win_value[2] = winValue[2];

	for (int player = 0; player < 3; player++)
		for (int suit = 0; suit < 4; suit++)
			input_data.player_not_have[player][suit] = player_not_have[player][suit];

	// Monte-Carlo Tree Search (UCT)
	msa::mcts::UCT<State, Action> Skat_MCTS;
	State current_state(input_data);

	// Time Constraint or Iteration Constraint
	Skat_MCTS.max_iterations = 800;

	Skat_MCTS.max_millis = 0;

	// MCTS
	Action best_action = Skat_MCTS.run(current_state);

	currentState[playSequence] = play(best_action.suit, best_action.value);
}

// Learning Approach
void Player::NNWPlayer(int* currentState, int playSequence, PyObject* pFunc) {
	string NNW_input;
	int length_counter = 0;
	int turn = 10 - numberAll;
	NNW_input.resize(200);

	// cards agent holds
	for (int suit = 0; suit < 4; suit++) {
		for (int card = 0; card < 8; card++) {
			NNW_input[length_counter++] = record[turn].cards[suit][card] + '0';
			NNW_input[length_counter++] = ' ';
		}
	}

	// cards other two players hold
	for (int suit = 0; suit < 4; suit++) {
		for (int card = 0; card < 8; card++) {
			NNW_input[length_counter++] = record[turn].gameState[suit][card] + '0';
			NNW_input[length_counter++] = ' ';
		}
	}

	// cards that have been played in this turn
	for (int suit = 0; suit < 4; suit++) {
		for (int card = 0; card < 8; card++) {
			if ((suit == currentState[0] / 10 && card == currentState[0] % 10)
				|| (suit == currentState[1] / 10 && card == currentState[1] % 10))
				NNW_input[length_counter++] = '1';
			else
				NNW_input[length_counter++] = '0';

			NNW_input[length_counter++] = ' ';
		}
	}

	//创建参数:
	PyObject *pArgs = PyTuple_New(2);		// set parameter number
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", NNW_input.c_str()));		// convert string(c++) data into string(python)
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", turn));

	PyObject *pReturn = NULL;
	pReturn = PyEval_CallObject(pFunc, pArgs);		// get the return value(python)
	//将返回值转换为int类型
	char* result_char;
	int flag = PyArg_Parse(pReturn, "s", &result_char);		// convert string(python) data into string(c++)
	string result_str = result_char;
	string single_data;
	float data[32];

	for (int counter = 0, pos1 = 0, pos2; counter < 32; counter++) {		// save data into an array
		pos2 = result_str.find(' ', pos1);
		single_data = result_str.substr(pos1, pos2 - pos1);
		data[counter] = atof(single_data.c_str());

		pos1 = pos2 + 1;
	}

	// get three actions that NNW suggests to take
	float action_prob[3] = { 0, 0, 0 };
	int action[3] = { Empty, Empty ,Empty };

	// first player
	if (playSequence == 0) {
		for (int suit = 0; suit < 4; suit++) {
			for (int card = 0; card < 8; card++) {
				if (cards[suit][card]) {
					if (data[suit * 8 + card] > action_prob[0]) {		// data -> actions[0] -> actions[1] -> actions[2]
						action_prob[2] = action_prob[1];
						action[2] = action[1];
						action_prob[1] = action_prob[0];
						action[1] = action[0];
						action_prob[0] = data[suit * 8 + card];
						action[0] = suit * 10 + card;
					}
					else if (data[suit * 8 + card] > action_prob[1]) {		// data -> actions[1] -> actions[2]
						action_prob[2] = action_prob[1];
						action[2] = action[1];
						action_prob[1] = data[suit * 8 + card];
						action[1] = suit * 10 + card;
					}
					else if (data[suit * 8 + card] > action_prob[2]) {		// data -> actions[2]
						action_prob[2] = data[suit * 8 + card];
						action[2] = suit * 10 + card;
					}
				}
			}
		}
	}
	// need to follow the suit
	else {
		int current_suit = currentState[0] / 10;

		for (int suit = 0; suit < 4; suit++) {
			for (int card = 0; card < 8; card++) {
				if (cards[suit][card] && (suit == trump || suit == current_suit || card == 0)) {
					if (data[suit * 8 + card] > action_prob[0]) {		// data -> actions[0] -> actions[1] -> actions[2]
						action_prob[2] = action_prob[1];
						action[2] = action[1];
						action_prob[1] = action_prob[0];
						action[1] = action[0];
						action_prob[0] = data[suit * 8 + card];
						action[0] = suit * 10 + card;
					}
					else if (data[suit * 8 + card] > action_prob[1]) {		// data -> actions[1] -> actions[2]
						action_prob[2] = action_prob[1];
						action[2] = action[1];
						action_prob[1] = data[suit * 8 + card];
						action[1] = suit * 10 + card;
					}
					else if (data[suit * 8 + card] > action_prob[2]) {		// data -> actions[2]
						action_prob[2] = data[suit * 8 + card];
						action[2] = suit * 10 + card;
					}
				}
			}
		}

		// no suitable card to play
		if (action[0] == Empty) {
			for (int suit = 0; suit < 4; suit++) {
				for (int card = 0; card < 8; card++) {
					if (cards[suit][card]) {
						if (data[suit * 8 + card] > action_prob[0]) {		// data -> actions[0] -> actions[1] -> actions[2]
							action_prob[2] = action_prob[1];
							action[2] = action[1];
							action_prob[1] = action_prob[0];
							action[1] = action[0];
							action_prob[0] = data[suit * 8 + card];
							action[0] = suit * 10 + card;
						}
						else if (data[suit * 8 + card] > action_prob[1]) {		// data -> actions[1] -> actions[2]
							action_prob[2] = action_prob[1];
							action[2] = action[1];
							action_prob[1] = data[suit * 8 + card];
							action[1] = suit * 10 + card;
						}
						else if (data[suit * 8 + card] > action_prob[2]) {		// data -> actions[2]
							action_prob[2] = data[suit * 8 + card];
							action[2] = suit * 10 + card;
						}
					}
				}
			}
		}
	}

	// randomly choose one of the actions NNW suggests
	int action_count = -1;
	while (action[++action_count] != Empty && action_count < 3);

	std::random_device rd;
	int random_action_prob = rd() % 100;

	if(random_action_prob >= 25 || action_count == 1)
		currentState[playSequence] = play(action[0] / 10, action[0] % 10);
	else if ((random_action_prob >= 13 && random_action_prob < 25) || action_count == 2)
		currentState[playSequence] = play(action[1] / 10, action[1] % 10);
	else
		currentState[playSequence] = play(action[2] / 10, action[2] % 10);

}

// update game state after every turn
void Player::updateState(int* currentState, int firstPlayer) {
	for (int card = 0; card < 3; card++)
		gameState[currentState[card] / 10][currentState[card] % 10] = true;

	// check if a player cannot follow the suit
	int suit;
	if (currentState[0] / 10 == trump || currentState[0] % 10 == 0)
		suit = trump;
	else
		suit = currentState[0] / 10;

	for (int player = 1; player < 3; player++) {
		if ((firstPlayer + player) % 3 == playerNo)
			continue;
		if (!(currentState[player] / 10 == trump || currentState[player] / 10 == suit || currentState[player] % 10 == 0))
			player_not_have[(firstPlayer + player) % 3][suit] = true;
	}
}

// compare two cards (first is the current suit), use decision tree
bool Player::isBigger(int Card1, int Card2) {
	// Card1 is Jack
	if (Card1 % 10 == 0) {
		if (Card2 % 10 == 0 && Card2 < Card1)
			return false;
		else
			return true;
	}
	// Card1 is Trump without Jack
	else if(Card1 / 10 == trump) {
		if (Card2 % 10 == 0 || (Card2 / 10 == trump && Card2 < Card1))
			return false;
		else
			return true;
	}
	// Card1 is not Trump
	else {
		if (Card2 % 10 == 0 || Card2 / 10 == trump || (Card1 / 10 == Card2 / 10 && Card2 < Card1))
			return false;
		else
			return true;
	}
}

// return the game history
void Player::historyRecord(History* gameHistory) {
	// do not include pointer -> copy directly
	for (int turn = 0; turn < 10; turn++)
		gameHistory[turn] = record[turn];
}

// find solution here
void Player::playCard(int* currentState, int playSequence) {
	int playCard = 0;	// integer parameter: [suit][value]
	currentState[playSequence] = Empty;
	// currentState[playSequence] = playCard;		// play the card here
	
	// **************************** record history **************************** //
	int turn = 10 - numberAll;

	for (int suit = 0; suit < 4; suit++) {
		for (int card = 0; card < 8; card++) {
			record[turn].cards[suit][card] = cards[suit][card];
			record[turn].gameState[suit][card] = gameState[suit][card];
		}
	}

	record[turn].currentState[0] = currentState[0];
	record[turn].currentState[1] = currentState[1];
	record[turn].currentState[2] = currentState[2];
	// ************************************************************************ //

	/* plug the play solution here */
	if (turn < this->turn_start_at) {
		// random actions until the required start turn
		randomPlayer(currentState, playSequence);
	}
	else {
		if (Manual) {
			manualPlayer(currentState, playSequence);
		}
		else if (Standard) {
			standardPlayer(currentState, playSequence);
		}
		else if (Random) {
			randomPlayer(currentState, playSequence);
		}
		else if (Greedy) {
			greedyPlayer(currentState, playSequence);
		}
		else if (MonteCarlo) {
			MCTSPlayer(currentState, playSequence);
		}
		else if (Learning) {
			// load the Keras NNW (python)
			if (pModule == NULL || pFunc == NULL) {
				// PyRun_SimpleString("import keras");
				pModule = PyImport_ImportModule("NNW_Player");
				pFunc = PyObject_GetAttrString(pModule, "nnw_player");
			}

			NNWPlayer(currentState, playSequence, pFunc);
		}
		else {
			cout << "not suitable player strategies" << endl;
		}
	}
	/* plug the play solution here */

	// **************************** record history **************************** //
	record[turn].action = currentState[playSequence];
	// ************************************************************************ //
}
// one who can bid the game value
int Player::bidCaller(int currentBid) {
	
	/* plug the bid solution here */
	if (currentBid == Bid_Start)
		bidNumber = minGameValue;
	else if (currentBid < maxGameValue)
		bidNumber = currentBid + 1;
	else
		bidNumber = Pass;

	return bidNumber;
	/* plug the bid solution here */
}
// one who can accept or pass
bool Player::bidResponder(int currentBid) {

	/* plug the bid solution here */
	if (currentBid <= maxGameValue) {		// accept
		bidNumber = currentBid;
		return true;
	}
	else {
		bidNumber = Pass;
		return false;
	}
	/* plug the bid solution here */
}
// set the game's value
void Player::setGameValue(int value) {
	this->gameValue = value;
}

int Player::numberOfCards() {
	return numberAll;
}
// bid winner declares the game
string Player::declareGame(int skat[2]) {
	/* plug the declare solution here */

	string gameType = "Suit";		// declare Suit game and get skat
	
	getCard(skat[0]);
	getCard(skat[1]);

	return gameType;
	/* plug the declare solution here */
}
// declare trump and discard 2 cards 
string Player::declareTrump() {
	string trump = "";

	// check if overbid
	bool isOverBid[4] = { false, false, false, false };
	int currentGameLevel = findGameLevel();
	if (bidNumber > Base_Value_Diamond * (currentGameLevel + 1)) {
		isOverBid[3] = true;
		if (bidNumber > Base_Value_Heart * (currentGameLevel + 1)) {
			isOverBid[2] = true;
			if (bidNumber > Base_Value_Spade * (currentGameLevel + 1)) {
				isOverBid[1] = true;
				if (bidNumber > Base_Value_Club * (currentGameLevel + 1)) {
					isOverBid[0] = true;
					return "Over Bid";
				}
			}
		}
	}

	/* plug the declare solution here */
	int suit;
	for (int i = 0, maxNumber = Empty; i < 4; i++) {
		if (isOverBid[i])		// cannot use this suit
			continue;

		if (maxNumber < number[i] || maxNumber == Empty) {		// choose the suit with largest number
			suit = i;
			maxNumber = number[i];
		}
		else if (maxNumber == number[i]) {	// if equal -> choose the suit with largest value
			int count1 = 0, count2 = 0;
			for (int j = 0; j < 8; j++)
				if(cards[suit][j] == true)
				count1 += j;
			for (int j = 0; j < 8; j++)
				if (cards[i][j] == true)
				count2 += j;

			suit = (count1 < count2) ? suit : i;
		}
	}

	switch (suit) {
	case 0:
		trump = "Club";
		break;
	case 1:
		trump = "Spade";
		break;
	case 2:
		trump = "Heart";
		break;
	case 3:
		trump = "Diamond";
		break;
	default:
		trump = "Over Bid";
		break;
	}
	/* plug the declare solution here */

	/* plug the discard solution here */
	int discardNum = 0, min[20];
	for (int i = 0; i < 4; i++) {
		if (i == suit)		// skip the trump suit
			continue;
		for (int j = 7; j >= 0 && j != number[i] - 3; j--) {		// get several cards with the lowest value
			if(cards[i][j] == true)
				min[discardNum++] = i * 10 + j;
			//cout << min[discardNum - 1] << endl;
		}
	}

	// discard two lowest value cards
	for (int i = 0, minValue = 0, position = 0; i < discardNum; i++) {
		if (minValue % 10 < min[i] % 10) {
			minValue = min[i];
			position = i;
		}

		if (i == discardNum - 1) {
			winCard(min[position]);
			play(minValue / 10, minValue % 10);
			min[position] = Empty;
			cout << "discard 1: " << winCards[0] << endl;
		}
	}
	for (int i = 0, minValue = 0, position = 0; i < discardNum; i++) {
		if (minValue % 10 < min[i] % 10) {
			minValue = min[i];
			position = i;
		}
		if (i == discardNum - 1) {
			winCard(min[position]);
			play(minValue / 10, minValue % 10);
			min[position] = Empty;
			cout << "discard 2: " << winCards[1] << endl;
		}
	}
	/* plug the discard solution here */
	return trump;
}
// tell player the game information
void Player::setGame(string gameType, string gameTrump) {
	this->gameType = gameType;

	if (gameTrump == "Club")
		this->trump = 0;
	else if (gameTrump == "Spade")
		this->trump = 1;
	else if (gameTrump == "Heart")
		this->trump = 2;
	else if (gameTrump == "Diamond")
		this->trump = 3;
}
// get the cards one wins
void Player::winCard(int card) {
	winCards[winCardsNumber++] = card;
}
// let player know who is the declarer
void Player::setDeclarer(int playerNo) {
	whosDeclarer = playerNo;
}
// output one assigned card
int Player::play(int suit, int num) {
	if (cards[suit][num] == true) {

		cards[suit][num] = false;
		numberAll--;
		number[suit]--;

		return suit * 10 + num;
	}
	else {
		cout << "cannot find the card" << endl;
		return Empty;
	}
}
// decide who's the winner for this turn
int Player::turnWinner(int* currentState) {
	int turnWinner = Empty;

	if (isBigger(currentState[0], currentState[1])) {
		if (isBigger(currentState[0], currentState[2]))
			turnWinner = 0;
		else
			turnWinner = 2;
	}
	else {
		if (isBigger(currentState[1], currentState[2]))
			turnWinner = 1;
		else
			turnWinner = 2;
	}

	return turnWinner;
}

// value for each card
int Player::cardValue(int Card) {
	int value = 0;
	switch (Card % 10) {
	case 0:		// Jack
		value = 2;
		break;
	case 1:		// Ace
		value = 11;
		break;
	case 2:		// 10
		value = 10;
		break;
	case 3:		// King
		value = 4;
		break;
	case 4:		// Queen
		value = 3;
		break;
	default:	// 7,8,9 without value
		break;
	}
	return value;
}

// calculate the value one holds
int Player::calculateValue() {
	int holdValue = 0;

	for (int num = 0; num < winCardsNumber; num++)
		holdValue += cardValue(winCards[num]);

	return holdValue;
}
// match integer with suit
string Player::identifySuit(int suitNo) {
	// types of cards: [0]:♣	[1]:♤	[2]:♡	[3]:◇
	string suit[4] = { "Club", "Spade", "Heart ", "Diamond" };

	return suit[suitNo];
}
// match integer with card value
string Player::identifyValue(int valueNo) {
	// values of cards
	string card[8] = { "J", "A", "10", "K", "Q", "9", "8", "7" };

	return card[valueNo];
}
// match integer with card
string Player::identifyCard(int cardNo) {
	return identifySuit(cardNo / 10) + identifyValue(cardNo % 10);
}

int Player::cardToInt(int suit, int value) {
	return 10 * suit + value;
}

void Player::updateValue(int updatedValue[3]) {
	winValue[0] += updatedValue[0];
	winValue[1] += updatedValue[1];
	winValue[2] += updatedValue[2];
}