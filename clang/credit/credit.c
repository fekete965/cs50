#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

// Declare constants
const string AMEX = "AMEX";
const int VALID_AMEX_START_DIGITS[2] = {34, 37};

const string MASTERCARD = "MASTERCARD";
const int VALID_MASTERCARD_START_DIGITS[5] = {51, 52, 53, 54, 55};

const string VISA = "VISA";
const int VISA_START_DIGIT = 4;

const string INVALID_CARD_LENGTH = "INVALID CARD LENGTH";
const string INVALID_CARD_TYPE = "INVALID CARD TYPE";
const string INVALID_CHECKSUM = "INVALID CHECKSUM";

const int MIN_CARD_LENGTH = 13;
const int MAX_CARD_LENGTH = 16;
const int EXPECTED_RESULT = 20;

// Pre-define function prototypes
long getCardNumber(void);
string validateCardNumber(long num);
bool validateCardProvider(int digits, int *validNums);
int concatInt(int x, int y);

int main(void)
{
    // Get the user's credit card
    long cardNumber = getCardNumber();
    // Execute Luhn's Algorithm
    string cardType = validateCardNumber(cardNumber);

    if (cardType[0] == '1')
    {
        return 1;
    }
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
    int sumOfOdds = 0, sumOfEvens = 0, cardLength = 0, current, lastButOne, doubled;
    long temp = initialNumber;

    do
    {
        // Take the last digit
        current = temp % 10;

        // Branch based on odd/even number count
        if (cardLength % 2 == 0)
        {
            // Sum all odd numbers
            sumOfOdds += current;
        }
        else
        {
            // Save the last but one
            lastButOne = current;
            // Multiply even numbers by 2
            doubled = current * 2;

            // Check if the multipication ended up with double digit
            if (doubled / 10 > 0)
            {
                // If so, add both digits to even sums
                sumOfEvens += doubled / 10 + doubled % 10;
            }
            else
            {
                // Otherwise just add the single digit to the even sums
                sumOfEvens += doubled;
            }
        }

        // Increase length counter
        cardLength++;
        // Move to the next digit by dividing our number by 10
        temp /= 10;
    } while (temp > 0);

    // Early return based on card length
    if (cardLength < MIN_CARD_LENGTH || cardLength > MAX_CARD_LENGTH)
    {
        printf("%s\n", INVALID_CARD_LENGTH);
        return "1";
    }

    // Perform final addition
    int total = sumOfEvens + sumOfOdds;
    // Check for card validity
    if (total != EXPECTED_RESULT)
    {
        printf("%s\n", INVALID_CHECKSUM);
        return "1";
    }

    // Concatenate the first two digits
    int firstTwoDigits = concatInt(current, lastButOne);

    // Validate if card belongs to AMEX
    if (validateCardProvider(firstTwoDigits, (int *)VALID_AMEX_START_DIGITS))
    {
        return AMEX;
    }

    // Validate if card belongs to MASTERCARD
    if (validateCardProvider(firstTwoDigits, (int *)VALID_MASTERCARD_START_DIGITS))
    {
        return MASTERCARD;
    }

    // Validate if card belongs to VISA
    if (current == VISA_START_DIGIT)
    {
        return VISA;
    }

    // Card is INVALID
    printf("%s\n", INVALID_CARD_TYPE);
    return "1";
}

bool validateCardProvider(int digits, int *validNums)
{
    int size = sizeof(validNums);

    for (int i = 0; i < size; i++)
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
