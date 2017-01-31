//  DiceGame.c
//  Plays the game of dice
//  Created by Brandon Chester on 2017-01-19.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// These functions are used to play the dice game
void playDiceGame(int32_t gamesToPlay);
int32_t diceRoll();
void printResults(int32_t numberOfWins, int32_t numberOfLosses);

int main() {
    int32_t gamesToPlay = 0;
    puts("Enter the number of games you would like to play.");
    scanf("%d" ,&gamesToPlay);
    playDiceGame(gamesToPlay);
    return 0;
}

//Play the game until we reach the user's desired number of games
void playDiceGame(int32_t gamesToPlay) {
    int32_t numberOfWins = 0;
    int32_t numberOfLosses = 0;
    int32_t gamesPlayed = 0;
    int32_t repeats = 0;
    
    int32_t point = 0;
    int32_t diceSum = 0;
    int32_t firstDice = 0;
    int32_t secondDice = 0;
    
    srand(time(NULL));
    
    while (gamesPlayed < gamesToPlay) {
        firstDice = diceRoll();
        secondDice = diceRoll();
        diceSum = firstDice + secondDice;
        
        //7 or 11 are win conditions, 2, 3, and 12 are loss conditions
        if (diceSum == 7 || diceSum == 11) {
            numberOfWins++;
        } else if (diceSum == 2 || diceSum == 3 || diceSum == 12) {
            numberOfLosses++;
        } else {
            //If the roll was not a win or a loss keep track of sum, play again
            point = diceSum;
            repeats = 0;
            
            //Limit of 100 retries before assuming a bad seed led to a loop
            while(repeats <= 100) {
                firstDice = diceRoll();
                secondDice = diceRoll();
                diceSum = firstDice + secondDice;
                
                //If new roll is equal to the original sum, user wins
                //If new roll is equal to 7, user loses
                if (diceSum == point) {
                    numberOfWins++;
                    break;
                } else if (diceSum == 7) {
                    numberOfLosses++;
                    break;
                }
                repeats++;
            }
            
            //If re-rolls occur 100 times in a row the seed probably led to a loop; end program
            if (repeats > 100) {
                puts("repeats exceeded limit of 100");
                printResults(numberOfWins, numberOfLosses);
                exit(0);
            }
        }
        gamesPlayed = numberOfWins + numberOfLosses;
    }
    printResults(numberOfWins, numberOfLosses);
}

//Generate pseudorandom values between 1 and 6 for dice
int32_t diceRoll() {
    int32_t diceValue = (rand() % 6) + 1;
    return diceValue;
}

//Prints the results of the dice roll game
void printResults(int32_t numberOfWins, int32_t numberOfLosses) {
    printf("Wins = %d. Losses = %d\n",numberOfWins, numberOfLosses);
}
