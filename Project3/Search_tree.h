#ifndef RPOVDGBQN9TIEO3P
#define RPOVDGBQN9TIEO3P

#include "Exception.h"
#include "ece250.h"
#include <cassert>

template <typename Type>
class Search_tree {
	public:
		class Iterator;

	private:
		class Node {
			public:
				Type node_value;
				int tree_height;

				// The left and right sub-trees
				Node *left_tree;
				Node *right_tree;

				// Hint as to how you can create your iterator
				// Point to the previous and next nodes in linear order
				Node *previous_node;
				Node *next_node;

				// Member functions
				Node( Type const & = Type() );

				void update_height();

				int height() const;
				bool is_leaf() const;
				Node *front();
				Node *back();
				Node *find( Type const &obj );

				//Extra Helper Function 
				int balanceFactor();
				void rotateR(Node*& curr_node);
				void rotateL(Node*& curr_node);
				void balance(Node*& curr_node);
				
				
				void clear();
				bool insert( Type const &obj, Node *&to_this );
				bool erase( Type const &obj, Node *&to_this );

		};

		Node *root_node;
		int tree_size;

		// Hint as to how to start your linked list of the nodes in order 
		Node *front_sentinel;
		Node *back_sentinel;

	public:
		class Iterator {
			private:
				Search_tree *containing_tree;
				Node *current_node;
				bool is_end;

				// The constructor is private so that only the search tree can create an iterator
				Iterator( Search_tree *tree, Node *starting_node );

			public:
				// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
				Type operator*() const;
				Iterator &operator++();
				Iterator &operator--();
				bool operator==( Iterator const &rhs ) const;
				bool operator!=( Iterator const &rhs ) const;

			// Make the search tree a friend so that it can call the constructor
			friend class Search_tree;
		};

		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Search_tree();
		~Search_tree();

		bool empty() const;
		int size() const;
		int height() const;

		Type front() const;
		Type back() const;

		Iterator begin();
		Iterator end();
		Iterator rbegin();
		Iterator rend();
		Iterator find( Type const & );

