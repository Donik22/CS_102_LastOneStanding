#include <iostream>
#include <vector>
#include <cstdlib> // Add this at the top of the file if not already included
#include <ctime>   // Add this at the top of the file if not already included
#include "gameAlgo.h"

using namespace std;

// Function to get input for the initial pile values
vector<int> getInput() {
    vector<int> numbers(3);
    cout << "Enter the initial values for piles A, B, and C (separated by spaces): ";
    cin >> numbers[0] >> numbers[1] >> numbers[2];

    // Check if input is valid
    while (cin.fail() || numbers[0] < 20 || numbers[1] < 20 || numbers[2] < 20) {
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter non-negative integers for piles A, B, and C: ";
        cin >> numbers[0] >> numbers[1] >> numbers[2];
    }
    return numbers;
}

// Function to validate the input
bool isValidInput(const vector<int>& numbers) {
    return !(cin.fail() || numbers[0] < 0 || numbers[1] < 0 || numbers[2] < 0);
}

// Function to calculate the sum of the piles
int sum(const vector<int>& numbers) {
    int total = 0;
    for (int num : numbers) {
        total += num;
    }
    return total;
}

bool isWinningPosition(const vector<int>& numbers) {
    int countOnesOrZeros = 0;
    for (int num : numbers) {
        if (num == 0 || num == 1) {
            countOnesOrZeros++;
        }
    }

    // Consider it risky if 2 or more piles are 0 or 1
    if (countOnesOrZeros >= 2) {
        return true;
    }

    // Fallback heuristic: avoid perfectly equal piles
    return !(numbers[0] == numbers[1] && numbers[1] == numbers[2]);
}

vector<int> bestMoveFromCriticalNumbers(const vector<int>& numbers) {
    vector<int> sortedNumbers = numbers;
    vector<int> move = numbers; // default move: no change
    sort(sortedNumbers.begin(), sortedNumbers.end());

    // Define specific critical winning moves
    if (sortedNumbers == vector<int>{0, 1, 1}) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 1) {
                move[i] = 0;
                return move;
            }
        }
    }
    if (sortedNumbers == vector<int>{0, 1, 2}) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 2) {
                move[i] = 0;
                return move;
            }
        }
    }
    if (sortedNumbers == vector<int>{1, 2, 3}) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 3) {
                move[i] = 0;
                return move;
            }
        }
    }
    if (sortedNumbers == vector<int>{2, 3, 5}) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 5) {
                move[i] = 0;
                return move;
            }
        }
    }
    if (sortedNumbers == vector<int>{3, 5, 6}) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 6) {
                move[i] = 2;
                return move;
            }
        }
    }
    // If sorted numbers is 0, 1 , n>0
    if (sortedNumbers[0] == 0 && sortedNumbers[1] == 0 && sortedNumbers[2] >= 2) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == sortedNumbers[2]) {
                move[i] = 1;
                return move;
            }
        }
    }
    if (sortedNumbers[0] == 0 && sortedNumbers[1] == 1 && sortedNumbers[2] > 2) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == sortedNumbers[2]) {
                move[i] = 0;
                return move;
            }
        }
    }

    if (sortedNumbers[0] == 0 && sortedNumbers[1] > 2 && sortedNumbers[2] > 2) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == sortedNumbers[2]) {
                move[i] = 2;
                return move;
            }
        }
    }

    if (sortedNumbers[0] == 0 && sortedNumbers[1] == 2 && sortedNumbers[2] == 2) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == 2) {
                move[i] = 1;
                return move;
            }
        }
    }
    
    /* Example to add an additional condition
    if (sortedNumbers[0] == 0 && sortedNumbers[1] == 1 && sortedNumbers[2] > 2) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] == sortedNumbers[2]) {
                move[i] = 1;
                return move;
            }
        }
    }
    */
    return {}; // No critical best move found
}

