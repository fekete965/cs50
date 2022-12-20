-- Keep a log of any SQL queries you execute as you solve the mystery.

-- The theft took place on July 28, 2021 and that it took place on Humphrey Street.

-- Use sqlite3 to access the db:
-- sqlite3 fiftyville.db

-- Pull database schema by typing .schema
-- CREATE TABLE crime_scene_reports (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     street TEXT,
--     description TEXT,
--     PRIMARY KEY(id)
-- );

-- CREATE TABLE interviews (
--     id INTEGER,
--     name TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     transcript TEXT,
--     PRIMARY KEY(id)
-- );

-- CREATE TABLE atm_transactions (
--     id INTEGER,
--     account_number INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     atm_location TEXT,
--     transaction_type TEXT,
--     amount INTEGER,
--     PRIMARY KEY(id)
-- );

-- CREATE TABLE bank_accounts (
--     account_number INTEGER,
--     person_id INTEGER,
--     creation_year INTEGER,
--     FOREIGN KEY(person_id) REFERENCES people(id)
-- );

-- CREATE TABLE airports (
--     id INTEGER,
--     abbreviation TEXT,
--     full_name TEXT,
--     city TEXT,
--     PRIMARY KEY(id)
-- );
--
-- CREATE TABLE flights (
--     id INTEGER,
--     origin_airport_id INTEGER,
--     destination_airport_id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     PRIMARY KEY(id),
--     FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
--     FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
-- );

-- CREATE TABLE passengers (
--     flight_id INTEGER,
--     passport_number INTEGER,
--     seat TEXT,
--     FOREIGN KEY(flight_id) REFERENCES flights(id)
-- );

-- CREATE TABLE phone_calls (
--     id INTEGER,
--     caller TEXT,
--     receiver TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     duration INTEGER,
--     PRIMARY KEY(id)
-- );

-- CREATE TABLE people (
--     id INTEGER,
--     name TEXT,
--     phone_number TEXT,
--     passport_number INTEGER,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );

-- CREATE TABLE bakery_security_logs (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     activity TEXT,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );

-- Get all the crime scene reports for Humphrey Street on 28/07/2021
SELECT id, street, description
FROM crime_scene_reports
WHERE street = 'Humphrey Street'
  AND year = 2021
  AND month = 7
  AND day = 28;
-- +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | id  |     street      |                                                                                                       description                                                                                                        |
-- +-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | 295 | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
-- | 297 | Humphrey Street | Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
-- +-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

-- Gather the interviews from 28/07/2021
SELECT id, name, transcript
FROM interviews
WHERE year = 2021
  AND month = 7
  AND day = 28;
-- +-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | id  |  name   |                                                                                                                                                     transcript                                                                                                                                                      |
-- +-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | 158 | Jose    | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”                                                                                                                               |
-- | 159 | Eugene  | “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”                                                                                                                                                                                      |
-- | 160 | Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.                                                                                                                   |
-- | 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
-- | 191 | Lily    | Our neighboring courthouse has a very annoying rooster that crows loudly at 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never bother us again. My sons have successfully arrived in Paris.                                                                        |
-- +-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

-- The three interviews related to the case are:
-- Ruth ------> Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene  ---> I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond ---> As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Notes what to check next:
-- 1. Security footage in 10 minutes frame of the theft
-- 2. Leggett Street ATM money withdraw records for the morning
-- 3. Check the earliest flight out of the city for tomorrow and who bought ticket for it

-- Get security footage and people with passport data for the afternoon (10 minutes was he theft frame)
SELECT p.name, p.passport_number, p.license_plate, p.phone_number, bsl.activity
FROM bakery_security_logs bsl
JOIN people p
  ON bsl.license_plate = p.license_plate
WHERE p.passport_number IS NOT NULL
  AND bsl.year = 2021
  AND bsl.month = 7
  AND bsl.day = 28;
