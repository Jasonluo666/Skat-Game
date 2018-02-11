#pragma once
#include "iostream"
#include <stdlib.h>
#include <time.h> 
#include "fstream"
#include "Python.h"

using namespace std;
#define	Bid_Start			0
#define Pass				-1
#define Empty				-1
#define Base_Value_Club		12
#define Base_Value_Spade	11
#define Base_Value_Heart	10
#define Base_Value_Diamond	9
#define INF					99999

struct History {
	bool cards[4][8];
	bool gameState[4][8];
	int currentState[3];
	int action;
};