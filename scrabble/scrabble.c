#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Define constant values
const int NUL = 0;
const int LOWERCASE_A = 97;
const int LOWERCASE_Z = 122;
const int UPPERCASE_A = 65;
const int UPPERCASE_Z = 90;
// Points assigned to each letter of the alphabet
const int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Pre-define function prototypes
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // If score 1 bigger, Player 1 won
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        return 0;
    }

    // If score 2 bigger, Player 2 won
    if (score2 > score1)
    {
        printf("Player 2 wins!\n");
        return 0;
    }

    // Otherwise it's a tie
    printf("Tie!\n");
    return 0;
}

int compute_score(string word)
{
    // Define our variables
    int i = 0, score = 0, charDec;

    // Start our loop
    do
    {
        // Get the character decimal equivalent
        charDec = (int) word[i];
        
        // Stop the loop once we have reached the end of the string
        if (charDec == NUL)
        {
            break;
        }

        // Check for lowecase character
        if (charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z)
        {
            score += POINTS[charDec - LOWERCASE_A];
        }

        // Check for uppercase character
        if (charDec >= UPPERCASE_A && charDec <= UPPERCASE_Z)
        {
            score += POINTS[charDec - UPPERCASE_A];
        }

        i++;
    }
    while (true);
    
    // Return final score
    return score;
}
