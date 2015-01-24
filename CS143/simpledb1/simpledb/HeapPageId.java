package simpledb;

/** Unique identifier for HeapPage objects. */
public class HeapPageId implements PageId {

	//private member variables
	private int m_tableId;
	private int m_pageNo;
    /**
     * Constructor. Create a page id structure for a specific page of a
     * specific table.
     *
     * @param tableId The table that is being referenced
     * @param pgNo The page number in that table.
     */
    public HeapPageId(int tableId, int pgNo) {
    	//set private member variables to corresponding arguments
        this.m_tableId = tableId;
        this.m_pageNo = pgNo;
    }

    /** @return the table associated with this PageId */
    public int getTableId() {
    	//returns table id
    	return this.m_tableId;
    }

    /**
     * @return the page number in the table getTableId() associated with
     *   this PageId
     */
    public int pageNumber() {
    	//returns page number
        return this.m_pageNo;
    }

    /**
     * @return a hash code for this page, represented by the concatenation of
     *   the table number and the page number (needed if a PageId is used as a
     *   key in a hash table in the BufferPool, for example.)
     * @see BufferPool
     */
    public int hashCode() {
    	//make m_tableID as the most significant 16 bits
    	//make m_pageNo as the least significant 16 bits
    	return (this.m_tableId << 16) | this.m_pageNo;
       // throw new UnsupportedOperationException("implement this");
    }

    /**
     * Compares one PageId to another.
     *
     * @param o The object to compare against (must be a PageId)
     * @return true if the objects are equal (e.g., page numbers and table
     *   ids are the same)
     */
    public boolean equals(Object o) {
        if(o instanceof HeapPageId)
        {
        	//check if page numbers and table numbers are the same
        	return ((PageId)this).getTableId() == ((PageId)o).getTableId() 
        	&& ((PageId)this).pageNumber() == ((PageId)o).pageNumber();
        }
        else
        	return false;
    }

    /**
     *  Return a representation of this object as an array of
     *  integers, for writing to disk.  Size of returned array must contain
     *  number of integers that corresponds to number of args to one of the
     *  constructors.
     */
    public int[] serialize() {
        int data[] = new int[2];

        data[0] = getTableId();
        data[1] = pageNumber();

        return data;
    }

}
