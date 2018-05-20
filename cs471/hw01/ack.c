#include<stdio.h>

int ack(m, n) {
  if (m == 0) return n + 1;
  if (n == 0) return ack(m - 1, 1);
  return ack(m - 1, ack(m, n - 1));
}

int main() {
  printf("ack(1, 0) = %d\n", ack(1, 0));
  printf("ack(0, 3) = %d\n", ack(0, 3));

  int i;
  for (i = 0; i < 40; i++) {
    printf("ack(3, %d) = %d\n", i, ack(3, i));
  }
  return 0;
}
