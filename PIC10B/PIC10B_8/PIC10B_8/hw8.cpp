/*
Jason Tieu
304047667
In this assignment, we create a linked list of integers by modifying the Node, Iterator, and List classes from Chapter 12.
*/

#include <string>
#include <iostream>
#include <cassert>

using namespace std;

class List;
class Iterator;

class Node
{
public:
   Node(int s);
   void flip();

private:
   int data;
   Node* previous;
   Node* next;

friend class List;
friend class Iterator;
};

class List
{
public:
   List();
   void push_back(int data);
   void insert(Iterator iter, int s);
   Iterator erase(Iterator iter);
   Iterator begin();
   Iterator end();
   int getSize() const;
   void reverse();
   void push_front(int value);
   void swap(List& other);
   void insert_in_order(int data);

private:
   Node* first;
   Node* last;
   int size;

friend class Iterator;
};

class Iterator
{
public:
 
   Iterator();
   int get() const;
   void next();
   void previous();
   bool equals(Iterator b) const;

   Iterator operator++();
   Iterator& operator++(int unused);
   Iterator operator--();
   Iterator& operator--(int unused);
   bool operator==(Iterator b);
   bool operator!=(Iterator b);

private:
   Node* position;
   List* container;

friend class List;
};

//nonmember functions
void downsize(List& l);
int maximum(List& l);
void sort(List& l);
List merge(List& l1, List& l2);
ostream& operator<<(ostream& out, List& l);

int main()
{
	List numbers;
	Iterator pos;
	int data, index, value;
	cout << "Please input a set of nonnegative numbers for a List (Enter -1 when you are finished):" << endl;
	//while loop used to allow user to input data until inputting -1
	while (1)
	{
		cin >> data;
		if (data != -1)
			numbers.push_back(data);
		else
			break;
	}
	cout << "Your list is" << endl;
	cout << numbers << endl << endl;

	//while loop used to allow user to input data until inputting -1
	while (1)
	{
		cout << "Select an index for insertion (enter -1 when finished): ";
		cin >> index;
		if (index != -1)
		{
			cout << "Select a value for insertion: ";
			cin >> value;
			pos = numbers.begin();
			for (int i = 0; i < index; i++)
				pos.next();
			numbers.insert(pos, value);
		}
		else 
			break;
	}
	cout << endl << "Your augmented List is " << endl;
	cout << numbers << endl << endl;

	cout << "The maximum of the List is : " << maximum(numbers) << endl;
	cout << "The size of the List is : " << numbers.getSize() << endl;
	cout << "When we remove every second value from this list we are left with " << endl;
	downsize(numbers);
	cout << numbers << endl;

	cout << "When we sort this downsized list we obtain " << endl;
	sort(numbers);
	cout << numbers << endl;

	cout << "And this sorted list in reverse order is " << endl;
	numbers.reverse();
	cout << numbers << endl;

	cout << "If we merge this list with the list (2,3,5,7,11) we obtain " << endl;
	//making a list and manually inputting values
	List ex;
	ex.push_back(2); ex.push_back(3); ex.push_back(5); ex.push_back(7); ex.push_back(11);
	cout << merge(numbers, ex) << endl << endl;

   system("pause");
   return 0;
}

/** 
	Constructs a node with a given data value.
    @param s the data to store in this node
*/
Node::Node(int s)
{  
   data = s;
   previous = NULL;
   next = NULL;
}

/**
	This function flips the next and previous nodes for the reverse list function
*/
void Node::flip()
{
	Node* temp;
	temp = next;
	next = previous;
	previous = temp;
}
/**
	Constructs an empty list;
*/
List::List()
{  
	first = NULL;
	last = NULL;
	size = 0;
}

/**
	Appends an element to the list.
    @param data the value to append
*/
void List::push_back(int data)
{  
   Node* new_node = new Node(data);
   if (last == NULL) // List is empty
   {  
      first = new_node;
      last = new_node;
   }
   else
   {  
      new_node->previous = last;
      last->next = new_node;
      last = new_node;
   }
   ++size;
}

/**
	Inserts an element into the list.
    @param iter the position before which to insert
    @param s the value to append
*/
void List::insert(Iterator iter, int s)
{  
   if (iter.position == NULL)
   {  
      push_back(s);
      return;
   }

   Node* after = iter.position;
   Node* before = after->previous;
   Node* new_node = new Node(s);
   new_node->previous = before;
   new_node->next = after;
   after->previous = new_node;
   if (before == NULL) // Insert at beginning
      first = new_node;
   else
      before->next = new_node;
   ++size;
}

/**
    Removes an element from the list.
    @param iter the position to remove
    @return an iterator pointing to the element after the erased element
*/
Iterator List::erase(Iterator iter)
{  
   assert(iter.position != NULL);
   Node* remove = iter.position;
   Node* before = remove->previous;
   Node* after = remove->next;
   if (remove == first)
      first = after;
   else
      before->next = after;
   if (remove == last)
      last = before;
   else
      after->previous = before;
   delete remove;
   Iterator r;
   r.position = after;
   r.container = this;
   --size;
   return r;
}

/**
    Gets the beginning position of the list.
    @return an iterator pointing to the beginning of the list
*/
Iterator List::begin()
{  
   Iterator iter;
   iter.position = first;
   iter.container = this;
   return iter;
}

/**
    Gets the past-the-end position of the list.
    @return an iterator pointing past the end of the list
*/
Iterator List::end()
{  
   Iterator iter;
   iter.position = NULL;
   iter.container = this;
   return iter;
}

/**
	This function gets the size of the list
	@return the size of the list
*/
int List::getSize() const
{
	return size;
}

