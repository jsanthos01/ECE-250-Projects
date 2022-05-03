#ifndef DOUBLE_SENTINEL_LIST_H
#define DOUBLE_SENTINEL_LIST_H

#include <iostream>
#include "Exception.h"

template <typename Type>
class Double_sentinel_list {
	public:
		class Double_node {
			public:
				Double_node( Type const & = Type(), Double_node * = nullptr, Double_node * = nullptr );

				Type value() const;
				Double_node *previous() const;
				Double_node *next() const;

				Type         node_value;
				Double_node *previous_node;
				Double_node *next_node;
		};

		Double_sentinel_list();
		Double_sentinel_list( Double_sentinel_list const & );
		Double_sentinel_list( Double_sentinel_list && );
		~Double_sentinel_list();

		// Accessors

		int size() const;
		bool empty() const;

		Type front() const;
		Type back() const;

		Double_node *begin() const;
		Double_node *end() const;
		Double_node *rbegin() const;
		Double_node *rend() const;

		Double_node *find( Type const & ) const;
		int count( Type const & ) const;

		// Mutators

		void swap( Double_sentinel_list & );
		Double_sentinel_list &operator=( Double_sentinel_list );
		Double_sentinel_list &operator=( Double_sentinel_list && );

		void push_front( Type const & );
		void push_back( Type const & );

		void pop_front();
		void pop_back();

		int erase( Type const & );

	private:
		Double_node *list_head;
		Double_node *list_tail;
		int list_size;

		// List any additional private member functions you author here
	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Double_sentinel_list<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                      Public member functions                        //
/////////////////////////////////////////////////////////////////////////


/* 
	Constructor: 
	- Creates the two sentinels nodes (each of type Double node<Type>).
	– Initializes the head and the tail pointers to point to the head and tail nodes, respectively.
	– Initializes the sentinel nodes correctly (node value of the sentinel nodes can be anything, you may use
	the C++ default initializer, i.e. Type{})
*/

template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list():
// Updated the initialization list here
list_head( nullptr ),
list_tail( nullptr ),
list_size( 0 )
{
	list_head = new Double_node();
	list_tail = new Double_node();

	list_head->node_value = Type();
	list_tail->node_value = Type();

	list_head->next_node = list_tail;
	list_head->previous_node = nullptr;

	list_tail->next_node = nullptr;
	list_tail->previous_node = list_head;
}


template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> const &list ):
// Updated the initialization list here
list_head( nullptr ),
list_tail( nullptr ),
list_size( 0 )
{
	// Enter your implementation here
}


template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> &&list ):
// Updated the initialization list here
list_head( nullptr ),
list_tail( nullptr ),
list_size( 0 )
{
	// Enter your implementation here
}


/* Destructor - deletes each node in the list including the sentinels */
template <typename Type>
Double_sentinel_list<Type>::~Double_sentinel_list() {

	/* Pseudocode:	
		-  current pointer holds head_sentinel node
		
		2 things : 
			- check if current is null, if not
				- temp holds the current's next_node
				- current node is deleted
				- current is set to temp node
			- if null this means either the list is empty or the end of the list has reached (list_tail)
		-  loop through until every node is deleted
		-  once list is deleted then size will be 0 
	*/ 

	Double_node *current = list_head;
	Double_node * temp;

	while (current != nullptr) {
		temp = current->next();
		delete current;
		current = temp;
	}

	delete temp;
	temp = nullptr;
	list_size = 0;	
}

template <typename Type>
int Double_sentinel_list<Type>::size() const {
	/* Returns the number of items in the list */
	return list_size;
}

template <typename Type>
bool Double_sentinel_list<Type>::empty() const {
	/* Returns true if the list is empty, false otherwise. */
	if (size() > 0 ) return false;
	return true;
}

template <typename Type>
Type Double_sentinel_list<Type>::front() const {
	/*
		Instruction: 
		- Retrieves the object stored in the first/front node. - basically the first value that the head_sentinel points to 
		– Throws a underflow if the list is empty
	*/
	if (empty()) throw underflow();
	return list_head->next()->value();
}