-- +---------+-----------------+---------------+----------------+----------+
-- |  name   | passport_number | license_plate |  phone_number  | activity |
-- +---------+-----------------+---------------+----------------+----------+
-- | Alice   | 1679711307      | 1M92998       | (031) 555-9915 | entrance |
-- | Peter   | 9224308981      | N507616       | (751) 555-6567 | entrance |
-- | Alice   | 1679711307      | 1M92998       | (031) 555-9915 | exit     |
-- | Peter   | 9224308981      | N507616       | (751) 555-6567 | exit     |
-- | Debra   | 2750542732      | 47MEFVA       |                | entrance |
-- | Debra   | 2750542732      | 47MEFVA       |                | exit     |
-- | Jordan  | 7951366683      | HW0488P       | (328) 555-9658 | entrance |
-- | Jordan  | 7951366683      | HW0488P       | (328) 555-9658 | exit     |
-- | Iman    | 7049073643      | L93JTIZ       | (829) 555-5269 | entrance |
-- | Bruce   | 5773159633      | 94KL13X       | (367) 555-5533 | entrance |
-- | George  | 4977790793      | L68E5I0       |                | entrance |
-- | Michael | 6117294637      | HOD8639       | (529) 555-7276 | entrance |
-- | Michael | 6117294637      | HOD8639       | (529) 555-7276 | exit     |
-- | George  | 4977790793      | L68E5I0       |                | exit     |
-- | Taylor  | 1988161715      | 1106N58       | (286) 555-6063 | entrance |
-- | Diana   | 3592750733      | 322W7JE       | (770) 555-1861 | entrance |
-- | Ralph   | 6464352048      | 3933NUH       | (771) 555-7880 | entrance |
-- | Ralph   | 6464352048      | 3933NUH       | (771) 555-7880 | exit     |
-- | Kelsey  | 8294398571      | 0NTHK55       | (499) 555-9472 | entrance |
-- | Joshua  | 3761239013      | 1FBL6TH       | (267) 555-2761 | entrance |
-- | Joshua  | 3761239013      | 1FBL6TH       | (267) 555-2761 | exit     |
-- | Carolyn | 3925120216      | P14PE2Q       | (234) 555-1294 | entrance |
-- | Carolyn | 3925120216      | P14PE2Q       | (234) 555-1294 | exit     |
-- | Luca    | 8496433585      | 4328GD8       | (389) 555-5198 | entrance |
-- | Vanessa | 2963008352      | 5P2BI95       | (725) 555-4692 | entrance |
-- | Barry   | 7526138472      | 6P58WS2       | (301) 555-4174 | entrance |
-- | Sofia   | 1695452385      | G412CB7       | (130) 555-0289 | entrance |
-- | Brandon | 7874488539      | R3G7486       | (771) 555-6667 | entrance |
-- | Sophia  | 3642612721      | 13FNH73       | (027) 555-1068 | entrance |
-- | Vanessa | 2963008352      | 5P2BI95       | (725) 555-4692 | exit     |
-- | Bruce   | 5773159633      | 94KL13X       | (367) 555-5533 | exit     |
-- | Barry   | 7526138472      | 6P58WS2       | (301) 555-4174 | exit     |
-- | Luca    | 8496433585      | 4328GD8       | (389) 555-5198 | exit     |
-- | Sofia   | 1695452385      | G412CB7       | (130) 555-0289 | exit     |
-- | Iman    | 7049073643      | L93JTIZ       | (829) 555-5269 | exit     |
-- | Diana   | 3592750733      | 322W7JE       | (770) 555-1861 | exit     |
-- | Kelsey  | 8294398571      | 0NTHK55       | (499) 555-9472 | exit     |
-- | Taylor  | 1988161715      | 1106N58       | (286) 555-6063 | exit     |
-- | Denise  | 4001449165      | NRYN856       | (994) 555-3373 | entrance |
-- | Thomas  | 6034823042      | WD5M8I6       | (286) 555-0131 | entrance |
-- | Jeremy  | 1207566299      | V47T75I       | (194) 555-5027 | entrance |
-- | Judith  | 8284363264      | 4963D92       |                | entrance |
-- | Vincent | 3011089587      | 94MV71O       |                | entrance |
-- | Daniel  | 7597790505      | FLFN3W0       | (971) 555-6468 | entrance |
-- | Frank   | 8336437534      | 207W38T       | (356) 555-6641 | entrance |
-- | Amanda  | 1618186613      | RS7I6A0       | (821) 555-5262 | entrance |
-- | John    | 8174538026      | 4468KVT       | (016) 555-9166 | entrance |
-- | Ethan   | 2996517496      | NAW9653       | (594) 555-6254 | entrance |
-- | Ethan   | 2996517496      | NAW9653       | (594) 555-6254 | exit     |
-- | Amanda  | 1618186613      | RS7I6A0       | (821) 555-5262 | exit     |
-- | Vincent | 3011089587      | 94MV71O       |                | exit     |
-- | Thomas  | 6034823042      | WD5M8I6       | (286) 555-0131 | exit     |
-- | John    | 8174538026      | 4468KVT       | (016) 555-9166 | exit     |
-- | Frank   | 8336437534      | 207W38T       | (356) 555-6641 | exit     |
-- | Denise  | 4001449165      | NRYN856       | (994) 555-3373 | exit     |
-- | Sophia  | 3642612721      | 13FNH73       | (027) 555-1068 | exit     |
-- | Jeremy  | 1207566299      | V47T75I       | (194) 555-5027 | exit     |
-- | Brandon | 7874488539      | R3G7486       | (771) 555-6667 | exit     |
-- | Daniel  | 7597790505      | FLFN3W0       | (971) 555-6468 | exit     |
-- | Judith  | 8284363264      | 4963D92       |                | exit     |
-- +---------+-----------------+---------------+----------------+----------+


