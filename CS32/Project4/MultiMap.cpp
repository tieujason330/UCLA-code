#include "MultiMap.h"
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;

/////////////////////Iterator Implementation/////////////////////////////

MultiMap::Iterator::Iterator()
: m_valid(false)
{}

MultiMap::Iterator::Iterator(Node* p)
: m_NodePtr(p)
{
	if (p == nullptr)
		m_valid = false;
	m_valid = true;
}

//return true if Iteraror is in a valid state, false otherwise
//run in O(1) time
bool MultiMap::Iterator::valid() const
{
	return m_valid;
}

std::string	MultiMap::Iterator::getKey() const
{
	//if invalid then stops program
	assert(valid());

	if (valid())
		return m_NodePtr->key;
}

unsigned int MultiMap::Iterator::getValue() const
{
	//if invalid then stops program
	assert(valid());
	
	if (valid())
		return m_NodePtr->valuelist->value;
}

bool MultiMap::Iterator::next()
{
	//if invalid then returns false
	if (!valid())
		return false;

	//if next value in value linked list is not null, then next() moves to next value
	if (m_NodePtr->valuelist->next_val != nullptr)
	{
		m_NodePtr->valuelist = m_NodePtr->valuelist->next_val;
		return true;
	}
	//if no right child (for BST right is always larger)
	if (m_NodePtr->right == nullptr)
	{
		//if tree nodes less than the Root node
		if (m_NodePtr->key <= m_NodePtr->ROOT)
		{
			//if no right child, then set to the parent's parent
			//since for inorder traversal that would be the next in sequence
			m_NodePtr = m_NodePtr->parent->parent;
			return true;
		}
		
		//if tree node greater than Root
		if (m_NodePtr->key > m_NodePtr->ROOT)
		{
			//if parent is larger, next is parent
			if (m_NodePtr->key < m_NodePtr->parent->key)
				m_NodePtr = m_NodePtr->parent;
			//if parent is smaller
			//node is the last in the BST
			else
			{
				m_valid = false;
				return false;
			}
		}
	}
	//there is a right child
	else
	{
		//if left most child of next right child is null
		if (m_NodePtr->right->left == nullptr)
			m_NodePtr = m_NodePtr->right;
		//the next in succession is the left most child of the next right child
		else
			m_NodePtr = m_NodePtr->right->left;
	}

	return true;
}

bool MultiMap::Iterator::prev()
{
	if (!valid())
		return false;

	//if prev value in value linked list is not null, then prev moves to prev value
	if (m_NodePtr->valuelist->prev_val != nullptr)
	{
		m_NodePtr->valuelist = m_NodePtr->valuelist->prev_val;
		return true;
	}
	//if no left child (for BST left is smaller)
	if (m_NodePtr->left == nullptr)
	{
		//for tree nodes less than Root
		if (m_NodePtr->key <= m_NodePtr->ROOT)
		{
			//if node is less than parent and has no left child
			//then it is the smallest node in the BST
			if (m_NodePtr->key < m_NodePtr->parent->key)
			{
				m_valid = false;
				return false;
			}
			//if node is greater than parent, then set node to parent
			else
				m_NodePtr = m_NodePtr->parent;
		}
		//for tree nodes greater than Root
		if (m_NodePtr->key > m_NodePtr->ROOT)
		{
			//if node is less than parent node
			//set node to parent's parent since for inorder traversal that would be
			//the next least in sequence
			if (m_NodePtr->key < m_NodePtr->parent->key)
				m_NodePtr = m_NodePtr->parent->parent;
			//if node is greater than parent, set it to parent
			else
				m_NodePtr = m_NodePtr->parent;
		}
	}
	//there is a left child
	else
	{
		//the next least one is the rightmost of the left child
		if (m_NodePtr->left->right == nullptr)
			m_NodePtr = m_NodePtr->left;
		else
			m_NodePtr = m_NodePtr->left->right;
	}
	return true;
}

//////////////////MultiMap Implementation/////////////////////////////////

MultiMap::MultiMap()
{
	m_root = nullptr; //no associations
}

MultiMap::~MultiMap()
{
	if (m_root != nullptr)
		clear();
}

