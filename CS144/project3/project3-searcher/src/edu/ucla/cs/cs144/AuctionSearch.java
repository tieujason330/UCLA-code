package edu.ucla.cs.cs144;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.util.Date;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.text.SimpleDateFormat;
import java.lang.Number;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.apache.lucene.document.Document;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

import edu.ucla.cs.cs144.DbManager;
import edu.ucla.cs.cs144.SearchRegion;
import edu.ucla.cs.cs144.SearchResult;

import java.sql.Timestamp;
import java.text.StringCharacterIterator;
import java.text.CharacterIterator;

public class AuctionSearch implements IAuctionSearch {

	/* 
         * You will probably have to use JDBC to access MySQL data
         * Lucene IndexSearcher class to lookup Lucene index.
         * Read the corresponding tutorial to learn about how to use these.
         *
	 * You may create helper functions or classes to simplify writing these
	 * methods. Make sure that your helper functions are not public,
         * so that they are not exposed to outside of this class.
         *
         * Any new classes that you create should be part of
         * edu.ucla.cs.cs144 package and their source files should be
         * placed at src/edu/ucla/cs/cs144.
         *
         */
	private IndexSearcher searcher = null;
	private QueryParser parser = null;
	private Connection conn = null;
	
	public AuctionSearch() {
		try {
			searcher = new IndexSearcher(DirectoryReader.open(FSDirectory.open(new File("/var/lib/lucene/index-directory/"))));
		} catch (Exception e) {
			System.out.println(e);
		}
		
		try {
			parser = new QueryParser("ItemContent", new StandardAnalyzer());
		} catch (Exception e) {
			System.out.println(e);
		}
		
		try {
			conn = DbManager.getConnection(true);
		} catch (SQLException e) {
			System.out.println(e);
		}
	}
	
	public SearchResult[] basicSearch(String query, int numResultsToSkip, 
			int numResultsToReturn) {
				
		// TODO: Your code here!
		//return new SearchResult[0];
		
		Query q = null;
		TopDocs topDocs = null;

		try {
			q = this.parser.parse(query);
		} catch (ParseException e) {
			System.out.println(e);
		}
		
		try {
			topDocs = this.searcher.search(q, numResultsToSkip + numResultsToReturn);
		} catch (IOException e) {
			System.out.println(e);
		}
		
		ScoreDoc[] hits = topDocs.scoreDocs;
		
		if (numResultsToSkip > hits.length) {
			return new SearchResult[0];
		}
		
		if (numResultsToReturn == 0) {
			numResultsToReturn = hits.length;
		}
		
		ArrayList<SearchResult> arr = new ArrayList<SearchResult>();
		
		for (int i = numResultsToSkip; i < hits.length; i++) {
			if (numResultsToReturn <= 0)
				break;
			
			Document doc = null;
			try {
				doc = searcher.doc(hits[i].doc); 
			} catch (IOException e) {
				System.out.println(e);
			}
			
			arr.add(new SearchResult(doc.get("ItemID"), doc.get("Name")));
			numResultsToReturn = numResultsToReturn - 1;
		}
		
		SearchResult[] results = new SearchResult[arr.size()];
		results = arr.toArray(results);
		//System.out.println("RESULTS num = " + results.length);
		return results;
	}

