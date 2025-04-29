#include <set>
#include "functions/dbHandler.h"
#include "functions/gameAlgo.h"
#include <vector>
#include <iostream>

using namespace std;

sqlite3* db;
set<vector<int>> visitedStates;
int gameIdCounter = 1; // Start counting games

void simulateGame(vector<int> piles, int moveNumber, bool isBot1Turn) {
    if (piles[0] == 0 && piles[1] == 0 && piles[2] == 0) {
        // Game over. Save winner.
        updateGameResult(db, gameIdCounter, isBot1Turn ? "Bot2" : "Bot1"); // Last mover loses
        gameIdCounter++;
        return;
    }

    if (visitedStates.count(piles)) {
        return; // Skip redundant paths
    }
    visitedStates.insert(piles);

    for (int i = 0; i < 3; ++i) {
        if (piles[i] > 0) {
            for (int sub = 1; sub <= piles[i]; ++sub) {
                vector<int> newPiles = piles;
                newPiles[i] -= sub;

                char pileChar = 'A' + i;
                string whoMoved = isBot1Turn ? "Bot1" : "Bot2";

                // Insert move into database
                insertData(db, gameIdCounter, moveNumber, piles[0], piles[1], piles[2],
                           whoMoved, pileChar, sub, newPiles[i], ""); // game_result blank for now

                simulateGame(newPiles, moveNumber + 1, !isBot1Turn);
            }
        }
    }
}

int main() {
    db = openDatabase("gameData.db");
    if (!db) {
        cerr << "Failed to open database. Exiting." << endl;
        return -1;
    }

    createTable(db);

    vector<int> initialPiles = {3, 3, 3}; // Small for testing first
    simulateGame(initialPiles, 1, true); // Bot1 starts

    closeDatabase(db);
    return 0;
}
