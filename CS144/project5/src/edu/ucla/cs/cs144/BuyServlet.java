package edu.ucla.cs.cs144;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;
import java.util.HashMap;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class BuyServlet extends HttpServlet implements Servlet {
       
  public BuyServlet() {}

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
  {
  	try
  	{
	  	String qType = request.getParameter("action");
	
	  	if (qType.equals("confirm"))
	  	{
	  		if (request.isSecure())
	  		{
	  			String id = request.getParameter("id");

		  		HttpSession session = request.getSession(true);
		  		HashMap<String, String[]> tempmap = (HashMap<String, String[]>)session.getAttribute("itemMap");
		  		if (tempmap.containsKey(id))
		  		{
		  			String[] values = (String[])tempmap.get(id);
			  		request.setAttribute("Name", values[0]);
						request.setAttribute("ID", id);
						request.setAttribute("BuyPrice", values[1]);

						request.setAttribute("CC", request.getParameter("CreditCard"));
						request.setAttribute("Time", new Date());
						request.getRequestDispatcher("/confirmation.jsp").forward(request, response);
					}
				}
	  	}
		/*
		request.setAttribute("Test", qType);
		request.setAttribute("Name", "Null");
		request.setAttribute("ID", "Null");
		request.setAttribute("BuyPrice", "Null");

		request.setAttribute("CC", "Null");
		request.setAttribute("Time", "Null");
		request.getRequestDispatcher("/confirmation.jsp").forward(request, response);
		*/
  	}
  	catch (Exception e)
  	{
  		String id = request.getParameter("id");
	  	HttpSession session = request.getSession(true);

	  	HashMap<String, String[]> tempmap = (HashMap<String, String[]>)session.getAttribute("itemMap");
	  	if (tempmap.containsKey(id)) {
	  		String[] itemVals = (String[])tempmap.get(id);
	  		request.setAttribute("Name", itemVals[0]);
			request.setAttribute("ID", id);
			request.setAttribute("BuyPrice", itemVals[1]);
			request.getRequestDispatcher("/creditcardinput.jsp").forward(request, response);
	  	}
		/*
		request.setAttribute("Test", qType);
		request.setAttribute("Name", "Null");
		request.setAttribute("ID", id);
		request.setAttribute("BuyPrice", "Null");
		request.getRequestDispatcher("/creditcardinput.jsp").forward(request, response);
		*/
	  	
		}
  }
  
  public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
  {
  	doGet(request, response);
  }
  
}