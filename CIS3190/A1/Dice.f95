        ! Dice.f95.
        ! Adapted by Brandon Chester from game of dice written by Dr. Michael Wirth
        ! Created on 2017-01-12

        ! Main program
        program DiceGame
        implicit none

        integer :: gamesToPlay = 0
        integer :: firstSeed = 0
        integer :: secondSeed = 0

        ! Grab two seed values and number of games to play
        write(*,*) "Enter stuff."

        read 100, firstSeed, secondSeed, gamesToPlay
100     format(I5, I5, I10)

        call playDiceGame(firstSeed, secondSeed, gamesToPlay)

        end program DiceGame


        ! Play the game until we reach the user's desired number of games
        subroutine playDiceGame(firstSeed, secondSeed, gamesToPlay)
            implicit none
            integer :: numberOfWins = 0
            integer :: numberOfLosses = 0
            integer :: gamesPlayed = 0
            integer :: repeats = 0

            integer :: firstSeed
            integer :: secondSeed
            integer :: gamesToPlay

            integer :: point = 0
            integer :: diceSum = 0
            integer, dimension(2) :: randomValues
            integer, dimension(2) :: dice

            do while(gamesPlayed < gamesToPlay)
                call diceRoll(firstSeed, secondSeed, randomValues, dice)
                diceSum = dice(1) + dice(2)

                ! 7 or 11 are win conditions, 2, 3, and 12 are loss conditions
                if (diceSum == 7 .or. diceSum == 11) then
                    numberOfWins = numberOfWins + 1
                else if (diceSum == 2 .or. diceSum == 3 .or. diceSum == 12) then
                    numberOfLosses = numberOfLosses + 1
                else
                    ! If the roll was not a win or a loss keep track of sum, play again
                    point = diceSum
                    repeats = 0

                    ! Limit of 100 retries before assuming a bad seed led to a loop
                    do while(repeats <= 100)
                        call diceRoll(firstSeed, secondSeed, randomValues, dice)
                        diceSum = dice(1) + dice(2)

                        ! If new roll is equal to the original sum, user wins
                        ! If new roll is equal to 7, user loses
                        if (diceSum == point) then
                            numberOfWins = numberOfWins + 1
                            exit
                        else if (diceSum == 7) then
                            numberOfLosses = numberOfLosses + 1
                            exit
                        end if
                        repeats = repeats + 1
                    end do

                    ! If re-rolls occur 100 times in a row the seed probably led to a loop; end program
                    if (repeats > 100) then
                        write(*,*) "Repeats exceeded limit of 100"
                        call printResults(numberOfWins, numberOfLosses, firstSeed, secondSeed)
                        stop
                    end if
                end if
                gamesPlayed = numberOfLosses + numberOfWins
            end do
            call printResults(numberOfWins, numberOfLosses, firstSeed, secondSeed)
        end subroutine

        ! Generate pseudorandom values between 1 and 6 for dice
        subroutine diceRoll(firstSeed, secondSeed, randomValues, dice)
            implicit none
            integer :: i = 0
            integer :: firstLimit = 947
            integer :: secondLimit = 941

            integer, intent(inout) :: firstSeed
            integer, intent(inout) :: secondSeed

            integer, dimension(2), intent(inout) :: randomValues
            integer, dimension(2), intent(inout) :: dice

            do i = 1,2
                firstSeed = firstSeed + firstSeed
                secondSeed = secondSeed + secondSeed
                if (firstSeed >= firstLimit) then
                    firstSeed = firstSeed - firstLimit
                end if

                if (secondSeed >= secondLimit) then
                    secondSeed = secondSeed - secondLimit
                end if

                randomValues(i) = firstSeed + secondSeed
                if (randomValues(i) >= 1000) then
                    randomValues(i) = randomValues(i) - 1000
                end if

                ! Integer division truncation causes 6*(randomValues(i)/6) to return a value
                ! in the range randomValues(i) - 5 to randomValues(i). By subtracting this
                ! and adding 1, we get a value between 1 and 6
                dice(i) = randomValues(i) - 6*(randomValues(i)/6) + 1
            end do
        end subroutine


        ! Prints the results of the dice roll game
        subroutine printResults(numberOfWins, numberOfLosses, firstSeed, secondSeed)
            implicit none
            integer :: numberOfWins, numberOfLosses, firstSeed, secondSeed

            write(*,200) numberOfWins,numberOfLosses,firstSeed,secondSeed
200         format('WINS = ',I8, ' LOSSES = ',I8, ' I = ', I4, ' J = ',I4)
        end subroutine