		void clear();
		bool insert( Type const & );
		bool erase( Type const & );

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree():
root_node( nullptr ),
tree_size( 0 ),
front_sentinel( new Search_tree::Node( Type() ) ),
back_sentinel( new Search_tree::Node( Type() ) ) {
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
	front_sentinel->previous_node = nullptr;
	back_sentinel->next_node = nullptr;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
	clear();  // might as well use it...
	delete front_sentinel;
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return ( root_node == nullptr );
}

template <typename Type>
int Search_tree<Type>::size() const {
	return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
	return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
	return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator( this, back_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator( this, front_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
	if ( empty() ) {
		return Iterator( this, back_sentinel );
	}

	typename Search_tree<Type>::Node *search_result = root_node->find( obj );

	if ( search_result == nullptr ) {
		return Iterator( this, back_sentinel );
	} else {
		return Iterator( this, search_result );
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if ( !empty() ) {
		root_node->clear();
		root_node = nullptr;
		tree_size = 0;
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert( Type const &obj ) {

	// if the tree is empty, that means there is only front_sentinel and back_sentinel
	if ( empty() ) {
		// create a root_node
		root_node = new Search_tree::Node( obj );
		// front_sentinel (head) should point to root node
		front_sentinel->next_node = root_node;
		// back_sentinel previous_node (tail) should point to root node
		back_sentinel->previous_node = root_node;
		// root_node's previous points to the front_sentinel (head)
		root_node->previous_node = front_sentinel;
		// root_node's next points to the back_sentinel (head)
		root_node->next_node = back_sentinel;
		// size of tree increased by 1
		tree_size = 1;
		return true;
	} else if ( root_node->insert( obj, root_node ) ) {
		++tree_size;
		return true;
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::erase( Type const &obj ) {
	if ( !empty() && root_node->erase( obj, root_node ) ) {
		--tree_size;
		return true;
	} else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
node_value( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
next_node( nullptr ),
previous_node( nullptr ),
tree_height( 0 ) {
	// does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
	tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	return ( left_tree == nullptr ) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
	if ( obj == node_value ) {
		return this;
	} else if ( obj < node_value ) {
		return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
	} else {
		return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if ( left_tree != nullptr ) {
		left_tree->clear();
	}

	if ( right_tree != nullptr ) {
		right_tree->clear();
	}

	delete this;
}

// INSERT FUNCTION MODIFIED TO FOLLOW AVL RULES
template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			Search_tree<Type>::Node *previous=to_this->previous_node;

			// this is a new node with the given value 
			left_tree = new Search_tree<Type>::Node( obj );
			// new node's previous_node points to whatever the original_node's (to_this) previous node was;
			left_tree->previous_node = to_this->previous_node;
			// new node's next_node points to the original node;
			left_tree->next_node = to_this;
			// previous_node of the to_this points to the left_tree 
			previous->next_node = left_tree;
			// new tree is becomes the previous node of the to_this
			to_this->previous_node = left_tree;

			update_height();
			//once new node is added, the BST tree needs to be balanced to make it follow AVL rules
			balance(to_this);
			return true;
		} else {
			if ( left_tree->insert( obj, left_tree ) ) {
				update_height();
				balance(to_this);
				return true;
			} else {
				return false;
			}
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			right_tree = new Search_tree<Type>::Node( obj );

			Search_tree<Type>::Node *next = to_this->next_node;
			// new node's previous_node points to the original_node's (to_this)
			right_tree->previous_node = to_this;
			// new node's next_node points to whatever the original node's next_node was;
			right_tree->next_node = next;
			// the previous node of to_this->next_node points to the new node 
			next->previous_node = right_tree;
			// original node (to_this) will point to new node
			to_this->next_node = right_tree;
			update_height();
			
			//once new node is added, the BST tree needs to be balanced to make it follow AVL rules
			balance(to_this);
			return true;
		} else {
			if ( right_tree->insert( obj, right_tree ) ) {
				update_height();
				//once new node is added, the BST tree needs to be balanced to make it follow AVL rules
				balance(to_this);
				return true;
			} else {
				return false;
			}
		}
	} else {
		//this case deals with duplicates
		return false;
	}
}


// ERASE FUNCTION MODIFIED TO FOLLOW AVL RULES
template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			return false;
		} else {
			if ( left_tree->erase( obj, left_tree ) ) {
				update_height();
				//once a node is erased , the BST tree needs to be balanced to make it follow AVL rules
				balance(to_this);
				return true;
			}

			return false;
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			return false;
		} else {
			if ( right_tree->erase( obj, right_tree ) ) {
				update_height();
				//once a node is erased , the BST tree needs to be balanced to make it follow AVL rules
				balance(to_this);
				return true;
			}

			return false;
		}
	} else {
		assert( obj == node_value );
		// original node's previous node
		Search_tree<Type>::Node *previous=to_this->previous_node;
		// original node's next node
		Search_tree<Type>::Node *next=to_this->next_node;
		
		// Node with nullptr as left_tree and right_tree
		if ( is_leaf() ) {
			// goal is to make a connection between to_this->previous_node and to_this->next_node so that to_this can be properly removed
			// set to_this->previous_node->next_node to point to to_this->next_node
			// set to_this->next_node->previous_node to point to to_this->previous_node
			previous->next_node = next;
			next->previous_node = previous;
			to_this = nullptr;
			delete this;
		
		// Node with nullptr as left_tree
		} else if ( left_tree == nullptr ) {
			// goal is to make a connection between to_this->previous_node and to_this->next_node so that to_this can be properly removed
			// set to_this->previous_node->next_node to point to to_this->next_node
			// set to_this->next_node->previous_node to point to to_this->previous_node
			previous->next_node = next;
			next->previous_node = previous;
			to_this = right_tree;
			delete this;
		// Node with nullptr as right_tree
		} else if ( right_tree == nullptr ) {
			// goal is to make a connection between to_this->previous_node and to_this->next_node so that to_this can be properly removed
			// set to_this->previous_node->next_node to point to to_this->next_node
			// set to_this->next_node->previous_node to point to to_this->previous_node
			previous->next_node = next;
			next->previous_node = previous;
			to_this = left_tree;
			delete this;
		} else {
			node_value = right_tree->front()->node_value;
			right_tree->erase( node_value, right_tree );
			update_height();
		}
		//once a node is erased , the BST tree needs to be balanced to make it follow AVL rules
		balance(to_this);
		return true;
	}
}

// HELPER MEMBER FUNCTIONS FOR NODE CLASS
//The main code for the helper functions were from Ahmed Fahmy's Tutorial on AVL Trees
template <typename Type>
int Search_tree<Type>::Node::balanceFactor() {
	// calculates the height of the right_tree and left_tree and gets the balanceFactor by subtracting both of the values
	// checks if right_tree and left_tree exists and if they do, calculate their height, else the value is 0
    return (right_tree ? right_tree->height() + 1 : 0) - (left_tree ? left_tree->height() + 1 : 0);
}

template <typename Type>
void Search_tree<Type>::Node::rotateL(Node*& curr_node){
	/*
		Implementation:

		- Temporarily hold the right subtree (right child node) of the parent node
		- Set parent node's right_tree to temp
		- tmp node's left tree holds the updated tree (curr_node)
		- set curr_node = tmp;
	*/

	Search_tree<Type>::Node *tmp = curr_node->right_tree;
	curr_node->right_tree = tmp->left_tree;
	tmp->left_tree = curr_node;
	curr_node = tmp;
}

template <typename Type>
void Search_tree<Type>::Node::rotateR(Node*& curr_node){
	/*
		Implementation:
		- Temporarily hold the left subtree (left child node) of the parent node
		- Set parent node's left_tree to temp node's right tree
		- tmp node's right_tree holds the updated tree (curr_node)
		- set curr_node = tmp;
	*/
	Search_tree<Type>::Node *tmp = curr_node->left_tree;
	curr_node->left_tree = tmp->right_tree;
	tmp->right_tree = curr_node;
	curr_node = tmp;
}

template <typename Type>
void Search_tree<Type>::Node::balance (Node*& curr_node){
	// update height for the current node and it will be used in balanceFactor
	curr_node->update_height();

	// Left heavy - If the left tree is unbalanced and longer
	if (curr_node->balanceFactor() < -1) {
		// Case: left-right - 2 rotations are required, first rotateLeft and then rotateRight.
		// When rotating left, check the balance factor of the left_tree's nodes (left_tree's right_tree and left_tree) 
		// make sure the balanceFactor is > 0 
		if (curr_node->left_tree->balanceFactor() > 0) {
			rotateL(curr_node->left_tree);
		}
		rotateR(curr_node);
	}

	// Right heavy - If the left tree is unbalanced and longer
	else if (curr_node->balanceFactor() > 1) {
		// Case: right-left- 2 rotations are required, first rotateRight and then rotateLeft.
		// When rotating right, check the balance factor of the right_tree's nodes (right_tree's right_tree and left_tree) 
		// make sure the balanceFactor is > 0 
		if (curr_node->right_tree->balanceFactor() < 0){
			rotateR(curr_node->right_tree);
		}
		rotateL(curr_node);
	}

	// Update the height of the right_tree if the current_node->right_tree exists
	if (curr_node->right_tree) curr_node->right_tree->update_height();
	// Update the height of the left_tree if the current_node->leftt_tree exists
	if (curr_node->left_tree) curr_node->left_tree->update_height();
	// Update the height of the current_node
	curr_node->update_height();
}



//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
containing_tree( tree ),
current_node( starting_node ) {
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing

	/* IMPLEMENTATION : 
		- if next_node node is not nullptr 
		then set current_node to hold the larger value (which is next_node)
	*/
	if (current_node->next_node != nullptr){
		current_node = current_node->next_node;
	}
	return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing

	/* IMPLEMENTATION : 
		- if previous node is not nullptr 
		then set current_node to hold the smaller value (which is previous_node)
	*/ 
	if (current_node->previous_node != nullptr){
		current_node = current_node->previous_node;
	}
	return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