	public SearchResult[] spatialSearch(String query, SearchRegion region,
			int numResultsToSkip, int numResultsToReturn) {
		// TODO: Your code here!
		//return new SearchResult[0];
		SearchResult[] basicSearchResults = basicSearch(query, 0, Integer.MAX_VALUE);
		
		ArrayList<SearchResult> arr = new ArrayList<SearchResult>();
		
		try {	
			Statement stmt = conn.createStatement();
			ResultSet rs = null;
			
			for(SearchResult sr : basicSearchResults) {
				
				String ItemID = sr.getItemId();
				Double srX, srY;
				rs = stmt.executeQuery("SELECT X(g) AS X, Y(g) AS Y FROM SpatialTable WHERE ItemID ='" + ItemID + "'");
			
				//if empty skip
				if( !rs.isBeforeFirst() ) {
					continue;
				}
				
				while( rs.next() ){
					srX = Double.parseDouble(rs.getString("X"));
					//System.out.println("srX = " + srX.toString());
					srY = Double.parseDouble(rs.getString("Y"));
					
					//if within the box, add to SearchResult
					if ( ( srX >= region.getLx() && srX <= region.getRx() )
					  && ( srY >= region.getLy() && srY <= region.getRy() ) ) {
						arr.add(sr);
					}
				}
			
			}
			
		} catch (SQLException e) {
			System.out.println(e);
		};
		
		//results less than numResultsToSkip
		if (arr.size() < numResultsToSkip ) {
			return new SearchResult[0];
		}
		
		if (numResultsToReturn == 0) {
			numResultsToReturn = arr.size();
		}
		
		ArrayList<SearchResult> arr2 = new ArrayList<SearchResult>();
		for (int i = numResultsToSkip; i < arr.size(); i++) {
			if (numResultsToReturn <= 0)
				break;
			
			arr2.add(arr.get(i));
			numResultsToReturn = numResultsToReturn - 1;
		}
		
		SearchResult[] spatialSearchResults = new SearchResult[arr.size()];
		spatialSearchResults = arr2.toArray(spatialSearchResults);
		return spatialSearchResults;
	}

	public String getXMLDataForItemId(String itemId) {
		// TODO: Your code here!
		//return "";
		String XMLstr = "<Item ItemID=\"" + itemId + "\">" + "\n";
		
		String Name, Currently, Buy_Price, First_Bid, Number_of_Bids, 
			Location, Latitude, Longitude, Country, Started, Ends, Seller_Rating, Seller_ID, Description;
		
		Name = "";
		Currently = "";
		Buy_Price = "";
		First_Bid = "";
		Number_of_Bids = "";
		Location = "";
		Latitude = "";
		Longitude = "";
		Country = "";
		Started = "";
		Ends = "";
		Seller_Rating = "";
		Seller_ID = "";
		Description = "";
		
		try {
			Statement stmt = conn.createStatement();
			
			ResultSet rs = stmt.executeQuery("SELECT * FROM Item WHERE ItemID ='" + itemId + "'");
			
			//if empty
			if( !rs.isBeforeFirst() ) {
				return "";
			}
			
			while (rs.next()) {
				Name = rs.getString("Name");
				Currently = String.format("%.2f", rs.getFloat("Currently"));
				Buy_Price = String.format("%.2f", rs.getFloat("Buy_Price"));
				First_Bid = String.format("%.2f", rs.getFloat("First_Bid"));
				Number_of_Bids = Integer.toString(rs.getInt("Number_of_Bids"));
				Location = rs.getString("Location");
				//Latitude = String.format("%.6f", rs.getFloat("Latitude"));
				//Longitude = String.format("%.6f", rs.getFloat("Longitude"));
				Latitude = rs.getString("Latitude");
				Longitude = rs.getString("Longitude");
				Country = rs.getString("Country");
				Started = rs.getTimestamp("Started").toString();
				Ends = rs.getTimestamp("ends").toString();
				Description = rs.getString("Description").toString();
			}
			
			XMLstr += "    <Name>" + toXML(Name) + "</Name>" + "\n";
			
			//Get all categories
			rs = stmt.executeQuery("SELECT * FROM ItemCategory WHERE ItemID = '" + itemId + "'");
			
			while (rs.next()) {
				XMLstr += "    <Category>" + toXML(rs.getString("Category")) + "</Category>" + "\n";
			}
			
			XMLstr += "    <Currently>" + "$" + toXML(Currently) + "</Currently>" + "\n";
			
			if (!Buy_Price.equals("0.00")) {
				XMLstr += "    <Buy_Price>" + "$" + toXML(Buy_Price) + "</Buy_Price>" + "\n";
			}
			
			XMLstr += "    <First_Bid>" + "$" + toXML(First_Bid) + "</First_Bid>" + "\n";
			
			XMLstr += "    <Number_of_Bids>" + toXML(Number_of_Bids) + "<Number_of_Bids>" + "\n";
			
			//Bids
			rs = stmt.executeQuery("SELECT * FROM Bid WHERE ItemID = '" + itemId + "'");
			
			//if empty
			if( !rs.isBeforeFirst() ) {
				XMLstr += "    <Bids />" + "\n";
			}
			else {
				XMLstr += "    <Bids>" + "\n";
				while(rs.next()) {
					XMLstr = BidXML(XMLstr, rs.getString("UserID"), rs.getTimestamp("Time").toString(),
						String.format("%.2f", rs.getFloat("Amount")));
				}
				XMLstr += "    </Bids>" + "\n";
			}
			
			//Location
			//if Lat and Long ""
			if(Latitude.equals("") && Longitude.equals("")) {
				XMLstr += "    <Location>" + toXML(Location) + "</Location>" + "\n";
			}
			else {
				XMLstr += "    <Location Latitude=\"" + toXML(Latitude) + "\" " + "Longitude=\"" + toXML(Longitude) + "\">" 
					+ toXML(Location) + "</Location>" + "\n";
			}
			
			XMLstr += "    <Country>" + toXML(Country) + "</Country>" + "\n";
			
			XMLstr += "    <Started>" + toXML(XMLtime(Started)) + "</Started>" + "\n";
			
			XMLstr += "    <Ends>" +  toXML(XMLtime(Ends)) + "</Ends>" + "\n";
			
			
			//Seller Rating and ID
			rs = stmt.executeQuery("SELECT * FROM User INNER JOIN Item " +
				"ON User.UserID = Item.UserID WHERE Item.ItemID = '" + itemId + "'");
				
			while (rs.next()) {
				Seller_Rating = Integer.toString(rs.getInt("SellerRating"));
				Seller_ID = rs.getString("UserID");
			}
			
			XMLstr += "    <Seller Rating=\"" + toXML(Seller_Rating) + "\" " + "UserID=\"" + toXML(Seller_ID) + "\" />" + "\n";
			
			
			XMLstr += "    <Description>" + toXML(Description) + "</Description>" + "\n";
			
			XMLstr += "</Item>";
			
		} catch (SQLException e) {};
		
		return XMLstr;
	}
	
