-- 1. Find the number of users in the database.
SELECT COUNT(*)
FROM User;

-- 2. Find the number of items in "New York", (i.e., items whose location is exactly the string "New York"). 
-- Pay special attention to case sensitivity. You should match the items in "New York" but not in "new york".
SELECT COUNT(*)
FROM Item
WHERE BINARY Location = 'New York';

-- 3. Find the number of auctions belonging to exactly four categories.
SELECT COUNT(*)
FROM
(SELECT COUNT(*) as cats 
	FROM ItemCategory
    GROUP BY ItemID
    HAVING cats = 4) as C;

-- 4. Find the ID(s) of current (unsold) auction(s) with the highest bid. Remember that the data was captured 
-- at the point in time December 20th, 2001, one second after midnight, so you can use this time point to 
-- decide which auction(s) are current. Pay special attention to the current auctions without any bid.
SELECT Bid.ItemID
FROM Bid
INNER JOIN Item
ON Bid.ItemID = Item.ItemID
WHERE Amount =(SELECT MAX(Amount) FROM Bid)
	AND Ends > '2001-12-20 00:00:01';
		
-- 5. Find the number of sellers whose rating is higher than 1000.
SELECT COUNT(*)
FROM User
WHERE BINARY SellerRating > 1000;

-- 6. Find the number of users who are both sellers and bidders.
SELECT COUNT(DISTINCT Bid.UserID)
FROM Bid
INNER JOIN Item
ON Bid.UserID = Item.UserID;

-- 7. Find the number of categories that include at least one item with a bid of more than $100.
SELECT COUNT(DISTINCT Category)
FROM ItemCategory
	INNER JOIN Bid
	ON ItemCategory.ItemID = Bid.ItemID
WHERE BINARY Amount > 100
AND Bid.ItemID
IN
	(SELECT Item.ItemID
	FROM Item
	INNER JOIN Bid
	ON Item.ItemID = Bid.ItemID);