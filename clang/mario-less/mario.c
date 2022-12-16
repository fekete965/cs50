#include <cs50.h>
#include <stdio.h>

int getHeight(void);

void printPyramid(int height);

int main(void)
{
    // Ask user for height
    int height = getHeight();
    // Build the pyramid and prompt it to the user
    printPyramid(height);
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

void printPyramid(int height)
{
    // Rows
    for (int i = 0; i < height; i++)
    {
        // Columns
        for (int j = height; j >= 0; j--)
        {
            // Only print # if the current column number is less or equal to the row number
            if (i >= j)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        // Start a new line
        printf("\n");
    }
}
