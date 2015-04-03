package edu.ucla.cs.cs144;

import java.io.IOException;
import java.net.HttpURLConnection;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.net.*;
import java.io.*;

public class ProxyServlet extends HttpServlet implements Servlet {
       
    public ProxyServlet() {}

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
        response.setContentType("text/xml"); 
        
        String baseurl = "http://google.com/complete/search?output=toolbar&";
		String charset = "UTF-8";
		String q = request.getParameter("q");

		String qq = String.format("q=%s", URLEncoder.encode(q, charset));
		
		URLConnection connection = new URL(baseurl + qq).openConnection();
		connection.setRequestProperty("Accept-Charset", charset);
		
		InputStreamReader isr = new InputStreamReader(connection.getInputStream());
		BufferedReader in = new BufferedReader(isr);
		String inputLine;
		StringBuffer buffer = new StringBuffer();
	 
		while ((inputLine = in.readLine()) != null) {
			buffer.append(inputLine);
		}
		
		PrintWriter out = response.getWriter();
        out.println(buffer.toString());
    }
}
