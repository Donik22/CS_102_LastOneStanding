#include <iostream>
#include "gameAlgo.h"
#include <sqlite3.h> 
#include <set>

// Create a database of winning and losing positions
/*
CREATE TABLE GameMoves (
    game_id INT,
    move_number INT,
    pile_A INT,
    pile_B INT,
    pile_C INT,
    who_moved VARCHAR(10),
    pile_chosen CHAR(1),
    amount_subtracted INT,
    new_pile_value INT,
    game_result VARCHAR(10) NULL
);
*/
// Function to open the database
sqlite3* openDatabase(const string& dbName) {
    sqlite3* db;
    int exit = sqlite3_open(dbName.c_str(), &db);
    if (exit) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    cout << "Database opened successfully!" << endl;
    return db;
}

// Function to create the GameMoves table
void createTable(sqlite3* db) {
    char* errorMessage = 0;

    // Create the GameMoves table if it doesn't exist
    string createGameMovesTable = R"(
        CREATE TABLE IF NOT EXISTS GameMoves (
            game_id INT,
            move_number INT,
            pile_A INT,
            pile_B INT,
            pile_C INT,
            who_moved VARCHAR(10),
            pile_chosen CHAR(1),
            amount_subtracted INT,
            new_pile_value INT,
            game_result VARCHAR(10) NULL
        );
    )";

    if (sqlite3_exec(db, createGameMovesTable.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating GameMoves table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    // Create the UniqueCombinations table if it doesn't exist
    string createUniqueCombinationsTable = R"(
        CREATE TABLE IF NOT EXISTS UniqueCombinations (
            pile_A INT NOT NULL,
            pile_B INT NOT NULL,
            pile_C INT NOT NULL,
            PRIMARY KEY (pile_A, pile_B, pile_C)
        );
    )";

    if (sqlite3_exec(db, createUniqueCombinationsTable.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating UniqueCombinations table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

// Function to insert data into the GameMoves table
void insertData(sqlite3* db, int gameId, int moveNumber, int pileA, int pileB, int pileC, const string& whoMoved, char pileChosen, int amountSubtracted, int newPileValue, const string& gameResult) {
    if (!db) return;

    string insertSQL = "INSERT INTO GameMoves (game_id, move_number, pile_A, pile_B, pile_C, who_moved, pile_chosen, amount_subtracted, new_pile_value, game_result) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    // Prepare the SQL statement
    int exit = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind values to the SQL statement
    sqlite3_bind_int(stmt, 1, gameId);
    sqlite3_bind_int(stmt, 2, moveNumber);
    sqlite3_bind_int(stmt, 3, pileA);
    sqlite3_bind_int(stmt, 4, pileB);
    sqlite3_bind_int(stmt, 5, pileC);
    sqlite3_bind_text(stmt, 6, whoMoved.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, &pileChosen, 1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, amountSubtracted);
    sqlite3_bind_int(stmt, 9, newPileValue);
    sqlite3_bind_text(stmt, 10, gameResult.c_str(), -1, SQLITE_STATIC);

    // Execute the SQL statement
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Data inserted successfully!" << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}

// Function to close the database
void closeDatabase(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
        cout << "Database connection closed." << endl;
    }
}

bool isCombinationUnique(sqlite3* db, const vector<int>& numbers) {
    string query = "SELECT COUNT(*) FROM UniqueCombinations WHERE pile_A = ? AND pile_B = ? AND pile_C = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, numbers[0]);
    sqlite3_bind_int(stmt, 2, numbers[1]);
    sqlite3_bind_int(stmt, 3, numbers[2]);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count == 0; // Return true if the combination is unique
}

void insertUniqueCombination(sqlite3* db, const vector<int>& numbers) {
    string query = "INSERT INTO UniqueCombinations (pile_A, pile_B, pile_C) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, numbers[0]);
    sqlite3_bind_int(stmt, 2, numbers[1]);
    sqlite3_bind_int(stmt, 3, numbers[2]);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void batchInsertUniqueCombinations(sqlite3* db, const set<vector<int>>& combinations) {
    string query = "INSERT INTO UniqueCombinations (pile_A, pile_B, pile_C) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    for (const auto& combination : combinations) {
        sqlite3_bind_int(stmt, 1, combination[0]);
        sqlite3_bind_int(stmt, 2, combination[1]);
        sqlite3_bind_int(stmt, 3, combination[2]);
        sqlite3_step(stmt);
        sqlite3_reset(stmt); // Reset the statement for the next insert
    }

    sqlite3_finalize(stmt);
}

void updateGameResult(sqlite3* db, int gameId, const string& winner) {
    string query = "UPDATE GameMoves SET game_result = ? WHERE game_id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, winner.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, gameId);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
