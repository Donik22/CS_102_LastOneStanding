# CS_102_LastOneStanding
CS_102 final group project

## Overview
This program will make decision on the best possible move to win a game (Last one standing).

## The game
This game is played by 2 players. To start the game Player 1 should give three digits greater or equal to 20. Player 2 & player 1 will then take turns subtracting a number from one of those three numbers until all are Zeros. The person who ends up with 3 zeros (0, 0, 0) after his turn losses.

## Program expectations
The program should repetedly prompt user to enter 3 inputs and make the best decision on which of the three integers should be subtracted and by how much. All while staing within the rules of the game and flagging invalid inputs.

### Files description
gameAlgo.cpp: Only handle game logic (Bot decisions, Human moves).
dbHandler.cpp: Only handle database logic (open DB, insert moves, close DB).
simulation.cpp: Only run simulations (call game logic + DB logic).
gameAlgo.h: Declare game-related functions.
dbHandler.h: Declare database-related functions.


[Git basics resources](https://louisanatalikaj.medium.com/git-commands-that-are-useful-in-collaborative-development-396deee22ee5)


