#!/bin/bash

# Jason Tieu 304047667
# runLoad.sh

# drop any existing tables
mysql CS144 < drop.sql

# create tables
mysql CS144 < create.sql

# compile java code
ant
ant run-all

# make sure there aren't any duplicates
sort -u item.dat > nodupItem.dat
# sort -u itemcoordinates.dat > nodupItemCoordinates.dat
# sort -u coordinateslocation.dat > nodupCoordinatesLocation.dat
sort -u itemcategory.dat > nodupItemCategory.dat
sort -u user.dat > nodupUser.dat
sort -u bid.dat  > nodupBid.dat

# load .dat files into MySQL
mysql CS144 < load.sql

# perform queries
mysql CS144 <  queries.sql

# remove all .dat files
rm *.dat