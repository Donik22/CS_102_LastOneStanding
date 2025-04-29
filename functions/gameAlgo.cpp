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

// Main bot decision-making: randomized subtraction from a random non-empty pile
vector<int> gameAlgo(vector<int> numbers) {
    // Seed randomness only once (ideally this should be done in main, but adding here for simplicity)
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    // Collect indices of non-empty piles
    vector<int> nonEmptyIndices;
    for (int i = 0; i < numbers.size(); ++i) {
        if (numbers[i] > 0) {
            nonEmptyIndices.push_back(i);
        }
    }

    if (!nonEmptyIndices.empty()) {
        int chosenIndex = nonEmptyIndices[rand() % nonEmptyIndices.size()];
        int maxSubtract = numbers[chosenIndex];
        int subtractAmount = (rand() % maxSubtract) + 1; // Random between 1 and current pile value

        numbers[chosenIndex] -= subtractAmount;

        cout << "Bot subtracts " << subtractAmount << " from pile "
             << static_cast<char>('A' + chosenIndex) << ".\n";
    }

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