/**
	This function reverses the nodes in a list
*/
void List::reverse()
{
	Node* temp = first;
	while (temp)
	{
		temp->flip();
		temp = temp->previous;
	}
	temp = last;
	last = first;
	first = temp;
}

/**
	This function adds a value to the beginning of a list
	@param value is the value being added
*/
void List::push_front(int value)
{
	//set up the new node
	Node* n = new Node(value);
	n->next = first;
	n->previous = NULL;
	//update the other nodes
	if (first != NULL)//if first is not NULL
	{
		first->previous = n;
	}
	//setting fist node in the list
	first = n;
	//set last to new node you inserted
	if (last == NULL)
	{
		last = n;
	}
	//update size
	++size;
}

/**
	This function swaps the elements on this list and other. Your method should work in O(1) time
	@param is the other list being swapped with
*/
void List::swap(List& other)
{
	List temp = *this;
	*this = other;
	other = temp;
}

/** 
	This function puts the list in numerical order
	@param data is the value being compared against the list values
*/
void List::insert_in_order(int data)
{
	for (Iterator it = begin(); !it.equals(end()); it.next())
	{
		if (it.get() > data)
		{
			insert(it, data);
			return;
		}
	}
	push_back(data);
}

/**
   Constructs an iterator that does not point into any list.
*/
Iterator::Iterator()
{  
   position = NULL;
   container = NULL;
}

/**  
   Looks up the value at a position.
   @return the value of the node to which the iterator points
*/
int Iterator::get() const
{  
   assert(position != NULL);
   return position->data;
}

/**
   Advances the iterator to the next node.
*/
void Iterator::next()
{  
   assert(position != NULL);
   position = position->next;
}

/**
   Moves the iterator to the previous node.
*/
void Iterator::previous()
{  
   assert(position != container->first);
   if (position == NULL)
      position = container->last;
   else 
      position = position->previous;
}

/**
   Compares two iterators
   @param b the iterator to compare with this iterator
   @return true if this iterator and b are equal
*/
bool Iterator::equals(Iterator b) const
{  
   return position == b.position;
}

/** 
	This is the postfix version of the iterator ++ operator
	@returns the iterator before the incrementation
*/
Iterator Iterator::operator++()
{
	assert(position != NULL);
    Iterator temp = *this;
    position = position -> next;
    return temp;
}

/** 
	This function is the prefix version of the iterator ++ operator
	@return unused is used to differentiate this function from the postfix one
	@return the iterator after the incrementation
*/
Iterator& Iterator::operator++(int unused)
{
	assert((*this).position != NULL);
	position = position -> next;
	return *this;
}

/** 
	This is the postfix version of the iterator -- operator
	@returns the iterator before the decrementation
*/
Iterator Iterator::operator--()
{
	assert(position != container->first);
	Iterator temp = *this;
    position = position -> previous;
    return temp;
}

/** 
	This function is the prefix version of the iterator -- operator
	@return unused is used to differentiate this function from the postfix one
	@return the iterator after the decrementation
*/
Iterator& Iterator::operator--(int unused)
{
	assert(position != container->first);
	position = position -> previous;
	return *this;
}

/**
	This function overloads the == operator to allow the comparison of iterators
	@param b is the other iterator being compared to the one calling this function
	@return true or false
*/
bool Iterator::operator==(Iterator b)
{
	return position == b.position;
}

/**
	This function overloads the != operator to allow the comparison of iterators
	@param b is the other iterator being compared to the one calling this function
	@return true or false
*/
bool Iterator::operator!=(Iterator b)
{
	return position != b.position;
}

/**
	This function removes every second value from a linked list.
	@param is the list being modified
*/
void downsize(List& l)
{
	Iterator it = l.begin();
	while (it != l.end())
	{
		it.next();
		if (it != l.end())
		{
			it = l.erase(it);
		}
	}
}

/**
	This function finds the maximum value of a list
	@param is the list being searched
	@return is the maximum value
*/
int maximum(List& l)
{
	int m = -1;
	for (Iterator it = l.begin(); !it.equals(l.end()); it.next())
	{
		if (m < it.get())
			m = it.get();
	}
	return m;
}

/**
	This function sorts the elements of a list without copying the elements into a vector
	@param l is the list being sorted
*/
void sort(List& l)
{
	List result;
	Iterator it = l.begin();
	while (!it.equals(l.end()))
	{
		result.insert_in_order(it.get());
		it.next();
	
	}
	l = result;
}

/**
	This function merges two lists together
	@param l1 is the first list
	@param l2 is the second list
	@return is the new merged list
*/
List merge(List& l1, List& l2)
{
	List l;
	Iterator it1 = l1.begin();
	Iterator it2 = l2.begin();
	while (it1 != l1.end() && it2 != l2.end())
	{
		l.push_back((it1).get());
		l.push_back((it2).get());
		it1++;
		it2++;
	}
	while (it1 != l1.end())
	{
		l.push_back(it1.get());
		it1++;
	}
	while (it2 != l2.end())
	{
		l.push_back(it2.get());
		it2++;
	}
	return l;
}

/**
	This function overloads the << operator to allow the printing of lists
	@param out is the output object
	@param l is the list being printed
	@return out is the output object
*/
ostream& operator<<(ostream& out, List& l)
{
	Iterator pos;
	cout << "(";
	for (pos = l.begin(); pos != l.end(); pos++)
	{
		//if pos is at beginning, only print value without comma
		if (pos == l.begin())
			cout << pos.get();
		//prints comma before value so it's not example: (1,1,1,)
		else
			cout << "," << pos.get();
	}
	cout << ")";
	return out;
}