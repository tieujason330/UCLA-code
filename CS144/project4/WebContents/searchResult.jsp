<%@ page import="edu.ucla.cs.cs144.SearchResult" %>
<%
	String q = "";
	int numResultsToSkip = 0;
	int numResultsToReturn = 0;
	if(request.getAttribute("q") != null) {
		q = request.getAttribute("q").toString();
		numResultsToSkip = Integer.parseInt(request.getAttribute("numResultsToSkip").toString());
		numResultsToReturn = Integer.parseInt(request.getAttribute("numResultsToReturn").toString());
	}
%>

<html>
	<head>
		<script type="text/javascript" src="googlesuggest.js"></script>
		<link rel="stylesheet" type="text/css" href="googlesuggest.css" />   
		
		<script type="text/javascript">
    	window.onload = function () {
				var txtbox = new GoogleSuggestController(document.getElementById("qb"));        
		}
		</script>		
	</head>	
	<body>
		
		<form action="/eBay/search">
			
			Enter Query: <input type="text" name="q" id="qb" value="<%= q %>" style="width:400px;" autocomplete="off" /> 
			
			<input type="hidden" name="numResultsToSkip" value="0" />
			<input type="hidden" name="numResultsToReturn" value="11" />
			<input type="submit" /> <br /> 
		</form>
		
		<hr>
		
		<%
		SearchResult[] basicResults = (SearchResult[]) request.getAttribute("results");
	
		int n = basicResults.length;
		
		if ( n == 0) {
		%>
			<p> No results found for "<%= q %>". </p>
		<%
		}
		if (n > 10){
			n = 10;
		}
		
		if (n > 0){
			if (!basicResults[0].getItemId().equals("-1")){
		%>
		
		<%
				n = 0;
				for (SearchResult r : basicResults) {
					if (n == 10){
						break;
					}
		%>
		<a href="/eBay/item?id=<%= r.getItemId() %>" > <%= r.getItemId() + ": " + r.getName() %> </a> <br /><br /> 
		<%
					n++;
				}		
			}
		%>
		
		<p> Showing Results <%= numResultsToSkip+1 %> to <%= numResultsToSkip+n %></p>
		<br />
		<hr>
		<% 
				if (numResultsToSkip > 0){
					%> 
					<a href="/eBay/search?q=<%= q %>&numResultsToSkip=<%= numResultsToSkip-10 %>&numResultsToReturn=11">Previous Page</a>
					<b>|</b>
					<%
				}
				if (basicResults.length - 10 > 0){
					%> 
					<a href="/eBay/search?q=<%= q %>&numResultsToSkip=<%= numResultsToSkip+10 %>&numResultsToReturn=11">Next Page</a>
					<%
				}
		} 
		%>	
	</body>
</html>