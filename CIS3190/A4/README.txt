To compile:

C: The -lm flag is required to link the math library on Linux
gcc sieveofatkin.c -std=c99 -Wall -pedantic -lm -o CSieve

Fortran:
gfortran sieveofatkin.f95 -Wall -pedantic -o FortranSieve

Ada:
gnatmake sieveofatkin.adb -o AdaSieve