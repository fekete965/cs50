#include <cs50.h>
#include <stdio.h>

int main(void) {

  int x = get_int("x: ");
  int y = get_int("y: ");

  // Devide x / y
  float z = (float) x / (float) y;

  printf("%.50f\n", z);
}
