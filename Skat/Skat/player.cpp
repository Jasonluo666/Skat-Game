﻿#include "player.h"

Player::Player(int playerNo) {
	this->playerNo = playerNo;
	numberAll = number[0] = number[1] = number[2] = number[3] = 0;
	minGameValue = 18;
	winCardsNumber = 0;
	Manual = Standard = Greedy = MonteCarlo = Learning = false;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++) {
			cards[i][j] = false;
			gameState[i][j] = false;
		}

	for (int i = 0; i < 32; i++)
		winCards[i] = Empty;
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
void Player::setPlayerType(string playerType) {
	if (playerType == "Manual")
		Manual = true;
	else if (playerType == "Standard")
		Standard = true;
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
// Greedy optimisation-based Approach
void Player::greedyPlayer(int* currentState, int playSequence) {
	int playCard = Empty, cardPosition = Empty;
	int gainValue = cardValue(currentState[0]) + cardValue(currentState[1]);

	// Declarer
	if (playerNo == whosDeclarer) {
		switch (playSequence) {
		case 0:
			
			break;
		case 1:

			break;
		case 2:
			// Ec = (Vc1 + Vc2 + Vc) * P(Vc)
			int Evaluation = 0;

			for (int suit = 0; suit < 4; suit++)
				for (int card = 0, currentValue = 0, winPossibility = 0; card < number[suit]; card++) {

				}
			break;
		}
	}
	// Opponents
	else {
		switch (playSequence) {
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		}
	}
	currentState[playSequence] = play(playCard, cardPosition);
}

// Monte-Carlo Tree Search
void Player::MCTSPlayer(int* currentState, int playSequence) {

}

// Learning Approach
void Player::NNWPlayer(int* currentState, int playSequence) {

}

// update game state after every turn
void Player::updateState(int* currentState) {
	for (int card = 0; card < 3; card++)
		gameState[currentState[card] / 10][currentState[card] % 10] = true;
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

// find solution here
void Player::playCard(int* currentState, int playSequence) {
	int playCard = 0;	// integer parameter: [suit][value]
	currentState[playSequence] = Empty;
	// currentState[playSequence] = playCard;		// play the card here

	/* plug the play solution here */
	if (Manual) {
		manualPlayer(currentState, playSequence);
	}
	else if (Standard) {
		standardPlayer(currentState, playSequence);
	}
	else if (Greedy) {
		greedyPlayer(currentState, playSequence);
	}
	else if (MonteCarlo) {
		MCTSPlayer(currentState, playSequence);
	}
	else if (Learning) {
		NNWPlayer(currentState, playSequence);
	}
	else {
		cout << "not suitable player strategies" << endl;
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