	public String BidXML(String XMLstr, String UserID, String Time, String Amount) {
		String Rating = "";
		String Location = "";
		String Country = "";
		
		try {
			Statement stmt = conn.createStatement();
			ResultSet rs = stmt.executeQuery("SELECT * FROM User WHERE UserID ='" + UserID + "'");
			
			while (rs.next()) {
				Rating = Integer.toString(rs.getInt("BuyerRating"));
				Location = rs.getString("Location");
				Country = rs.getString("Country");
			}
			
			XMLstr += "      <Bid>" + "\n";
			XMLstr += "        <Bidder " + "Rating=\"" + toXML(Rating) + "\" " + "UserID=\"" + toXML(UserID) + "\">" + "\n";
			XMLstr += "          <Location>" + toXML(Location) + "</Location>" + "\n";
			XMLstr += "          <Country>" + toXML(Country) + "</Country>" + "\n";
			XMLstr += "        </Bidder>" + "\n";
			XMLstr += "        <Time>" + toXML(XMLtime(Time)) + "</Time>" + "\n";
			XMLstr += "        <Amount>$" + toXML(Amount) + "</Amount>" + "\n";
			XMLstr += "      </Bid>" + "\n";
			
		} catch (SQLException e) {};
		
		return XMLstr;
	}
	
	public static String toXML(String str) {
		final StringBuilder res = new StringBuilder();
		final StringCharacterIterator iter = new StringCharacterIterator(str);
		char c = iter.current();
		
		while (c != CharacterIterator.DONE) {
			if(c == '<') {
				res.append("&lt;");
			}
			else if(c == '>') {
				res.append("&gt;");
			}
			else if (c == '\\') {
				res.append("\\");
			}
			else if (c == '&') {
				res.append("&amp;");
			}
			else if (c == '\"') {
				res.append("&quot;");
			}
			else {
				res.append(c);
			}

			c = iter.next();
		}
		return res.toString();
	}
	
	public static String XMLtime(String time) {
		try {
			SimpleDateFormat XMLf = new SimpleDateFormat("MMM-dd-yy HH:mm:ss");
			SimpleDateFormat dbf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			
			time = XMLf.format(dbf.parse(time));
			
		} catch (Exception e) {
			System.out.println("Cannot parse (ERROR)");
		}
		return time;
	}
	
	public String echo(String message) {
		return message;
	}

}
