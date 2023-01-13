#include <cs50.h>
#include <stdio.h>
#include <math.h>

// Define constants
const int DOT = 46;
const int EXCLAMATION_MARK = 33;
const int LOWERCASE_A = 65;
const int LOWERCASE_Z = 90;
const int NUL = 0;
const int QUESTION_MARK = 63;
const int UPPERCASE_A = 97;
const int UPPERCASE_Z = 122;
const int WHITE_SPACE = 32;

// Define function prototypes
int *gatherData(string text);

int main(void)
{
    string text = get_string("Text: ");

    // Tuple [letters, words, sentences]
    int *textData = gatherData(text);

    // Calculate the book's grade
    // L is the average number of letters per 100 words
    double L = (double)textData[0] / (double)textData[1] * (double)100;

    // S is the average number of sentences per 100 words
    double S = (double)textData[2] / (double)textData[1] * (double)100;
    double grade = roundf(0.0588 * L - 0.296 * S - 15.8);

    // Print out the grade accordingly
    if (grade < 1.0)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)grade);
    }
}

// Define a function which count the letters, words and senteces in a given text
int *gatherData(string text)
{
    // We defined tuple [letters, words, sentences]
    static int data[3] = {0, 0, 0};
    int i = 0, charDec;

    do
    {
        int charDec = text[i];

        // We only break out of the loop when we hit nul
        if (charDec == NUL)
        {
            break;
        }

        // Check for WHITE_SPACE
        if (charDec == WHITE_SPACE)
        {
            // Increase word count
            data[1]++;
        }
        // Check for DOT, EXCLAMATION_MARK and QUESTION_MARK
        else if (charDec == DOT || charDec == EXCLAMATION_MARK || charDec == QUESTION_MARK)
        {
            data[2]++;

            // Increase the word count if we reached the end of the text
            if (text[i + 1] == NUL)
            {
                data[1]++;
            }
        }
        // Check if character is a-zA-Z
        else if ((charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z) || (charDec >= UPPERCASE_A && charDec <= UPPERCASE_Z))
        {
            data[0]++;
        }

        i++;
    }
    while (true);

    return data;
}
