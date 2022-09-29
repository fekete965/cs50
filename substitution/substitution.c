#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Pre-define constant values
const int LOWERCASE_A = 97;
const int LOWERCASE_Z = 122;
const int NUL = 0;
const int UPPERCASE_A = 65;
const int UPPERCASE_Z = 90;

// Pre-define function prototypes
string cipherMe(string text, string key);

int main(int argc, string argv[]) 
{
    // Check if the user gave enough arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Get the "key" from the cli arugments
    string key = argv[1];
    // Count the length of the "key"
    int keyLength = strlen(key);

    // Validate the "key" length
    if (keyLength < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // Ask the user for a cipherable text
    string plainText = get_string("plaintext:  ");
    // Perform the cipher
    string cipherText = cipherMe(plainText, key);
    // Prompt the cyphered text
    printf("plaintext:  %s\n", cipherText);
    
    return 0;
}

string cipherMe(string text, string key)
{
    // Pre-define variable
    int i = 0, charDec, lowerCharDec;
    char currentChar;
    string ciphertext;
    
    // Start looping
    do
    {
        // Get current character
        currentChar = text[i];
        // Save the decimal value of the character
        charDec = (int) currentChar;
        // Save the lowercase decimal value of the character
        lowerCharDec = (int) tolower(currentChar);

        // Break out of the loop once we have reached the end of the string
        if (charDec == NUL)
        {
            break;
        }

        // If we deal with a lowercase character
        if (charDec >= LOWERCASE_A && charDec <= LOWERCASE_Z)
        {
            // We use the same index character from the key and lowercase it
            ciphertext[i] = tolower(key[charDec - LOWERCASE_A]);
        }
        // If we deal with an uppercase character
        else if (charDec >= UPPERCASE_A && charDec <= UPPERCASE_Z)
        {
            // We use the same index character from the key and uppercase it
            ciphertext[i] = toupper(key[charDec - UPPERCASE_A]);
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
