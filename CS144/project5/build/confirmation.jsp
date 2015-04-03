<html>
	<body>
	
	<h1 style="text-align:center;">Confirmation Page</h1>

    <p><b>Item ID:</b> <%= request.getAttribute("ID") %></p> 
    <p><b>Item Name:</b> <%= request.getAttribute("Name") %></p> 
   	<p><b>Buy Price:</b> <%= request.getAttribute("BuyPrice") %></p> 
    <p><b>Credit Card:</b> <%= request.getAttribute("CC") %></p>
    <p><b>Time:</b> <%= request.getAttribute("Time") %></p>
    	
	</body>
</html>