#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Pre-define constants
const int ALPHABET_COUNT = 26;
const int LOWERCASE_A = 97;
const int LOWERCASE_Z = 122;
const int NUL = 0;
const int UPPERCASE_A = 65;
const int UPPERCASE_Z = 90;

// Pred-efine function prototypes
string cipher(int key, string text);

int main(int argc, string argv[]) 
{
    // End the program if we are missing the necessary argument(s)
    if (argc != 2)
    {
        printf("Missing command line argument\n");
        return 1;
    }

    // Convert string numeric to int numeric
    int key = atoi(argv[1]);

    // Ask the user for a text
    string plainText = get_string("plaintext:  ");
    // Cipher the text
    string cipherText = cipher(key, plainText);
    // Prompt the cyphered text
    printf("ciphertext: %s\n", cipherText);
    // End the program
    return 0;
}

string cipher(int key, string text)
{
    // Pre-declare variables
    int i = 0, charDec, cipherCharDec;
    string cipherText;

    int test;
    // Loop throught the string
    do
    {
        // Cast character to int
        charDec = (int) text[i];
        
        // Stop the loop if we reached the end of the string
        if (charDec == NUL)
        {
            break;
        }
        // Check if the character is between lowercase a and z
        if (charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z)
        {
            // Perform the calculation
            cipherCharDec = LOWERCASE_A + ((charDec + key - LOWERCASE_A) % ALPHABET_COUNT);
        }
        // Check if the character is between uppercase a and z
        else if (charDec >= UPPERCASE_A && charDec <= UPPERCASE_Z)
        {
            // Perform the calculation
            cipherCharDec = UPPERCASE_A + ((charDec + key - UPPERCASE_A) % ALPHABET_COUNT);
        }
        // Otherwise use the same character
        else
        {
            cipherCharDec = charDec;
        }
        // Force cast int to readable character 
        cipherText[i] = (char) cipherCharDec;
        // Increment counter
        i++;
    }
    while (true);

    return cipherText;
}
