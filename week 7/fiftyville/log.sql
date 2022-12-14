-- Keep a log of any SQL queries you execute as you solve the mystery.
The theft took place on July 28, 2021, on Humphrey Street

-- Initial investigations
.schema
SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;

SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

-- What we know:
-- Duck stolen from Humphrey Street bakery at 10:15am on July 28th.
-- Within ten minutes, the thief entered a car in the bakery car park and drove away
-- The thief withdrew money from the ATM on Leggett Street before 10:15am
-- The thief phoned someone for less than a minute as leaving the bakery
-- Planned to take the earliest flight out on the 29th, and the callee bought the flight ticket

-- Find the elligible vehicles (leaves 9 options):
SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND activity = 'exit';

-- Turn this into elligible drivers:
SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND activity = 'exit');

-- Now get accounts that could have made the transaction (8 options)
SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street';

-- And get the corresponding people (ids):
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street');

-- Narrow down to people who could have both withdrawn the money and driven out:
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street')
INTERSECT
SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND activity = 'exit');

-- Now we're down to five people.
-- Who could have made the phone call?
SELECT caller FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <=60;

-- Ten people, who's ids are:
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <=60);

-- Put all this together:
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street')
INTERSECT
SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND activity = 'exit')
INTERSECT
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <=60);

-- Down to three people.
-- Which flights could it have been?
SELECT id FROM airports WHERE city = 'Fiftyville';
-- Let's use 8 as a magic number for simplicity
SELECT id, destination_airport_id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8;

-- Narrow down to the first flight out
SELECT id, destination_airport_id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = 8
ORDER BY hour ASC
LIMIT 1;
-- Flight id 36, destination id 4
SELECT city FROM airports WHERE id = 4;
-- The destination was New York City

-- Now list people who were on the flight
SELECT id FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers
WHERE flight_id = 36);

-- Put it all together again:
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street')
INTERSECT
SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND activity = 'exit')
INTERSECT
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <=60)
INTERSECT
SELECT id FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers
WHERE flight_id = 36);

-- Two possibilities
-- Try narrowing down the car-exit by minute
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street')
INTERSECT
SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND minute < 26 AND activity = 'exit')
INTERSECT
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <60)
INTERSECT
SELECT id FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers
WHERE flight_id = 36);

-- 'Tis 686048
-- Work out who:
SELECT name FROM people WHERE id = 686048;
-- Bruce
-- And who he called:
SELECT receiver FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller IN
(SELECT phone_number FROM people WHERE id = 686048);
-- (375) 555-8161
-- Get the person:
SELECT name FROM people WHERE phone_number = '(375) 555-8161';
-- Robin