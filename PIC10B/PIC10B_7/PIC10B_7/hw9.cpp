/*
Jason Tieu 
304047667

In this project, we implement a Binary Search Tree and print out the locations for values 1 through 9 if they exist in the Tree.
*/
#include <iostream>
#include <string>
using namespace std;

class TreeNode
{
public:
	TreeNode(int data_input);
    void insert_node(TreeNode* new_node);
    void print_nodes() const;
    string find(int value) const;
	TreeNode* left_most();
private:
    int data;
	TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
friend class BinarySearchTree;
friend class TreeIterator;
};

class BinarySearchTree
{
public:
    BinarySearchTree();
    void insert(int data);
    void erase(int data);
    int count(int data) const;
	int largest() const;
	int smallest() const;
	int getSize() const;
	string BinaryCode(int value);
    void print() const;
private:
    TreeNode* root;
	int size;
};

class TreeIterator
{
public:
	TreeIterator(TreeNode* position_input);
	//prefix ++operator moves to the left
	TreeIterator operator++();
	//postfix ++operator moves to the right
	TreeIterator operator++(int unused);
	//prefix --operator moves to the left
	TreeIterator operator--();
	//postfix --operatoer moves to the right
	TreeIterator operator--(int unused);
	int get();
private:
	TreeNode* position;
};

void printData(BinarySearchTree t);
void printBinaryCode(BinarySearchTree t);


int main()
{  
	BinarySearchTree t;
	int num, insert, erase;
	cout << "Please input a set of distinct nonnegative numbers for a Tree "
		<< "(Enter -1 when you are finished):" << endl;
	while (1)
	{
		cin >> num;
		if (num != -1)
			t.insert(num);
		else 
			break;
	}

	printData(t);

	while (1)
	{
		cout << "Select a value for insertion (enter -1 when finished): ";
		cin >> insert;
		if (insert != -1)
			t.insert(insert);
		else
			break;
	}
	
	printData(t);
	printBinaryCode(t);
	cout << endl;

	while(1)
	{
		cout << "Select a value to erase (enter -1 when finished): ";
		cin >> erase;
		if (erase != -1)
			t.erase(erase);
		else
			break;
	}

	cout << endl;
	printBinaryCode(t);
	cout << endl;

	system("pause");
    return 0;
}

/**
	Constructor for Treenode
	@param data_input is the data value being inputted
*/
TreeNode::TreeNode(int data_input)
{
	data = data_input;
	parent = NULL;
	left = NULL;
	right = NULL;
}

/**
	Constructor for BinarySearchTree
*/
BinarySearchTree::BinarySearchTree()
{  
	root = NULL;
	size = 0;
}

/**
	This function prints the nodes of a Binary Search Tree
*/
void BinarySearchTree::print() const
{  
    if (root != NULL)
        root->print_nodes();
}

/** 
	This function inserts values into the Binary Search Tree
	@param data is the value being inserted
*/
void BinarySearchTree::insert(int data)
{
	if (BinaryCode(data) == "2")
	{
		++size;
		TreeNode* new_node = new TreeNode(data);
		if (root == NULL) 
			root = new_node;
		else 
			root->insert_node(new_node);
	}
}

/**
	This function inserts a new node in the the tree that has left/right children
	@param new_node is inserted with value data
*/
void TreeNode::insert_node(TreeNode* new_node)
{  
    if (new_node->data < data)
    {  
		if (left == NULL) 
		{
			left = new_node;
			new_node->parent = this; 
		}
        else left->insert_node(new_node);
    }
    else if (data < new_node->data)
    {  
		if (right == NULL)
		{ 
			right = new_node; 
			new_node->parent = this; 
		}
        else right->insert_node(new_node);
    }
}

/**
	This function counts the amount of time a value appears
	@param data is the value counted
	@return the amount of times the value is counted
*/
int BinarySearchTree::count(int data) const
{
    if (root == NULL) return 0;
    else if (root->find(data) != string("2")) 
		return 1;
    else 
		return 0;
}

/**
	This function finds the largest value of a Binary Search Tree
	@return the largest value
*/
int BinarySearchTree::largest() const
{
	if (!root)
	{
		return -1;
	}
	TreeNode* temp = root;
	while (temp->right)
	{
		temp = temp->right;
	}
	return temp->data;
}

/**
	This function finds the smallest value of a Binary Search Tree
	@return the smallest value
*/
int BinarySearchTree::smallest() const
{
	if (!root)
	{
		return -1;
	}
	TreeNode* temp = root;
	while (temp->left)
	{
		temp = temp->left;
	}
	return temp->data;
}

/**
	This function gets the size of a Binary Search Tree
	@return the size of the Tree
*/
int BinarySearchTree::getSize() const
{
	return size;
}

