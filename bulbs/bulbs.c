#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
int *getBinaryArr(int charDec);
void printBulbs(int *array);

int main(void)
{
    // Get a message from the user
    char *message = get_string("Message: ");
    // Get the length of the string
    int messageLength = strlen(message);
    // Loop on the message itself
    for (int i = 0; i < messageLength; i++)
    {
        // Get a character
        int charDec = (int)message[i];
        // Turn the character into a binary array
        int *binaryArray = getBinaryArr(charDec);
        // Print out the light bulbs
        printBulbs(binaryArray);
        // Free up the allocated memory
        free(binaryArray);
    }
}

int *getBinaryArr(int charDec)
{
    // Allocate memory for our array
    int *binaryArray = malloc(sizeof(int) * BITS_IN_BYTE);
    // Check for NULL pointer
    if (binaryArray == NULL)
    {
        printf("Could not allocate enough memory for binary array\n");
        exit(1);
    }

    // Populate the binary array with zeros
    int i = 0;
    for (int i = 0; i < BITS_IN_BYTE; i++)
    {
        binaryArray[i] = 0;
    }

    // Reset the index
    i = 0;
    // Loop until we have 0 as the decimal number
    do
    {
        // Get the remainder of the decimal number,
        // and assign its value to the last element in the array
        binaryArray[BITS_IN_BYTE - i - 1] = charDec % 2;
        // Perform integer division on the decimal number
        charDec /= 2;
        // Increase index
        i++;
    }
    while (charDec > 0);
    // Return the final array
    return binaryArray;
}

void printBulbs(int *array)
{
    // Loop on binary array
    for (int i = 0; i < BITS_IN_BYTE; i++)
    {
        // Print out the correct light bulb emoji
        print_bulb(array[i]);
    }
    // Insert a new line
    printf("\n");
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
