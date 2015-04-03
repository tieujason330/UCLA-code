-- Jason Tieu --
-- ID: 304047667 --

-- Part B.1 --
CREATE TABLE Actors( Name char(40), Movie char(80), Year int, Role char(40));

-- Part B.2 --
LOAD DATA LOCAL INFILE '~/data/actors.csv' INTO TABLE Actors
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"';

-- Part B.3 --
SELECT A.Name
FROM Actors A
WHERE A.Movie = 'Die Another Day';

-- Part B.4 --
DROP TABLE IF EXISTS Actors;
