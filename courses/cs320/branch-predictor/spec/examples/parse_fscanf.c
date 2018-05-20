/* Example
 * Parsing Branch Traces
 */

#include<stddef.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[]) {

  // Temporary variables
  unsigned long long addr;
  char behavior[10];

  // Open file for reading
  FILE *input = fopen("./test_input.txt", "r");

  // The following loop will read a hexadecimal number and
  // a string each time and then output them
  while(fscanf(input, "%llx %10s\n", &addr, behavior) != EOF) {
    if(!strncmp(behavior, "T", 2)) {
      printf("%lld -> taken\n", addr);
    }else {
      printf("%lld -> not taken\n", addr);
    }
  }

  return 0;
}
