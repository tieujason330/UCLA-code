package simpledb;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Serializable;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/**
 * The Catalog keeps track of all available tables in the database and their
 * associated schemas.
 * For now, this is a stub catalog that must be populated with tables by a
 * user program before it can be used -- eventually, this should be converted
 * to a catalog that reads a catalog table from disk.
 * 
 * @Threadsafe
 */
public class Catalog {

	/**
     * A help class to facilitate organizing the information of each table
     * */
    public static class TableInfo implements Serializable {

        private static final long serialVersionUID = 1L;
        
        public final DbFile m_dbfile;
        public final String m_pkey;
        public final String m_name;
        
        /**
         * TableInfo stores the data for each table
         * @param dbf dbf the DbFile to be inserted
         * @param name the name to be inserted
         * @param pkey the primary key to be inserted
         */
        public TableInfo(DbFile dbf, String name, String pkey) {
        	this.m_dbfile = dbf;
        	this.m_pkey = pkey;
        	this.m_name = name;
        }

        public String toString() {
        	return "DbFile = " + this.m_dbfile.toString() + ", Name = " 
        			+ this.m_name + ", Primary Key = " + this.m_pkey;
        }
    }
    
    //using hashmap - id as key and TableInfo as value
	HashMap<Integer,TableInfo> m_id2info;
	//using hashmap - string as key and id as value
	HashMap<String, Integer> m_name2id;
	//making a vector of ids for iterator
	Vector<Integer> m_ids;
	//list of TableInfo
	List<TableInfo> m_tableList;

    /**
     * Constructor.
     * Creates a new, empty catalog.
     */
    public Catalog() {
    	//initialize the hashmaps
    	this.m_id2info = new HashMap<Integer, TableInfo>();
    	this.m_name2id = new HashMap<String, Integer>();
    	this.m_ids = new Vector<Integer>();
    	this.m_tableList = new ArrayList<TableInfo>();
    }

    /**
     * Add a new table to the catalog.
     * This table's contents are stored in the specified DbFile.
     * @param file the contents of the table to add;  file.getId() is the identfier of
     *    this file/tupledesc param for the calls getTupleDesc and getFile
     * @param name the name of the table -- may be an empty string.  May not be null.
     * @param pkeyField the name of the primary key field
     * conflict exists, use the last table to be added as the table for a given name.
     */
    public void addTable(DbFile file, String name, String pkeyField) {
    	if(name == null)
    	{
    		throw new IllegalArgumentException("name is null.");
    	}
    	//make a TableInfo with the arguments
        TableInfo ti = new TableInfo(file, name, pkeyField);
        //add id and TableInfo into hashmap
        this.m_id2info.put(file.getId(), ti);
        //add name and id into another hashmap
        this.m_name2id.put(name, file.getId());
        //add id to vector
        this.m_ids.add(file.getId());
        //add ti to list
        this.m_tableList.add(ti);
    }

    public void addTable(DbFile file, String name) {
        addTable(file, name, "");
    }

    /**
     * Add a new table to the catalog.
     * This table has tuples formatted using the specified TupleDesc and its
     * contents are stored in the specified DbFile.
     * @param file the contents of the table to add;  file.getId() is the identfier of
     *    this file/tupledesc param for the calls getTupleDesc and getFile
     */
    public void addTable(DbFile file) {
        addTable(file, (UUID.randomUUID()).toString());
    }

    /**
     * Return the id of the table with a specified name,
     * @throws NoSuchElementException if the table doesn't exist
     */
    public int getTableId(String name) throws NoSuchElementException {
       
    	//check to see if table exists
    	if(this.m_name2id.containsKey(name))
        		return this.m_name2id.get(name).intValue();
    	//if table doesn't exist then throws exception
        throw new NoSuchElementException("Table does not exist.");
    }

    /**
     * Returns the tuple descriptor (schema) of the specified table
     * @param tableid The id of the table, as specified by the DbFile.getId()
     *     function passed to addTable
     * @throws NoSuchElementException if the table doesn't exist
     */
    public TupleDesc getTupleDesc(int tableid) throws NoSuchElementException {
        //if table exists, gets the TupleDesc of the table
    	if(this.m_id2info.containsKey(tableid))
    		return this.m_id2info.get(tableid).m_dbfile.getTupleDesc();
    	//if table doesn't exist
    	throw new NoSuchElementException("Table does not exist.");
    }

