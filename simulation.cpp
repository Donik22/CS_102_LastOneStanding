#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include "functions/dbHandler.h"
#include "functions/gameAlgo.h"

using namespace std;

sqlite3* db;
vector<int> initialPiles; // Store starting configuration
set<vector<int>> visitedStates;
int gameIdCounter = 1; // Track each game separately

// Simulate one full game between two bots
void simulateGame(vector<int> piles, int moveNumber, bool isBot1Turn) {
    if (piles[0] == 0 && piles[1] == 0 && piles[2] == 0) {
        updateGameResult(db, gameIdCounter, isBot1Turn ? "Bot2" : "Bot1");
        return;
    }

    if (visitedStates.count(piles)) {
        return;
    }
    visitedStates.insert(piles);

    // Bot makes a move
    vector<int> newPiles = gameAlgo(piles);

    // Insert move into database
    insertData(db, gameIdCounter, moveNumber, piles[0], piles[1], piles[2],
               isBot1Turn ? "Bot1" : "Bot2", '-', 0, "", "");

    simulateGame(newPiles, moveNumber + 1, !isBot1Turn);
}

int main() {
    db = openDatabase("gameData.db");
    if (!db) {
        cerr << "Failed to open database. Exiting." << endl;
        return 1;
    }

    createTable(db);

    // Step 1: Ask for maximum pile size
    int maxPile;
    cout << "Enter maximum allowed pile size (>= 20): ";
    cin >> maxPile;
    if (maxPile < 20) {
        cerr << "Invalid maximum pile size. Must be at least 20. Exiting." << endl;
        return 1;
    }

    // Step 2: Ask for starting piles
    initialPiles.resize(3);
    cout << "Enter the starting values for the three piles (each between 20 and " << maxPile << "): ";
    cin >> initialPiles[0] >> initialPiles[1] >> initialPiles[2];

    // Step 3: Validate pile input
    bool valid = true;
    for (int pile : initialPiles) {
        if (pile < 20 || pile > maxPile) {
            valid = false;
            break;
        }
    }

    if (!valid) {
        cerr << "Invalid input. Each pile must be between 20 and " << maxPile << ". Exiting." << endl;
        return 1;
    }

    // Step 4: Simulation loop
    while (true) {
        visitedStates.clear();
        simulateGame(initialPiles, 1, true);
        gameIdCounter++;
    }

    closeDatabase(db);
    return 0;
}
