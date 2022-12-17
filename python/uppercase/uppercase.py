from cs50 import get_string

before = get_string("Before: ")
print(f"After: {before.upper()}")

# C version in python below:
# print(f"After: ", end = "")

# for char in before:
#   print(char.upper(), end = "")

# print("")