void MultiMap::clear()
{
	DeleteTree(m_root);//will also take care of deleting value lists
	m_root = nullptr;
}

void MultiMap::insert(std::string key, unsigned int value)
{
	//if there are nodes in the tree
	//set m_root to new node
	if (m_root == nullptr)
	{
		Node* newNode = new Node(key, value, key);//third argument is to set a data member to the root of the tree
		m_root = newNode;
		return;
	}
	//set a temp var to the node we'll insert after
	Node* temp_parent = FindNode(m_root, key);

	Node::ValueNode* vNode;
	if (temp_parent != nullptr)
	{
		//set vNode to beginning of value list
		vNode = temp_parent->valuelist;
		assert(vNode != nullptr);
		//if key is already in the tree
		if (key == temp_parent->key)
		{
			//insert the value at the end of the value list
			Node::ValueNode* newVal;
			for (newVal = vNode; newVal != nullptr; newVal = newVal->next_val)
			{
				//increments vNode along the linked list
				//always the prev of newVal
				if (vNode->next_val != nullptr)
					vNode = vNode->next_val;
			}
			//if list is empty
			assert(vNode != nullptr);
			if (vNode == nullptr)
			{
				newVal = new Node::ValueNode(value);
				temp_parent->valuelist = newVal;
				return;
			}
			//if list isn't empty
			//make a new value and set it's previous ptr to vNode
			//set vNode's next ptr to newVal
			else
			{
				newVal = new Node::ValueNode(value);
				newVal->prev_val = vNode;
				newVal->next_val = nullptr;
				vNode->next_val = newVal;
				return;
			}
		}
		//if key is > than the temp_parent key, insert right child
		if (key > temp_parent->key)
		{
			Node* rightChild = new Node(key, value, m_root->key);
			temp_parent->right = rightChild;
			rightChild->parent = temp_parent;
		}
		//else insert left child
		if (key < temp_parent->key)
		{
			Node* leftChild = new Node(key, value, m_root->key);
			temp_parent->left = leftChild;
			leftChild->parent = temp_parent;
		}
	}
}

MultiMap::Iterator MultiMap::findEqual(std::string key) const
{
	return binarySearch(m_root, key);
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string key) const
{
	return FindEqualorNextNode(m_root, key);
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string	key) const
{
	return FindEqualorPrevNode(m_root, key);
}

///////MultiMap Private Functions/////////////////

void MultiMap::DeleteTree(Node* p)
{
	//base case
	if (p == nullptr)
		return;
	//postorder so left => right => root (in this case p)
	DeleteTree(p->left);
	DeleteTree(p->right);

	//then deletes p
	delete p;
}

MultiMap::Node* MultiMap::FindNode(Node* p, std::string s) const
{
	//if p has no children then return p
	if (p->left == nullptr && p->right == nullptr)
		return p;
	//if p has same key, return p
	if (s == p->key)
		return p;

	//if the left/right is valid then perform recursion
	if (s > p->key && p->right != nullptr)
		return FindNode(p->right, s);
	if (s < p->key && p->left != nullptr)
		return FindNode(p->left, s);
	//if the required child doesn't exist then return p
	else
		return p;
}