    /**
     * Returns the DbFile that can be used to read the contents of the
     * specified table.
     * @param tableid The id of the table, as specified by the DbFile.getId()
     *     function passed to addTable
     */
    public DbFile getDatabaseFile(int tableid) throws NoSuchElementException {
        //if table exists, gets the DbFile of the table
    	if(this.m_id2info.containsKey(tableid))
    		return this.m_id2info.get(tableid).m_dbfile;
    	//if table doesn't exist
    	throw new NoSuchElementException("Table does not exist.");
    }

    public String getPrimaryKey(int tableid) {
    	//if table exists, get the primary key related to the id
    	if(this.m_id2info.containsKey(tableid))
    		return this.m_id2info.get(tableid).m_pkey;
    	//if table doesn't exist
    	throw new NoSuchElementException("Table does not exist.");
    }

    public Iterator<Integer> tableIdIterator() {
        Iterator<Integer> it = new Iterator<Integer>(){
        	
        		private int curIdx = 0;

				@Override
				public boolean hasNext() {
					//check to see if current index is less than amount of ids
					//and to make sure id is not null at current index
					return curIdx < m_ids.size() && m_ids.elementAt(curIdx) != null;
				}

				@Override
				public Integer next() {
					//returns the next element after current index
					return m_ids.elementAt(curIdx++);
				}

				@Override
				public void remove() {
					throw new UnsupportedOperationException();
				}
		
			};
			return it;
    }

    public String getTableName(int id) {
        //if table exists, get the table name related to the id
    	if(this.m_id2info.containsKey(id))
    		return this.m_id2info.get(id).m_name;
    	//if table doesn't exist
    	throw new NoSuchElementException("Table does not exist.");
    }
    
    /** Delete all tables from the catalog */
    public void clear() {
    	//clear the hashmaps
    	this.m_id2info.clear();
        this.m_name2id.clear();
        this.m_ids.clear();
        this.m_tableList.clear();
    }
    
    /** Get list of TableInfo */
    public List<TableInfo> getTableInfo()
    {
    	return this.m_tableList;
    }
    /**
     * Reads the schema from a file and creates the appropriate tables in the database.
     * @param catalogFile
     */
    public void loadSchema(String catalogFile) {
        String line = "";
        String baseFolder=new File(new File(catalogFile).getAbsolutePath()).getParent();
        try {
            BufferedReader br = new BufferedReader(new FileReader(new File(catalogFile)));
            
            while ((line = br.readLine()) != null) {
                //assume line is of the format name (field type, field type, ...)
                String name = line.substring(0, line.indexOf("(")).trim();
                //System.out.println("TABLE NAME: " + name);
                String fields = line.substring(line.indexOf("(") + 1, line.indexOf(")")).trim();
                String[] els = fields.split(",");
                ArrayList<String> names = new ArrayList<String>();
                ArrayList<Type> types = new ArrayList<Type>();
                String primaryKey = "";
                for (String e : els) {
                    String[] els2 = e.trim().split(" ");
                    names.add(els2[0].trim());
                    if (els2[1].trim().toLowerCase().equals("int"))
                        types.add(Type.INT_TYPE);
                    else if (els2[1].trim().toLowerCase().equals("string"))
                        types.add(Type.STRING_TYPE);
                    else {
                        System.out.println("Unknown type " + els2[1]);
                        System.exit(0);
                    }
                    if (els2.length == 3) {
                        if (els2[2].trim().equals("pk"))
                            primaryKey = els2[0].trim();
                        else {
                            System.out.println("Unknown annotation " + els2[2]);
                            System.exit(0);
                        }
                    }
                }
                Type[] typeAr = types.toArray(new Type[0]);
                String[] namesAr = names.toArray(new String[0]);
                TupleDesc t = new TupleDesc(typeAr, namesAr);
                HeapFile tabHf = new HeapFile(new File(baseFolder+"/"+name + ".dat"), t);
                addTable(tabHf,name,primaryKey);
                System.out.println("Added table : " + name + " with schema " + t);
            }
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(0);
        } catch (IndexOutOfBoundsException e) {
            System.out.println ("Invalid catalog entry : " + line);
            System.exit(0);
        }
    }
}

