#include <cs50.h>
#include <stdio.h>

int getStartSize();
int getEndSize(int start);
int calculateYears(int start, int end);

const int MIN_POPULATION = 9;

int main(void)
{
    // Prompt for start size
    int startSize = getStartSize();

    // Prompt for end size
    int endSize = getEndSize(startSize);

    // Calculate number of years until we reach threshold
    int years = calculateYears(startSize, endSize);

    // Print number of years
    printf("Years: %i\n", years);

    return 0;
}

int getStartSize()
{
    int startSize = 0;

    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < MIN_POPULATION);

    return startSize;
}

int getEndSize(int start)
{
    int endSize = 0;

    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < start);

    return endSize;
}


int calculateYears(int start, int end)
{
    int years = 0;

    while (start < end)
    {
        start = start + start / 3 - start / 4;
        years++;
    }

    return years;
}
