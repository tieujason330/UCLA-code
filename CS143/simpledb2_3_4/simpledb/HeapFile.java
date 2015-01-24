package simpledb;

import java.io.*;
import java.util.*;

/**
 * HeapFile is an implementation of a DbFile that stores a collection of tuples
 * in no particular order. Tuples are stored on pages, each of which is a fixed
 * size, and the file is simply a collection of those pages. HeapFile works
 * closely with HeapPage. The format of HeapPages is described in the HeapPage
 * constructor.
 * 
 * @see simpledb.HeapPage#HeapPage
 * @author Sam Madden
 */
public class HeapFile implements DbFile {

	//private data members
	File m_file;
	TupleDesc m_td;
	
    /**
     * Constructs a heap file backed by the specified file.
     * 
     * @param f
     *            the file that stores the on-disk backing store for this heap
     *            file.
     */
    public HeapFile(File f, TupleDesc td) {
    	this.m_file = f;
    	this.m_td = td;
    }

    /**
     * Returns the File backing this HeapFile on disk.
     * 
     * @return the File backing this HeapFile on disk.
     */
    public File getFile() {
        return this.m_file;
    }

    /**
     * Returns an ID uniquely identifying this HeapFile. Implementation note:
     * you will need to generate this tableid somewhere ensure that each
     * HeapFile has a "unique id," and that you always return the same value for
     * a particular HeapFile. We suggest hashing the absolute file name of the
     * file underlying the heapfile, i.e. f.getAbsoluteFile().hashCode().
     * 
     * @return an ID uniquely identifying this HeapFile.
     */
    public int getId() {
    	//hashing absolute file name 
        return this.m_file.getAbsoluteFile().hashCode();
    }

    /**
     * Returns the TupleDesc of the table stored in this DbFile.
     * 
     * @return TupleDesc of this DbFile.
     */
    public TupleDesc getTupleDesc() {
        return this.m_td;
    }

    // see DbFile.java for javadocs
    public Page readPage(PageId pid) {
    	if(pid.getTableId() != this.getId())
    		throw new NoSuchElementException("Page does not exist.");
    	
    	try
    	{
    		//create a RandomAccessFile object that is read only ("r")
    		//RandomAccessFile allows reading from a file
    		RandomAccessFile RAF = new RandomAccessFile(this.m_file, "r");
    		
    		//an array to store the page's data
    		byte[] pgData = new byte[BufferPool.PAGE_SIZE];
    		
    		//get offset into RAF
    		int offset = pid.pageNumber() * BufferPool.PAGE_SIZE;
    		//use seek to sets file-pointer current position to the offset
    		RAF.seek(offset);
    		
    		//read page's data into pgData
    		RAF.read(pgData, 0, BufferPool.PAGE_SIZE);
    		//close the RandomAccessFile after using it to stop accessing it
    		RAF.close();
    		return new HeapPage((HeapPageId)pid, pgData);
    	}
    	catch(FileNotFoundException e)
    	{
    		e.printStackTrace();
    	}
    	catch(IOException e2)
    	{
    		e2.printStackTrace();
    	}
    	//should not be reached
    	throw new IllegalArgumentException();
    }

    // see DbFile.java for javadocs
    public void writePage(Page page) throws IOException {
    	//create a RandomAccessFile
        RandomAccessFile RAF = new RandomAccessFile(this.m_file, "rw");
        //look for the page with the correct offset
        RAF.seek(page.getId().pageNumber() * Database.getBufferPool().PAGE_SIZE);
        //write to the page
        RAF.write(page.getPageData(), 0, Database.getBufferPool().PAGE_SIZE);
        RAF.close();
    }

    /**
     * Returns the number of pages in this HeapFile.
     */
    public int numPages() {
    	return (int)Math.ceil(this.m_file.length() / BufferPool.PAGE_SIZE);
    }

    // see DbFile.java for javadocs
    public ArrayList<Page> insertTuple(TransactionId tid, Tuple t)
            throws DbException, IOException, TransactionAbortedException {
        ArrayList<Page> pgList = new ArrayList<Page>();
        
        int pgNum;
        
        //need to check this file's pages for empty slots
        for(pgNum = 0; pgNum < this.numPages(); pgNum++)
        {
        	//make a new PageId consisting of this page number
        	PageId pgID = new HeapPageId(this.getId(), pgNum);
        	
        	//access page using pgID from bufferpool
        	HeapPage hp = (HeapPage)Database.getBufferPool().getPage(tid, pgID, Permissions.READ_WRITE);
        	
        	//make sure has empty slots
        	if(hp.getNumEmptySlots() != 0)
        	{
        		//insert the tuple into the page
        		hp.insertTuple(t);
        		//add page to list of modified pages
        		pgList.add(hp);
        		return pgList;
        	}
        }
        
        //if no empty page was found, need to create new one
        HeapPageId hpID = new HeapPageId(this.getId(), pgNum);
        HeapPage hp = new HeapPage(hpID, HeapPage.createEmptyPageData());
        //insert the tuple
        hp.insertTuple(t);
        //add the page to list of modified pages
        pgList.add(hp);
        
        //write the page to the disk
        this.writePage(hp);
    
        return pgList;
    }

