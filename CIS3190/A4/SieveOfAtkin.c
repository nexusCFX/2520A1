//
//  main.c
//  SieveOfAtkin
//
//  Created by Brandon Chester on 2017-03-21.
//  Copyright © 2017 Brandon Chester. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    int limit = 0;
    puts("Enter the limit of the prime numbers you wish to find.");
    scanf("%d",&limit);
    bool* sieve = calloc(limit + 1, sizeof(bool));
    
    sieve[0] = false;
    sieve[1] = false;
    sieve[2] = true;
    sieve[3] = true;
    
    int sqrtOfLimit = (int)(sqrt((double)limit));
    for (int i = 0; i <= sqrtOfLimit; i++) {
        for (int j = 0; j <= sqrtOfLimit; j++) {
            int quadratic = (4*i*i) + (j*j);
            if ((quadratic % 12 == 1 || quadratic % 12 == 5) && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
            
            quadratic = (3*i*i) + (j*j);
            if (quadratic % 12 == 7 && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
            
            quadratic = (3*i*i) - (j*j);
            if (quadratic % 12 == 11 && i > j && quadratic <= limit) {
                sieve[quadratic] = !sieve[quadratic];
            }
        }
    }
    
    for (int k = 5; k < sqrtOfLimit; k++) {
        if (sieve[k]) {
            int a = k*k;
            for (int l = a; l <= limit; l = l + a) {
                sieve[l] = false;
            }
        }
    }
    
    // Ignore 0 and 1 since primes are natural numbers > 1
    for (int i = 2; i <= limit; i++) {
        if (sieve[i]) {
            printf("%d\n",i);
        }
    }
}
