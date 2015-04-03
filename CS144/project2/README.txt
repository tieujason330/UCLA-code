Jason Tieu
304047667

Part B: Design your relational schema

1) List your relations. Please specify all keys that hold on each relation. You need not specify attribute types at this stage.

Item(Item-ID*, UserID, Name, Currently, Buy_Price, First_Bid, Number_of_Bids, Started, Ends, Description, Location, Country, Latitude, Longitude)

ItemCategory(Item-ID*, Category)

User(UserID*, Seller-Rating, Buyer-Rating, Location, Country)

Bid(UserID*, Time*, Item-ID, Amount)

(* denotes the keys)


2) List all completely nontrivial functional dependencies that hold on each relation, excluding those that effectively specify keys

The function relational dependencies in the relations are for keys.


3) Are all of your relations in Boyce-Codd Normal Form (BCNF)? If not, either redesign them and start over, or explain why you feel it is advantageous to use non-BCNF relations.

  All relations are in BCNF.

4) Are all of your relations in Fourth Normal Form (4NF)? If not, either redesign them and start over, or explain why you feel it is advantageous to use non-4NF relations.

  All relations are in 4NF.