-- Pull people data by their licence plate who also has a passport
SELECT *
FROM people
WHERE passport_number IS NOT NULL
  AND license_plate IN (
  SELECT DISTINCT(license_plate)
  FROM bakery_security_logs
  WHERE activity = 'exit'
    AND year = 2021
    AND month = 7
    AND day = 28
);
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 210245 | Jordan  | (328) 555-9658 | 7951366683      | HW0488P       |
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 325548 | Brandon | (771) 555-6667 | 7874488539      | R3G7486       |
-- | 360948 | Carolyn | (234) 555-1294 | 3925120216      | P14PE2Q       |
-- | 379932 | Joshua  | (267) 555-2761 | 3761239013      | 1FBL6TH       |
-- | 384013 | Debra   |                | 2750542732      | 47MEFVA       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 542503 | Michael | (529) 555-7276 | 6117294637      | HOD8639       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 565511 | Vincent |                | 3011089587      | 94MV71O       |
-- | 604497 | Ralph   | (771) 555-7880 | 6464352048      | 3933NUH       |
-- | 620297 | Peter   | (751) 555-6567 | 9224308981      | N507616       |
-- | 632023 | Amanda  | (821) 555-5262 | 1618186613      | RS7I6A0       |
-- | 652412 | Denise  | (994) 555-3373 | 4001449165      | NRYN856       |
-- | 660982 | Thomas  | (286) 555-0131 | 6034823042      | WD5M8I6       |
-- | 677698 | John    | (016) 555-9166 | 8174538026      | 4468KVT       |
-- | 682850 | Ethan   | (594) 555-6254 | 2996517496      | NAW9653       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 745650 | Sophia  | (027) 555-1068 | 3642612721      | 13FNH73       |
-- | 747078 | Judith  |                | 8284363264      | 4963D92       |
-- | 748674 | Jeremy  | (194) 555-5027 | 1207566299      | V47T75I       |
-- | 750165 | Daniel  | (971) 555-6468 | 7597790505      | FLFN3W0       |
-- | 768248 | George  |                | 4977790793      | L68E5I0       |
-- | 809194 | Alice   | (031) 555-9915 | 1679711307      | 1M92998       |
-- | 926715 | Frank   | (356) 555-6641 | 8336437534      | 207W38T       |
-- +--------+---------+----------------+-----------------+---------------+

-- Pull ATM records from
SELECT *
FROM atm_transactions
WHERE atm_location = 'Leggett Street'
  AND transaction_type = 'withdraw'
  AND year = 2021
  AND month = 7
  AND day = 28;
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+

-- Get personal data based on their account number
SELECT p.name, ba.account_number, p.passport_number, p.license_plate
FROM people p
JOIN bank_accounts ba
  ON p.id = ba.person_id
WHERE ba.account_number IN (
  SELECT at.account_number
  FROM atm_transactions at
  WHERE atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
    AND year = 2021
    AND month = 7
    AND day = 28
  );
