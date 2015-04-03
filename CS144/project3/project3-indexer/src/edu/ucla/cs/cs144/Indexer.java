package edu.ucla.cs.cs144;

import java.io.IOException;
import java.io.StringReader;
import java.io.File;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

public class Indexer {
    
    /** Creates a new instance of Indexer */
    public Indexer() {
    }
 
    public void rebuildIndexes() throws Exception {

        Connection conn = null;
		
		Directory indexDir = null;
		IndexWriterConfig config = null;
		IndexWriter writer = null;

			// create a connection to the database to retrieve Items from MySQL
		try {
			conn = DbManager.getConnection(true);
		} catch (SQLException ex) {
			System.out.println(ex);
		}
		
		/*
		 * Add your code here to retrieve Items using the connection
		 * and add corresponding entries to your Lucene inverted indexes.
			 *
			 * You will have to use JDBC API to retrieve MySQL data from Java.
			 * Read our tutorial on JDBC if you do not know how to use JDBC.
			 *
			 * You will also have to use Lucene IndexWriter and Document
			 * classes to create an index and populate it with Items data.
			 * Read our tutorial on Lucene as well if you don't know how.
			 *
			 * As part of this development, you may want to add 
			 * new methods and create additional Java classes. 
			 * If you create new classes, make sure that
			 * the classes become part of "edu.ucla.cs.cs144" package
			 * and place your class source files at src/edu/ucla/cs/cs144/.
		 * 
		 */
		try {
			indexDir = FSDirectory.open(new File("/var/lib/lucene/index-directory/"));
			config = new IndexWriterConfig(Version.LUCENE_4_10_2, new StandardAnalyzer());
			config.setOpenMode(IndexWriterConfig.OpenMode.CREATE);
			writer = new IndexWriter(indexDir, config);
		} catch (IOException e) {
			System.out.println(e);
		}
		
		ResultSet rs = retrieveItemInfo(conn);
		
		if (rs != null) {
			while ( rs.next() ) {
				Document doc = new Document();
				//add ItemID, Name, and Description of each Item
				doc.add(new StringField("ItemID", Integer.toString(rs.getInt("ItemID")), Field.Store.YES));
				doc.add(new StringField("Name", rs.getString("Name"), Field.Store.YES));
				doc.add(new TextField("Description", rs.getString("Description"), Field.Store.YES));
				
				String ItemCategories = retrieveItemCategories(conn, rs.getInt("ItemID"));
				
				doc.add(new TextField("Category", ItemCategories, Field.Store.YES));
				
				String fullSearchableText = rs.getString("Name") + " " + rs.getString("Description") + " " + ItemCategories;
				doc.add(new TextField("ItemContent", fullSearchableText, Field.Store.NO));
				writer.addDocument(doc);
			}
		}

		//close IndexWriter
		try {
			writer.close();
		} catch (IOException e) {
			System.out.println(e);
		}
		
		// close the database connection
		try {
			conn.close();
		} catch (SQLException ex) {
			System.out.println(ex);
		}
    }    
	
	//gets ItemID, Name, and Description from Item table
	private static ResultSet retrieveItemInfo(Connection conn) {
		Statement stmt = null;
		ResultSet rs = null;
		
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery("SELECT ItemID, Name, Description FROM Item");
		} catch(SQLException e) {
			System.err.println("SQLException: " + e.getMessage());
		}
		return rs;
	}
	
	//gets a string that appends all the Item categories together
	private static String retrieveItemCategories(Connection conn, int ItemID) {
		Statement stmt = null;
		String categories = "";
		
		try {
			stmt = conn.createStatement();
			ResultSet rs = stmt.executeQuery("SELECT group_concat(Category separator ' ') as Item_Categories FROM ItemCategory WHERE ItemID = '" + ItemID + "'");
			if( rs.next() ) {
				categories = rs.getString("Item_Categories");
			}
		} catch (SQLException e) {
			System.err.println("SQLException: " + e.getMessage());
		}
		return categories;
	}
	
    public static void main(String args[]) throws Exception{
        Indexer idx = new Indexer();
        idx.rebuildIndexes();
    }   
}
