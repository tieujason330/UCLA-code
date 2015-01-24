package simpledb;

import java.io.Serializable;
import java.util.*;

import javax.naming.NameNotFoundException;

/**
 * TupleDesc describes the schema of a tuple.
 */
public class TupleDesc implements Serializable {

    /**
     * A help class to facilitate organizing the information of each field
     * */
    public static class TDItem implements Serializable {

        private static final long serialVersionUID = 1L;

        /**
         * The type of the field
         * */
        public final Type fieldType;
        
        /**
         * The name of the field
         * */
        public final String fieldName;

        public TDItem(Type t, String n) {
            this.fieldName = n;
            this.fieldType = t;
        }

        public String toString() {
            return fieldName + "(" + fieldType + ")";
        }
    }

    /**
     * @return
     *        An iterator which iterates over all the field TDItems
     *        that are included in this TupleDesc
     * */
    public Iterator<TDItem> iterator() {
    	Iterator<TDItem> it = new Iterator<TDItem>(){
    		
    		private int curIndex = 0;
				
			@Override
			public boolean hasNext() {
				//check to see if current index is less than amount of fields
				//and to make sure TDItem is not null at current index
				return (curIndex < numFields()) 
						&& m_TupleDesc.elementAt(curIndex) != null;
			}

			@Override
			public TDItem next() {
				//returns the next element after current index
				return m_TupleDesc.elementAt(curIndex++);
			}

			@Override
			public void remove() {
				//prevents remove operation
				throw new UnsupportedOperationException();
			}
		};
		return it;
    }

    private static final long serialVersionUID = 1L;
    
    private Vector<TDItem> m_TupleDesc;

    /**
     * Create a new TupleDesc with typeAr.length fields with fields of the
     * specified types, with associated named fields.
     * 
     * @param typeAr
     *            array specifying the number of and types of fields in this
     *            TupleDesc. It must contain at least one entry.
     * @param fieldAr
     *            array specifying the names of the fields. Note that names may
     *            be null.
     */
    public TupleDesc(Type[] typeAr, String[] fieldAr) {
        
    	//initialize m_TupleDesc to vector with size of typeAr
    	m_TupleDesc = new Vector<TDItem>(typeAr.length);
    	
    	for(int i = 0; i < typeAr.length; i++)
    	{
    		//create a new TDItem based on ith index of typeAr and fieldAr
    		TDItem new_field = new TDItem(typeAr[i], fieldAr[i]);
    		//add to m_TupleDesc
    		m_TupleDesc.add(new_field);
    	}
    }

    /**
     * Constructor. Create a new tuple desc with typeAr.length fields with
     * fields of the specified types, with anonymous (unnamed) fields.
     * 
     * @param typeAr
     *            array specifying the number of and types of fields in this
     *            TupleDesc. It must contain at least one entry.
     */
    public TupleDesc(Type[] typeAr) {
        
    	//initialize m_TupleDesc to vector with typeAr size
    	m_TupleDesc = new Vector<TDItem>(typeAr.length);
    	
    	for(int i = 0; i < typeAr.length; i++)
    	{
    		//make a new TDItem with an unnamed field and add it to m_TupleDesc
    		TDItem new_field = new TDItem(typeAr[i], "");
    		m_TupleDesc.add(new_field);
    	}
    }

    /**
     * @return the number of fields in this TupleDesc
     */
    public int numFields() {
    	//get the size of m_TupleDesc vector
        return m_TupleDesc.size();
    }

    /**
     * Gets the (possibly null) field name of the ith field of this TupleDesc.
     * 
     * @param i
     *            index of the field name to return. It must be a valid index.
     * @return the name of the ith field
     * @throws NoSuchElementException
     *             if i is not a valid field reference.
     */
    public String getFieldName(int i) throws NoSuchElementException {
    	
    	try
    	{
    		if(i < this.numFields())
    			//return field name at index i
    			return m_TupleDesc.elementAt(i).fieldName;
    	}
    	catch(NoSuchElementException e)
    	{
    		System.out.println("i is not a valid field reference.");
    	}
    	
    	return null;
    }

    /**
     * Gets the type of the ith field of this TupleDesc.
     * 
     * @param i
     *            The index of the field to get the type of. It must be a valid
     *            index.
     * @return the type of the ith field
     * @throws NoSuchElementException
     *             if i is not a valid field reference.
     */
    public Type getFieldType(int i) throws NoSuchElementException {
    	try
        {
        	if(i < this.numFields())
        		//gets the field type at ith element
        		return m_TupleDesc.elementAt(i).fieldType;
        }
        catch(NoSuchElementException e)
        {
        	System.out.println("** i is not a valid field reference.");
        }
        return null;
    }