// Main bot decision-making: randomized subtraction from a random non-empty pile
vector<int> gameAlgo(vector<int> numbers) {
    vector<int> criticalMove = bestMoveFromCriticalNumbers(numbers);
    if (!criticalMove.empty()) {
        cout << "Bot plays critical best move based on known winning positions.\n";
        return criticalMove;
    }

    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    vector<int> nonEmptyIndices;
    for (int i = 0; i < numbers.size(); ++i) {
        if (numbers[i] > 0) {
            nonEmptyIndices.push_back(i);
        }
    }

    vector<int> bestMove = numbers;
    for (int idx : nonEmptyIndices) {
        for (int sub = 1; sub <= numbers[idx]; ++sub) {
            vector<int> testMove = numbers;
            testMove[idx] -= sub;
            if (!isWinningPosition(testMove)) {
                bestMove = testMove;
                cout << "Bot plays safe move: subtract " << sub << " from pile " << static_cast<char>('A' + idx) << ".\n";
                return bestMove;
            }
        }
    }

    // If no safe move found, fallback to random move
    int chosenIndex = nonEmptyIndices[rand() % nonEmptyIndices.size()];
    int maxSubtract = numbers[chosenIndex];
    int subtractAmount = (rand() % maxSubtract) + 1;

    numbers[chosenIndex] -= subtractAmount;

    cout << "Bot plays risky move: subtract " << subtractAmount << " from pile "
         << static_cast<char>('A' + chosenIndex) << ".\n";

    return numbers;
}

void humanSubtractMove(vector<int>& numbers) {
    char pileChoice;
    int newAmount;
    cout << "Your turn. Enter pile (A, B, or C) followed by the new amount (separated by space): ";
    cin >> pileChoice >> newAmount;
    pileChoice = toupper(pileChoice);

    while (pileChoice < 'A' || pileChoice > 'C') {
        cout << "Invalid pile. Enter A, B, or C: ";
        cin >> pileChoice >> newAmount;
        pileChoice = toupper(pileChoice);
    }

    int pileIndex = pileChoice - 'A';

    // Check if the pile has a valid range for the new amount
    if (numbers[pileIndex] <= 0) {
        cout << "Pile " << pileChoice << " is already empty. You cannot subtract from it.\n";
        return;
    }

    while (newAmount < 0 || newAmount >= numbers[pileIndex]) {
        cout << "Invalid new amount for pile " << pileChoice << ". It must be between 0 and " << numbers[pileIndex] - 1 << ". Try again: ";
        cin >> pileChoice >> newAmount;
        pileChoice = toupper(pileChoice);
        pileIndex = pileChoice - 'A';

        // Exit if the pile becomes invalid during input
        if (numbers[pileIndex] <= 0) {
            cout << "Pile " << pileChoice << " is already empty. You cannot subtract from it.\n";
            return;
        }
    }

    int subtracted = numbers[pileIndex] - newAmount;
    numbers[pileIndex] = newAmount;

    cout << "You changed pile " << pileChoice << " to " << newAmount << " (subtracted " << subtracted << ").\n";
}

void humanAddMove(vector<int>& numbers) {
    int pileChoice, addAmount;
    cout << "Choose a pile to add to (1-3): ";
    cin >> pileChoice;

    while (pileChoice < 1 || pileChoice > 3) {
        cout << "Invalid choice. Choose a pile between 1-3: ";
        cin >> pileChoice;
    }

    cout << "Enter amount to add (must be between 1 and current pile value " << numbers[pileChoice - 1] << "): ";
    cin >> addAmount;

    while (addAmount <= 0 || addAmount >= numbers[pileChoice - 1]) {
        cout << "Invalid amount. Enter an amount greater than 0 and less than " << numbers[pileChoice - 1] << ": ";
        cin >> addAmount;
    }

    numbers[pileChoice - 1] += addAmount;

    cout << "You added " << addAmount << " to pile " << pileChoice << ".\n";
}
