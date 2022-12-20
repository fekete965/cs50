from cs50 import SQL

db = SQL("sqlite:///favorites.db")

title = input("Title: ").strip().upper()

rows = db.execute("SELECT title FROM favorites WHERE title LIKE ?", title)

for row in rows:
  print(row["title"])
