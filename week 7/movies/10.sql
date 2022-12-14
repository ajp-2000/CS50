SELECT people.name
FROM people INNER JOIN(
SELECT directors.person_id
FROM directors INNER JOIN ratings
ON directors.movie_id = ratings.movie_id
WHERE ratings.rating >= 9.0)
ON people.id = person_id;