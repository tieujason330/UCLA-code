package simpledb;

import java.io.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import simpledb.Catalog.TableInfo;

/**
 * BufferPool manages the reading and writing of pages into memory from
 * disk. Access methods call into it to retrieve pages, and it fetches
 * pages from the appropriate location.
 * <p>
 * The BufferPool is also responsible for locking;  when a transaction fetches
 * a page, BufferPool checks that the transaction has the appropriate
 * locks to read/write the page.
 * 
 * @Threadsafe, all fields are final
 */
public class BufferPool {
    /** Bytes per page, including header. */
    public static final int PAGE_SIZE = 4096;

    private static int pageSize = PAGE_SIZE;
    
    /** Default number of pages passed to the constructor. This is used by
    other classes. BufferPool should use the numPages argument to the
    constructor instead. */
    public static final int DEFAULT_PAGES = 50;

    //capacity of BufferPool
    private int m_max_numPages;
    
    //HashMap that stores page id as key and page as value
    private HashMap<PageId, Page> m_Pid2Page;
    /**
     * Creates a BufferPool that caches up to numPages pages.
     *
     * @param numPages maximum number of pages in this buffer pool.
     */
    public BufferPool(int numPages) {
        this.m_max_numPages = numPages;
        this.m_Pid2Page = new HashMap<PageId, Page>();
    }
    
    public static int getPageSize() {
      return pageSize;
    }
    
    // THIS FUNCTION SHOULD ONLY BE USED FOR TESTING!!
    public static void setPageSize(int pageSize) {
    	BufferPool.pageSize = pageSize;
    }

    /**
     * Retrieve the specified page with the associated permissions.
     * Will acquire a lock and may block if that lock is held by another
     * transaction.
     * <p>
     * The retrieved page should be looked up in the buffer pool.  If it
     * is present, it should be returned.  If it is not present, it should
     * be added to the buffer pool and returned.  If there is insufficient
     * space in the buffer pool, an page should be evicted and the new page
     * should be added in its place.
     *
     * @param tid the ID of the transaction requesting the page
     * @param pid the ID of the requested page
     * @param perm the requested permissions on the page
     */
    public  Page getPage(TransactionId tid, PageId pid, Permissions perm)
        throws TransactionAbortedException, DbException {
        //if HashMap contains the pid, return the associated page
    	if(this.m_Pid2Page.containsKey(pid))
        {
        	return this.m_Pid2Page.get(pid);
        }
    	//else we must retrieve from disk
        else
        {
        	//retrieve TableInfo from Catalog
        	List<TableInfo> tableList = Database.getCatalog().getTableInfo();
        	
        	//for each TableInfo, compare it's DbFile's id to pid's table id
        	for(TableInfo t : tableList)
        	{
        		//tableInfo containing page 
        		if(t.m_dbfile.getId() == pid.getTableId())
        		{
        			Page diskPage = t.m_dbfile.readPage(pid);
        			
        			//if BufferPool is full, evict a page
        			if(this.m_max_numPages == this.m_Pid2Page.size())
        				this.evictPage();

        			//adds pid and diskPage to HashMap
        			this.m_Pid2Page.put(pid, diskPage);
        			
        			return diskPage;
        		}
        	}
        	//if page isn't found in BufferPool or on disk
        	throw new DbException("Page not found in database.");
        }
    }

    /**
     * Releases the lock on a page.
     * Calling this is very risky, and may result in wrong behavior. Think hard
     * about who needs to call this and why, and why they can run the risk of
     * calling it.
     *
     * @param tid the ID of the transaction requesting the unlock
     * @param pid the ID of the page to unlock
     */
    public  void releasePage(TransactionId tid, PageId pid) {
        // some code goes here
        // not necessary for lab1|lab2
    }

    /**
     * Release all locks associated with a given transaction.
     *
     * @param tid the ID of the transaction requesting the unlock
     */
    public void transactionComplete(TransactionId tid) throws IOException {
        // some code goes here
        // not necessary for lab1|lab2
    }

    /** Return true if the specified transaction has a lock on the specified page */
    public boolean holdsLock(TransactionId tid, PageId p) {
        // some code goes here
        // not necessary for lab1|lab2
        return false;
    }

    /**
     * Commit or abort a given transaction; release all locks associated to
     * the transaction.
     *
     * @param tid the ID of the transaction requesting the unlock
     * @param commit a flag indicating whether we should commit or abort
     */
    public void transactionComplete(TransactionId tid, boolean commit)
        throws IOException {
        // some code goes here
        // not necessary for lab1|lab2
    }

