#include <cs50.h>
#include <stdio.h>

int main(void)
{
    const int points = get_int("How many points did you lose? ");
    int MINE = 2;

    if (points < MINE)
    {
        printf("You lost fewer points than me.\n");
    }
    else if (points > MINE)
    {
        printf("You lost more points than me.\n");
    }
    else
    {
        printf("You lose the same point as me.\n");
    }
}