    // see DbFile.java for javadocs
    public ArrayList<Page> deleteTuple(TransactionId tid, Tuple t) throws DbException,
            TransactionAbortedException {
    	ArrayList<Page> pgList = new ArrayList<Page>();
    	//get the page from bufferpool
        HeapPage hp = (HeapPage)Database.getBufferPool().getPage(tid, t.getRecordId().getPageId(), Permissions.READ_WRITE);
        //delete the tuple
        hp.deleteTuple(t);
        //tuple RecordId set to null since no longer stored on any page
        t.setRecordId(null);
        //add page to list of modified pages
        pgList.add(hp);
        return pgList;
    }

    // see DbFile.java for javadocs
    public DbFileIterator iterator(TransactionId tid) {
        return new HeapFileTupleIter(tid);
    }
    
    //a class to use as an iterator
    private class HeapFileTupleIter implements DbFileIterator {
    	
    	private static final long serialVersionUID = 1L;
    	//iterator for tuples
    	private Iterator<Tuple> tuple_iter = null;
    	//tuples that will be iterated over
    	private Iterable<Tuple> tuples = null;
    	// page num in HeapFile
    	private int pgNo;
    	// ID of transaction that requests the iterator
    	private TransactionId tid;
    	//checks if iterator is open
    	private boolean isOpen;
    	/**
    	 * this returns a list of tuples from a specified page
    	 * @param pageNumber the page in a heapfile that we want the tuples from
    	 * @return List of tuples
    	 * @throws TransactionAbortedException
    	 * @throws DbException
    	 */
    	private List<Tuple> getTuples(int pageNumber) throws TransactionAbortedException, 
    		DbException
    	{
    		PageId pageId = new HeapPageId(getId(), pageNumber);
    		Page pg = Database.getBufferPool().getPage(tid, pageId, Permissions.READ_ONLY);
    		
    		//create an ArrayList of tuples
    		List<Tuple> list_tuples = new ArrayList<Tuple>();
    		//get all tuples from first page using HeapPage iterator
    		HeapPage heap_pg = (HeapPage)pg;
    		Iterator<Tuple> itr = heap_pg.iterator();
    		
    		while(itr.hasNext())
    		{
    			list_tuples.add(itr.next());
    		}
    		return list_tuples;
    	}
    	
    	public HeapFileTupleIter(TransactionId tid){
    		this.tid = tid;
    		this.isOpen = false;
    	}

		@Override
		public void open() throws DbException, TransactionAbortedException {
			if(this.isOpen == false)
			{
			//start at the first page
			this.pgNo = 0;
			//get tuples at first page
			this.tuples = getTuples(this.pgNo);
			//set tuple iterator to first tuple of first page
			this.tuple_iter = this.tuples.iterator();
			this.isOpen = true;
			}
		}

		@Override
		public boolean hasNext() throws DbException,
				TransactionAbortedException {
			if(this.tuple_iter == null)
			{
				return false;
			}
			if(this.tuple_iter.hasNext())
			{
				return true;
			}
			else if(!this.tuple_iter.hasNext() && this.pgNo < numPages() - 1)
			{
				//there are no more tuples in this page, go to next page
				List<Tuple> nextPageTup = getTuples(this.pgNo + 1);
				//check to see if next page has tuples
				if(nextPageTup.size() != 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				//no tuple in this page and there are no more pages afterwards
				return false;
			}
			
		}

		@Override
		public Tuple next() throws DbException, TransactionAbortedException,
				NoSuchElementException {
			if(this.tuple_iter == null)
			{
				throw new NoSuchElementException("next() is null.");
			}
			if(this.tuple_iter.hasNext())
			{
				return this.tuple_iter.next();
			}
			//no tuples on current page, but still more pages
			else if(!this.tuple_iter.hasNext() && this.pgNo < numPages() - 1)
			{
				//go to next page
				pgNo++;
				this.tuples = getTuples(pgNo);
				this.tuple_iter = this.tuples.iterator();
				
				if(this.tuple_iter.hasNext())
				{
					return this.tuple_iter.next();
				}
				else
				{
					//no more tuples in next page
					throw new NoSuchElementException("No more tuples available.");
				}
			}
			else
			{
				//no more tuples in file
				throw new NoSuchElementException("No more tuples available.");
			}
		}

		@Override
		public void rewind() throws DbException, TransactionAbortedException {
			//close sets tuple iterator to null
			this.close();
			//open sets tuple iterator to first page's tuple
			this.open();
			
		}

		@Override
		public void close() {
			this.tuple_iter = null;
			this.isOpen = false;
		}
    }
}

