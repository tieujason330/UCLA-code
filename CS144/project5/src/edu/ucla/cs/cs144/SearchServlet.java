package edu.ucla.cs.cs144;

import java.io.IOException;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import edu.ucla.cs.cs144.SearchResult;
import org.apache.axis2.AxisFault;
import edu.ucla.cs.cs144.AuctionSearchClient;
import java.io.PrintWriter;

public class SearchServlet extends HttpServlet implements Servlet {
       
    public SearchServlet() {}

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
        String q = request.getParameter("q");
		int numResultsToReturn = 0;
		int numResultsToSkip = 0;
		SearchResult[] basicResults;
		try
		{
			if (q != "") {
				numResultsToReturn = Integer.parseInt(request.getParameter("numResultsToReturn"));
				numResultsToSkip = Integer.parseInt(request.getParameter("numResultsToSkip"));
			}
        }
        catch (Exception e)
        {
        }

			basicResults = AuctionSearchClient.basicSearch(q, numResultsToSkip, numResultsToReturn);

		request.setAttribute("results", basicResults);
		request.setAttribute("q", q);
		request.setAttribute("numResultsToSkip", numResultsToSkip);
		request.setAttribute("numResultsToReturn", numResultsToReturn);
			
		request.getRequestDispatcher("/searchResult.jsp").forward(request, response);
    }
}