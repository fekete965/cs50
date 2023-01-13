#include <cs50.h>
#include <stdio.h>

int getHeight(void);
void printXTimes(char character, int times);
void buildPyramid(int start, int end);

int main(void)
{
    // Ask user for height
    int height = getHeight();
    // Build the pyramid and prompt it to the user
    buildPyramid(1, height);
}

int getHeight(void)
{
    // Init a hight placeholder
    int height;
    do
    {
        // Ask the user for a height
        height = get_int("Height: ");
    }
    // until it is greater or equal to 0 but less than 8
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

    printXTimes(' ', whiteSpaces);
    printXTimes('#', current);
    printf("\n");

    buildPyramid(current + 1, target);
}
