# Define get_int
def get_int(prompt: str):
    
    value = input(prompt)

    try:
        number = int(value)
    except:
        get_int(prompt)

    return number

# Define printPyramid which will build the pyramid recursively
def printPyramid(countFrom: int, until: int):
    if countFrom == until:
        return 

    for i in range(until): 
        if i < until - countFrom - 1:
            print(" ", end='')
        else:
            print("#", end='')

    print('')
    printPyramid(countFrom + 1, until)

# Ask the user for a height
height = get_int("Height: ")
# Build and prompt the pyramid to the user
printPyramid(0, height)
