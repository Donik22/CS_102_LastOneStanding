#include <iostream>
#include <vector>
#include "functions/gameAlgo.h"

using namespace std;

void displayPiles(const vector<int>& numbers) {
    cout << "Piles: ";
    cout << "A=" << numbers[0] << " ";
    cout << "B=" << numbers[1] << " ";
    cout << "C=" << numbers[2] << endl;
}

int main()
{
    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y')
    {
        vector<int> numbers = getInput();
        bool isHumanTurn = false; // Bot moves first

        while (isValidInput(numbers))
        {
            if (!isHumanTurn) {
                numbers = gameAlgo(numbers); // Bot makes a regular move
                cout << "Bot has moved.\n";
                displayPiles(numbers);
                isHumanTurn = true;
            }
            else {
                humanSubtractMove(numbers);
                isHumanTurn = false;
            }

            if (sum(numbers) == 0) {
                if (isHumanTurn) {
                    cout << "You lose\n";
                } else {
                    cout << "Bot loses\n";
                }
                break;
            }
            else if (sum(numbers) == 1) {
                if (isHumanTurn) {
                    cout << "You lose\n";
                } else {
                    cout << "Bot loses\n";
                }
                break;
            }
        }

        if (!isValidInput(numbers)) {
            cout << "Invalid entry\n";
        }

        cout << "Play again? (y/n): ";
        cin >> playAgain;
    }

    return 0;
}
