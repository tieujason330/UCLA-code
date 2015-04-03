/* CS144
 *
 * Parser skeleton for processing item-???.xml files. Must be compiled in
 * JDK 1.5 or above.
 *
 * Instructions:
 *
 * This program processes all files passed on the command line (to parse
 * an entire diectory, type "java MyParser myFiles/*.xml" at the shell).
 *
 * At the point noted below, an individual XML file has been parsed into a
 * DOM Document node. You should fill in code to process the node. Java's
 * interface for the Document Object Model (DOM) is in package
 * org.w3c.dom. The documentation is available online at
 *
 * http://java.sun.com/j2se/1.5.0/docs/api/index.html
 *
 * A tutorial of Java's XML Parsing can be found at:
 *
 * http://java.sun.com/webservices/jaxp/
 *
 * Some auxiliary methods have been written for you. You may find them
 * useful.
 */

package edu.ucla.cs.cs144;

import java.io.*;
import java.text.*;
import java.util.*;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.FactoryConfigurationError;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.w3c.dom.Text;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.ErrorHandler;


class MyParser {
    
    static final String columnSeparator = "|*|";
    static DocumentBuilder builder;
     
	 /*
	  * need to write xml data to character stream outputs defined as file names
	  * that'll be loaded in MySQL tables
	 */
	 private static BufferedWriter itemWriter, itemCategoryWriter,
		userWriter, bidWriter; //itemCoordinatesWriter, coordinatesLocationWriter
	
	//Hashmap to store categories with and CategoryID
	//Map categories = new HashMap();
	
    static final String[] typeName = {
	"none",
	"Element",
	"Attr",
	"Text",
	"CDATA",
	"EntityRef",
	"Entity",
	"ProcInstr",
	"Comment",
	"Document",
	"DocType",
	"DocFragment",
	"Notation",
    };
	
	//Hashmap to store UserID -> (Buyer_Rating, Seller_Rating)
	static Map<String, String> UserID_SellerRating_Map = new HashMap<String, String>();
	static Map<String, String> UserID_BuyerRating_Map = new HashMap<String, String>();
	static Map<String, UserLocation> UserID_Location_Country_Map = new HashMap<String, UserLocation>();
	
	static class UserLocation {
		public String Location;
		public String Country;
		
		public UserLocation(String Loc, String Con) {
			this.Location = Loc;
			this.Country = Con;
		}
	}
    
    static class MyErrorHandler implements ErrorHandler {
        
        public void warning(SAXParseException exception)
        throws SAXException {
            fatalError(exception);
        }
        
        public void error(SAXParseException exception)
        throws SAXException {
            fatalError(exception);
        }
        
        public void fatalError(SAXParseException exception)
        throws SAXException {
            exception.printStackTrace();
            System.out.println("There should be no errors " +
                               "in the supplied XML files.");
            System.exit(3);
        }
        
    }
    
    /* Non-recursive (NR) version of Node.getElementsByTagName(...)
     */
    static Element[] getElementsByTagNameNR(Element e, String tagName) {
        Vector< Element > elements = new Vector< Element >();
        Node child = e.getFirstChild();
        while (child != null) {
            if (child instanceof Element && child.getNodeName().equals(tagName))
            {
                elements.add( (Element)child );
            }
            child = child.getNextSibling();
        }
        Element[] result = new Element[elements.size()];
        elements.copyInto(result);
        return result;
    }
    
    /* Returns the first subelement of e matching the given tagName, or
     * null if one does not exist. NR means Non-Recursive.
     */
    static Element getElementByTagNameNR(Element e, String tagName) {
        Node child = e.getFirstChild();
        while (child != null) {
            if (child instanceof Element && child.getNodeName().equals(tagName))
                return (Element) child;
            child = child.getNextSibling();
        }
        return null;
    }
    
    /* Returns the text associated with the given element (which must have
     * type #PCDATA) as child, or "" if it contains no text.
     */
    static String getElementText(Element e) {
        if (e.getChildNodes().getLength() == 1) {
            Text elementText = (Text) e.getFirstChild();
            return elementText.getNodeValue();
        }
        else
            return "";
    }
    