-- +---------+----------------+-----------------+---------------+
-- |  name   | account_number | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Bruce   | 49610011       | 5773159633      | 94KL13X       |
-- | Diana   | 26013199       | 3592750733      | 322W7JE       |
-- | Brooke  | 16153065       | 4408372428      | QX4YZN3       |
-- | Kenny   | 28296815       | 9878712108      | 30G67EN       |
-- | Iman    | 25506511       | 7049073643      | L93JTIZ       |
-- | Luca    | 28500762       | 8496433585      | 4328GD8       |
-- | Taylor  | 76054385       | 1988161715      | 1106N58       |
-- | Benista | 81061156       | 9586786673      | 8X428L0       |
-- +---------+----------------+-----------------+---------------+

-- Pull every flight's airport's city and full name and destination airport id
-- narrowed it down to 1 day in advance to the morning from the event's date and place (Fiftyville, 28/07/2021)
SELECT f.id AS 'Flight Id' , a.city, a.full_name, f.day, f.hour, f.minute, f.destination_airport_id
FROM flights f
JOIN airports a
  ON f.origin_airport_id = a.id
WHERE year = 2021
  AND month = 7
  AND day = 29
  AND hour < 12
  AND f.origin_airport_id = (
    SELECT id FROM airports WHERE city = 'Fiftyville'
  )
ORDER BY year, month, day, hour, minute
LIMIT 1;
-- +-----------+------------+-----------------------------+-----+------+--------+------------------------+
-- | Flight Id |    city    |          full_name          | day | hour | minute | destination_airport_id |
-- +-----------+------------+-----------------------------+-----+------+--------+------------------------+
-- | 36        | Fiftyville | Fiftyville Regional Airport | 29  | 8    | 20     | 4                      |
-- +-----------+------------+-----------------------------+-----+------+--------+------------------------+

-- Get the destination airport data by id #4
SELECT * FROM airports WHERE id = 4;
-- +----+--------------+-------------------+---------------+
-- | id | abbreviation |     full_name     |     city      |
-- +----+--------------+-------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport | New York City |
-- +----+--------------+-------------------+---------------+

-- Get flight Passengers for flight id #36
SELECT * FROM passengers WHERE flight_id = 36;
-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 36        | 7214083635      | 2A   |
-- | 36        | 1695452385      | 3B   |
-- | 36        | 5773159633      | 4A   |
-- | 36        | 1540955065      | 5C   |
-- | 36        | 8294398571      | 6C   |
-- | 36        | 1988161715      | 6D   |
-- | 36        | 9878712108      | 7A   |
-- | 36        | 8496433585      | 7B   |
-- +-----------+-----------------+------+

-- Get people information basd on their passport number for flight id #36
SELECT *
FROM people
WHERE passport_number IN (
  SELECT passport_number
  FROM passengers
   WHERE flight_id = 36
);
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- +--------+--------+----------------+-----------------+---------------+

-- Compare the list above with the bakery visiter list
SELECT p.id, p.name, p.passport_number, p.license_plate, p.phone_number
FROM bakery_security_logs bsl
JOIN people p
  ON p.license_plate = bsl.license_plate
WHERE activity = 'exit'
  AND year = 2021
  AND month = 7
  AND day = 28
  AND p.id IN (
    SELECT id
    FROM people
    WHERE passport_number IN (
      SELECT passport_number
      FROM passengers
      WHERE flight_id = 36
    )
);
-- +--------+--------+-----------------+---------------+----------------+
-- |   id   |  name  | passport_number | license_plate |  phone_number  |
-- +--------+--------+-----------------+---------------+----------------+
-- | 398010 | Sofia  | 1695452385      | G412CB7       | (130) 555-0289 |
-- | 449774 | Taylor | 1988161715      | 1106N58       | (286) 555-6063 |
-- | 467400 | Luca   | 8496433585      | 4328GD8       | (389) 555-5198 |
-- | 560886 | Kelsey | 8294398571      | 0NTHK55       | (499) 555-9472 |
-- | 686048 | Bruce  | 5773159633      | 94KL13X       | (367) 555-5533 |
-- +--------+--------+-----------------+---------------+----------------+

-- Pull down phone calls for the list of people above for this list of phone numbers
-- who had a conversation less then a minute:
-- ('(130) 555-0289', '(286) 555-6063', '(389) 555-5198', '(499) 555-9472', '(367) 555-5533')
SELECT pc.caller, pc.receiver, pc.duration
FROM phone_calls pc
WHERE duration < 60
  AND year = 2021
  AND month = 7
  AND day = 28
  AND (
    pc.receiver
      IN ('(130) 555-0289', '(286) 555-6063', '(389) 555-5198', '(499) 555-9472', '(367) 555-5533')
    OR pc.caller
      IN ('(130) 555-0289', '(286) 555-6063', '(389) 555-5198', '(499) 555-9472', '(367) 555-5533')
  )
