<% String encryptedURL = "https://" + request.getServerName() + ":8443" + request.getContextPath() + "/buy"; %>

<html>
	<body>
		<h1 style="text-align:center;">Credit Card Input Page</h1>
		
		<form method="post" action="<%= encryptedURL %>">

		<p><b>Item ID:</b> <%= request.getAttribute("ID") %></p> 
    	<p><b>Item Name:</b> <%= request.getAttribute("Name") %></p> 
    	<p><b>Buy Price:</b> <%= request.getAttribute("BuyPrice") %></p> <br />
    	
        <input type="hidden" name="id" value="<%= request.getAttribute("ID") %>" />
    	<input type="hidden" name="action" value="confirm" />
    	<b>Enter Credit Card #:</b> <input type="text" name="CreditCard" /> <br />
    	<input type="submit" value="Submit" />
		
		</form>
		
	</body>
</html>