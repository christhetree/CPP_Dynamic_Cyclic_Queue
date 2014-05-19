// Author: Christopher Mitcheltree
// Date: Fall 2013

#ifndef DYNAMIC_QUEUE_AS_ARRAY_H
#define DYNAMIC_QUEUE_AS_ARRAY_H

template <typename Type>
class Dynamic_queue_as_array {
	private:
		int initial_capacity;
		int array_capacity;
		Type *array;
		int ihead;
		int itail;
		int entry_count;
		
	public:
		Dynamic_queue_as_array( int = 10 );
		Dynamic_queue_as_array( Dynamic_queue_as_array const & );
		~Dynamic_queue_as_array();

		Type head() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void swap( Dynamic_queue_as_array & );
		Dynamic_queue_as_array &operator = ( Dynamic_queue_as_array );
		void enqueue( Type const & );
		Type dequeue();
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator << ( std::ostream &, Dynamic_queue_as_array<T> const & );
};

// Constructor: creates a new instance of the queue data structure.
template <typename Type>
Dynamic_queue_as_array<Type>::Dynamic_queue_as_array( int n ):
initial_capacity( std::max( n, 1 ) ), // Prevents a queue of size 0 from being implemented.
array_capacity( initial_capacity ),
array( new Type[initial_capacity] ),
ihead( initial_capacity - 1 ), // Highlights the cyclic nature of the queue.
itail( 0 ),
entry_count( 0 ) {
	// Empty constructor.
}

// Copy Constructor: creates a copy of the queue data structure.
template <typename Type>
Dynamic_queue_as_array<Type>::Dynamic_queue_as_array( Dynamic_queue_as_array const &queue ):
initial_capacity( queue.initial_capacity ),
array_capacity( queue.array_capacity ),
array( new Type[array_capacity] ),
ihead( queue.ihead ),
itail( queue.itail ),
entry_count( queue.entry_count ) {
	// The above initializations copy the values of the appropriate
	// member variables and allocate memory for the data structure;
	// however, you must still copy the stored objects.
	for(int i = 0; i < queue.array_capacity; ++i) { // Cycles through the entire queue and copies all the elements over to the copy of the queue.
		array[i] = queue.array[i];
	}
}

// Deconstructor: deletes the queue data structure.
template <typename Type>
Dynamic_queue_as_array<Type>::~Dynamic_queue_as_array() {
	delete [] array;
}

// Size Accessor: returns the number of entries currently in the queue.
template <typename Type>
int Dynamic_queue_as_array<Type>::size() const {
	return entry_count;
}

// Capacity Accessor: returns the current array size (i.e. capacity) of the queue.
template <typename Type>
int Dynamic_queue_as_array<Type>::capacity() const {
	return array_capacity;
}

// Empty Accessor: returns if the queue is empty or not.
template <typename Type>
bool Dynamic_queue_as_array<Type>::empty() const {
	return (entry_count == 0);
}

// Head Accessor: returns the element that is stored at the head of the queue.
template <typename Type>
Type Dynamic_queue_as_array<Type>::head() const {
	if(empty()) { // Prevents accessor from accessing an empty queue.
		throw underflow();
	}
	return array[ihead];
}

template <typename Type>
void Dynamic_queue_as_array<Type>::swap( Dynamic_queue_as_array<Type> &queue ) {
	std::swap( initial_capacity, queue.initial_capacity );
	std::swap( array_capacity, queue.array_capacity );
	std::swap( array, queue.array );
	std::swap( ihead, queue.ihead );
	std::swap( itail, queue.itail );
	std::swap( entry_count, queue.entry_count );
}

template <typename Type>
Dynamic_queue_as_array<Type> &Dynamic_queue_as_array<Type>::operator = ( Dynamic_queue_as_array<Type> rhs ) {
	swap( rhs );
	
	return *this;
}

// Enqueue Mutator: adds an entry to the end (i.e. tail) of the queue and doubles the queue array size if it is full.
template <typename Type>
void Dynamic_queue_as_array<Type>::enqueue( Type const &obj ) {
	if(empty()) {
		ihead = itail; // If there is only one entry then the head and the tail of the queue are pointing to the same entry.
	}
	else if(size() == array_capacity) { // If queue is full.
		Type *newArray = new Type[array_capacity * 2]; // Create new array with double the capacity.

		for(int i = 0; i < entry_count; ++i) {
			newArray[i] = array[(ihead + i) % (array_capacity)]; // Copy the entries of the old array into the new one while normalizing them (i.e. starting the head at index 0).
		}

		delete [] array; // Delete the old array.
		array = newArray; // Point array to the new array.
		array_capacity *= 2; // Update array capacity, ihead and itail accordingly.
		ihead = 0; // ihead is now 0 since new array has been normalized.
		itail = size(); // itail is now size() (i.e. size() - 1 + 1) since new array has been normalized.
	}
	else {
		itail = (itail + 1) % (array_capacity); // Move itail over by one index.
	}

	array[itail] = obj; // Insert entry at the tail of the queue.
	++entry_count;
}

// Dequeue Mutator: removes an entry from the head of the queue and halves the queue array size if after this operation the queue is over 75% empty.
template <typename Type>
Type Dynamic_queue_as_array<Type>::dequeue() {
	if(empty()) { // Prevents mutator from mutating an empty queue.
		throw underflow();
	}

	Type element = array[ihead]; // Store entry that is being removed.
	
	if(size() == 1) {
		// Do nothing.
	}
	else {
		ihead = (ihead + 1) % (array_capacity); // Move ihead over by one index.
	}
	--entry_count;

	if((size() <= array_capacity / 4) && (array_capacity > initial_capacity)) { // Halve queue array size if it is 75% empty and larger than the original queue array size.
		Type *newArray = new Type[array_capacity / 2]; // Create new array with half the capacity.
		for(int i = 0; i < entry_count; ++i) { 
			newArray[i] = array[(ihead + i) % (array_capacity)]; // Copy the entries of the old array into the new one while normalizing them (i.e. starting the head at index 0).
		}
		delete [] array; // Delete the old array.
		array = newArray; // Point array to the new array.
		array_capacity /= 2; // Update array capacity, ihead and itail accordingly.
		ihead = 0; // ihead is now 0 since new array has been normalized.
		itail = size() - 1; // itail is now size() - 1 since new array has been normalized.
	}
	return element; // Return entry being removed.
}

// Clear Mutator: clears the queue making it empty.
template <typename Type>
void Dynamic_queue_as_array<Type>::clear() {
	array_capacity = initial_capacity; // Reset array capacity.
	delete [] array; // Delete the queue array.
	array = new Type[initial_capacity]; // Point array to new array of initial capacity.
	ihead = initial_capacity - 1; // Reset itail, ihead and entry_count.
	itail = 0;
	entry_count = 0;
}
#endif