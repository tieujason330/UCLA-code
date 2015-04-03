Jason Tieu
304047667 

project3-searcher.zip

Part B: Spatial Search

In this part, you will implement two remaining methods of IAuctionSearch interface: spatialSearch() and getXMLDataForItemId().
public interface IAuctionSearch {
    public SearchResult[] basicSearch(String query, int numResultsToSkip, int numResultsToReturn);
    public SearchResult[] spatialSearch(String query, SearchRegion region, int numResultsToSkip, int numResultsToReturn);
    public String getXMLDataForItemId(String itemId);
    public String echo(String message);
}

The spatialSearch() method is an extension of the basicSearch() method and it returns items if they satisfy both of the following two conditions:
The items should have at least one keyword in the query parameter in their title, category, or description fields (this is exactly the same condition used in basicSearch()) and
The items should be located within the rectangular region specified in the region parameter. A region is specified with four coordinates, lx, ly, rx, and ry, where (lx, ly) corresonds to (latitude, longitude) of the lower left corner of the rectangle and (rx, ry) to (latitude, longitude) of the upper right corner. The SearchRegion class is defined in the SearchRegion.java file located in src/edu/ucla/cs/cs144.
We will explain the getXMLDataForItemId() method later.
B.1: Learn about Spatial Index Support in MySQL

You can use the Lucene index(es) that you built earlier to check the keyword-matching condition of spatialSearch(). To check the spatial condition, it is possible build two separate B+tree indexes on latitude and longitude attributes and look up both indexes using the specified region. This approach is likely to be better than a full scan of the entire table(s), but a better approach is to build a spatial index that indexes on both attributes simultaneously and intelligently divides the two dimensional space, so that items within a given region can be identified much more efficiently with a single index lookup.

In MySQL, it is possible to create a spatial index (in particular, R-tree index) on geometric (or spatial) attributes for efficient spatial-query support. Go over MySQL manual pages on Using Spatial Data. Pay particular attention to the section on Creating Spatial Indexes and Using Spatial Indexes to learn how to create and use a spatial index in MySQL.

B.2: Build Spatial Index in MySQL

Now that you have learned how to use a spatial index in MySQL, it is time to create one. Note that in MySQL 5.5, a spatial index can be built only for MyISAM tables only, so we will first need to create a table that contains information on items' latitude and longitude using the MyISAM storage engine. In particular, the table should contain (itemId, latitude, longitude) of each item, where (latitude, longitude) is stored as a single column of POINT type so that a spatial index can be created on the column.

Write an SQL script that (1) creates such a table using MyISAM storage engine, (2) populate the table with itemId, latitude, and longitude information, and (3) create a spatial index on latitude and longitude. You can use POINT() function in MySQL to convert a pair of numeric values to a POINT. Name the SQL script as buildSQLIndex.sql. Run this script once, to create the table and the spatial index.

Write another SQL script containing the DROP TABLE and DROP INDEX statements that delete the tables and indexes for spatial query support. Name this script as dropSQLIndex.sql. (You MUST NOT run this script! You will need the spatial index for the next step.)

Note: Remember that MySQL automatically creates an index on the primary key of each table, so you will not have to create an index on the primary key of any table.

B.3: Implement spatialSearch Method

Now implement the spatialSearch() method in the AunctionSearch.java file. Your implementation of spatialSearch() must use Lucene index(es) to handle the keyword constraint and MySQL spatial index for spatial constraint. If you followed our instruction in Part A, your Lucene index(es) must be located below /var/lib/lucene/. For querying and retrieving data from MySQL, make sure that your code uses the database CS144 and connect it as the user "cs144" with empty password.

Note: You may use any ranking method for ordering the results from spatialSearch.

B.4: Implement getXMLDataFromItemId Method

The third getXMLDataForItemId() method of IAuctionSearch should return the XML-formatted item data as string given its itemID. Make sure that the output from getXMLDataForItemId() conforms to the DTD of the original eBay XML data, except that the root element should now be <ITEM>, not <ITEMS> (note that this method returns only one eBay item, not multiple items). Also make sure that the format of each XML element is as close to the original XML data as possible, particularly if the SQL data type is TIMESTAMP. You will have to escape the character "<" to "&lt;" and ">" to "&gt", and so on (Reference) if they are not part of an XML tag. Finally, if there is no matching Item for a given itemId, return an empty string.

Note: You can use the Java class SimpleDateFormat to parse and format date strings.

We have included a simple test file AuctionSearchTest.java in project3-searcher.zip. You can use the ant target "run" to invoke this AuctionSearchTest class for basic testing of your code. Before you move on to the next step, add more test cases to AuctionSearchTest, thoroughly test your code, and make sure that your implementation is correct.

Part C: Publish Your Java Class as Web Service

Now that you have finished your implementation of the AuctionSearch class, you need to publish it as a Web service at the URL http://localhost:1448/axis2/services/AuctionSearchService. This will help other applications search and access your eBay data remotely over the Internet. Apache Tomcat server with Axis2 makes the publication of an existing Java class as a Web service extremely simple.
C.1: Learn about Apache Tomcat and Axis2

Apache Tomcat is a Java Servlet container and web server. A web server dishes out web pages in response to requests from a user sitting at a web browser. But web servers are not limited to serving up static HTML pages; they can also run programs in response to user requests and return the dynamic results to the user's browser. In particular, Tomcat supports both Java servlet and JavaServerPages (JSP) technologies to serve dynamic contents. Read the Apache Tomcat access instruction to learn how to access a few pages from the preinstalled Tomcat server from your host.

