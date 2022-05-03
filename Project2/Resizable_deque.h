#ifndef DYNAMIC_DEQUE_H
#define DYNAMIC_DEQUE_H

#include "Exception.h"

template <typename Type>
class Resizable_deque {
	public:
		Resizable_deque( int = 16 );
		Resizable_deque( Resizable_deque const & );
		Resizable_deque( Resizable_deque && );
		~Resizable_deque();

		Type front() const;
		Type back() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void swap( Resizable_deque & );
		Resizable_deque &operator=( Resizable_deque const& );
		Resizable_deque &operator=( Resizable_deque && );
		void push_front( Type const & );
		void push_back( Type const & );
		void pop_front();
		void pop_back();
		void clear();

	private:
		// Your member variables
		Type *deque;  					// pointer to the deque 
		int initial_capacity; 			// holds the initial size of the deque
		int deque_capacity; 			// hold the changing size of the deque 
		int elements_count; 			// holds the number of elements in the deque
		int head;   					// holds the head of the deque
		int tail;						// holds the tail of the deque

		// Any private member functions
		//   - helper functions for resizing your deque?
		void resize_deque(const char * type);


	// Friends
	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Resizable_deque<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( int n ) :
elements_count(0),
initial_capacity(n),
deque_capacity(n),
head(0),
tail(0)
{
	/*
		- takes as an argument the initial capacity of the deque
		- allocated memory for the deque
		- initial capacity to be >= 16 and if not default size = 16
		- head and tail starts at the first index of the deque
	*/

	if (n < 16 ) initial_capacity = deque_capacity = 16 ;
	deque = new Type [deque_capacity];

}

// Copy Constructor (OPTIONAL)
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque )
// Your initalization list
{
	// Enter your implementation here
}

// Move Constructor (OPTIONAL)
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque )
// Your initalization list
{
	// Enter your implementation here
}

// Destructor
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
	delete [] deque;
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
int Resizable_deque<Type>::size() const {
	// Returns the number of elements in the deque
	return elements_count;
}

template <typename Type>
int Resizable_deque<Type>::capacity() const {
	// Returns the capacity of the deque 
	return deque_capacity;
}

template <typename Type>
bool Resizable_deque<Type>::empty() const {
	// Checks if the deque is empty or not based on the number of elements in the deque
	return elements_count == 0 ? true : false;
}

template <typename  Type>
Type Resizable_deque<Type>::front() const {
	/*
		- if deque is empty throw underflow error
		- else return the object stored at the front of the deque
		- object at the front of the deque is always at the index that head points to  (first element)
	*/
	if (empty()) throw underflow();
	else return deque[head];
}

template <typename  Type>
Type Resizable_deque<Type>::back() const {
	/*
		- if deque is empty throw underflow error
		-  else return the object stored at the back of the deque
		- object at the back of the deque is always at the index that tail points to  (last element)

	*/
	if (empty()) throw underflow();
	else return deque[tail];
}

template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) {
	// Swaps all the member variables of *this deque with those of the passed argument deque
	// std::swap( variable, deque.variable );
	std::swap( elements_count, deque.elements_count );
	std::swap( initial_capacity, deque.initial_capacity );
	std::swap( deque_capacity, deque.deque_capacity );
	std::swap( head, deque.head );
	std::swap( tail, deque.tail );
	std::swap( deque, deque.this->deque ); // idk about this
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> const &rhs ) {
	// This is done for you...
	Resizable_deque<Type> copy( rhs );
	swap( copy );

	return *this;
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> &&rhs ) {
	// This is done for you...
	swap( rhs );
	return *this;
}

template <typename Type>
void Resizable_deque<Type>::push_front( Type const &obj ) {
	/*
		Instruction: 
			- inserts obj at the front of the deque
			- if deque is filled, capacity of the deque should be doubled and element is then added to deque

		Implementation:
			- if full, resize the deque. The deque capacity needs to be doubled by calling the resize_deque function;
			- else check for three conditions
				a) if deque is empty, push element to index 0 (head = 0)
				b) if not empty and head = 0 , then head points to the last index of deque. push the element to the last index (circular array). 
			- increase the element_count by 1
	*/

	if (elements_count == deque_capacity) resize_deque("double");
	if (head == 0 && !empty()) head = capacity()-1;
	else if (head != 0) head--;
	else ;

	deque[head] = obj;
	elements_count++;
}

