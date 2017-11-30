#pragma once
#include "global.h"

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
	void playCard(int* currentState, int playSequence);
	void winCard(int card);
	void setDeclarer(int playerNo);
	int turnWinner(int* currentState);
	int calculateValue();
	int numberOfCards();
	void setPlayerType(string playerType);
	bool isBigger(int Card1, int Card2);
	void updateState(int* currentState);
	int cardToInt(int suit, int value);
	void updateValue(int updatedValue[3]);
	int cardValue(int Card);
private:
	void sortCard();
	int findGameLevel();
	string identifySuit(int suitNo);
	string identifyValue(int valueNo);
	string identifyCard(int cardNo);
	int play(int suit, int num);

	// strategies
	void manualPlayer(int* currentState, int playSequence);
	void standardPlayer(int* currentState, int playSequence);
	// Greedy optimisation-based Approach
	void greedyPlayer(int* currentState, int playSequence);
	// Monte-Carlo Tree Search
	void MCTSPlayer(int* currentState, int playSequence);
	// Learning Approach
	void NNWPlayer(int* currentState, int playSequence);

	int playerNo;
	int numberAll;
	int number[4];
	bool cards[4][8];
	int maxGameValue;	// the highest value one could bid
	int minGameValue;	// the lowest value one could bid
	int gameValue;		// current game's value
	int winCards[32];	// cards one wins during the game
	int winCardsNumber;
	int whosDeclarer;
	string gameType;
	int trump;
	int bidNumber;
	bool gameState[4][8];
	float greedyEstimation[10][2];
	int winValue[3];

	bool Manual;
	bool Standard;
	bool Greedy;
	bool MonteCarlo;
	bool Learning;
};