Axis2 is a Java package that can be installed within a Tomcat server to help the server handle Web-service requests. Once installed, Axis2 (1) monitors the HTTP requests to your Tomcat server to detect any Web Service request (2) invokes the appropriate Java method corresponding to the request and passes along the parameters in the request and (3) sends back the return value from the Java method to the remote client. Therefore, when you use Apache Tomcat together with Axis2, you do not need to do much to publish your Java class as a Web service! Even better, for all Web serivces published through Axis2, Axis2 automatically generates their WSDL (Web-Service Description Language) definitions, which describes the name, URL, and the parameters of your published Java methods. Follow the Axis2 installation guide to install the Axis2 package to your Tomcat server. Once successfully installed, all HTTP requests with the URL prefix http://localhost:1448/axis2/services will be interpreted as a Web service request and will be handled by Axis2.

In order to publish your Java classes as a Web service, you need to "package" them as a service archive file. Before you proceed to the next step, read

Tutorial on Publishig a Java Class as a Web Service Using Axis2
to learn how you can create a service archive file from your class file(s).
C.2: Package Your Java Class for Axis2 Deployment

Now that you understand how to publish a Java class as a Web service, you need to package your classes as a service archive file (AAR file). Your Web service needs to be published at http://localhost:1448/axis2/services/AuctionSearchService, so you will have to prepare your service archive file in compliance with this address. The WSDL of your service must also be available at http://localhost:1448/axis2/services/AuctionSearchService?wsdl, but since you are using Axis2, it generates this WSDL automatically for you.

The Ant build.xml file included in project3-searcher.zip has the target "build" that does the basic packaging of your Java classes and create the service archive file at build/AuctionSearchService.aar. Go over the "build" target in the build.xml file to understand how it does it. Note that all the files in WebContents directory are automatically added to the AAR file, including WebContents/META-INF/services.xml. Read the services.xml file to see the mapping between Web service requests and the AuctionSearch class. If necessary, modify build.xml to make it suitable for your code. Even after your modification, the AAR file should still be generated at build/AuctionSearchService.aar.

Finally, remember that if you use external libraries that are not part of the standard Java libraries, you have to make sure that the external libraries are available when your class(es) are deployed to Tomcat. Even if your classes compile and run fine under your development environment, it may not run properly within a Tomcat server if your external library files are not available in Tomcat. To avoid this problem make sure you include all external library files that you use in the lib subdirectory of your AuctionSearchService.aar. If you use any third-party library that was not preinstalled on our VM, make sure to place library files in the lib subdirectory of your current working directory. The provided build.xml file copies every jar file in lib directory to the aar file.

C.3: Deploy Your Web Service

Now we are ready to deploy your Web service. Simply drop your service archive file prepared in the previous step to $CATALINA_BASE/webapps/axis2/WEB-INF/services. If this directory does not exist yet, create it first and copy your archive file. Tomcat supports hotdeployment (adding new services without restarting) and hotupdate (updating existing services without restarting). Read about their pros/cons and then enable them as you see fit. They can help speed up development, but don't always work as you'd expect. When in doubt, restart Tomcat by running the command "sudo /etc/init.d/tomcat7 restart".

When your Java class is deployed as a Web service, its "working directory" is likely to be different from when you run your class a "standalone application" --- the working directory of any Tomcat process is often (but not always) $CATALINA_HOME/bin. If you get unexpected errors like "File Not Found" or "Segment Not Found" after deployment, the difference in the working directory may be the cause of the error. Check your code to see whether you made any implicit assumption on the current working directory.

Test your Web service deployment and make sure it works correctly. The basicSearch, getXMLDataForItemId, and echo methods require only simple input, and are thus easily accessible through a REST-style interface for some basic testing. For example accesing the URL http://localhost:1448/axis2/services/AuctionSearchService/echo?message=helloThere should return an XML response from your Java method echo. More thorough testing of your Web service will be done in the next step.

Notes on the namespace of the published service: When you publish an existing Java class as a Web service using Axis2 v1.6 (the version on the VM), the package name of your class becomes the namespace of the service by default. That is, the package name "edu.ucla.cs.cs144" of AuctionSearch implies that the namespace of the AuctionSearchService will be "http://cs144.cs.ucla.edu". The mapping of the package name to the service namespace varies slightly among different versions of Axis2, so make sure that you use the version on our VM. Do not change the default namespace by overriding it in your services.xml file. Otherwise, you will get errors in the next step.

Part D: Test Your Web Service

In general, to test the functions of a SOAP Web service, you will have to implement a SOAP client that can access your service. Axis2 not only helps you publish your Java classes as a Web service, but also provides the tools to generate the basic Web-service client code ("Web service client stub") automatically from a WSDL description of a service. (e.g., you can use wsdl2java.sh in $AXIS2_HOME/bin for this purpose.)

While you can use this tool to generate the client code of your service, the output is a bit too complex and ugly to read (like most autogenerated codes are), so we are providing a "cleaned-up" version of the client code as AuctionSearchClient.java in project3-client.zip. By calling the methods of the AuctionSearchClient class, your testing code can access the "remote" Web-service functions like any other local public static method. We have also included a simple test code in AuctionSearchTest.java. You can invoke the test class with the ant target "run" of the included build.xml. Add more test cases to AuctionSearchTest to test all four Web service functions thoroughly.

Notes on CLASSPATH: The provided AuctionSearchClient.java depends on Axis2 Java library files that are not part of the standard Java Runtime environment. Again, since the standard Java compiler and runtime engine are NOT aware of Axis2 library files, they may generate "class not found" error when you try to compile and run the AuctionSearchClient. The included build.xml file in project3-client.zip tries to avoid this problem by passsing the location of Axis2 library files as the classpath parameter of java and javac.