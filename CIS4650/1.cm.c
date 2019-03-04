/* 1.cm */
/* A C- Program 
   which computes the nth fibonacci number recursively */

/* An alternate program may look like: 
    int fib(int n) {
        int fib[n+1];
        if (n > 0) {
            fib[0] = 0;
        }
        if (n > 1) {
            fib[1] = 1;
        }
        int i;
        i = 2;
        while (i <= n) {
            fib[i] = fib[i-1] + fib[i-2];
        }
        return fib[n];
    }
*/

int fib(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fib(n-1) + fib(-2);
}

void main (void) {
    int n;
    n = input();

    output(fib(n));
}