#include "player.h"

//qsort for cards
int compare(const void *a, const void *b)
{
	int *pa = (int*)a;
	int *pb = (int*)b;
	return (*pa) - (*pb);  //card value ranking from highest to lowest
}


Player::Player(int playerNo) {
	this->playerNo = playerNo;
	numberAll = number[0] = number[1] = number[2] = number[3] = 0;
	minGameValue = 18;
	winCardsNumber = 0;
	manualOperation = false;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 10; j++)
			cards[i][j] = Empty;

	for (int i = 0; i < 32; i++)
		winCards[i] = Empty;
}
// find the Matadors
int Player::findGameLevel() {
	int gameLevel = 1;

	if (cards[0][0] == 0) {	// with Jack of Club
		if (cards[1][0] == 0) {
			gameLevel++;
			if (cards[2][0] == 0) {
				gameLevel++;
				if (cards[3][0] == 0) {
					gameLevel++;
				}
			}
		}
	}
	else {					// without Jack of Club
		if (cards[1][0] != 0) {
			gameLevel++;
			if (cards[2][0] != 0) {
				gameLevel++;
				if (cards[3][0] != 0) {
					gameLevel++;
				}
			}
		}
	}

	return gameLevel;
}

//sort the card by value
void Player::sortCard() {
	for (int suit = 0; suit < 4; suit++)
		qsort(cards[suit], number[suit], sizeof(int), compare);

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
}
// collect the cards
void Player::getCard(int card) {
	int suit = card / 10;
	int value = card % 10;
	// get the card
	cards[suit][number[suit]++] = value;
	numberAll++;

	cout << "player " << playerNo << " got " << card << endl;

	if (numberAll == 10 || numberAll == 12)
		this->sortCard();
}
// human player
void Player::setManual() {
	manualOperation = true;
}
// find solution here
void Player::playCard(int* currentState) {
	int playCard = 0;	// integer parameter: [suit][value]
	currentState[playerNo] = Empty;
	// currentState[playerNo] = playCard;		// play the card here

	/* plug the play solution here */
	int suit;
	if (manualOperation) {
		int card = Empty;
		cout << "------------------" << "human player" << playerNo << "------------------" << endl;
		for (int priorPlayer = 0; priorPlayer < playerNo; priorPlayer++)
			cout << "Player " << priorPlayer << " plays " << identifyCard(currentState[priorPlayer]).c_str() << endl;
		cout << "------------------your cards------------------" << endl;
		for (int suit = 0; suit < 4; suit++) {
			cout << identifySuit(suit).c_str() << ": ";
			for (int num = 0; num < number[suit]; num++)
				cout << identifyValue(cards[suit][num]).c_str() << " ";
			cout << endl;
		}
		cout << "------------------your turn------------------" << endl;
		cout << "please input an two-degit number ([Club->0, Spade->1, Heart->2, Diamond->3][the n'th cards in this suit]):" << endl;
		for (bool isValid = false; !isValid; ) {
			cin >> card;
			if (cards[card / 10][card % 10] == Empty)
				cout << "you do not have this card." << endl;
			else if (currentState[0] != Empty) {
				int suit;
				if (currentState[0] % 10 == 0)
					suit = trump;
				else
					suit = currentState[0] / 10;

				if (cards[card / 10][card % 10] % 10 == 0 || card / 10 == trump || card / 10 == suit || number[suit] == 0) {
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
		currentState[playerNo] = play(card / 10, card % 10);
	}
	else {
		switch (playerNo) {
			case 0: {
				// forehand (F)
				// first player plays the largest card
				for (int suit = 0; suit < 4; suit++)
					if (cards[suit][0] == 0) {
						currentState[playerNo] = play(suit, 0);
						break;
					}

				if (currentState[playerNo] == Empty) {		// not Jack found
					if (number[trump] > 0)		// play trump first
						currentState[playerNo] = play(trump, 0);
					else {
						int position;
						for (int suit = 0, maxValue = INF; suit < 4; suit++) {
							if (number[suit] == 0)
								continue;
							if (maxValue > cards[suit][0]) {
								maxValue = cards[suit][0];
								position = suit;
							}
						}
						currentState[playerNo] = play(position, 0);
					}
				}
				break;
			}
			case 1: {		// middlehand (M)
							//if (playerNo == whosDeclarer || whosDeclarer == 0) {		// play the largest card if could win, otherwise play the smallest one
				if (currentState[0] % 10 == 0)		// Jack belongs to trump
					suit = trump;
				else
					suit = currentState[0] / 10;


				if (currentState[0] % 10 == 0) {	// player 1 played Jack
					for (int i = 0; i < currentState[0] / 10; i++)
						if (cards[i][0] == 0) {
							currentState[playerNo] = play(i, 0);
							return;
						}
				}
				else if (number[suit] > 0 || number[trump] > 0) {		// player 1 didn't play Jack
					for (int suit = 0; suit < 4; suit++)		// play Jack
						if (cards[suit][0] == 0) {
							currentState[playerNo] = play(suit, 0);
							return;
						}

					if (suit == trump && cards[suit][0] < currentState[0]) {		// play trump
						currentState[playerNo] = play(suit, 0);
						return;
					}
					else if (suit != trump) {
						if (number[trump] > 0) {
							currentState[playerNo] = play(trump, 0);
							return;
						}
						else if (number[suit] > 0 && cards[suit][0] < currentState[0]) {
							currentState[playerNo] = play(suit, 0);
							return;
						}
					}
				}
				// couldn't win this turn
				if (currentState[0] % 10 == 0)
					suit = trump;
				else
					suit = currentState[0] / 10;

				if (number[suit] > 0)		// have current suit
					currentState[playerNo] = play(suit, number[suit] - 1);
				else {		// don't have current suit
					int minValue = Empty, position;
					for (int i = 0; i < 4; i++) {
						if (i == trump || number[i] == 0)
							continue;
						if (minValue < cards[i][number[i] - 1]) {
							minValue = cards[i][number[i] - 1];
							position = i;
						}
					}

					if (minValue != Empty)		// have non-trump cards
						currentState[playerNo] = play(position, number[position] - 1);
					else						// need to play trump
						currentState[playerNo] = play(trump, number[trump] - 1);
				}
				//}
				//else {
				//
				//}
				break;
			}
			case 2: {	// rearhand (R)
						//if (playerNo == whosDeclarer) {		// play the largest card
				int largestCard;
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
						if (cards[i][0] == 0) {
							currentState[playerNo] = play(i, 0);
							return;
						}
				}
				else if (number[suit] > 0 || number[trump] > 0) {		// player 1 didn't play Jack
					for (int suit = 0; suit < 4; suit++)		// play Jack
						if (cards[suit][0] == 0) {
							currentState[playerNo] = play(suit, 0);
							return;
						}

					if (suit == trump && cards[suit][0] < largestCard) {		// play trump
						currentState[playerNo] = play(suit, 0);
						return;
					}
					else if (suit != trump) {
						if (number[trump] > 0) {
							currentState[playerNo] = play(trump, 0);
							return;
						}
						else if (number[suit] > 0 && cards[suit][0] < largestCard) {
							currentState[playerNo] = play(suit, 0);
							return;
						}
					}
				}
				// couldn't win this turn
				if (currentState[0] % 10 == 0)
					suit = trump;
				else
					suit = currentState[0] / 10;

				if (number[suit] > 0)		// have current suit
					currentState[playerNo] = play(suit, number[suit] - 1);
				else {		// don't have current suit
					int minValue = Empty, position;
					for (int i = 0; i < 4; i++) {
						if (i == trump || number[i] == 0)
							continue;
						if (minValue < cards[i][number[i] - 1]) {
							minValue = cards[i][number[i] - 1];
							position = i;
						}
					}

					if (minValue != Empty)		// have non-trump cards
						currentState[playerNo] = play(position, number[position] - 1);
					else						// need to play trump
						currentState[playerNo] = play(trump, number[trump] - 1);
				}
				//}
				//else {
				//
				//}
				break;
			}
		}
	}
	/* plug the play solution here */

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
			for (int j = 0; j < number[suit]; j++)
				count1 += cards[suit][j];
			for (int j = 0; j < number[i]; j++)
				count2 += cards[i][j];

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
	int discardNum = 0, min[8];
	for (int i = 0; i < 4; i++) {
		if (i == suit)		// skip the trump suit
			continue;
		for (int j = number[i] - 1; j >= 0 && j != number[i] - 3; j--) {		// get several cards with the lowest value
			min[discardNum++] = i * 10 + cards[i][j];
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
			cards[minValue / 10][--number[minValue / 10]] = Empty;
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
			cards[minValue / 10][--number[minValue / 10]] = Empty;
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
	int card;
	if ((card = cards[suit][num]) != Empty) {
		for (int i = num; i < number[suit] - 1; i++)
			cards[suit][i] = cards[suit][i + 1];

		cards[suit][--number[suit]] = Empty;
		numberAll--;

		return suit * 10 + card;
	}
	else {
		cout << "cannot find the card" << endl;
		return Empty;
	}
}
// decide who's the winner for this turn
int Player::turnWinner(int* currentState) {
	int turnWinner = Empty;

	if (currentState[0] % 10 == 0 || currentState[1] % 10 == 0 || currentState[2] % 10 == 0) {		// with Jack
		for (int i = 0; i < 3; i++) {
			if (currentState[i] % 10 == 0) {
				if (turnWinner == Empty)
					turnWinner = i;
				else if (currentState[i] < currentState[turnWinner])
					turnWinner = i;
			}
		}
		return turnWinner;
	}
	else if (currentState[0] / 10 == trump || currentState[1] / 10 == trump || currentState[2] / 10 == trump) {	// with trump but without Jack
		for (int i = 0; i < 3; i++) {
			if (currentState[i] / 10 == trump) {
				if (turnWinner == Empty)
					turnWinner = i;
				else if (currentState[i] < currentState[turnWinner])
					turnWinner = i;
			}
		}
		return turnWinner;
	}
	else {		// without trump
		int currentSuit = currentState[0] / 10;
		if (currentState[1] / 10 == currentSuit && currentState[1] < currentState[0])
			turnWinner = 1;
		else
			turnWinner = 0;

		if (currentState[2] / 10 == currentSuit && currentState[2] < currentState[turnWinner])
			turnWinner = 2;

		return turnWinner;
	}
}
// calculate the value one holds
int Player::calculateValue() {
	int holdValue = 0;

	for (int num = 0; num < winCardsNumber; num++) {
		switch (winCards[num] % 10) {
		case 0:		// Jack
			holdValue += 2;
			break;
		case 1:		// Ace
			holdValue += 11;
			break;
		case 2:		// 10
			holdValue += 10;
			break;
		case 3:		// King
			holdValue += 4;
			break;
		case 4:		// Queen
			holdValue += 3;
			break;
		default:	// 7,8,9 without value
			break;
		}
	}

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