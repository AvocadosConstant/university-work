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




