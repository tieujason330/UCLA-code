package edu.ucla.cs.cs144;

import java.io.IOException;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import edu.ucla.cs.cs144.AuctionSearchClient;
import org.apache.axis2.AxisFault;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;
import java.io.StringReader;
import java.io.PrintWriter;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.servlet.jsp.JspWriter;

import javax.servlet.http.HttpSession;
import java.util.HashMap;

public class ItemServlet extends HttpServlet implements Servlet {
       
    public ItemServlet() {}

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
        
		String queryId = request.getParameter("id");
		String xmlString = AuctionSearchClient.getXMLDataForItemId(queryId);
		String[] Categories = new String[1];
		String[] BidUID = new String[1];
		String[] BidCountry = new String[1];
		String[] BidTime = new String[1];
		String[] BidRating = new String[1];
		String[] BidLocation = new String[1];
		String Latitude = "";
		String Longitude = "";
		String[] BidAmount = new String[1];

		String Name = "";
		String Description = "";
		String Currently = ""; 
		String Buy_Price = ""; 
		String First_Bid = ""; 
		String Number_of_Bids = "";
		String Started = "";
		String Seller_UID = "";
		String Seller_Rating = ""; 
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder b;
		Document doc = null;
		
		//PrintWriter out = response.getWriter();
		//out.println("TESTSTEST");
		//out.print(xmlString);
		try {
			b = factory.newDocumentBuilder();
			doc = b.newDocument();
			doc = b.parse(new InputSource (new StringReader(xmlString)));
			//doc = b.newDocument();
		} catch (Exception e) {}
		try {
			Name = doc.getElementsByTagName("Name").item(0).getTextContent();
			Currently = doc.getElementsByTagName("Currently").item(0).getTextContent();
			
			NodeList categoryList = doc.getElementsByTagName("Category");

			Categories = new String[categoryList.getLength()];

			for (int i = 0; i < categoryList.getLength(); i++) {
				Categories[i] = categoryList.item(i).getTextContent();
			}
			
			Started = doc.getElementsByTagName("Started").item(0).getTextContent();
			First_Bid = doc.getElementsByTagName("First_Bid").item(0).getTextContent();
			
			if (doc.getElementsByTagName("Buy_Price").getLength() != 0) {
				Buy_Price = doc.getElementsByTagName("Buy_Price").item(0).getTextContent();
			}

			Number_of_Bids = doc.getElementsByTagName("Number_of_Bids").item(0).getTextContent();

			NodeList SellerList = doc.getElementsByTagName("Seller");
			Node sellernode = SellerList.item(0);
			Element sellerelement = (Element) sellernode;

			Seller_UID = sellerelement.getAttribute("UserID");
			Seller_Rating = sellerelement.getAttribute("Rating");
			Description = doc.getElementsByTagName("Description").item(0).getTextContent();

			NodeList bidderList = doc.getElementsByTagName("Bidder");
			BidUID = new String[bidderList.getLength()];
			BidRating = new String[bidderList.getLength()];
			Element bidderElement;
			for (int i = 0; i < bidderList.getLength(); i++) {
				Node bid = bidderList.item(i);
				bidderElement = (Element) bid;
				BidUID[i] = bidderElement.getAttribute("UserID");
				BidRating[i] = bidderElement.getAttribute("Rating");
			}
			
			NodeList timeList = doc.getElementsByTagName("Time");
			BidTime = new String[timeList.getLength()];
			for (int i = 0; i < timeList.getLength(); i++) {
				BidTime[i] = timeList.item(i).getTextContent();
			}

			NodeList amountList = doc.getElementsByTagName("Amount");
			BidAmount = new String[amountList.getLength()];
			for (int i = 0; i < amountList.getLength(); i++) {
				BidAmount[i] = amountList.item(i).getTextContent();
			}

			NodeList locationList = doc.getElementsByTagName("Location");
			BidLocation = new String[locationList.getLength()];
			for (int i = 0; i < locationList.getLength(); i++) {
				BidLocation[i] = locationList.item(i).getTextContent();
				Element LocationElement= (Element) locationList.item(i);
				Latitude = LocationElement.getAttribute("Latitude");
				Longitude = LocationElement.getAttribute("Longitude");
			}
			
			/*
			NodeList latitudeList = doc.getElementsByTagName("Latitude");
			Latitude = new String[latitudeList.getLength()];
			for (int i = 0; i < latitudeList.getLength(); i++) {
				Latitude[i] = latitudeList.item(i).getTextContent();
			}
			
			NodeList longitudeList = doc.getElementsByTagName("Longitude");
			Longitude = new String[longitudeList.getLength()];
			for (int i = 0; i < longitudeList.getLength(); i++) {
				Longitude[i] = longitudeList.item(i).getTextContent();
			}
			*/

			NodeList countryList = doc.getElementsByTagName("Country");
			BidCountry = new String[countryList.getLength()];
			for (int i = 0; i < countryList.getLength(); i++) {
				BidCountry[i] = countryList.item(i).getTextContent();
			}


		}
		catch (Exception e) {
		}
		request.setAttribute("ItemID", queryId);
		request.setAttribute("Name", Name);
		request.setAttribute("Categories", Categories);
		request.setAttribute("Description", Description);
		request.setAttribute("Number_of_Bids", Number_of_Bids);
		request.setAttribute("Seller_UID", Seller_UID);
		request.setAttribute("Seller_Rating", Seller_Rating);
		request.setAttribute("Currently", Currently);
		request.setAttribute("Buy_Price", Buy_Price);
		request.setAttribute("Started", Started);
		request.setAttribute("First_Bid", First_Bid);
		request.setAttribute("BidUID", BidUID);		
		request.setAttribute("BidTime", BidTime);
		request.setAttribute("BidCountry", BidCountry);
		request.setAttribute("BidRating", BidRating);
		request.setAttribute("BidAmount", BidAmount);
		request.setAttribute("BidLocation", BidLocation);
		request.setAttribute("Latitude", Latitude);
		request.setAttribute("Longitude", Longitude);
		
		if (!Buy_Price.equals("")) {
			HttpSession sess = request.getSession(true);
			
			HashMap<String, String[]> itemMap;
			
			if ( sess.getAttribute("itemMap") == null ) {
				itemMap = new HashMap<String, String[]>();
			}
			else {
				itemMap = (HashMap<String, String[]>)sess.getAttribute("itemMap");
			}
			
			if (!itemMap.containsKey(queryId)) {
				String[] mapVal = new String[2];
				mapVal[0] = Name;
				mapVal[1] = Buy_Price;
				itemMap.put(queryId, mapVal);
				sess.setAttribute("itemMap", itemMap);
			}
		}
		request.getRequestDispatcher("/getItemResult.jsp").forward(request, response);
	}
	
};