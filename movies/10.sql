-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
-- Your query should output a table with a single column for the name of each person.
-- If a person directed more than one movie that received a rating of at least 9.0, they should only appear in your results once.

SELECT DISTINCT(people.name)
FROM movies
  JOIN directors
    ON movies.id = directors.movie_id
  JOIN people
    ON people.id = directors.person_id
  JOIN ratings
    ON movies.id = ratings.movie_id
WHERE ratings.rating >= 9.0;
