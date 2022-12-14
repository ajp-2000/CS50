SELECT title FROM
ratings INNER JOIN(
movies INNER JOIN(
SELECT movie_id FROM stars
WHERE person_id IN
(SELECT id FROM people
WHERE name = "Chadwick Boseman"))
ON movies.id = movie_id)
on ratings.movie_id = movies.id
ORDER BY rating DESC
LIMIT 5;