    /* Returns the text (#PCDATA) associated with the first subelement X
     * of e with the given tagName. If no such X exists or X contains no
     * text, "" is returned. NR means Non-Recursive.
     */
    static String getElementTextByTagNameNR(Element e, String tagName) {
        Element elem = getElementByTagNameNR(e, tagName);
        if (elem != null)
            return getElementText(elem);
        else
            return "";
    }
    
    /* Returns the amount (in XXXXX.xx format) denoted by a money-string
     * like $3,453.23. Returns the input if the input is an empty string.
     */
    static String strip(String money) {
        if (money.equals(""))
            return money;
        else {
            double am = 0.0;
            NumberFormat nf = NumberFormat.getCurrencyInstance(Locale.US);
            try { am = nf.parse(money).doubleValue(); }
            catch (ParseException e) {
                System.out.println("This method should work for all " +
                                   "money values you find in our data.");
                System.exit(20);
            }
            nf.setGroupingUsed(false);
            return nf.format(am).substring(1);
        }
    }
	
	/* convert date to MySQL format (yyyy-MM-dd HH:mm:ss) from (MMM-dd-yy HH:mm:ss)
	*/
	static String MySQL_date_format(String date) {
		
		String orig_format = "MMM-dd-yy HH:mm:ss";
		String MySQL_format = "yyyy-MM-dd HH:mm:ss";
		
		SimpleDateFormat f = new SimpleDateFormat(orig_format);
		
		String new_date = "";
		
		try {
			Date d = f.parse(date);
			f.applyPattern(MySQL_format);
			new_date = f.format(d);
		} catch (ParseException e) {
			System.out.println("Unable to parse date: " + orig_format);
		}
		
		return new_date;
	}
    
    /* Process one items-???.xml file.
     */
    static void processFile(File xmlFile) {
        Document doc = null;
        try {
            doc = builder.parse(xmlFile);
        }
        catch (IOException e) {
            e.printStackTrace();
            System.exit(3);
        }
        catch (SAXException e) {
            System.out.println("Parsing error on file " + xmlFile);
            System.out.println("  (not supposed to happen with supplied XML files)");
            e.printStackTrace();
            System.exit(3);
        }
        
        /* At this point 'doc' contains a DOM representation of an 'Items' XML
         * file. Use doc.getDocumentElement() to get the root Element. */
        System.out.println("Successfully parsed - " + xmlFile);
        
        /* Fill in code here (you will probably need to write auxiliary
            methods). */
        
		Element[] items_arr = getElementsByTagNameNR(doc.getDocumentElement(), "Item");
		
		try {
			for ( int i = 0; i < items_arr.length; i++) {
				parseItem(items_arr[i]);
				//parseItemCoordinates(items_arr[i]);
				//parseCoordinatesLocation(items_arr[i]);
				parseItemCategory(items_arr[i]);
				parseUser(items_arr[i]);
				parseBid(items_arr[i]);
			}
			
			//load all the users
			loadAllUsers();
		} catch (IOException e) {
			e.printStackTrace();
		}
        
        
        /**************************************************************/
        
    }
	/*
	 * parses an item to get the attributes/elements of that item
     * gets Item-	 
	 */
	public static void parseItem(Element item) throws IOException {
		//get variables from each column
		
		String ItemID = item.getAttribute("ItemID");
		
		String seller_UserID = getElementByTagNameNR(item, "Seller").getAttribute("UserID");
		
		String Name = getElementTextByTagNameNR(item, "Name");
		
		String Currently = strip( getElementTextByTagNameNR(item, "Currently") );
		
		String Buy_Price = strip( getElementTextByTagNameNR(item, "Buy_Price") );
		
		String First_Bid = strip( getElementTextByTagNameNR(item, "First_Bid") );
		
		String Number_of_Bids = getElementTextByTagNameNR(item, "Number_of_Bids");
		
		String Started = MySQL_date_format( getElementTextByTagNameNR(item, "Started") );
		
		String Ends = MySQL_date_format( getElementTextByTagNameNR(item, "Ends") );
		
		String Description = getElementTextByTagNameNR(item, "Description");
		//if over 4000 characters, truncate at 4000
		if ( Description.length() > 4000 ) {
			Description = Description.substring(0, 4000);
		}
		
		String Location = getElementTextByTagNameNR(item, "Location");
		
		String Latitude = getElementByTagNameNR(item, "Location").getAttribute("Latitude");
		if(Latitude == null)
			Latitude = "";
		
		String Longitude = getElementByTagNameNR(item, "Location").getAttribute("Longitude");
		if(Longitude == null)
			Longitude = "";
		
		String Country = getElementTextByTagNameNR(item, "Country");
		//write out to itemFileWriter
		load(itemWriter, ItemID, seller_UserID, Name, Currently, Buy_Price, First_Bid, 
		Number_of_Bids, Started, Ends, Description, Location, Latitude, Longitude, Country);
	}
	