MultiMap::Iterator MultiMap::FindEqualorPrevNode(Node* p, std::string s) const
{
	//base case
	if (s == p->key)
		return Iterator(p);

	//if there are no child nodes
	if (p->left == nullptr && p->right == nullptr)
	{
		//if s is greater than p's key, return p
		if (s > p->key)
			return Iterator(p);
		
		//if s is less than p's key and p has a parent node and s is greater than parent node's key
		//return Iterator to p's parent
		if (s < p->key && p->parent != nullptr && s > p->parent->key)
			return Iterator(p->parent);
		//if s is less than p's key, then return invalid Iterator
		if (s < p->key)
			return Iterator(); //invalid
	}

	//if only one child available
	if ((p->left != nullptr && p->right == nullptr) || (p->left == nullptr && p->right != nullptr))
	{
		//if there is a left child only
		if (p->left != nullptr && p->right == nullptr)
		{
			//if s is greater than node's key
			//return a Iterator to current node
			if (s > p->key)
				return Iterator(p);
			//if s is less than node's key AND s is greater than node's left child key
			//return an Iterator pointing to node's next
			if (s < p->key && s > p->left->key)
				return Iterator(p->left);
			//if s is greater than node's key AND less than node's parent key
			//return Iterator to node's parent
			if (s > p->key && s < p->parent->key)
				return Iterator(p->parent);
		}
		//if there is a right child only
		if (p->left == nullptr && p->right != nullptr)
		{
			//if s is larger than the node's key and smaller than the node's right key
			//returns and Iterator to current node
			if (s > p->key && s < p->right->key)
				return Iterator(p);
			//if s is less than node's key and p's parent is null
			//this means it would be the the least key, and there could be nothing before it
			if (s < p->key && p->parent == nullptr)
				return Iterator();
			//if s is less than node's key and p parent isn't null
			//return Iterator to p's parent
			if (s < p->key && p->parent != nullptr)
				return Iterator(p->parent);
		}
	}
	//if there are both children nodes
	//if s is less than p's key, recursion on left child
	if (s < p->key && p->left != nullptr)
		return FindEqualorPrevNode(p->left, s);
	//if s is greater than p's key, recursion on right childs
	if (s > p->key && p->right != nullptr)
		return FindEqualorPrevNode(p->right, s);
}

MultiMap::Iterator MultiMap::FindEqualorNextNode(Node* p, std::string s) const
{
	//base case
	if (s == p->key)
		return Iterator(p);

	//if there are no child nodes
	if (p->left == nullptr && p->right == nullptr)
	{
		
		//if s is greater than current node's key and less than node's parent's key
		//return Iterator to parent
		if (s > p->key && p->parent != nullptr && s < p->parent->key)
			return Iterator(p->parent);
		//if s is greater than current node's key and greater than node's parent key
		//return Iterator to node's parent's parent
		if (s > p->key && p->parent != nullptr && s > p->parent->key)
			return Iterator(p->parent->parent);
		///////////////
		//if s is greater than p's key, return invalid
		if (s > p->key)
			return Iterator();
		//if s is less than p's key, return Iterator to current node
		if (s < p->key)
			return Iterator(p);
	}
	//if only one child available
	if ((p->left != nullptr && p->right == nullptr) || (p->left == nullptr && p->right != nullptr))
	{
		//if there is a left child only
		if (p->left != nullptr && p->right == nullptr)
		{
			//if s is greater than node's key
			//return an invalid Iterator
			if (s > p->key)
				return Iterator();
			//if s is less than node's key AND s is greater than node's left child key
			//return an Iterator to current node
			if (s < p->key && s > p->left->key)
				return Iterator(p);
		}
		//if there is a right child only
		if (p->left == nullptr && p->right != nullptr)
		{
			//if s is larger than the node's key and smaller than the node's right key
			//returns and Iterator to current node's right child
			if (s > p->key && s < p->right->key)
				return Iterator(p->right);
			//if s is less than node's key return Iterator to node
			if (s < p->key)
				return Iterator(p);
			//if s is greater than node's right key
			//then it is supposed to be the largest node so return invalid
			if (s > p->right->key)
				return Iterator();
		}
	}
	//if there are both children nodes
	//if s is less than p's key, recursion on left child
	if (s < p->key && p->left != nullptr)
		return FindEqualorNextNode(p->left, s);
	//if s is greater than p's key, recursion on right childs
	if (s > p->key && p->right != nullptr)
		return FindEqualorNextNode(p->right, s);
}

MultiMap::Iterator MultiMap::binarySearch(Node* p, std::string s) const
{
	//base cases
	if (p == nullptr)
		return Iterator(); //invalid iterator
	if (p->key == s)
		return Iterator(p);

	if (s > p->key)
		return binarySearch(p->right, s);
	else
		return binarySearch(p->left, s);
}
/*
//test functions
void MultiMap::print(Node* p)
{
	if (p == nullptr)
		return;
	cout << "(";
	print(p->left);
	cout << p->key << ",";
	Node::ValueNode* vv = p->valuelist;
	for (vv; vv != nullptr; vv = vv->next_val)
		cout << vv->value << "-";
	print(p->right);
	cout << ")";
}
*/