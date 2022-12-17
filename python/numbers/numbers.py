import sys

numbers = [4, 6, 8, 2, 7, 5, 0]

if 0 in numbers:
  print("Found")
  sys.exit(0)

print("Not found")
sys.exit(1)
