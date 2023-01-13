#include <cs50.h>
#include <stdio.h>

int getHeight(void);
void buildPyramid(int current, int target);
void printXTimes(char character, int times);

int main(void)
{
    // Ask user for height
    int height = getHeight();
    // Build the double pyramid
    buildPyramid(1, height);
}

int getHeight(void)
{
    int height;
    do
    {
        // Ask user for integer input
        height = get_int("Height: ");
    }
    // Until value is more than 0 but less than 9
    while (height <= 0 || height > 8);
    return height;
}

void printXTimes(char character, int times)
{
    for (int i = 0; i < times; i++)
    {
        printf("%c", character);
    }
}

void buildPyramid(int current, int target)
{
    if (current > target)
    {
        return;
    }

    int whiteSpaces = target - current;

    // Print white spaces on the left hand side
    printXTimes(' ', whiteSpaces);
    // Print "blocks" on the left hand side
    printXTimes('#', current);
    // Print white spaces on the right hand side
    printXTimes(' ', 2);
    // Print "blocks" on the right hand side
    printXTimes('#', current);
    printf("\n");

    buildPyramid(current + 1, target);
}
