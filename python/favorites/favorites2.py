import csv

titles = []

with open("./favorites.csv", "r") as file:
  reader = csv.DictReader(file)

  for row in reader:
    title = row["title"].strip().upper()
    if not title in titles:
      titles.append(title)

for title in titles:
  print(title)
