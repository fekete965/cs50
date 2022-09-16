#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void printUppercase(string s);

int main(void) {

  string s = get_string("Before: ");
  printf("After: ");

   for (int i = 0, n = strlen(s); i < n; i ++) {
    char current = s[i];

    if (islower(current)) {
      printf("%c", toupper(current));
    } else {
      printf("%c", current);
    }
  }
  printf("\n");

}

// Custom implementation
void printUppercase(string s) {
  for (int i = 0, n = strlen(s); i < n; i ++) {
    char current = s[i];

    if (current >= 'a' || current <= 'z') {
      current = (int) current - 32;
    }
      printf("%c", current);
  }
  printf("\n");
}
