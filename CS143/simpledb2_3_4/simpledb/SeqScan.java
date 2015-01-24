package simpledb;

import java.util.*;

import simpledb.TupleDesc.TDItem;

/**
 * SeqScan is an implementation of a sequential scan access method that reads
 * each tuple of a table in no particular order (e.g., as they are laid out on
 * disk).
 */
public class SeqScan implements DbIterator {

    private static final long serialVersionUID = 1L;

    //private member data
    private TransactionId m_tid;
    private int m_tableID;
    private String m_tableAlias;
    private DbFileIterator  m_DbFileIter = null;
    private boolean m_isOpen;
    /**
     * Creates a sequential scan over the specified table as a part of the
     * specified transaction.
     * 
     * @param tid
     *            The transaction this scan is running as a part of.
     * @param tableid
     *            the table to scan.
     * @param tableAlias
     *            the alias of this table (needed by the parser); the returned
     *            tupleDesc should have fields with name tableAlias.fieldName
     *            (note: this class is not responsible for handling a case where
     *            tableAlias or fieldName are null. It shouldn't crash if they
     *            are, but the resulting name can be null.fieldName,
     *            tableAlias.null, or null.null).
     */
    public SeqScan(TransactionId tid, int tableid, String tableAlias) {
        this.m_tid = tid;
        this.m_tableID = tableid;
        this.m_tableAlias = tableAlias;
        this.m_isOpen = false;
    }

    /**
     * @return
     *       return the table name of the table the operator scans. This should
     *       be the actual name of the table in the catalog of the database
     * */
    public String getTableName() {
        return Database.getCatalog().getTableName(this.m_tableID);
    }
    
    /**
     * @return Return the alias of the table this operator scans. 
     * */
    public String getAlias()
    {
        return this.m_tableAlias;
    }

    /**
     * Reset the tableid, and tableAlias of this operator.
     * @param tableid
     *            the table to scan.
     * @param tableAlias
     *            the alias of this table (needed by the parser); the returned
     *            tupleDesc should have fields with name tableAlias.fieldName
     *            (note: this class is not responsible for handling a case where
     *            tableAlias or fieldName are null. It shouldn't crash if they
     *            are, but the resulting name can be null.fieldName,
     *            tableAlias.null, or null.null).
     */
    public void reset(int tableid, String tableAlias) {
        this.m_tableID = tableid;
        this.m_tableAlias = tableAlias;
    }

    public SeqScan(TransactionId tid, int tableid) {
        this(tid, tableid, Database.getCatalog().getTableName(tableid));
    }

    public void open() throws DbException, TransactionAbortedException {
    	if(this.m_isOpen == false) //don't open if false
    	{
    		this.m_DbFileIter = Database.getCatalog().getDatabaseFile(this.m_tableID).iterator(this.m_tid);
        	this.m_DbFileIter.open();
        	this.m_isOpen = true;
    	}
    }

    /**
     * Returns the TupleDesc with field names from the underlying HeapFile,
     * prefixed with the tableAlias string from the constructor. This prefix
     * becomes useful when joining tables containing a field(s) with the same
     * name.
     * 
     * @return the TupleDesc with field names from the underlying HeapFile,
     *         prefixed with the tableAlias string from the constructor.
     */
    public TupleDesc getTupleDesc() {
    	//create a Catalog object and get the TupleDesc
    	TupleDesc td = Database.getCatalog().getTupleDesc(this.m_tableID);
        
        String alias = this.m_tableAlias;
        //case where alias is null
        if(alias == null)
        {
        	alias = this.getTableName();
        }
        //we want the field names to be tableAlias.fieldname
        Type[] typeAr = new Type[td.numFields()];
        String[] fnameAr = new String[td.numFields()];
        for(int i = 0; i < td.numFields(); i++)
        {
        	typeAr[i] = td.getFieldType(i);
        	//field name is now tableAlias.fieldname
        	String FieldName = td.getFieldName(i);
        	//case if field name is null then it'd be tableAlias.null
        	if(FieldName == null)
        	{
        		FieldName = "null";
        	}
        	String fieldname = alias + "." + FieldName;
        	fnameAr[i] = fieldname;
        }
        
        return new TupleDesc(typeAr, fnameAr);
    }

    public boolean hasNext() throws TransactionAbortedException, DbException {
        if(this.m_DbFileIter == null)
        {
        	return false;
        }
    	return this.m_DbFileIter.hasNext();
    }

    public Tuple next() throws NoSuchElementException,
            TransactionAbortedException, DbException {
        if(this.m_DbFileIter == null)
        {
        	throw new NoSuchElementException("DbFileIterator does not exist.");
        }
        //get the tuple using next() method
        Tuple tuple = this.m_DbFileIter.next();
        //check to see if tuple is not null
        if(tuple != null)
        {
        	return tuple;
        }
        else
        {
        	throw new NoSuchElementException("Tuple not found.");
        }
    }

    public void close() {
        this.m_DbFileIter = null;
        this.m_isOpen = false;
    }

    public void rewind() throws DbException, NoSuchElementException,
            TransactionAbortedException {
    	//sets m_DbFileIter to close
        this.m_DbFileIter.close();
        //reopens m_DbFileIter
        this.m_DbFileIter.open();
    }
}
