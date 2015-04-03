-- Jason Tieu
-- 304047667

-- Item Table --
CREATE TABLE IF NOT EXISTS Item (
	ItemID INTEGER NOT NULL,
	UserID VARCHAR(100) NOT NULL,
	Name VARCHAR(100) NOT NULL,
	Currently DECIMAL(8,2) NOT NULL,
	Buy_Price DECIMAL(8,2) DEFAULT NULL,
	First_Bid DECIMAL(8,2) NOT NULL,
	Number_of_Bids INTEGER NOT NULL,
	Started TIMESTAMP NOT NULL,
	Ends TIMESTAMP NOT NULL,
	Description VARCHAR(4000) NOT NULL,
	Location VARCHAR (100) NOT NULL,
	Latitude VARCHAR(100) NOT NULL,
	Longitude VARCHAR(100) NOT NULL,
	Country VARCHAR(100) NOT NULL,
	PRIMARY KEY (ItemID)
);

-- UNUSED TABLE
-- Item Coordinates Table --
-- CREATE TABLE IF NOT EXISTS ItemCoordinates (
--	ItemID INTEGER NOT NULL,
--	Latitude VARCHAR(100) NOT NULL,
--	Longitude VARCHAR(100) NOT NULL
	
	-- PRIMARY KEY (Latitude, Longitude) --
-- );

-- UNUSED TABLE
-- Coordinate Location Table --
-- CREATE TABLE IF NOT EXISTS CoordinatesLocation (
--	Latitude VARCHAR(100) NOT NULL,
--	Longitude VARCHAR(100) NOT NULL,
--	Location VARCHAR(100) NOT NULL,
--	Country VARCHAR(100) NOT NULL
	
	-- PRIMARY KEY (Latitude, Longitude)
-- );

-- Item Category Table --
CREATE TABLE IF NOT EXISTS ItemCategory (
	ItemID INTEGER NOT NULL,
	Category VARCHAR(100) NOT NULL,
	PRIMARY KEY (ItemID, Category)
);

-- User Table --
CREATE TABLE IF NOT EXISTS User (
	UserID VARCHAR(100) NOT NULL,
	SellerRating INT(11) NOT NULL,
	BuyerRating INT(11) NOT NULL,
	Location VARCHAR(100) NOT NULL,
	Country VARCHAR(100) NOT NULL,
	
	PRIMARY KEY (UserID)
);

-- Bid Table --
CREATE TABLE IF NOT EXISTS Bid (
	UserID VARCHAR(100) NOT NULL,
	Time TIMESTAMP NOT NULL,
	ItemID INTEGER NOT NULL,
	Amount DECIMAL(8,2) NOT NULL,
	PRIMARY KEY(UserID, Time)
);