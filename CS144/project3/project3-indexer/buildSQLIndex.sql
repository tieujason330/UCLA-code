-- Jason Tieu --
-- 304047667 -- 

-- Create table with (ItemID, Latitude, Longitude) where (Latitude, Longitude) is stored
-- as single column 
-- first have dummy columns for Latitude and Longitude
CREATE TABLE IF NOT EXISTS SpatialTable ( 
	ItemID INTEGER NOT NULL,
	Latitude VARCHAR(100) NOT NULL,
	Longitude VARCHAR(100) NOT NULL,
	PRIMARY KEY (ItemID)
) ENGINE=MyISAM;

-- Get ItemID, Latitude, Longitude from Item table
INSERT INTO SpatialTable(ItemID, Latitude, Longitude)
SELECT ItemID, Latitude, Longitude
FROM Item
WHERE Latitude <> '' AND Longitude <> '';

-- Add column for geometry (points)
ALTER TABLE SpatialTable ADD COLUMN (g GEOMETRY NOT NULL);

-- Use dummy columns for Latitude and Longitude to fill in Points
UPDATE SpatialTable SET g = GeomFromText(CONCAT('POINT(',Latitude,' ',Longitude,')'));

-- Get rid of temp columns to fill in Points
ALTER TABLE SpatialTable DROP COLUMN Latitude;
ALTER TABLE SpatialTable DROP COLUMN Longitude;

-- Create R-Tree (spatial index) on latitude and longitude
CREATE SPATIAL INDEX sp_index ON SpatialTable(g);