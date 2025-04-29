#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>

using namespace std;

// New database-related function declarations
sqlite3* openDatabase(const string& dbName);
void createTable(sqlite3* db);
void insertData(sqlite3* db, int gameId, int moveNumber, int pileA, int pileB, int pileC, const string& whoMoved, char pileChosen, int amountSubtracted, int newPileValue, const string& gameResult);
void closeDatabase(sqlite3* db);
bool isCombinationUnique(sqlite3* db, const vector<int>& numbers);
void insertUniqueCombination(sqlite3* db, const vector<int>& numbers);
void updateGameResult(sqlite3* db, int gameId, const string& winner);
