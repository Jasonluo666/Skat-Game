#pragma once
#include "global.h"
#include "player.h"

void dealCard(Player player[3]);
int Biding(Player player[3]);
void Game(Player player[3], string gameType, string trump);
bool valueCalculation(Player player[3], int declarer);
void getSkat(bool mapDeal[4][8], int skat[2]);
bool skatGame(string playerType[2], History *NNWdata, int turn_start_at);