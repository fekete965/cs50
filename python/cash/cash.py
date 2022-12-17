from cs50 import get_float

QUARTER = 25
DIME = 10
NICKEL = 5
PENNY = 1


def main():
  # Ask how many cents the customer is owed
  cents = getCents()

  # Calculate the number of quarters to give the customer
  quarters = calculateQuarters(cents)
  cents = cents - quarters * QUARTER

  # Calculate the number of dimes to give the customer
  dimes = calculateDimes(cents)
  cents = cents - dimes * DIME

  # Calculate the number of nickels to give the customer
  nickels = calculateNickels(cents)
  cents = cents - nickels * NICKEL

  # Calculate the number of pennies to give the customer
  pennies = calculatePennies(cents)
  cents = cents - pennies * PENNY

  # Sum coins
  coins = quarters + dimes + nickels + pennies

  # Print total number of coins to give the customer
  print(coins)


def getCents():
  while (True):
    cents = get_float("Change owed: ")

    if (cents > 0):
      return cents * 100
  

def calculateQuarters(cent: float):
  return cent // QUARTER


def calculateDimes(cent: float):
  return cent // DIME


def calculateNickels(cent: float):
  return cent // NICKEL


def calculatePennies(cent: float):
  return cent // PENNY


main()
