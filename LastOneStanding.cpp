/*
Work Group 
date 02/20/2024
List of students

Largest Prime Group 1 Names:
Armando rosales
Salih Mohammed
Neha Jindal
Octavio perez

*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <tuple>
#include "functions/gameAlgo.h"

using namespace std;

int main()
{
    cout << "Enter three integers \n"; //Taking 3 inputs from the user
    
    int x , y , z;
    cin >> x >> y >> z;

    while(!cin.fail() || (x!=0 && y!=0 && z!=0)) //Looping through 
    {
        if (cin.fail() || (x<0 && y<0 && z<0))
        {
            cout << "Invalid entry \n";
            break; //for testing
        }
        else if ((x>0 || y>0 || z>0)) // GAME ON ! 
        {
            tie(x, y, z) = gameAlgo(x, y, z); //Update new 3 digits with function output
            if (sum(x,y,z) == 0) //Losing condition
                {
                    cout << "You lose \n";
                    break;
                }
            else
            {
                cout << "Waiting for another 3 digits: ";
                cin >> x >> y >> z;
            }   
            
        }
        else if (sum(x,y,z) == 1) //Winning condition
        {
            cout << "You win \n";
            break;
        } 

    }
}

/*
Game rule Conditions to be added
1) You can only change 1 number at a time
2) For the first player, input will have to be grater or equal to 20
3) Function output cannot be negative
4) Make sure the sume of player2 input is at least 1 number less than the prevous plater 1 output
*/

