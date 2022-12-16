#include <cs50.h>
#include <stdio.h>

// FN placeholders
int getHeight(void);
void buildPyramid(int height);
const int DELTA = 1;

int main(void)
{
    // Ask user for height
    int height = getHeight();
    // Build the double pyramid
    buildPyramid(height);
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

void buildPyramid(int height)
{
    // Define total rows
    int rows = height;
    // Define total columns
    int cols = height * 2;

    // Iterate on rows
    for (int i = 0; i < rows; i++)
    {
        // Iterate on columns
        for (int j = 0; j <= cols; j++)
        {
            // Check if we should to build the left half of the pyramind
            bool isLeft = j >= height - i - DELTA;
            // Check if we should to build the left right of the pyramind
            bool isRight = j - height <= i + DELTA;

            // Print # if we are on the left hand side and should build
            if (j < height && isLeft)
            {
                printf("#");
            }
            // Print # if we are on the reight hand side and should build
            else if (j > height && isRight)
            {
                printf("#");
            }
            // Otherwise, we are in the middle hence we should leave an empty space
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