/**
	This function erases a targetted value
	@param data is the value that is going to be erased if it exists
*/
void BinarySearchTree::erase(int data)
{
    // Find node to be removed

    TreeNode* to_be_removed = root;
    TreeNode* parent = NULL;
    bool found = false;
    while (!found && to_be_removed != NULL)
    {
        if (to_be_removed->data < data)
        {
            parent = to_be_removed;
            to_be_removed = to_be_removed->right;
        }
        else if (data < to_be_removed->data)
        {
            parent = to_be_removed;
            to_be_removed = to_be_removed->left;
        }
        else found = true;
    }

    if (!found) return;
    // to_be_removed contains data
	--size;//******
    // If one of the children is empty, use the other

    if (to_be_removed->left == NULL || to_be_removed->right == NULL)
    {
        TreeNode* new_child;
        if (to_be_removed->left == NULL) 
            new_child = to_be_removed->right;
        else 
			new_child = to_be_removed->left;
		//conect with parent
        if (parent == NULL) // Found in root
		{
            root = new_child;
			if (new_child)
				new_child->parent = NULL;
		}
        else if (parent->left == to_be_removed)
		{
            parent->left = new_child;
			if (new_child)
				new_child->parent = parent;
		}
        else 
		{
            parent->right = new_child;
			if (new_child)
				new_child->parent = parent;
		}
        delete to_be_removed;
        return;
    }
      
    // Neither subtree is empty

    // Find largest element in the left subtree

    TreeNode* largest_parent = to_be_removed;
    TreeNode* largest = to_be_removed->left;
    while (largest->right != NULL)
    {
        largest_parent = largest;
        largest = largest->right;////
    }

    // largest contains largest child in left subtree
       
    // Move contents, unlink child
    to_be_removed->data = largest->data;
    if (largest_parent == to_be_removed) 
        largest_parent->left = largest->left; 
    else 
        largest_parent->right = largest->right;
    delete largest;
}
 
/**
	This function recursively finds a value and returns it's path in binary (0 for left, 1 for right)
	@param value is the value targetted
	@return string is the binary code of the value
*/
string TreeNode::find(int value) const
{
	string s;
    if (value < data)
    {
        if (left == NULL) 
			return "2";
		s = "0" + left->find(value);
		if (s[s.length()-1] == '2')
			return "2";
		return s;
    }
    else if (data < value)
    {
        if (right == NULL) 
			return "2";
		s = "1" + right->find(value);
		if (s[s.length()-1] == '2')
			return "2";
		return s;
    }
    else 
        return "";
}

/**
	This function returns the binary code of a value
	@param value is the value being targetted
	@return string is the code being called from find function
*/
string BinarySearchTree::BinaryCode(int value)
{
	if(root)
	{
		return root->find(value);
	}
	return "2";
}

/**
	This function prints the nodes of a Tree
*/
void TreeNode::print_nodes() const
{  
    if (left != NULL)
        left->print_nodes();
    cout << data << "\n";
    if (right != NULL)
        right->print_nodes();
}

/**
	Constructor for TreeIterator
	@param position_input is the position of a node
*/
TreeIterator::TreeIterator(TreeNode* position_input)
{
	position = position_input;
}

/**
	This is the prefix version of the ++ operator. It travels to the next element on the left
*/
TreeIterator TreeIterator::operator++()
{
	TreeIterator it = *this;
	if (position)
	{
		position = position->left;
	}
	return it;
}

/**
	This is the postfix version of the ++ operator. It travels to the next element on the right
	@param unused is used to distinguish postfix from prefix
*/
TreeIterator TreeIterator::operator++(int unused)
{
	TreeIterator it = *this;
	if (position)
	{
		position = position->right;
	}
	return it;
}

/**
	This is the prefix version of the -- operator. It travels to the previous element on the left
*/
TreeIterator TreeIterator::operator--()
{
	if(position)
		position = position->parent;
	return *this;
}

/**
	This is the postfix version of the -- operator. It travels to the previous element on the right
	@param unused is used to distinguish postfix from prefix
*/
TreeIterator TreeIterator::operator--(int unused)
{
		TreeIterator it = *this;
		--(*this);//decrements original value
		return it;//returns copy of original value
}

/**
	This function gets the value of the targetted position
*/
int TreeIterator::get()
{
	return position->data;
}

/**
	This function prints out the largest and smallest value of a tree. Prints out size of tree also.
	@param t is the Binary Search Tree 
*/
void printData(BinarySearchTree t)
{
	cout << endl << "The maximum of the entries is: " << t.largest() << endl;
	cout << "The minimum of the entries is: " << t.smallest() << endl;
	cout << "The size of the Tree is: " << t.getSize() << endl;
	cout << endl;
}

/**
	This function prints out the elements 1 - 9 in BinaryCode (0 - left, 1 - right)
	@param t is the Binary Search Tree
*/
void printBinaryCode(BinarySearchTree t)
{
	for(int i = 1; i <= 9; i++)
	{
		cout << "The binary code for " << i << " is: " << t.BinaryCode(i) << endl;
	}
}