template <typename Type>
Type Double_sentinel_list<Type>::back() const {
	/*
		– Retrieves the object stored in the last/back node.
		– Throws a underflow if the list is empty
	*/
	if (empty()) throw underflow();
	return  list_tail->previous()->value();
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::begin() const {
	// Returns the address of the first node in the list. --> returns the address that the head_sentinel holds 
	return list_head->next();
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::end() const {
	// Returns the address of the tail_sentinel 
	return list_tail;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rbegin() const {
	// Returns the address of the last_node in the list 
	return list_tail->previous();
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rend() const {
	// Returns the address of the head_sentinel 
	return list_head;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::find( Type const &obj ) const {

	/* Pseudocode:	
		- Goal is to go through the entire list and find the node with a value == to obj 
		- current point to the first non-sentinel node
		- if the value matches a node's value, return the address
		- change current to point to the next node
		- loop should stop at the last node and not reach list_tail
		- if nothing has been find return end()
	*/ 

	Double_node * current = list_head->next();
	while (current != list_tail) {
		if ((current->value()) == obj) return current;
		current = current->next();
	}
	return end();
}

template <typename Type>
int Double_sentinel_list<Type>::count( Type const &obj ) const {
	/*
		Returns the number of nodes in the linked list storing a value equal to the argument
	*/
	/* Pseudocode:	
		- Goal is to go through the entire list and count the number of nodes with a value == to obj 
		- current point to the first non-sentinel node
		- if the value matches a node's value, increase count
		- change current to point to the next node
		- loop should stop at the last node and not reach list_tail
		- return count
	*/ 

	int count = 0;
	Double_node * current = list_head->next();
	while (current != list_tail) {
		if ((current->value()) == obj) count++;
		current = current->next();
	}
	return count;
}

template <typename Type>
void Double_sentinel_list<Type>::swap( Double_sentinel_list<Type> &list ) {
	// This is done for you
	std::swap( list_head, list.list_head );
	std::swap( list_tail, list.list_tail );
	std::swap( list_size, list.list_size );
}

// The assignment operator
template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

// The move operator
template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> &&rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

template <typename Type>
void Double_sentinel_list<Type>::push_front( Type const &obj ) {
	/* Pseudocode:	
		- Goal is to inserts a node, storing new value, at the front of the list. 
		- create a new node and set its value to obj
		- temporarily store list_head->next_node which was the original first node; 
		- set new node's next_node to whatever head_sentinel was pointing to;
	 	- set new node's previous_node to head_sentinel
		- make list_head (head_sentinel) point to new_node
		- the temp_first_node's (original first node) previous should point to new_node
		- increase size of the linked list
	*/ 
	
	Double_node * temp_first_node = list_head->next();
	Double_node * new_node = new Double_node();
	new_node->node_value = obj;
	new_node->next_node = temp_first_node;
	new_node->previous_node = list_head;
	list_head->next_node = new_node;
	temp_first_node->previous_node = new_node;
	list_size++;
}

template <typename Type>
void Double_sentinel_list<Type>::push_back( Type const &obj ) {
	/* Pseudocode:	
		- Goal is to inserts a node, storing new value, at the back of the list. 
		- create a new node and set its value to obj
		- temporarily store list_tail->previous_node which was the original last node; 
		- set new node's next_node to tail_sentinel;
	 	- set new node's previous_node to whatever tail_sentinel was pointing to
		- make list_tail (tail_sentinel) previous_node point to new_node
		- the temp_last_node's (original last node) next should point to new_node
		- increase size of the linked list
	*/ 
	
	Double_node * temp_last_node = list_tail->previous();
	Double_node * new_node = new Double_node();
	new_node->node_value = obj;
	new_node->previous_node= temp_last_node;
	new_node->next_node = list_tail;
	list_tail->previous_node = new_node;
	temp_last_node->next_node = new_node;
	list_size++;
}

template <typename Type>
void Double_sentinel_list<Type>::pop_front() {

	/* Pseudocode:	
		- Goal is to remove the first (non-sentinel) node in the list. 
		if list empty throw underflow 
		else:
		- node_one - points to the first node
		- node_two - points to the second node
		- node_one (first node) will be deleted
		- node_two will become the first node, so it's previous_node needs to point to list_head (head_sentinel)
		- list_head will need to point to node_two (second non-sentinel node)
		- size of list decreases since node is deletes
	*/ 

	if(empty()){
		throw underflow();
	} else {

		Double_node *node_one = list_head->next();			 
		Double_node *node_two = list_head->next()->next();  

		node_two->previous_node = list_head;    
		list_head->next_node = node_two; 	
		
		delete node_one;
		node_one = nullptr;
		list_size--;					
	}
}

template <typename Type>
void Double_sentinel_list<Type>::pop_back() {
	/* Pseudocode:	
		- Goal is to remove the last (non-sentinel) node in the list. 
		if list empty throw underflow 
		else:
		- node_one - points to the last non-sentinel node
		- node_two - points to the second-last non-sentinel node
		- node_one (last node) will be deleted
		- since the last node is going to be deleted, set the node_two's next_node to point to tail_sentinel
		- list_tail's previous_node needs to point to node_two
		- size of list decreases since node is deletes
	*/ 
	if (empty()){
		throw underflow();
	} else {
		Double_node *node_one = list_tail->previous();    				
		Double_node *node_two = list_tail->previous()->previous(); 
		
		node_two->next_node = list_tail;							
		list_tail->previous_node = node_two; 						

		delete node_one;
		node_one = nullptr;
		list_size--;														
	} 

}

template <typename Type>
int Double_sentinel_list<Type>::erase( Type const &obj ) {
	/* Pseudocode:	
		Goal - Deletes all the nodes in the linked list that have a value equal to the argument value and Returns the number of nodes that were deleted.
		- Go through the entire linked list using a do-while loop and once it hits list_tail, stop the loop
		- while in the loop, 
			if the current node's value == obj, 
				- if so, temperarily store the previous and next nodes inside of node_before and node_after
				- node_before should point to the current_node's next 
				- node_after should point to the current_node's previous
				- delete the current node
				- current_node should point to node_after because it's the next node in line
				- list_size is decreased
			else 
				- move to the next node
	*/

	int count = 0;
	Double_node * current = list_head->next();

	do {
		if (current->value() == obj) {
			count++;

			Double_node *node_before = current->previous();
			Double_node *node_after = current->next();

			node_before->next_node = current->next();
			node_after->previous_node = current->previous();

			delete current;	
			list_size--;
			current = node_after;		
		} else {
			current = current->next();
		}
	} while (current != list_tail);

	return count;
}

template <typename Type>
Double_sentinel_list<Type>::Double_node::Double_node(
	Type const &nv,
	typename Double_sentinel_list<Type>::Double_node *pn,
	typename Double_sentinel_list<Type>::Double_node *nn ):
// Updated the initialization list here
node_value( Type() ), // This assigns 'node_value' the default value of Type
previous_node( nullptr ),
next_node( nullptr )
{
	// initialize the Node member variables 
	node_value = nv;
	previous_node = pn; 
	next_node = nn;
}

template <typename Type>
Type Double_sentinel_list<Type>::Double_node::value() const {
	// returns the value of the node
	return node_value;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::previous() const {
	// return the previous pointer
	return previous_node;
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::next() const {
	// return the next pointer
	return next_node;
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// If you author any additional private member functions, include them here

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Double_sentinel_list<T> const &list ) {
	out << "head";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.rend(); ptr != nullptr; ptr = ptr->next() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0" << std::endl << "tail";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.end(); ptr != nullptr; ptr = ptr->previous() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0";

	return out;
}

#endif
