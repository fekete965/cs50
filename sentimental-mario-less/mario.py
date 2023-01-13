# Define get_int
import sys


def get_int(prompt: str):
    number: int = 0
    value = input(prompt)

    try:
        number = int(value)
    except:
        get_int(prompt)

    return number

# Define printPyramid which will build the pyramid recursively


def printPyramid(current: int, target: int):
    if current > target:
        return 

    whiteSpaces = target - current

    for _ in range(whiteSpaces): 
        print(" ", end='')
    
    for _ in range(current):
        print("#", end='')

    print('')

    printPyramid(current + 1, target)


# Ask the user for a height
height = None

while (True):
    height = get_int("Height: ")
    if (height >= 1 and height < 9):
        break
# Build and prompt the pyramid to the user
printPyramid(1, height)
