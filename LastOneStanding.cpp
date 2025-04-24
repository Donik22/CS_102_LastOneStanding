/*
Group 1
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

using namespace std;



bool turn;
// A function that takes 3 integer inputs, Processes them and returns 3 integer outputs.

int gameAlgo(int a, int b, int c) {
cout << "what are the three numbers for your game"<<endl;
cin>> a >> b >> c;
cout << "type true if it's your turn and false if it is not your turn"<<endl;
cin >> turn;
if(turn == true) 
{

    a = 0;

    cout << "Integers: " << a << " " << b << " " << c << "\n" ;
    cin>> a >> b >> c;
    if(b <= 3 && c <=3) 
    {

        b = 1;
        
        cout << "New integers: " << a << " " << b << " " << c << "\n" ;
        cin>> a >> b >> c;
        if(b<=1 && c<= 2)
        {
            c = 0;
            cout << "New integers: " << a << " " << b << " " << c << "\n" ;
            cin>> a >> b >> c;
            
            
        }

    }
    

}



else if(turn == false)
{
    
    cout << "Integers: " << a << " " << b << " " << c << "\n" ;
    cin>> a >> b >> c;
   

}

else if(cin.fail())
{
gameAlgo(a,b,c);
}

}

// Function to sum three integers
int sum(int a, int b, int c) {
    return a + b + c;
}

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