	/* UNUSED FUNCTION 
	public static void parseItemCoordinates(Element item) throws IOException {
		
		String ItemID = item.getAttribute("ItemID");
		
		String Latitude = getElementByTagNameNR(item, "Location").getAttribute("Latitude");
		
		String Longitude = getElementByTagNameNR(item, "Location").getAttribute("Longitude");
		
		load(itemCoordinatesWriter, ItemID, Latitude, Longitude);
	}
	*/
	
	/* UNUSED FUNCTION 
	public static void parseCoordinatesLocation(Element item) throws IOException {
		
		String Latitude = getElementByTagNameNR(item, "Location").getAttribute("Latitude");
		if(Latitude == null)
			Latitude = "";
		
		String Longitude = getElementByTagNameNR(item, "Location").getAttribute("Longitude");
		if(Longitude == null)
			Longitude = "";
		
		String Location = getElementTextByTagNameNR(item, "Location");
		
		String Country = getElementTextByTagNameNR(item, "Country");
		
		load(coordinatesLocationWriter, Latitude, Longitude, Location, Country);
	}
	*/
	public static void parseItemCategory(Element item) throws IOException {
		String ItemID = item.getAttribute("ItemID");
		
		Element[] categories = getElementsByTagNameNR(item, "Category");
		
		for(int i = 0; i < categories.length; i++) {
			String Category = getElementText(categories[i]);
			load(itemCategoryWriter, ItemID, Category);
		}
	}
	
	/* Gets the seller and all bidders 
	   does not write out to file since need to store seller and buyer rating for each user
	 */
	public static void parseUser(Element item) throws IOException {
		
		Element Seller = getElementByTagNameNR(item, "Seller");
		String Seller_UserID = Seller.getAttribute("UserID");
		String Seller_Rating = Seller.getAttribute("Rating");
		
		//store in UserID -> Seller Rating map
		UserID_SellerRating_Map.put(Seller_UserID, Seller_Rating);
		
		Element[] bids = getElementsByTagNameNR(getElementByTagNameNR(item, "Bids"), "Bid");
		
		for( int i = 0 ; i < bids.length ; i++ ) {
			Element Bidder = getElementByTagNameNR(bids[i], "Bidder");
			
			String Bidder_UserID = Bidder.getAttribute("UserID");
			String Bidder_Rating = Bidder.getAttribute("Rating");
			
			//store in UserID -> Bidder Rating map
			UserID_BuyerRating_Map.put(Bidder_UserID, Bidder_Rating);
			
			String Bidder_Location = getElementTextByTagNameNR(Bidder, "Location");
			if(Bidder_Location == null)
				Bidder_Location = "";
			
			String Bidder_Country = getElementTextByTagNameNR(Bidder, "Country");
			if(Bidder_Country == null)
				Bidder_Country = "";
			
			UserLocation loc_coun = new UserLocation(Bidder_Location, Bidder_Country);
			
			//store in UserID -> Location & Country Map
			UserID_Location_Country_Map.put(Bidder_UserID, loc_coun);
		}
	}
	
	public static void parseBid(Element item) throws IOException {
		
		String ItemID = item.getAttribute("ItemID");
		
		Element[] bids = getElementsByTagNameNR(getElementByTagNameNR(item, "Bids"), "Bid");
		
		for( int i = 0 ; i < bids.length ; i++ ) {
			Element Bidder = getElementByTagNameNR(bids[i], "Bidder");
			
			String UserID = Bidder.getAttribute("UserID");
			
			String Rating = Bidder.getAttribute("Rating");
			
			String Time = MySQL_date_format( getElementTextByTagNameNR(bids[i], "Time") );
			
			String Amount = strip( getElementTextByTagNameNR(bids[i], "Amount"));
			
			load(bidWriter, UserID, Time, ItemID, Amount);
		}
	}
	
