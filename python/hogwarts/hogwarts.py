import csv

houses = {
  "Gryffindor": 0,
  "Hufflepuff": 0,
  "Ravenclaw": 0,
  "Slytherin": 0,
}

# 
with open("Hogwarts.csv", "r") as file:
  reader = csv.DictReader(file)

  for row in reader:
    house = row["House"]
    houses[house] += 1

for house in houses:
  count = houses[house]
  print(f"{house}: {count}")
