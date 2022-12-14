SELECT DISTINCT people.name FROM people INNER JOIN(
SELECT stars.person_id
FROM movies
INNER JOIN stars
ON movies.id = stars.movie_id
WHERE movies.year = 2004)
ON people.id = person_id
ORDER BY people.birth;