#include "game.h"

/*
Suit games:
clubJ - spadeJ - heartJ - diamondJ - A - 10 - K - Q - 9 - 8 - 7.
*/

// values of cards
string card[8] = { "J", "A", "10", "K", "Q", "9", "8", "7" };
// types of cards: [0]:♣	[1]:♤	[2]:♡	[3]:◇
string suit[4] = { "Club", "Spade", "Heart ", "Diamond" };

bool mapDeal[4][8];
int skat[2];

void dealCard(Player player[3]) {
	/*
	actual game: The dealer deals a batch of three cards to each player,
	then two cards face down in the centre of the table to form the skat,
	then a batch of four cards to each player, and finally another batch of three cards each.

	implement in computer: use random() instead, only restriction is -> 10 cards for each & 2 skat cards
	*/

	srand((unsigned)time(0));
	cout << "Deal the card.." << endl;

	for (int playerNo = 0; playerNo < 3; playerNo++)
		while (player[playerNo].numberOfCards() < 10) {
			int suit = rand() % 4;
			int value = rand() % 8;
			//	if the card is available, then feed the player
			if (!mapDeal[suit][value]) {
				player[playerNo].getCard(suit * 10 + value);	// integer parameter: [suit][value]
				mapDeal[suit][value] = true;
			}
		}

	cout << "finished" << endl;
}
//  ( forehand (F) vs. middlehand (M) ) vs. rearhand (R)
int Biding(Player player[3]) {
	/*
	presume player[0~2] are forehand (F), middlehand (M) and rearhand (R)
	1. Who will be declarer for the round, and thus eligible for picking up the Skat
	2. The minimum Game Value needed in order for declarer to win
	*/

	cout << "Bid.." << endl;

	int winner, bidValue = Bid_Start;
	// forehand (F) vs. middlehand (M)
	while (1) {
		if (player[1].bidCaller(bidValue) == Pass) {
			cout << "player 1 Pass" << endl;
			winner = 0;
			break;
		}
		else {
			bidValue = player[1].bidCaller(bidValue);
			cout << "player 1 bids " << bidValue << endl;
		}

		if (!player[0].bidResponder(bidValue)) {
			cout << "player 0 Pass" << endl;
			winner = 1;
			break;
		}
		else
			cout << "player 0 accepts" << endl;
	}

	// ( forehand (F) vs. middlehand (M) ) vs. rearhand (R)
	while (1) {
		if (player[2].bidCaller(bidValue) == Pass) {
			cout << "player 2 Pass" << endl;
			break;
		}
		else {
			bidValue = player[2].bidCaller(bidValue);
			cout << "player 2 bids " << bidValue << endl;
		}

		if (!player[winner].bidResponder(bidValue)) {
			cout << "player " << winner << " Pass" << endl;
			winner = 2;
			break;
		}
		else
			cout << "player " << winner << " accepts" << endl;
	}
	cout << "player " << winner << " becomes the declarer" << endl;
	// inform other players
	player[0].setDeclarer(winner);
	player[1].setDeclarer(winner);
	player[2].setDeclarer(winner);

	cout << "finished" << endl;
	return winner;
}
// the card game start
void Game(Player player[3], string gameType, string trump) {
	/*
	Play is clockwise. No matter who is the declarer, forehand always leads to the first trick.
	Players must follow suit if they can. A player with no card of the suit led may play any card.
	Note that in Suit and Grand games the jacks belong to the trump suit, not to the suits marked on them.
	*/

	player[0].setGame(gameType, trump);		// tell players the game types (Suit, Grand, Null) and trump suit
	player[1].setGame(gameType, trump);
	player[2].setGame(gameType, trump);

	//presume player[0~2] are forehand (F), middlehand (M) and rearhand (R)
	for (int playerNo = 0, counter = 1, currentState[3]; counter <= 10; playerNo = (playerNo + 1) % 3) {
		player[playerNo].playCard(currentState);

		if (playerNo == 2) {		// one turn finished
			cout << "Turn " << counter << " finished" << endl;
			cout << "player 0 plays " << suit[currentState[0] / 10].c_str() << card[currentState[0] % 10].c_str() << endl;
			cout << "player 1 plays " << suit[currentState[1] / 10].c_str() << card[currentState[1] % 10].c_str() << endl;
			cout << "player 2 plays " << suit[currentState[2] / 10].c_str() << card[currentState[2] % 10].c_str() << endl;
			counter++;
			// winner get 3 cards
			int turnWinner = player[0].turnWinner(currentState);
			cout << "player " << turnWinner << " win this turn" << endl;
			player[turnWinner].winCard(currentState[0]);
			player[turnWinner].winCard(currentState[1]);
			player[turnWinner].winCard(currentState[2]);
		}
	}

	cout << "Game over" << endl;
}
void valueCalculation(Player player[3], int declarer) {
	int calcDeclarer = 0, calcOpponents = 0;
	for (int playerNo = 0, currentValue; playerNo < 3; playerNo++) {
		currentValue = player[playerNo].calculateValue();

		cout << "Player " << playerNo << " gets value of " << currentValue << endl;

		if (playerNo == declarer)
			calcDeclarer = currentValue;
		else
			calcOpponents += currentValue;
	}
	if (calcDeclarer > calcOpponents)		// get more than 61 points
		cout << "Declarer wins with the highest value " << calcDeclarer << endl;
	else
		cout << "Opponents wins with the highest value " << calcOpponents << endl;
}

// find the 2 skat cards
void getSkat(bool mapDeal[4][8], int skat[2]) {
	int num = 0;
	for (int i = 0; i < 4 && num != 2; i++)
		for (int j = 0; j < 8 && num != 2; j++) {
			if (!mapDeal[i][j]) {
				skat[num++] = i * 10 + j;
			}
		}
	cout << "skat: " << suit[skat[0] / 10].c_str() << card[skat[0] % 10].c_str() << " and "
		<< suit[skat[1] / 10].c_str() << card[skat[1] % 10].c_str() << endl;

}
// skat game platform
void skatGame(bool isManual, int humanPlayerNo) {
	// initialization
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
			mapDeal[i][j] = false;

	Player player[3] = { 0,1,2 };
	// set human player
	if (isManual)
		player[humanPlayerNo].setManual();

	dealCard(player);

	int declarerNo = Biding(player);

	getSkat(mapDeal, skat);

	string gameType = player[declarerNo].declareGame(skat), trump = player[declarerNo].declareTrump();

	if (trump == "Over Bid") {
		cout << "Player " << declarerNo << " Over Bid." << endl;
		return;
	}

	cout << "Player " << declarerNo << " wins the bid and declares " << gameType.c_str() << " game with trump->" << trump.c_str() << endl;

	Game(player, gameType, trump);

	valueCalculation(player, declarerNo);
}