from cs50 import SQL

db = SQL("sqlite:///favorites.db")

title = input("Title: ").strip().upper()

rows = db.execute("SELECT COUNT(*) as counter FROM favorites WHERE title LIKE ?", title)

row = rows[0]

print(row["counter"])
