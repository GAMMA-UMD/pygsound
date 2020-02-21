/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_OM_PRIORITY_QUEUE_H
#define INCLUDE_OM_PRIORITY_QUEUE_H


#include "omUtilitiesConfig.h"


#include "omAllocator.h"


//##########################################################################################
//****************************  Start Om Framework Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a max-heap-based priority queue.
/**
  * The class uses an array-based heap where larger elements are
  * stored towards the front. When elements are added or removed
  * from the heap, internal state is kept consistent so that the 
  * first item in the queue is the largest.
  *		Element comparisons are done using the greater-than operator (>).
  * Therefore, any class that the user wishes to use with this implementation
  * must provide an overloaded greater-than operator.
  */
template < typename T, typename SizeType = Size, typename AllocatorType = Allocator >
class PriorityQueue
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty priority queue with the default capacity.
			OM_INLINE PriorityQueue()
				:	numElements( 0 ),
					capacity( 0 ),
					array( NULL )
			{
			}
			
			
			/// Create a new empty priority queue with the specified initial capacity.
			OM_INLINE PriorityQueue( Size newCapacity )
				:	numElements( 0 ),
					capacity( newCapacity ),
					array( AllocatorType::template allocate<T>(newCapacity) )
			{
			}
			
			
			/// Create a copy of an existing priority queue, performing a deep copy.
			OM_INLINE PriorityQueue( const PriorityQueue<T>& other )
				:	numElements( other.numElements ),
					capacity( other.capacity ),
					array( AllocatorType::template allocate<T>(other.capacity) )
			{
				Allocator::copyArray( array, other.array, numElements );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a priority queue.
			/**
			  * This destructor reclaims all memory previously used
			  * by this priority queue.
			  */
			OM_INLINE ~PriorityQueue()
			{
				if ( array != NULL )
				{
					// Call the destructors of all objects that were constructed and deallocate the internal array.
					Allocator::destructArray( array, numElements );
					AllocatorType::deallocate( array );
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign this priority queue with the contents of another, performing a deep copy.
			OM_INLINE PriorityQueue<T>& operator = ( const PriorityQueue<T>& other )
			{
				if ( this != &other )
				{
					if ( array != NULL )
					{
						// Call the destructors of all objects that were constructed.
						Allocator::destructArray( array, numElements );
					}
					
					numElements = other.numElements;
					
					if ( numElements == SizeType(0) )
						return *this;
					else
					{
						if ( numElements > capacity || array == NULL )
						{
							// Deallocate the internal array.
							if ( array != NULL )
								AllocatorType::deallocate( array );
							
							// Allocate a new array.
							capacity = other.capacity;
							array = AllocatorType::template allocate<T>( capacity );
						}
						
						// copy the elements from the other queue.
						Allocator::copyArray( array, other.array, numElements );
					}
				}
				
				return *this;
			}
			
			
			
		//********************************************************************************
		//******	Equality Operators
			
			
			/// Return whether or not whether every entry in this queue is equal to another queue's entries.
			OM_INLINE Bool operator == ( const PriorityQueue<T>& other ) const
			{
				// If the queues point to the same data, they are equal.
				if ( array == other.array )
					return true;
				else if ( numElements != other.numElements )
					return false;
				
				// Do an element-wise comparison otherwise.
				const T* a = array;
				const T* b = other.array;
				const T* const aEnd = a + numElements;
				
				while ( a != aEnd )
				{
					if ( !(*a == *b) )
						return false;
					
					a++;
					b++;
				}
				
				return true;
			}
			
			
			OM_INLINE Bool operator != ( const PriorityQueue<T>& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Add Methods
			
			
			/// Add a new element to this priority queue.
			OM_INLINE void add( const T& newElement )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				SizeType i = numElements;
				SizeType parent = getParentIndex(i);
				
				new (array + numElements) T( newElement );
				numElements++;
				
				// reorder the queue's heap so that the new element is in the correct place.
				while ( array[parent] < array[i] )
				{
					T temp = array[parent];
					array[parent] = array[i];
					array[i] = temp;
					
					i = parent;
					parent = getParentIndex(i);
				}
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the largest element from the priority queue.
			OM_INLINE void remove()
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != SizeType(0), "Cannot remove element from an empty priority queue." );
				
				// Decrement the number of elements in the queue.
				numElements--;
				
				// Copy the last element in the queue's array into the largest element's location.
				array[0] = array[numElements];
				
				// Call the destructor for the old last element.
				array[numElements].~T();
				
				// Convert the new array into a heap, starting at the first element.
				this->heapify( 0 );
			}
			
			
			/// Remove the element with the specified value from the priority queue.
			/**
			  * The method returns whether or not the value was able to be removed.
			  */
			OM_INLINE Bool remove( const T& element )
			{
				SizeType i;
				
				if ( !getIndexRecursive( element, 0, i ) )
					return false;
				
				// Decrement the number of elements in the queue.
				numElements--;
				
				// Copy the last element in the queue's array into the largest element's location.
				array[i] = array[numElements];
				
				// Call the destructor for the old last element.
				array[numElements].~T();
				
				// Convert the new array into a heap, starting at the removed element.
				this->heapify( i );
				
				return true;
			}
			
			
			/// Remove the element at the specified index from the priority queue.
			/**
			  * Indices start at 0 for the largest element in the queue and must be
			  * less than the number of elements in the queue.
			  */
			OM_INLINE void removeAtIndex( SizeType i )
			{
				OM_DEBUG_ASSERT_MESSAGE( i < numElements, "Cannot remove element at invalid index in priority queue." );
				
				// Decrement the number of elements in the queue.
				numElements--;
				
				// Copy the last element in the queue's array into the largest element's location.
				array[i] = array[numElements];
				
				// Call the destructor for the old last element.
				array[numElements].~T();
				
				// Convert the new array into a heap, starting at the first element.
				this->heapify( i );
			}
			
			
		//********************************************************************************
		//******	Update Methods
			
			
			/// Ensure that the heap is propery ordered after the specified element was changed.
			OM_INLINE void update( SizeType i )
			{
				if ( i > 0 )
				{
					SizeType parent = getParentIndex(i);
					
					// Did the entry increase its value?
					if ( array[parent] < array[i] )
					{
						do
						{
							T temp = array[parent];
							array[parent] = array[i];
							array[i] = temp;
							
							i = parent;
							parent = getParentIndex(i);
						}
						while ( i > 0 && array[parent] < array[i] );
						
						return;
					}
				}
				
				this->heapify( i );
			}
			
			
		//********************************************************************************
		//******	Index Accessor Methods
			
			
			/// Get the index of the specified value in this priority queue.
			/**
			  * The method returns whether or not the given value was contained
			  * in the queue.
			  */
			OM_INLINE Bool getIndex( const T& value, SizeType& index ) const
			{
				return getIndexRecursive( value, 0, index );
			}
			
			
		//********************************************************************************
		//******	Contains Method
			
			
			/// Return whether or not the specified element is in the priority queue.
			OM_INLINE Bool contains( const T& element ) const
			{
				return containsRecursive( element, 0 );
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return a reference to the largest element in the priority queue.
			OM_INLINE T& getFirst()
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != SizeType(0), "Cannot get first element from empty priority queue" );
				
				return *array;
			}
			
			
			/// Return a const reference to the largest element in the priority queue.
			OM_INLINE const T& getFirst() const
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != SizeType(0), "Cannot get first element from empty priority queue" );
				
				return *array;
			}
			
			
			/// Return a reference to the element in this priority queue at the specified index.
			OM_INLINE T& operator [] ( SizeType i )
			{
				OM_DEBUG_ASSERT_MESSAGE( i < numElements, "Cannot get invalid element from priority queue" );
				
				return array[i];
			}
			
			
			/// Return a const reference to the element in this priority queue at the specified index.
			OM_INLINE const T& operator [] ( SizeType i ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( i < numElements, "Cannot get invalid element from priority queue" );
				
				return array[i];
			}
			
			
		//********************************************************************************
		//******	Clear Methods
			
			
			/// Clear the contents of this priority queue.
			/**
			  * This method calls the destructors of all elements in the priority queue
			  * and sets the number of elements to zero while maintaining the
			  * queue's capacity.
			  */
			OM_INLINE void clear()
			{
				if ( array != NULL )
					Allocator::destructArray( array, numElements );
				
				numElements = SizeType(0);
			}
			
			
			/// Clear the contents of this priority queue and reclaim the allocated memory.
			/**
			  * This method performs the same function as the clear() method, but
			  * also deallocates the previously allocated internal array. Calling this
			  * method is equivalent to assigning a new queue instance to this one.
			  */
			OM_INLINE void reset()
			{
				if ( array != NULL )
				{
					Allocator::destructArray( array, numElements );
					AllocatorType::deallocate( array );
				}
				
				capacity = SizeType(0);
				array = NULL;
				numElements = SizeType(0);
			}
			
			
			/// Clear the contents of this priority queue and reclaim the allocated memory.
			/**
			  * This method performs the same function as the clear() method, but
			  * also deallocates the previously allocated internal array and reallocates
			  * it to a small initial starting size. Calling this method is equivalent
			  * to assigning a new priority queue instance to this one. This version of
			  * the reset() method allows the caller to specify the new starting
			  * capacity of the queue.
			  */
			OM_INLINE void reset( SizeType newCapacity )
			{
				if ( array != NULL )
				{
					Allocator::destructArray( array, numElements );
					util::deallocate( array );
				}
				
				capacity = newCapacity;
				array = util::allocate<T>( capacity );
				numElements = SizeType(0);
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return whether or not the priority queue has any elements.
			/** 
			  * This method returns TRUE if the size of the priority queue
			  * is greater than zero, and FALSE otherwise.
			  * 
			  * @return whether or not the priority queue has any elements.
			  */
			OM_INLINE Bool isEmpty() const
			{
				return numElements == SizeType(0);
			}
			
			
			/// Get the number of elements in the priority queue.
			/** 
			  * @return the number of elements in the priority queue.
			  */
			OM_INLINE SizeType getSize() const
			{
				return numElements;
			}
			
			
			/// Get the current capacity of the priority queue.
			/**
			  * The capacity is the maximum number of elements that the
			  * priority queue can hold before it will have to resize its
			  * internal array.
			  * 
			  * @return the current capacity of the priority queue.
			  */
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
		//********************************************************************************
		//******	Parent/Child Index Methods
			
			
			/// Return the index of a child elements's parent element given the child element's index.
			/**
			  * If the child index is zero, the method returns 0 because this child element is
			  * at the top of the heap and has no parent by definition.
			  */
			OM_INLINE static SizeType getParentIndex( SizeType child )
			{
				if ( child == SizeType(0) )
					return SizeType(0);
				
				return ((child - SizeType(1)) >> 1);
			}
			
			
			/// Return the index of the left child element given the parent element's index.
			OM_INLINE static SizeType getChildIndex1( SizeType parent )
			{
				return (parent << 1) + SizeType(1);
			}
			
			
			/// Return the index of the right child element given the parent element's index.
			OM_INLINE static SizeType getChildIndex2( SizeType parent )
			{
				return (parent << 1) + SizeType(2);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Heap Methods
			
			
			/// Recursively query the priority queue to find the index of a value.
			/**
			  * This method has O(log n) complexity due to the priority queue's
			  * internal heap data structure.
			  */
			OM_INLINE Bool getIndexRecursive( const T& value, SizeType i, SizeType& index ) const
			{
				if ( i >= numElements || array[i] < value )
					return false;
				else if ( array[i] == value )
				{
					index = i;
					return true;
				}
				
				if ( getIndexRecursive( value, getChildIndex1(i), index ) ||
					getIndexRecursive( value, getChildIndex2(i), index ) )
					return true;
				
				return false;
			}
			
			
			/// Recursively query the priorit queue to see if a value is contained.
			/**
			  * This method has O(log n) complexity due to the priority queue's
			  * internal heap data structure.
			  */
			OM_INLINE Bool containsRecursive( const T& value, SizeType i ) const
			{
				if ( i >= numElements || array[i] < value )
					return false;
				else if ( array[i] == value )
					return true;
				
				if ( containsRecursive( value, getChildIndex1(i) ) ||
					containsRecursive( value, getChildIndex2(i) ) )
					return true;
				
				return false;
			}
			
			
			/// Convert the specified sub-heap, with root at index i, into a heap.
			OM_INLINE void heapify( SizeType i )
			{
				while ( i < numElements )
				{
					SizeType childIndex1 = getChildIndex1(i);
					SizeType childIndex2 = getChildIndex2(i);
					SizeType max;
					
					if ( childIndex1 < numElements && array[i] < array[childIndex1] )
						max = childIndex1;
					else
						max = i;
					
					if ( childIndex2 < numElements && array[max] < array[childIndex2] )
						max = childIndex2;
					
					if ( max == i )
						break;
					
					T temp = array[max];
					array[max] = array[i];
					array[i] = temp;
					i = max;
				}
			}
			
			
		//********************************************************************************
		//******	Private Array-Management Methods
			
			
			OM_INLINE void doubleCapacity()
			{
				// check to see if the array has not yet been allocated.
				if ( capacity == 0 )
				{
					// allocate the array to hold elements.
					capacity = DEFAULT_INITIAL_CAPACITY;
					array = AllocatorType::template allocate<T>( capacity );
				}
				else
					resize( capacity << 1 );
			}
			
			
			/// Double the size of the element array to increase the capacity of the priority queue.
			/**
			  * This method deallocates the previously used array, and then allocates
			  * a new block of memory with a size equal to double the previous size.
			  * It then copies over all of the previous elements into the new array.
			  */
			void resize( SizeType newCapacity )
			{
				// Update the capacity and allocate a new array.
				capacity = newCapacity;
				T* oldArray = array;
				array = AllocatorType::template allocate<T>( capacity );
				
				// Dopy the elements from the old array to the new array.
				Allocator::moveArray( array, oldArray, numElements );
				
				// Deallocate the old array.
				AllocatorType::deallocate( oldArray );
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the array containing all elements in the queue.
			/**
			  * Data is stored in this array using an array-based max-heap representation.
			  */
			T* array;
			
			
			/// The number of elements in the priority queue.
			SizeType numElements;
			
			
			/// The number of possible elements that the current queue could hold without resizing.
			SizeType capacity;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default initial capacity of a priority queue.
			/**
			  * This value is used whenever the creator of the queue does
			  * not specify the initial capacity of the priority queue.
			  */
			static const SizeType DEFAULT_INITIAL_CAPACITY = 8;
			
			
};




//##########################################################################################
//****************************  End Om Framework Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_PRIORITY_QUEUE_H
