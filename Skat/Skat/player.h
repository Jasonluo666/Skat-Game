#pragma once
#include "global.h"

int compare(const void *a, const void *b);
class Player {
public:
	Player(int playerNo);	
	void getCard(int card);
	int bidCaller(int currentBid);
	bool bidResponder(int currentBid);
	void setGameValue(int value);
	string declareGame(int skat[2]);
	string declareTrump();
	void setGame(string gameType, string gameTrump);
	void playCard(int* currentState);
	void winCard(int card);
	void setDeclarer(int playerNo);
	int turnWinner(int* currentState);
	int calculateValue();
	int numberOfCards();
	void setManual();
private:
	void sortCard();
	int findGameLevel();
	string identifySuit(int suitNo);
	string identifyValue(int valueNo);
	string identifyCard(int cardNo);
	int play(int suit, int num);
	int playerNo;
	int numberAll;
	int number[4];
	int cards[4][10];
	int maxGameValue;	// the highest value one could bid
	int minGameValue;	// the lowest value one could bid
	int gameValue;		// current game's value
	int winCards[32];	// cards one wins during the game
	int winCardsNumber;
	int whosDeclarer;
	string gameType;
	int trump;
	int bidNumber;

	bool manualOperation;
};