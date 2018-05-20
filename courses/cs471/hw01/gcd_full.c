#include <stdio.h>
//#include "gcd.h"


/* Imperative style modification of example 1.20  */

int gcdI(int i, int j) {
    while ( i != j) { 
        if (i > j) {
            i = i - j;
        } else {
            j = j - i;
        }
    }      
    return i;
}

/* Modiification of 1.4 page 38 */
int  gcdM(int i, int j) {
    while ( i != j) {
        if (i > j) {
            i = i % j;
        } else {
            j = j % i;
        }
    }
    return i;
}

/* ADD code for the recursive style implementation
  Also
  For exercise 4 modify the main function to call gcdR instead of gcdI  
*/

int gcdR(int i, int j) {
    if (i % j == 0) return j;
    return gcdR(j, i % j);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("%s usage: [I] [J]\n", argv[0]);
    return 1;
  }
  int i = atoi(argv[1]);
  int j = atoi(argv[2]);
  printf("gcdI: %d\n", gcdI(i, j));
  printf("gcdR: %d\n", gcdR(i, j));
  return 0;
}




