Jason Tieu
304047667

Part A: 
For Lucene Indexes, I made indexes on the Name, Description, 
and Category attributes of the Item and ItemCategory tables.

***SPECIAL NOTE: Building Lucene Indexes was taking around 15 minutes initially.
 I was able to reduce the build time to around 15 seconds by stating the primary
keys for each table in create.sql in Project2 folder.

------------------------------------------------------------------------------------------------

Part A: Keyword Search

In Part A, you need to implement the following basic keyword search functionality over eBay data.

Basic keyword search: return the itemId and name of all items that contain a set of keywords. The search should be performed over the union of the item name, category, and description attributes.
For example, for the query "Disney", your basic search function should return the itemId and name of items that have the keyword "Disney" in the union of the name, category or description attributes. Also, for multiple word queries, such as "star trek", you should consider that as "star" OR "trek". That is, you should return any item if it has either "star" or "trek" in the union of the keywords in the name, category and description attributes.

To support the basic search function, you will have to build an inverted index capable of searching over the union of the three attributes in your database schema, using an open-source library called Lucene.

A.1: Learn About JDBC and Lucene.

In order to access MySQL data from a Java program, you will need to use JDBC (Java DataBase Connectivity) API. You will also use Lucene to build inverted indexes for text fields and search over them. To learn how to use JDBC and Lucene, first go over the following two tutorials.

A short tutorial on JDBC
A short tutorial on Lucene
Also, if you haven't gone over our short tutorial on Ant in Project 2, please do it now because we heavily rely on Ant for this part of the project. Proceed to the next step only after you have gone over the above tutorials.
A.2: Write Java Application to Create Lucene Index

In order to support the basic keyword search function described before, you will first need to create a Lucene index on a number of attributes of your database tables. Decide which index(es) to create on which attribute(s). Document your design choices and briefly discuss why you've chosen to create your particular index(es) in a plaintext file called README.txt.

Once you have chosen what Lucene index(es) to create, write a Java application to create them, iterating through your database tables to add the appropriate index entries to the index(es). As a starting point, we have provided skeleton code DbManager.java and Indexer.java in the src directory of project3-indexer.zip.

Note that the two Java classes are defined as part of the edu.ucla.cs.cs144 package, so the java files are located at src/edu/ucla/cs/cs144 to be precise. If you are not clear about how a package name is related to the source directory, read Section 1.8 of A Crash Course from C++ to Java again. As part of your implementation, you will probably want to create new Java classes for modular development. If you do, make sure that your new classes become part of edu.ucla.cs.cs144 package and place your source files in the src/edu/ucla/cs/cs144. directory.

The provided DbManager class encapsulates the database connection information for JDBC, so that the information is separate from your main code. For any reason, if you decide not to use this class, make sure to use the database CS144 and connect it as the user "cs144" with empty password. The Indexer class contains the bare minimum code for your Lucene index creation module. Inside this class, you will have to use JDBC to retrieve Item information from MySQL and use Lucene IndexWriter class to create Lucene indexes. Again, if you are not familiar with how to use JDBC and Lucene API, read JDBC tutorial, and Lucene tutorial.

When you create your Lucene index(es), make sure to create them under /var/lib/lucene/ directory. For example, if you want to create your (first) Lucene index in the index1 subdirectory, it should be created at /var/lib/lucene/index1/.

The project3-indexer.zip file contains ant build.xml file, which has the target "compile" to compile your Java source code and the target "run" to invoke edu.ucla.cs.cs144.Indexer for indexing. If needed, update your build.xml, so that the Ant target "run" always executes your Lucene indexing module and builds the index(es).

Notes on MySQL and Lucene library files: As we explained in the JDBC and Lucene tutorials, if you want to use MySQL JDBC and Lucene libraries in your code, you need to pass the location of the two jar files (mysql-connector-java.jar and lucene-*.jar in /usr/share/java) through the classpath option of javac and java. Otherwise you are likely to get "class not found" error. Since the provided build.xml file sets the classpath option automatically, you are unlikely to encounter this problem for this project as long as you use our build.xml file.

A.3: Implement Basic Keyword Search Function

Now that you have created a (set of) Lucene index(es), you will implement the basic search functionality. In particular, you will need to implement the basicSearch function of the following Java interface (spatialSearch and getXMLDataForItemId functions will be implemented later in Part B):

public interface IAuctionSearch {
    public SearchResult[] basicSearch(String query, int numResultsToSkip, int numResultsToReturn);
    public SearchResult[] spatialSearch(String query, SearchRegion region, int numResultsToSkip, int numResultsToReturn);
    public String getXMLDataForItemId(String itemId);
    public String echo(String message);
}

You'll find this interface definition in the IAuctionSearch.java file located in the src/edu/ucla/cs/cs144 directory of project3-searcher.zip. Roughly, given a query, basicSearch() should return numResultsToReturn items after skipping the top numResultsToSkip matching items. For example, if numResultsToSkip = 10 and numResultsToReturn = 20, the functions should return top-11 through top-30 matching auction items as the result. Read IAuctionSearch.java to understand what each method should do, which is desribed as a comment within the file. The SearchResult.java file has the definition for the SearchResult class, which is used by the basicSearch method to return search results. It has two attribues, itemId and name, that are the id and the name of a returned eBay item.

The sample code AuctionSearch.java in the same directory of the zip file has the echo() method implemented for you. You will have to add your code to the parts marked as "TODO: Your code here!". To identify the items the contain the query keywords, your implementation of basicSearch() method must use Lucene index(es) that you built in Part A.2, which is located below /var/lib/lucene/. The search should be performed over the union of the item name, category, and description attributes and your method should return (id, name) pairs of matching items.

Note: You may use any ranking method for ordering the results from basicSearch. Lucene's default ranking may be a good choice, for example.

Java String comparison: To compare Strings in Java, you must use the equals method, because the operator == in Java compares object references not the object values.

To help you debug your basicSearch() methods, we provide some sample queries and the number of matching items.

"superman": 68 matches
"kitchenware": 1462
"star trek": 770
Note: In order to get the above results, you have to assign datatype varchar(4000) to the Description field, as we suggested in Project 2.