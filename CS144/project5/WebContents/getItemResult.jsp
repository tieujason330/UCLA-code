<%@ page import="edu.ucla.cs.cs144.SearchResult" %>
<%@ page import="javax.xml.parsers.DocumentBuilderFactory" %>
<%@ page import="javax.xml.parsers.DocumentBuilder" %>


<html>

<body onload="initialize()"> 
<form action="/eBay/item">
	Item ID: <input type="text" name="id" />
	<input type="submit" />
	<br />
</form>

<hr>

<div style="width:100%">
<% if (request.getAttribute("Name") == "")
	{
	%>
		<h1 style="text-align:center;">Item ID is not valid.</h1>
	<%
	} 
	%>
<h1 style="text-align:center;"><%= request.getAttribute("Name") %></h1>

<div style="float: left; width: 40%">	
	<p><b>Categories:</b> 
	<% String[] Categories = (String[]) request.getAttribute("Categories");
			
			for(int i = 0; i < Categories.length; i++) 
			{
				if (i == 0) {
			%>
			<%=Categories[i]%>
			<%
				}
				else {
				%>
				<%= ", " + Categories[i]%>

	<%
			}
			}		
			%>
	</p>
	

	
	<p><b>Started:</b>  <%= request.getAttribute("Started") %></p>

	<p><b>Seller:</b>  
	<table style="text-align:center; width:300px" >
		<tr>
			<th>Seller UserID</th>
			<th>Seller Rating</th>
		</tr>
		
		<tr>
			<td style="width:50%"><%= request.getAttribute("Seller_UID") %></td>
			<td style="width:50%"><%= request.getAttribute("Seller_Rating") %></td>
		</tr>
	</table>

	<% if (request.getAttribute("Description") != "")
	{
	%>
		<p><b>Description:</b>  <%= request.getAttribute("Description") %></p>
	<%
	} 
	%>

	
	<% 	
		String[] BidLocation = (String[]) request.getAttribute("BidLocation");
		String[] BidCountry = (String[]) request.getAttribute("BidCountry");
	%>
	<table style="text-align:center; width:450px" >
		<tr>
			<th><b>Location</b> </th>
			<th><b>Country</b> </th>
		</tr>
		
		<tr>
			<td><%= BidLocation[BidLocation.length-1] %></td>
			<td><%= BidCountry[BidCountry.length-1] %></td>
		</tr>
	</table>
	<% 
		String Latitude = (String) request.getAttribute("Latitude");
		String Longitude = (String) request.getAttribute("Longitude");
	%>
	
	<% if (Latitude != "" && Longitude != "") { %>
	<p><b>Latitude, Longitude: </b>  <%= request.getAttribute("Latitude") %> , <%= request.getAttribute("Longitude")%></p>
	<% } %>
	
	<% String geoLocation = BidLocation[BidLocation.length-1] + ", " + BidCountry[BidCountry.length-1]; %>
	
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" /> 

	<script type="text/javascript" 
		src="https://maps.googleapis.com/maps/api/js?sensor=false"> 
	</script> 
	<script type="text/javascript"> 
	  function initialize() { 
		//lat = <%=Latitude%> === "";
		//longi = <%=Longitude%> === "";
		console.log("Latitude = " + "<%=Latitude%>");
		console.log("Longitude = " + "<%=Longitude%>");
		//console.log("lat = " + lat);
		//console.log("longi = " + longi);
		
		if ("<%=Latitude%>" !== ""  && "<%=Longitude%>" !== "") {
			var latlng = new google.maps.LatLng(parseFloat("<%=Latitude%>"), parseFloat("<%=Longitude%>"));
			var myOptions = {
				zoom: 14,
				center: latlng,
				mapTypeId: google.maps.MapTypeId.ROADMAP
			};
			var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions); 
			return;
		}
		
		//If latitude and longitude aren't specified -> use location and country
		geocoder = new google.maps.Geocoder();
		var address = "<%=geoLocation%>";
		geocoder.geocode( { 'address': address}, function(results, status) {
		  if (status == google.maps.GeocoderStatus.OK) {
			var myOptions = {
					zoom: 14,
					center: results[0].geometry.location,
					mapTypeId: google.maps.MapTypeId.ROADMAP
				};
			var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions); 
			map.setCenter(results[0].geometry.location);
			var marker = new google.maps.Marker({
				map: map,
				position: results[0].geometry.location
			});
		  } 
		});
	  } 
	</script> 
	<div id="map_canvas" style="width:500px; height:400px;border: 1px solid black;"></div> 
	
</div>
	
<div style="float:right; width:50%;">
<p><b>Currently Priced At:</b>  <%= request.getAttribute("Currently") %></p>

	<% if (request.getAttribute("Buy_Price") != "")
	{
	%>
		<form action="/eBay/buy">
			<p style="color:red"><b>Buy Price:</b>  <%= request.getAttribute("Buy_Price") %></p>
			
			<input type="hidden" name="id" value="<%= request.getAttribute("ItemID") %>" />
			<input type="submit" value="Pay Now" />
		</form>
	<%
	} 
	%>
	<p><b>First Bid:</b>  <%= request.getAttribute("First_Bid") %></p>
	<p><b>Number of Bids:</b>  <%= request.getAttribute("Number_of_Bids") %></p>
	<p><b>Bidders:</b></p>
	<p>
	<% String[] BidUID = (String[]) request.getAttribute("BidUID");
		String[] BidRating = (String[]) request.getAttribute("BidRating");
		String[] BidLoc = (String[]) request.getAttribute("BidLocation");
		String[] BidCoun = (String[]) request.getAttribute("BidCountry");
		String[] BidTime = (String[]) request.getAttribute("BidTime");
		String[] BidAmount = (String[]) request.getAttribute("BidAmount");
			
			if (BidUID.length == 0) {
			%>
			<p>There are no bidders.</p>
			<%
			}
			for(int i = 0; i < BidUID.length; i++) 
			{
			if (i == 0) {
			%>
			<table style="text-align:center;" border="1" cellpadding="5">
				<tr>
					<th>Bid #</th>
					<th>UserID </th>
					<th>Rating </th>
					<th>Location </th>
					<th>Country </th>
					<th>Time </th>
					<th>Amount </th>
				</tr>
			<%
		}
			%>
			<tr> 
				<td><%= i+1 %></td> 
				<td><%= BidUID[i]%></td>
				<td><%= BidRating[i]%></td>
				<td><%= BidLoc[i] %></td>
				<td><%= BidCoun[i] %></td>
				<td><%= BidTime[i] %></td>
				<td><%= BidAmount[i] %></td>
			</tr>
			<% if (i == (BidUID.length-1)) { %>
			</table>
		
			<%
			}
			%>	

			<%
			}		
			%>
	</p>
</div>
</div>
</body>
</html>