#ifndef MULTIMAP_INCLUDED
#define MULTIMAP_INCLUDED

#include <string>
#include <iostream>
using namespace std;

class MultiMap
{
private:

	//declaring Node first to prevent compilation errors
	//structure for a tree node
	struct Node
	{
		//within the tree nodes, making a linked list for the values
		//since we can have multiple values per key
		struct ValueNode
		{
			ValueNode(unsigned int val)
			{
				value = val;
				next_val = prev_val = nullptr;
			}
			unsigned int value;
			ValueNode* next_val;
			ValueNode* prev_val;
		};
		
		std::string ROOT;
		std::string key;
		ValueNode* valuelist;
		Node* left;
		Node* right;
		Node* parent;

		Node(std::string str, unsigned int val, std::string root = "EMPTY")
			:ROOT(root)
		{
			key = str;
			valuelist = new ValueNode(val);
			left = right = parent = nullptr;
		}
		//node destructor deletes the value linked list
		~Node()
		{
			ValueNode* p = NULL;
			while (valuelist != nullptr)
			{
				p = valuelist->next_val;
				delete valuelist;
				valuelist = p;
			}
		}
	};
	
public:
	//	You	must	implement	this	public	nested	MultiMap::Iterator	class	
	class Iterator
	{
	public:
		//default constructor creates Iterator in an invalid state
		//runs in O(1) time
		Iterator();

		//Constructor takes in a node ptr
		//runs in O(1) time
		Iterator(Node* p);

		//return true if Iteraror is in a valid state, false otherwise
		//run in O(1) time
		bool valid() const;

		//if Iterator is in a valid state, getKey method must return key
		//from association indicated by Iterator
		//runs in O(1) time
		//if invalid, crash
		std::string	getKey() const;

		//if Iterator is valid, then return from association indicated by iterator
		//runs in O(1) time
		unsigned int getValue() const;

		//if iterator is in an invalid state, next method does nothing and returns false
		//if association indicated by Iterator is last one in MultiMap,
		//next() puts Iterator in invalid state and returns false
		//otherwise next() makes Iterator indicate association in the sequence that comes
		//immediately after the one it currently indicates and returns true
		//run in average case O(logN) time and worst O(N) time
		bool next();

		//if Iterator is invalid, prev() does nothing and returns false
		//if Iterator is first one in MultiMap, invalidate Iterator and return false
		//otehrwise, prev() makes Iterator indicate association that comes immediately 
		//before the one it currently indicates and return true
		//run in average case O(logN) time and worst O(N) time
		bool prev();
	private:
		bool m_valid;
		Node* m_NodePtr;
	};

	//default constructor that contains no associations
	//runs in O(1) time
	MultiMap();
	
	//destructor releases all resources held by MultiMap
	//must run in O(N) time
	~MultiMap();

	//removes all associations from MultiMap.
	//must run in O(N) time
	void clear();

	//insert adds to MultiMap a new association of the key/value pair
	//insert must run in average case O(logN) time, worst case O(N) time (due to imbalanced binary search tree)
	void insert(std::string	key, unsigned int value);

	//returns an invalid Iterator if no association found equal to key
	//otherwise return a valid Iterator of the earliest association of the key
	// runs average O(logN) and worst O(N) time.
	Iterator findEqual(std::string key) const;

	//returns invalid Iterator if no association matches key
	//if atleast one association has key greater than or equal to key parameter,
	//then returns Iterator to earliest association
	//"earliest" means that in an ordered sequence of all associations in the Multimap,
	//the association indicated by Iterator comes BEFORE any other with same or greater key
	//run in average case O(logN) time, worst case O(N) time
	Iterator findEqualOrSuccessor(std::string key) const;

	//returns invalid Iterator if no association matches key
	//if atleast one association has key greater than or equal to key parameter,
	//then returns Iterator to latest association.
	//"latest" means that in an ordered sequence of all associations, the association
	//indicated by the return Iterator comes after any others with the same key or a lesser key
	//run in average case O(logN) time, worst case O(N) time
	Iterator findEqualOrPredecessor(std::string	key) const;

	///////////////test/////////////////////
	/*
	/////DELETE BEFORE TURNING IN////////
	Node* getRoot() { return m_root; }
	void print(Node* p);
	*/
private:
	//to keep track of the root of the binary tree
	Node* m_root;

	//using post order traversal so every node won't have children when deleting
	//using recursion
	//will also take care of deleting value linked list
	void DeleteTree(Node* p);

	//determines where to insert by taking in a Node ptr and a std::string
	//returns a node ptr
	//using recursion
	Node* FindNode(Node* p, std::string s) const;

	//finds the node equal to the std::string argument
	//if doesn't exist then takes the node that would be previous to
	//where it would have been 
	//using recursion, returns an Iterator
	Iterator FindEqualorPrevNode(Node* p, std::string s) const;

	//finds a node equal to the std::string argument
	//if doesn't exist then takes teh node that would be next to
	//where it would have been
	//using recursion, returns a Node ptr
	Iterator FindEqualorNextNode(Node* p, std::string s) const;

	//takes in a node ptr and a std::string and checks if the std::string exits
	//returns an interator
	//using recursion
	Iterator binarySearch(Node* p, std::string s) const;
	
	//	To	prevent	Multimaps	from	being	copied	or	assigned,	declare	these	members	
	//	private	and	do	not	implement	them.
	MultiMap(const MultiMap& other);
	MultiMap& operator=(const MultiMap&	rhs);
};

#endif MULTIMAP_INCLUDED