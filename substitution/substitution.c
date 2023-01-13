#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Pre-define constant values
const int LOWERCASE_A = 97;
const int LOWERCASE_Z = 122;
const int NUL = 0;
const int UPPERCASE_A = 65;
const int UPPERCASE_Z = 90;
const int KEY_LENGTH = 26;

// Pre-define function prototypes
int isCharAlreadyEncountered(int *tempArr, int charDec);
char *cipherMe(char *text, char *key);
int isKeyValid(char *key);

int main(int argc, char *argv[])
{
    // Check if the user gave enough arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Get the "key" from the cli arugments
    char *key = argv[1];

    if (!isKeyValid(key))
    {
        return 1;
    }

    // Ask the user for a cipherable text
    char *plainText = get_string("plaintext:  ");
    // Perform the cipher
    char *ciphertext = cipherMe(plainText, key);
    // Prompt the cyphered text
    printf("ciphertext:  %s\n", ciphertext);

    // Free allocated memory
    free(ciphertext);

    return 0;
}

int isCharAlreadyEncountered(int *tempArr, int charDec)
{
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        if (tempArr[i] == charDec)
        {
            return 1;
        }
    }

    return 0;
}

int isKeyValid(char *key)
{
    // Count the length of the "key"
    int keyLength = strlen(key);
    int *tempArr = malloc(sizeof(int) * KEY_LENGTH);

    if (tempArr == NULL)
    {
        printf("Could not allocate enough memory for tempArr");
        exit(1);
    }

    // Validate the "key" length
    if (keyLength < KEY_LENGTH)
    {
        printf("Key must contain %i characters.\n", KEY_LENGTH);
        return 0;
    }

    // Loop through the "key"
    for (int i = 0; i < keyLength; i++)
    {
        // Get the current character as a decimal number
        int charDec = tolower((int)key[i]);

        // If the character is not part of the alphabet, return 0
        int isLowerCaseChar = charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z;
        if (!isLowerCaseChar)
        {
            return 0;
        }

        // Check if the character
        if (isCharAlreadyEncountered(tempArr, charDec))
        {
            return 0;
        }

        tempArr[i] = charDec;
    }

    // Free memory
    free(tempArr);

    return 1;
}

char *cipherMe(char *text, char *key)
{
    // Pre-define variable
    int i = 0, charDec, cipheredCharDec;
    char currentChar;

    // Allocate memory for the text
    char *ciphertext = malloc(sizeof(char) * strlen(text) + 1);

    // Check for NULL pointer
    if (ciphertext == NULL)
    {
        printf("Could not allocate enough memory for ciphertext");
        exit(1);
    }

    // Start looping through the text
    do
    {
        // Get current character
        currentChar = text[i];
        // Save the decimal value of the character
        charDec = (int)currentChar;

        // Break out of the loop once we have reached the end of the string
        if (charDec == NUL)
        {
            // Add null terminator to end of the string
            ciphertext[i] = '\0';
            break;
        }

        // If we deal with a lowercase character
        if (charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z)
        {
            // We use the same index character from the key and lowercase it
            cipheredCharDec = tolower((int)key[charDec - LOWERCASE_A]);
            ciphertext[i] = (char)cipheredCharDec;
        }
        // If we deal with an uppercase character
        else if (charDec >= UPPERCASE_A && charDec <= UPPERCASE_Z)
        {
            // We use the same index character from the key and uppercase it
            cipheredCharDec = toupper((int)key[charDec - UPPERCASE_A]);
            ciphertext[i] = (char)cipheredCharDec;
        }
        // Otherwise, just use the original character
        else
        {
            ciphertext[i] = currentChar;
        }

        i++;
    }
    while (true);

    return ciphertext;
}
