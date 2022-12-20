import csv

titleInput = input("Title: ").strip().upper()
counter = 0

with open("./favorites.csv", "r") as file:
  reader = csv.DictReader(file)

  for row in reader:
    title = row["title"].strip().upper()

    if title == titleInput:
      counter += 1
    
    
print(f"Number of people who like {titleInput}: {counter}")