template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) {
	/*
		Instruction: 
			- inserts obj at the back of the deque
			- if deque is filled, capacity of the deque should be doubled and element is then added to deque

		Implementation:
			- if full, resize the deque. The deque capacity needs to be doubled;
			-  else check for three conditions
				a) if deque is empty, push element to index 0 (tail = 0)
				b) if not empty , then increment tail++ and add new element where tail points to
			- increase element_count by 1
	*/
	if (elements_count == deque_capacity) resize_deque("double");
	if (tail != 0 || (tail == 0 && !empty())) tail++;
	deque[tail] = obj;	
	elements_count++;
}

template <typename Type>
void Resizable_deque<Type>::pop_front() {
	/*
		Instruction:
		- Removes the element at the front of the deque.
		– Throws an underflow exception if the deque is empty.
		– The capacity of the deque is halved if, after the element is removed, the deque is 1/4 full or less and
			the new (halved) deque capacity is greater than or equal to the initial capacity.

		Implementation
		- if head == tail, then their is only one element left.
		- if head != tail, then : 
	 		a) check if head reached the last index, if so then set its next index back to the start of the deque
			b) if not then increment the head index
		- decrease element count by 1
	
	*/

	if (empty()) throw underflow();


	if (head != tail){
		if(head == capacity() -1) head = (head+1)%capacity();
		else head++;
	}

	elements_count--;
	if ( (size() <= (capacity()/4)) && ((capacity()/2) >= initial_capacity)) resize_deque("halved");
	
}

template <typename Type>
void Resizable_deque<Type>::pop_back() {	
	/*
		Instruction:
		- Removes the element at the front of the deque.
		– Throws an underflow exception if the deque is empty.
		– The capacity of the deque is halved if, after the element is removed, the deque is 1/4 full or less and
			the new (halved) deque capacity is greater than or equal to the initial capacity.

		Implementation
		- if head == tail, then their is only one element left.
		- if head != tail, then : 
	 		a) check if tail reached the first index, if so then set its next index back to the end of the deque
			b) if not then decrement the tail index
		- decrease element count by 1
	*/
	
	if (empty()) throw underflow();
	if (head != tail){
		if(tail == 0) tail = capacity() - 1;
		else tail--;
	}

	elements_count--;
	if ( (size() <= (capacity()/4)) && ((capacity()/2) >= initial_capacity)) resize_deque("halved");

}

template <typename Type>
void Resizable_deque<Type>::clear() {
	// Empties the deque by resetting the member variables
	// the array is then resized to the initial capacity
	elements_count = 0;
	head = 0;
	tail = 0;
	deque_capacity = initial_capacity;

	delete [] deque;
	deque = new Type [initial_capacity];
}



/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Enter any private member functions (helper functions) here
template <typename Type>
void Resizable_deque<Type>::resize_deque(const char * type){
	Type *old_deque = deque;
	int old_capacity = capacity();
	if (type == "double") deque_capacity *= 2;
	else deque_capacity = capacity()/2;

	/*
		Implementation:
			- create a new deque with the new deque size calculated (double or halved)
			- copy the elements from the old array to the new array
			- when copying the elements, copy them in the order that the deque was placed (from front to back)
			- once copied, set head to point to the first index and tail to point to the last index of the old deque
	
	*/
	
	deque = new Type [deque_capacity];
	for (int i = 0 ; i < size(); i++){
		deque[i] = old_deque[head];
		if (head == old_capacity-1) head = (head+1)%old_capacity;
		else head++;
	}
	
	head = 0;
	tail = size() - 1;
	delete [] old_deque;
}

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Resizable_deque<T> const &list ) {
	// out << "not yet implemented";
	for (int i = list.head; i < list.size(); i = (i+1)%list.capacity()){
		out << list.deque[i] ;
		out << ", ";
	}

	return out;
}

#endif
