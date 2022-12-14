SELECT people.name FROM people INNER JOIN(
SELECT stars.person_id
FROM movies
INNER JOIN stars
ON movies.id = stars.movie_id
WHERE movies.title = 'Toy Story')
ON people.id = person_id;