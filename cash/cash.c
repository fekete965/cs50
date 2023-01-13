#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

const int QUARTER = 25;
const int DIME = 10;
const int NICKEL = 5;
const int PENNY = 1;

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * QUARTER;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * DIME;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * NICKEL;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * PENNY;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int cents;

    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    return cents;
}

int calculate_quarters(int cents)
{
    return cents / QUARTER;
}

int calculate_dimes(int cents)
{
    return cents / DIME;
}

int calculate_nickels(int cents)
{
    return cents / NICKEL;
}

int calculate_pennies(int cents)
{
    return cents / PENNY;
}