    /**
     * Find the index of the field with a given name.
     * 
     * @param name
     *            name of the field.
     * @return the index of the field that is first to have the given name.
     * @throws NoSuchElementException
     *             if no field with a matching name is found.
     */
    public int fieldNameToIndex(String name) throws NoSuchElementException {
        
    	//check to see if name is null
    	if (name == null)
    		throw new NoSuchElementException("name is null.");

	    for(int i = 0; i < m_TupleDesc.size(); i++)
	    {
	    	//if name is in the TupleDesc then return i
	    	if(name.equals(this.getFieldName(i)))
	    		return i;
	    }
	    //throw NoSuchElementException if no field is found
	    throw new NoSuchElementException("No field with a matching name is found.");
    }

    /**
     * @return The size (in bytes) of tuples corresponding to this TupleDesc.
     *         Note that tuples from a given TupleDesc are of a fixed size.
     */
    public int getSize() {
        int size = 0;
        for(int i = 0; i < this.numFields(); i++)
        {
        	//gets the length of each field and adds it to size
        	size += this.getFieldType(i).getLen();
        }
        //System.out.println("size = " + size);
        return size;
    }

    /**
     * Merge two TupleDescs into one, with td1.numFields + td2.numFields fields,
     * with the first td1.numFields coming from td1 and the remaining from td2.
     * 
     * @param td1
     *            The TupleDesc with the first fields of the new TupleDesc
     * @param td2
     *            The TupleDesc with the last fields of the TupleDesc
     * @return the new TupleDesc
     */
    public static TupleDesc merge(TupleDesc td1, TupleDesc td2) {
    	//create two arrays with size of td1+td2
    	Type [] typeAr = new Type [td1.numFields() + td2.numFields()];
    	String [] fieldAr = new String [td1.numFields() + td2.numFields()];
    	
    	//add the type and name to the respective arrays for td1
    	for(int i = 0; i < td1.numFields(); i++)
    	{
    		typeAr[i] = td1.getFieldType(i);
    		fieldAr[i] = td1.getFieldName(i);
    	}
    	//add the type and name to respective arrays for td2
    	//j starts at next available slot of vector and ends at (td1 + td2) - 1
    	int k = 0;
    	for(int j = td1.numFields(); j < (td1.numFields() + td2.numFields()); j++)
    	{
    		typeAr[j] = td2.getFieldType(k); //starts at beginning of td2
    		fieldAr[j] = td2.getFieldName(k);
    		k++;//increments k after adding the td2 kth elements
    	}
    	
    	//use TupleDesc constructor to create a new TupleDesc
    	return new TupleDesc(typeAr, fieldAr);
    }

    /**
     * Compares the specified object with this TupleDesc for equality. Two
     * TupleDescs are considered equal if they are the same size and if the n-th
     * type in this TupleDesc is equal to the n-th type in td.
     * 
     * @param o
     *            the Object to be compared for equality with this TupleDesc.
     * @return true if the object is equal to this TupleDesc.
     */
    public boolean equals(Object o) {
        
        //check that o is an instance of TupleDesc
        if(o instanceof TupleDesc)
        {
        	//if do not have same size
        	if(this.numFields() != ((TupleDesc)o).numFields())
            	return false;
            
            for(int i = 0; i < this.numFields(); i++)
            {
            	//if i-th type is not equal to o's i-th type
            	if(this.getFieldType(i) != ((TupleDesc)o).getFieldType(i))
            		return false;
            }
            return true;
        }
        else
        	return false;
    }

    public int hashCode() {
        // If you want to use TupleDesc as keys for HashMap, implement this so
        // that equal objects have equals hashCode() results
        throw new UnsupportedOperationException("unimplemented");
    }

    /**
     * Returns a String describing this descriptor. It should be of the form
     * "fieldType[0](fieldName[0]), ..., fieldType[M](fieldName[M])", although
     * the exact format does not matter.
     * 
     * @return String describing this descriptor.
     */
    public String toString() {
    	//create a string buffer
    	StringBuffer strbuf = new StringBuffer();
       for(int i = 0; i < this.numFields(); i++)
       {
    	   //first element doesn't need a comma at beginning
    	   if(i == 0)
    	   {
    		   strbuf.append(this.getFieldType(i));
    		   strbuf.append("(" + this.getFieldName(i) + ")");
    	   }
    	   //if not first element then adds comma beforehand
    	   else
    	   {
    		   strbuf.append(", " + this.getFieldType(i));
    		   strbuf.append("(" + this.getFieldName(i) + ")");
    	   }
       }
       String s = strbuf.toString();
       return s;
    }
}
