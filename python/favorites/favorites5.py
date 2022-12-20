import csv

titles = {}

with open("./favorites.csv", "r") as file:
  reader = csv.DictReader(file)

  for row in reader:
    title = row["title"].strip().upper()
    if not title in titles:
      titles[title] = 0
    
    titles[title] += 1

for title in titles:
  count = titles[title]
  print(f"{title} - {count}")
