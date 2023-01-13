# Define get_int
def get_int(prompt: str):
    number: int = 0
    value = input(prompt)

    try:
        number = int(value)
    except:
        get_int(prompt)

    return number

# Define buildPyramid which will build the pyramid recursively


def buildPyramid(current: int, target: int):
    # Define the base case
    if (current > target):
        return 

    # Calculate total of columns
    whiteSpaces = target - current

    # Print white spaces on the left hand side
    for _ in range(whiteSpaces):
        print(' ', end='')
    # Print white spaces on the left hand side
    for _ in range(current):
        print('#', end='')
    # Print white spaces on the left hand side
    for _ in range(2):
        print(' ', end='')
    # Print white spaces on the left hand side
    for _ in range(current):
        print('#', end='')
    # Start a new line
    print('')
    
    # Call the function with updated arguments
    buildPyramid(current + 1, target)


while (True):
    # Ask the user for a height
    height = get_int("Height: ")
    if (height > 0 and height < 9):
        break

# Build and prompt the pyramid to the user
buildPyramid(1, height)