ORDER BY duration;
-- +----------------+----------------+----------+
-- |     caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (286) 555-6063 | (676) 555-6554 | 43       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- +----------------+----------------+----------+

-- Get the caller's personal data related to their phone number
SELECT p.name, p.phone_number, p.passport_number, p.license_plate
FROM people p
WHERE p.phone_number IN
  (
    '(499) 555-9472',
    '(286) 555-6063',
    '(367) 555-5533',
    '(499) 555-9472',
    '(130) 555-0289'
  );
-- +--------+----------------+-----------------+---------------+
-- |  name  |  phone_number  | passport_number | license_plate |
-- +--------+----------------+-----------------+---------------+
-- | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+----------------+-----------------+---------------+

-- Get the callee's personal data related to their phone number
SELECT p.name, p.phone_number, p.passport_number, p.license_plate
FROM people p
WHERE p.phone_number IN
  (
  '(892) 555-8872',
  '(676) 555-6554',
  '(375) 555-8161',
  '(717) 555-1342',
  '(996) 555-8899'
);
-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | James   | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- | Larry   | (892) 555-8872 | 2312901747      | O268ZZ0       |
-- | Jack    | (996) 555-8899 | 9029462229      | 52R0Y8U       |
-- | Melissa | (717) 555-1342 | 7834357192      |               |
-- | Robin   | (375) 555-8161 |                 | 4V16VO0       |
-- +---------+----------------+-----------------+---------------+

-- The two tables next to each other:
-- +-----------------------+------------------------+----------+
-- |         caller        |        receiver        | duration |
-- +-----------------------+------------------------+----------+
-- | Kelsey (499) 555-9472 | Larry   (892) 555-8872 |    36    |
-- | Taylor (286) 555-6063 | James   (676) 555-6554 |    43    |
-- | Bruce  (367) 555-5533 | Robin   (375) 555-8161 |    45    |
-- | Kelsey (499) 555-9472 | Melissa (717) 555-1342 |    50    |
-- | Sofia  (130) 555-0289 | Jack    (996) 555-8899 |    51    |
-- +-----------------------+------------------------+----------+

-- List of the target flight's passanger
-- +--------+--------+----------------+-----------------+---------------+-----------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate | POTENTIAL |
-- +--------+--------+----------------+-----------------+---------------+-----------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |    NO     |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |    YES    |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |    YES    |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |    NO     |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |    YES    |
-- | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |    NO     |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |    YES    |
-- | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |    NO     |
-- +--------+--------+----------------+-----------------+---------------+-----------+

-- List of people who withdrawn money in the morning from the ATM on Leggett Street
-- +---------+----------------+-----------------+---------------+
-- |  name   | account_number | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Bruce   | 49610011       | 5773159633      | 94KL13X       |
-- | Diana   | 26013199       | 3592750733      | 322W7JE       |
-- | Brooke  | 16153065       | 4408372428      | QX4YZN3       |
-- | Kenny   | 28296815       | 9878712108      | 30G67EN       |
-- | Iman    | 25506511       | 7049073643      | L93JTIZ       |
-- | Luca    | 28500762       | 8496433585      | 4328GD8       |
-- | Taylor  | 76054385       | 1988161715      | 1106N58       |
-- | Benista | 81061156       | 9586786673      | 8X428L0       |
-- +---------+----------------+-----------------+---------------+

-- The narrowed list of thiefs and their accomplice contains 4 people
-- +-----------------------+----------------------+----------+
-- |         Caller        |         Callee       | duration |
-- +-----------------------+----------------------+----------+
-- | Taylor (286) 555-6063 | James (676) 555-6554 |    43    |
-- | Bruce  (367) 555-5533 | Robin (375) 555-8161 |    45    |
-- +-----------------------+----------------------+----------+

-- Based on the witnesses statement, the caller asked for 1 ticket to the earliest fight next day morning.
-- From the list above only both Taylor and James has passports so it would be logical that both of them gets on the flight.
-- On the other hand, betwwen Bruce and Robin, only bruce has a passport.
-- I suspect Bruce to be the theif and Robin his accomplice and Bruce tried to fled to New York City
