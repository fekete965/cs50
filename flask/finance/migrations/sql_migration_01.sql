-- sql_migration_01
CREATE TABLE user_shares (
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  user_id INTEGER NOT NULL,
  name TEXT NOT NULL,
  user_action TEXT NOT NULL,
  price REAL NOT NULL,
  symbol TEXT NOT NULL,
  shares INTEGER NOT NULL,
  total_value REAL NOT NULL,
  created_at TEXT NOT NULL,
  FOREIGN KEY(user_id) REFERENCES users(id)
);
