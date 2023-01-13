#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Declare constants
const string AMEX = "AMEX";
const int SIZE_OF_AMEX_DIGITS = 2;
const int VALID_AMEX_START_DIGITS[SIZE_OF_AMEX_DIGITS] = {34, 37};

const string MASTERCARD = "MASTERCARD";
const int SIZE_OF_MASTERCARD_DIGITS = 5;
const int VALID_MASTERCARD_START_DIGITS[SIZE_OF_MASTERCARD_DIGITS] = {51, 52, 53, 54, 55};

const string VISA = "VISA";
const int VISA_START_DIGIT = 4;

const string INVALID = "INVALID";

const int MIN_CARD_LENGTH = 13;
const int MAX_CARD_LENGTH = 16;

// Pre-define function prototypes
long getCardNumber(void);
string validateCardNumber(long num);
bool validateCardProvider(int digits, int *validNums, int arraySize);
int concatInt(int x, int y);

int main(void)
{
    // Get the user's credit card
    long cardNumber = getCardNumber();
    // Execute Luhn's Algorithm
    string cardType = validateCardNumber(cardNumber);

    printf("%s\n", cardType);

    return 0;
}

long getCardNumber(void)
{
    long cardNumber;

    do
    {
        // Ask the user for a credit card number
        cardNumber = get_long("Number: ");
    }
    // Until it is a valid number
    while (!cardNumber);

    return cardNumber;
}

string validateCardNumber(long initialNumber)
{
    // Predefine our variables
    int total = 0, cardLength = 0, current, lastButOne, doubled;
    long temp = initialNumber;

    do
    {
        // Take the last digit
        current = temp % 10;

        // Branch based on odd/even number count
        if (cardLength % 2 == 0)
        {
            // Sum all odd numbers
            total += current;
        }
        else
        {
            // Multiply even numbers by 2
            doubled = current * 2;

            // Check if the multipication ended up with double digit
            if (doubled / 10 > 0)
            {
                // If so, add both digits to even sums
                total += doubled / 10 + doubled % 10;
            }
            else
            {
                // Otherwise just add the single digit to the even sums
                total += doubled;
            }
        }

        // Increase length counter
        cardLength++;
        // Move to the next digit by dividing our number by 10
        temp /= 10;
        if (temp > 0)
        {
            // Save the last but one
            lastButOne = current;
        }
    }
    while (temp > 0);

    // Early return based on card length
    if (cardLength < MIN_CARD_LENGTH || cardLength > MAX_CARD_LENGTH)
    {
        // Card is INVALID
        return INVALID;
    }

    // Check for card validity
    if (total % 10 != 0)
    {
        // Card is INVALID
        return INVALID;
    }

    // Concatenate the first two digits
    int firstTwoDigits = concatInt(current, lastButOne);

    // Validate if card belongs to AMEX
    if (validateCardProvider(firstTwoDigits, (int *)VALID_AMEX_START_DIGITS, SIZE_OF_AMEX_DIGITS))
    {
        return AMEX;
    }

    // Validate if card belongs to MASTERCARD
    if (validateCardProvider(firstTwoDigits, (int *)VALID_MASTERCARD_START_DIGITS, SIZE_OF_MASTERCARD_DIGITS))
    {
        return MASTERCARD;
    }

    // Validate if card belongs to VISA
    if (current == VISA_START_DIGIT)
    {
        return VISA;
    }

    // Card is INVALID
    return INVALID;
}

bool validateCardProvider(int digits, int *validNums, int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (digits == validNums[i])
        {
            return true;
        }
    }

    return false;
}

int concatInt(int x, int y)
{
    int pow = 10;

    while (y >= pow)
    {
        pow *= 10;
    }

    return x * pow + y;
}
