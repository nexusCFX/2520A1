/* SieveOfAtkin.c.
 * Adapted by Brandon Chester from Wikipedia algorithm for Sieve of Atkin
 * https://en.wikipedia.org/wiki/Sieve_of_Atkin
 * Created on 2017-03-23
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

int main(int argc, const char * argv[]) {
    int limit = 0;
    puts("Enter the limit of the prime numbers you wish to find.");
    scanf("%d",&limit);
    
    //CPU Time Measurement Code
    //clock_t start = clock();
    bool* sieve = calloc(limit + 1, sizeof(bool));
    
    sieve[0] = false;
    sieve[1] = false;
    sieve[2] = true;
    sieve[3] = true;
    
    int sqrtOfLimit = (int)(sqrt((double)limit));
    
    // Wheel factorization
    for (int i = 0; i <= sqrtOfLimit; i++) {
        for (int j = 0; j <= sqrtOfLimit; j++) {
            int quadratic = (4*i*i) + (j*j);
            // Simplified form of Atkins' if quadratic mod 60 E {1,13,17,29,37,41,49,53}
            if ((quadratic % 12 == 1 || quadratic % 12 == 5) && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
            
            quadratic = (3*i*i) + (j*j);
            // Simplified form of Atkins' if quadratic mod 60 E {7,19,31,43}
            if (quadratic % 12 == 7 && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
            
            quadratic = (3*i*i) - (j*j);
            // Simplified form of Atkins' if quadratic mod 60 E {11,23,47,59} and x > y
            if (quadratic % 12 == 11 && i > j && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
        }
    }
    
    // Sieve occurences marked as possibly prime on the wheel
    for (int k = 5; k < sqrtOfLimit; k++) {
        if (sieve[k]) {
            int a = k*k;
            for (int i = a; i <= limit; i = i + a) {
                sieve[i] = false;
            }
        }
    }
    /* //CPU Time Measurement Code
     clock_t end = clock();
     double executionTime = (double)(end - start) / CLOCKS_PER_SEC;
     printf("CPU execution time: %lf seconds\n", executionTime);
     */
    
    FILE* outputFile = fopen("CPrimes.txt", "w");
    fprintf(outputFile, "All primes up to %d\n",limit);
    // Ignore 0 and 1 since primes are natural numbers > 1
    for (int i = 2; i <= limit; i++) {
        if (sieve[i]) {
            fprintf(outputFile, "%d\n",i);
        }
    }
    fclose(outputFile);
}