	/* After storing all the users' data into maps, now load into the userWriter
	*/
	public static void loadAllUsers() throws IOException {
		
		//get sets for SellerRating Map and BuyerRating Map
		Set<String> keysInSeller = new HashSet<String>(UserID_SellerRating_Map.keySet());
		Set<String> keysInBuyer = new HashSet<String>(UserID_BuyerRating_Map.keySet());
		
		//keys only in Sellers
		Set<String> inSellerNotBuyer = new HashSet<String>(keysInSeller);
		inSellerNotBuyer.removeAll(keysInBuyer);
		
		//keys only in Buyers
		Set<String> inBuyerNotSeller = new HashSet<String>(keysInBuyer);
		inBuyerNotSeller.removeAll(keysInSeller);
		
		//keys common to both maps
		Set<String> commonSet = new HashSet<String> (keysInSeller);
		commonSet.retainAll(keysInBuyer);
		
		for (String UserID : commonSet) {
			String SellerRating = UserID_SellerRating_Map.get(UserID);
			if( SellerRating == null)
				SellerRating = "";
			
			String BuyerRating = UserID_BuyerRating_Map.get(UserID);
			if (BuyerRating == null)
				BuyerRating = "";
			
			UserLocation UL = UserID_Location_Country_Map.get(UserID);
			
			String Location = UL.Location;
			if (Location == null)
				Location = "";
			
			String Country = UL.Country;
			if (Country == null)
				Country = "";
			
			load(userWriter, UserID, SellerRating, BuyerRating, Location, Country);
		}
		
		for (String UserID : inSellerNotBuyer) {
			String SellerRating = UserID_SellerRating_Map.get(UserID);
			String BuyerRating = "";
			String Location = "";
			String Country = "";
			
			load(userWriter, UserID, SellerRating, BuyerRating, Location, Country);
		}
		
		for (String UserID : inBuyerNotSeller) {
			String SellerRating = UserID_SellerRating_Map.get(UserID);
			
			String BuyerRating = UserID_BuyerRating_Map.get(UserID);
			
			UserLocation UL = UserID_Location_Country_Map.get(UserID);
			
			String Location = UL.Location;
			if (Location == null)
				Location = "";
			
			String Country = UL.Country;
			if (Country == null)
				Country = "";
			
			load(userWriter, UserID, SellerRating, BuyerRating, Location, Country);
		}
	}
	
	//all column separators to match MySQL
	private static String formatRow( String[] inputs ) {
		String formatted_input = "";
		
		int index = 0;
		
		for( ; index < inputs.length-1; index++ ) 
			formatted_input += inputs[index] + columnSeparator;
		
		formatted_input += inputs[index]; //last one
		
		return formatted_input;
	}
	
	private static void load(BufferedWriter output, String... arguments) throws IOException {
		output.write( formatRow(arguments) );
		output.newLine();
	}
    
    public static void main (String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: java MyParser [file] [file] ...");
            System.exit(1);
        }
        
        /* Initialize parser. */
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            factory.setValidating(false);
            factory.setIgnoringElementContentWhitespace(true);      
            builder = factory.newDocumentBuilder();
            builder.setErrorHandler(new MyErrorHandler());
        }
        catch (FactoryConfigurationError e) {
            System.out.println("unable to get a document builder factory");
            System.exit(2);
        } 
        catch (ParserConfigurationException e) {
            System.out.println("parser was unable to be configured");
            System.exit(2);
        }
		
		try {
			itemWriter = new BufferedWriter(new FileWriter("item.dat", true));
			//itemCoordinatesWriter = new BufferedWriter(new FileWriter("itemcoordinates.dat", true));
			//coordinatesLocationWriter = new BufferedWriter(new FileWriter("coordinateslocation.dat", true));
			itemCategoryWriter = new BufferedWriter(new FileWriter("itemcategory.dat", true));
			userWriter = new BufferedWriter(new FileWriter("user.dat", true));
			bidWriter = new BufferedWriter(new FileWriter("bid.dat", true));
			/* Process all files listed on command line. */
			for (int i = 0; i < args.length; i++) {
				File currentFile = new File(args[i]);
				processFile(currentFile);
			}
			
			itemWriter.close();
			//itemCoordinatesWriter.close();
			//coordinatesLocationWriter.close();
			itemCategoryWriter.close();
			userWriter.close();
			bidWriter.close();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
        

    }
}
