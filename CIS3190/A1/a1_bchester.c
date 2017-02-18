//  DiceGame.c
//  Plays the game of dice
//  Created by Brandon Chester on 2017-01-19.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// These functions are used to play the dice game
void playDiceGame(int32_t firstSeed, int32_t secondSeed, int32_t gamesToPlay);
void diceRoll(int32_t* firstSeed, int32_t* secondSeed, int32_t* randomValues, int32_t* dice);
void printResults(int32_t numberOfWins, int32_t numberOfLosses, int32_t firstSeed, int32_t secondSeed);

int main() {
    int32_t gamesToPlay, firstSeed, secondSeed;
    puts("Enter two seed values and the nmnber of games to play.");
    scanf("%d %d %d" ,&gamesToPlay, &firstSeed, &secondSeed);
    playDiceGame(gamesToPlay, firstSeed, secondSeed);
    return 0;
}

//Play the game until we reach the user's desired number of games
void playDiceGame(int32_t firstSeed, int32_t secondSeed, int32_t gamesToPlay) {
    int32_t repeats, point, diceSum;
    int32_t randomValues[2], dice[2];
    int32_t numberOfWins = 0;
    int32_t numberOfLosses = 0;
    int32_t gamesPlayed = 0;
    
    while (gamesPlayed < gamesToPlay) {
        diceRoll(&firstSeed, &secondSeed, randomValues, dice);
        diceSum = dice[0] + dice[1];
        
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
                diceRoll(&firstSeed, &secondSeed, randomValues, dice);
                diceSum = dice[0] + dice[1];
                
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
                printResults(numberOfWins, numberOfLosses, firstSeed, secondSeed);
                exit(0);
            }
        }
        gamesPlayed = numberOfWins + numberOfLosses;
    }
    printResults(numberOfWins, numberOfLosses, firstSeed, secondSeed);
}

//Generate pseudorandom values between 1 and 6 for dice
void diceRoll(int32_t* firstSeed, int32_t* secondSeed, int32_t* randomValues, int32_t* dice) {
    int32_t firstLimit = 947;
    int32_t secondLimit = 941;
    
    for (int i = 0; i < 2; i++) {
        *firstSeed*=2;
        *secondSeed*=2;
        
        if (*firstSeed >= firstLimit) {
            *firstSeed-=firstLimit;
        }
        
        if (*secondSeed >= secondLimit) {
            *secondSeed-=secondLimit;
        }
        
        randomValues[i] = *firstSeed + *secondSeed;
        if (randomValues[i] >= 1000) {
            randomValues[i]-=1000;
        }
        
        dice[i] = randomValues[i] - 6*(randomValues[i]/6) + 1;
    }
}

//Prints the results of the dice roll game
void printResults(int32_t numberOfWins, int32_t numberOfLosses, int32_t firstSeed, int32_t secondSeed) {
    printf("Wins = %d. Losses = %d. First seed = %d. Second seed = %d.\n",
           numberOfWins, numberOfLosses, firstSeed, secondSeed);
}