    /**
     * Add a tuple to the specified table on behalf of transaction tid.  Will
     * acquire a write lock on the page the tuple is added to and any other 
     * pages that are updated (Lock acquisition is not needed for lab2). 
     * May block if the lock(s) cannot be acquired.
     * 
     * Marks any pages that were dirtied by the operation as dirty by calling
     * their markDirty bit, and updates cached versions of any pages that have 
     * been dirtied so that future requests see up-to-date pages. 
     *
     * @param tid the transaction adding the tuple
     * @param tableId the table to add the tuple to
     * @param t the tuple to add
     */
    public void insertTuple(TransactionId tid, int tableId, Tuple t)
        throws DbException, IOException, TransactionAbortedException {
    	
    	//get the table (file) to add the tuple to
    	HeapFile dbFile = (HeapFile)Database.getCatalog().getDatabaseFile(tableId);
    	//insert the tuple into the file, returns an arraylist
    	ArrayList<Page> pgList = dbFile.insertTuple(tid, t);
    	//get the first pg of the page list
    	Page pg = pgList.get(0);
    	//mark the page as dirty
    	pg.markDirty(true, tid);
    	//keep track of the page in the HashMap
    	this.m_Pid2Page.put(pg.getId(), pg);
    	
    }

    /**
     * Remove the specified tuple from the buffer pool.
     * Will acquire a write lock on the page the tuple is removed from and any
     * other pages that are updated. May block if the lock(s) cannot be acquired.
     *
     * Marks any pages that were dirtied by the operation as dirty by calling
     * their markDirty bit, and updates cached versions of any pages that have 
     * been dirtied so that future requests see up-to-date pages. 
     *
     * @param tid the transaction deleting the tuple.
     * @param t the tuple to delete
     */
    public  void deleteTuple(TransactionId tid, Tuple t)
        throws DbException, IOException, TransactionAbortedException {
    	//get the table id
    	int tableId = t.getRecordId().getPageId().getTableId();
    	//get the table (file) to add the tuple to
    	HeapFile dbFile = (HeapFile)Database.getCatalog().getDatabaseFile(tableId);
    	//insert the tuple into the file, returns an arraylist
    	ArrayList<Page> pgList = dbFile.deleteTuple(tid, t);
    	//get the first pg of the page list
    	Page pg = pgList.get(0);
    	//mark the page as dirty
    	pg.markDirty(true, tid);
    }

    /**
     * Flush all dirty pages to disk.
     * NB: Be careful using this routine -- it writes dirty data to disk so will
     *     break simpledb if running in NO STEAL mode.
     */
    public synchronized void flushAllPages() throws IOException {
    	//if hashmap containing pages isn't empty
        if(!this.m_Pid2Page.isEmpty())
        {
        	//create a set of pageIds
        	Set<PageId> pgIdSET = this.m_Pid2Page.keySet();
        	for(PageId pgId : pgIdSET)
        	{
        		//flush each page
        		this.flushPage(pgId);
        	}
        }
    }

    /** Remove the specific page id from the buffer pool.
        Needed by the recovery manager to ensure that the
        buffer pool doesn't keep a rolled back page in its
        cache.
    */
    public synchronized void discardPage(PageId pid) {
        // some code goes here
        // only necessary for lab5
    }

    /**
     * Flushes a certain page to disk
     * @param pid an ID indicating the page to flush
     */
    private synchronized  void flushPage(PageId pid) throws IOException {
        Page flushPg = this.m_Pid2Page.get(pid);
        
        //page is dirty
        if(flushPg.isDirty() != null)
        	Database.getCatalog().getDatabaseFile(pid.getTableId()).writePage(flushPg);
    }

    /** Write all pages of the specified transaction to disk.
     */
    public synchronized  void flushPages(TransactionId tid) throws IOException {
        // some code goes here
        // not necessary for lab1|lab2
    }

    /**
     * Discards a page from the buffer pool.
     * Flushes the page to disk to ensure dirty pages are updated on disk.
     */
    private synchronized  void evictPage() throws DbException {
        List<PageId> pageIdList = new ArrayList<PageId>(this.m_Pid2Page.keySet());
        
        //always evicts first page in list
        PageId evictPage = pageIdList.get(0);
        
        try {
			flushPage(evictPage);
			this.m_Pid2Page.remove(evictPage);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

}
