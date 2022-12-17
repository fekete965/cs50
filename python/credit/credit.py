from cs50 import get_int

AMEX = 'AMEX'
VALID_AMEX_START_DIGITS = set(['34', '37'])

MASTERCARD = 'MASTERCARD'
VALID_MASTERCARD_START_DIGITS = set(['51', '52', '53', '54', '55'])

VISA = 'VISA'
VISA_START_DIGIT = 4

INVALID_CARD_LENGTH = 'INVALID CARD LENGTH'
INVALID_CARD_TYPE = 'INVALID CARD TYPE'
INVALID_CHECKSUM = 'INVALID CHECKSUM'

MIN_CARD_LENGTH = 13
MAX_CARD_LENGTH = 16


def main():
  # Get the user's credit card
  cardNumber = getCardNumber()
  # Execute Luhn's Algorithm
  cardType = validateCardNumber(cardNumber)

  if cardType == None:
    print('')
    exit()
  
  print(cardType)


def validateCardNumber(cardNumber: int):
  cardStr = str(cardNumber)
  cardLength = len(cardStr)

  # Early return based on card length
  if (cardLength < MIN_CARD_LENGTH or cardLength > MAX_CARD_LENGTH):
    print(INVALID_CARD_LENGTH)
    exit()

  # Predefine our variables
  total = 0

  for i in range(1, cardLength + 1):
    current = int(cardStr[cardLength - i])
    if i % 2 == 0:
      doubled = current * 2
      if doubled >= 10:
        total += doubled // 10
        total += doubled % 10
      else:
        total += doubled
    else:
      total += current
    
  # Check for card validity
  if (total % 10 != 0):
    print(INVALID_CHECKSUM)
    exit()
  
  # Concatenate the first two digits
  firstTwoDigits = cardStr[0:2]

  # Validate if card belongs to AMEX
  if validateCardProvider(firstTwoDigits, VALID_AMEX_START_DIGITS):
    return AMEX

  # Validate if card belongs to MASTERCARD
  if validateCardProvider(firstTwoDigits, VALID_MASTERCARD_START_DIGITS):
    return MASTERCARD

  # Validate if card belongs to VISA
  if current == VISA_START_DIGIT:
    return VISA

  # Card is INVALID
  print(INVALID_CARD_TYPE)
  exit()


def validateCardProvider(digits: str, validNums: set[str]):
  return digits in validNums


def getCardNumber():
  while (True):
    cardNumber = get_int('Number: ')

    if (cardNumber > 0):
      return cardNumber


main()
