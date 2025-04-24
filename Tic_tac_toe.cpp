#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <unordered_map>
using namespace std;

// Function to check if game is over
bool isGameOver(const vector<int>& numbers) {
    return numbers[0] == 0 && numbers[1] == 0 && numbers[2] == 0;
}

// Display current numbers
void display(const vector<int>& numbers) {
    cout << "Current numbers: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        cout << numbers[i] << " ";
    }
    cout << endl;
}

vector<int> simulateMove(vector<int> state, int index, int amount) {
    state[index] -= amount;
    return state;
}

unordered_map<string, bool> memo;

string encodeState(const vector<int>& state, bool isBotTurn) {
    return to_string(state[0]) + "," + to_string(state[1]) + "," + to_string(state[2]) + ":" + to_string(isBotTurn);
}

bool isLosingPositionMemo(vector<int> state, bool isBotTurn) {
    string key = encodeState(state, isBotTurn);
    if (memo.count(key)) return memo[key];
    if (isGameOver(state)) return memo[key] = !isBotTurn;

    for (int i = 0; i < 3; ++i) {
        for (int amt = 1; amt <= state[i]; ++amt) {
            vector<int> nextState = state;
            nextState[i] -= amt;
            if (!isLosingPositionMemo(nextState, !isBotTurn)) {
                return memo[key] = false;
            }
        }
    }
    return memo[key] = true;
}

int evaluateOpponentResponse(vector<int> state) {
    for (int i = 0; i < 3; ++i) {
        for (int amt = 1; amt <= state[i]; ++amt) {
            vector<int> nextState = state;
            nextState[i] -= amt;
            if (!isLosingPositionMemo(nextState, true)) {
                return 1; // opponent has a winning move
            }
        }
    }
    return 0; // opponent cannot avoid losing
}

pair<int, int> simulateBotBestMove(vector<int> numbers) {
    pair<int, int> bestMove = {-1, -1}; // {index, amount}
    int bestOutcome = -1;

    for (int i = 0; i < 3; ++i) {
        for (int amt = 1; amt <= numbers[i]; ++amt) {
            vector<int> newState = simulateMove(numbers, i, amt);

            if (isGameOver(newState)) {
                return {i, amt}; // winning move
            }

            if (isLosingPositionMemo(newState, false)) continue;

            // Misère logic
            int nonZeros = count_if(newState.begin(), newState.end(), [](int x) { return x > 0; });
            int ones = count(newState.begin(), newState.end(), 1);
            int overOnes = nonZeros - ones;

            if (overOnes <= 1) {
                // Misère logic: leave an odd number of 1s if you're not finishing the game
                if (overOnes == 1 && ones % 2 == 0) continue;
                if (overOnes == 0 && ones % 2 == 1) continue;
            }

            int opponentScore = evaluateOpponentResponse(newState);
            if (opponentScore > bestOutcome) {
                bestOutcome = opponentScore;
                bestMove = {i, amt};
            }
        }
    }
    if (bestMove.first == -1 || bestMove.second <= 0) {
        for (int i = 0; i < 3; ++i) {
            if (numbers[i] > 0) {
                return {i, 1}; // Fallback to a safe move
            }
        }
    }
    return bestMove;
}

int botChoiceNim(vector<int>& numbers) {
    int xorSum = numbers[0] ^ numbers[1] ^ numbers[2];

    // Try to make the move that results in a zero XOR sum
    for (int i = 0; i < 3; ++i) {
        int target = numbers[i] ^ xorSum;
        if (target < numbers[i]) {
            return i;
        }
    }

    // If no strategic move found, just return the first available
    for (int i = 0; i < 3; ++i) {
        if (numbers[i] > 0)
            return i;
    }

    return -1;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<int> numbers(3);
    cout << "Enter three numbers greater than 20 (separated by spaces): ";
    for (int& number : numbers) {
        cin >> number;
        while (number <= 20) {
            cout << "Each number must be greater than 20. Re-enter: ";
            cin >> number;
        }
    }

    int currentPlayer = rand() % 2; // Randomly choose who starts: 0 = human, 1 = bot
    cout << (currentPlayer == 0 ? "You" : "Bot") << " will start the game.\n";

    while (!isGameOver(numbers)) {
        display(numbers);

        size_t choice = -1;
        if (currentPlayer == 0) {
            // Human player
            do {
                cout << "Your turn. Choose a number to subtract from (1-3): ";
                int tempChoice;
                cin >> tempChoice;
                choice = static_cast<size_t>(tempChoice - 1);
            } while (choice >= 3 || numbers[choice] == 0);

            int amount;
            do {
                cout << "Enter amount to subtract (1 to " << numbers[choice] << "): ";
                cin >> amount;
            } while (amount < 1 || amount > numbers[choice]);
            numbers[choice] -= amount;
        } else {
            // Bot player
            pair<int, int> botMove = simulateBotBestMove(numbers);
            choice = static_cast<size_t>(botMove.first);
            int amount = botMove.second;
            if (choice < 3 && numbers[choice] > 0 && amount >= 1 && amount <= numbers[choice]) {
                cout << "Bot subtracts " << amount << " from number " << (choice + 1) << ".\n";
                numbers[choice] -= amount;
            } else {
                // fallback safety move
                for (size_t i = 0; i < 3; ++i) {
                    if (numbers[i] > 0) {
                        cout << "Bot defaulted to subtract 1 from number " << (i + 1) << ".\n";
                        numbers[i] -= 1;
                        break;
                    }
                }
            }
        }

        if (isGameOver(numbers)) {
            display(numbers);
            cout << "All numbers are now 0!\n";
            cout << (currentPlayer == 0 ? "You" : "Bot") << " lose the game.\n";
            break;
        }

        currentPlayer = 1 - currentPlayer; // Switch turn
    }

    return 0;
}
