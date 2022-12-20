import csv

titles = {}

with open("./favorites.csv", "r") as file:
  reader = csv.DictReader(file)

  for row in reader:
    title = row["title"].strip().upper()
    if not title in titles:
      titles[title] = 0
    
    titles[title] += 1

def get_value(title):
  return titles[title]

for title in sorted(titles, key = get_value, reverse = True):
  count = titles[title]
  print(f"{title} - {count}")
