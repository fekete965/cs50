#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  string names[] = { "Bill", "Charlie", "Fred", "George", "Percy", "Ron"};

  for (int i = 0; i < 7; i++)
  {
    if (strcmp(names[i], "Ron") == 0)
    {
      printf("Found\n");
      return 0;
    }
  }
  printf("Not Found");
  return 1;
}
