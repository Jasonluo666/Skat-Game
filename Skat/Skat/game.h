#pragma once
#include "global.h"
#include "player.h"

void dealCard(Player player[3]);
int Biding(Player player[3]);
void Game(Player player[3], string gameType, string trump);
void valueCalculation(Player player[3], int declarer);
void getSkat(bool mapDeal[4][8], int skat[2]);
void skatGame(bool isManual, int humanPlayerNo);