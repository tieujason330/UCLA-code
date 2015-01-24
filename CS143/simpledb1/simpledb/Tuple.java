package simpledb;

import java.io.Serializable;
import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Vector;

/**
 * Tuple maintains information about the contents of a tuple. Tuples have a
 * specified schema specified by a TupleDesc object and contain Field objects
 * with the data for each field.
 */
public class Tuple implements Serializable {

    private static final long serialVersionUID = 1L;
    
    //private data members
    private Vector<Field> m_tuple;
    private TupleDesc m_td;
    private RecordId m_recid;
    /**
     * Create a new tuple with the specified schema (type).
     * 
     * @param td
     *            the schema of this tuple. It must be a valid TupleDesc
     *            instance with at least one field.
     */
    public Tuple(TupleDesc td) {
    	
        this.m_td = td;
        //initialize m_tuple to vector of fields with size of tupledesc
        this.m_tuple = new Vector<Field>(td.numFields());
    }

    /**
     * @return The TupleDesc representing the schema of this tuple.
     */
    public TupleDesc getTupleDesc() {
        return this.m_td;
    }

    /**
     * @return The RecordId representing the location of this tuple on disk. May
     *         be null.
     */
    public RecordId getRecordId() {
        return this.m_recid;
    }

    /**
     * Set the RecordId information for this tuple.
     * 
     * @param rid
     *            the new RecordId for this tuple.
     */
    public void setRecordId(RecordId rid) {
        this.m_recid = rid;
    }

    /**
     * Change the value of the ith field of this tuple.
     * 
     * @param i
     *            index of the field to change. It must be a valid index.
     * @param f
     *            new value for the field.
     */
    public void setField(int i, Field f) {
    	//uses vector add method at index i and f is the field to be set
    	//if(i < this.m_tuple.size())
    		this.m_tuple.add(i, f);
    }

    /**
     * @return the value of the ith field, or null if it has not been set.
     * 
     * @param i
     *            field index to return. Must be a valid index.
     */
    public Field getField(int i) {
    	if(i >= this.m_tuple.size())
    		return null;
    	return this.m_tuple.elementAt(i);
    	//else
    		//return null;
    	//throw new NoSuchElementException("Invalid index");
    }

    /**
     * Returns the contents of this Tuple as a string. Note that to pass the
     * system tests, the format needs to be as follows:
     * 
     * column1\tcolumn2\tcolumn3\t...\tcolumnN\n
     * 
     * where \t is any whitespace, except newline, and \n is a newline
     */
    public String toString() {
        //create a stringbuffer
    	StringBuffer strbuf = new StringBuffer();
    	for(int i = 0; i < this.m_tuple.size(); i++)
    	{
    		//if first field
    		if(i == 0)
    			strbuf.append(this.getField(i));
    		//if last field add a \n at end
    		if(i == this.m_tuple.size() - 1)
    			strbuf.append(" " + this.getField(i) + "\n");
    		else
    			strbuf.append(" " + this.getField(i));
    	}
    	String s = strbuf.toString();
    	return s;
        //throw new UnsupportedOperationException("Implement this");
    }
    
    /**
     * @return
     *        An iterator which iterates over all the fields of this tuple
     * */
    public Iterator<Field> fields()
    {
        Iterator<Field> it = new Iterator<Field>(){

        	int curIndex = 0;
        	
			@Override
			public boolean hasNext() {
				//check to see curIndex less than size of tuple
				//and element at curIndex is not null
				return curIndex < m_tuple.size() 
						&& m_tuple.elementAt(curIndex) != null;
			}

			@Override
			public Field next() {
				return m_tuple.elementAt(curIndex++);
			}

			@Override
			public void remove() {
				//prevents remove method
				throw new UnsupportedOperationException();
			}
        };
        return it;
    }
    
    /**
     * reset the TupleDesc of this tuple
     * */
    public void resetTupleDesc(TupleDesc td)
    {
        this.m_td = td;
    }
}
