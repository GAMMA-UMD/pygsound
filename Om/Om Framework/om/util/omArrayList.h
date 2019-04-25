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

#ifndef INCLUDE_OM_ARRAY_LIST_H
#define INCLUDE_OM_ARRAY_LIST_H


#include "omUtilitiesConfig.h"


#include "omAllocator.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An array-based list class.
/** 
  * This class is an unordered collection of an arbitrary
  * number of generic elements. It is array-based, making 
  * random access fast, but removing and inserting slow. It allocates
  * a contiguous block of memory for the elements which it contains.
  * The default initial capacity for the list is 8 elements, and the
  * user can specify a custom initial capacity if desired. This array
  * list implementation is not thread-safe. In threaded envirnments,
  * use the ThreadSafeArrayList
  */
template < typename T, typename SizeType = Size, typename AllocatorType = Allocator >
class ArrayList
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty array list with the default capacity of 8 elements.
			/**
			  * This is a lightweight operation and the array list doesn't allocate any
			  * memory until an element is added to the stack.
			  */
			OM_INLINE ArrayList()
				:	numElements( 0 ),
					capacity( 0 ),
					array( NULL )
			{
			}
			
			
			/// Create a new empty array list with the specified initial capacity.
			/** 
			  * The initial capacity of the array list can be set by the
			  * parameter, which can be used to pad the array list to avoid having
			  * to resize the array list often.
			  * 
			  * @param newCapacity - the initial capacity of the array list
			  */
			OM_INLINE ArrayList( SizeType newCapacity )
				:	numElements( 0 ),
					capacity( newCapacity ),
					array( AllocatorType::template allocate<T>(newCapacity) )
			{
			}
			
			
			/// Create a new array list with its internal array initialized with element from an external array.
			/** 
			  * The initial capacity and size of the array list is set to the number of
			  * elements that are to be copied from the given array.
			  * 
			  * @param elements - an array of contiguous element objects from which to initialize this array list.
			  * @param newNumElements - the number of elements to copy from the element array.
			  */
			OM_INLINE ArrayList( const T* elements, SizeType newNumElements )
				:	numElements( newNumElements ),
					capacity( newNumElements ),
					array( AllocatorType::template allocate<T>(newNumElements) )
			{
				Allocator::copyArray( array, elements, numElements );
			}
			
			
			/// Create a copy of an existing array list. This is a deep copy.
			OM_INLINE ArrayList( const ArrayList<T,SizeType>& arrayList ) 
				:	numElements( arrayList.numElements ),
					capacity( arrayList.capacity ),
					array( AllocatorType::template allocate<T>(arrayList.capacity) )
			{
				Allocator::copyArray( array, arrayList.array, numElements );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~ArrayList()
			{
				if ( array != NULL )
				{
					// Call the objects' destructors and deallocate the internal array.
					Allocator::destructArray( array, numElements );
					AllocatorType::deallocate( array );
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one array list to another, performing a deep copy.
			OM_NO_INLINE ArrayList& operator = ( const ArrayList& other );
			
			
			
		//********************************************************************************
		//******	Equality Operators
			
			
			/// Return whether or not whether every entry in this list is equal to another list's entries.
			OM_INLINE Bool operator == ( const ArrayList& other ) const
			{
				// If the arraylists point to the same data, they are equal.
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
			
			
			/// Return whether or not whether any entry in this list is not equal to another list's entries.
			OM_INLINE Bool operator != ( const ArrayList& other ) const
			{
				return !(*this == other);
			}
			
			
			
		//********************************************************************************
		//******	Add Methods
			
			
			/// Add an element to the end of the list.
			/** 
			  * If the capacity of the array list is not great enough to hold
			  * the new element, then the internal array is reallocated to be 
			  * double the size and all elements are copied to the new array.
			  * 
			  * @param newElement - the new element to add to the end of the list
			  */
			OM_INLINE void add( const T& newElement )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( newElement );
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			/** 
			  * If the capacity of the array list is not great enough to hold
			  * the new element, then the internal array is reallocated to be 
			  * double the size and all elements are copied to the new array.
			  */
			OM_INLINE void addNew()
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T();
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1 >
			OM_INLINE void addNew( const P1& p1 )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( p1 );
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2 >
			OM_INLINE void addNew( const P1& p1, const P2& p2 )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( p1, p2 );
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE void addNew( const P1& p1, const P2& p2, const P3& p3 )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( p1, p2, p3 );
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE void addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( p1, p2, p3, p4 );
				numElements++;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE void addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
			{
				if ( numElements == capacity )
					doubleCapacity();
				
				new (array + numElements) T( p1, p2, p3, p4, p5 );
				numElements++;
			}
			
			
			/// Add the contents of one ArrayList to another.
			/**
			  * This method has the effect of adding each element of
			  * the given list to the end of this array list in order.
			  * 
			  * @param list - the list to be added to the end of this list
			  */
			OM_INLINE void addAll( const ArrayList& list )
			{
				// resize the internal array if necessary.
				if ( numElements + list.numElements > capacity )
					doubleCapacityUpTo( numElements + list.numElements );
				
				Allocator::copyArray( array + numElements, list.array, list.numElements );
				
				numElements += list.numElements;
			}
			
			
			/// Add the contents of the specified array pointer to the end of the list.
			/**
			  * This method has the effect of adding each element of
			  * the given array to the end of this array list in order.
			  */
			OM_INLINE void addAll( const T* otherArray, SizeType number )
			{
				// resize the internal array if necessary.
				if ( numElements + number > capacity )
					doubleCapacityUpTo( numElements + number );
				
				Allocator::copyArray( array + numElements, otherArray, number );
				
				numElements += number;
			}
			
			
			/// Insert an element at the specified index of the list.
			/** 
			  * The method returns TRUE if the element was successfully inserted
			  * into the array list. If the index is outside of the bounds of the
			  * array list, then FALSE is returned, indicating that the element
			  * was not inserted. If needed, the array list is resized to double
			  * its current size in order to hold the new element. This method
			  * has time complexity of O(n/2) because all subsequent elements in
			  * the array list have to be moved towards the end of the list by one
			  * index.
			  * 
			  * @param newElement - the new element to insert into the array list.
			  * @param index - the index at which to insert the new element.
			  * @return whether or not the element was successfully inserted into the array list.
			  */
			Bool insert( SizeType index, const T& newElement );
			
			
		//********************************************************************************
		//******	Set Method
			
			
			/// Set an element at the specified index of the list to a new value.
			/** 
			  * This method returns TRUE if the specified index is within the bounds
			  * of the array list, indicating that the element was successfully set
			  * at that position in the array list. Otherwise, FALSE is returned, 
			  * indicating that the index was out of bounds of the array list. This
			  * method has worst-case time complexity of O(1).
			  * 
			  * @param newElement - the new element to set in the list.
			  * @param index - the index at which to set the new element.
			  * @return whether or not the element was successfully set to the new value.
			  */
			OM_INLINE Bool set( SizeType index, const T& newElement )
			{
				if ( index < numElements )
				{
					// destroy the old element.
					array[index].~T();
					
					// replace it with the new element.
					new (array + index) T(newElement);
					
					return true;
				}
				else
					return false;
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the element at the specified index, ordered version.
			/** 
			  * If the index is within the bounds of the list ( >= 0 && < getType() ),
			  * then the list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the list
			  * is unaffected. The order of the list is unaffected, meaning that 
			  * all of the elements after the removed element must be copied one 
			  * index towards the beginning of the list. This gives the method
			  * an average case performance of O(n/2) where n is the number of 
			  * elements in the array list.
			  * 
			  * @param index - the index of the list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeAtIndex( SizeType index )
			{
				if ( index < numElements )
				{
					// shift all elements forward in the array one index.
					numElements--;
					
					// Destroy the element to be removed.
					array[index].~T();
					
					// Move the objects to fill the hole in the array.
					Allocator::moveArray( array + index, array + index + 1, numElements - index );
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the element at the specified index, unordered version.
			/** 
			  * If the index is within the bounds of the list ( >= 0 && < getSizeType() ),
			  * then the list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the list is unaffected. 
			  * The order of the list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the list. This 
			  * gives the method a worst case time complexity of O(1), that is
			  * much faster than the ordered remove methods.
			  * 
			  * @param index - the index of the list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeAtIndexUnordered( SizeType index )
			{
				if ( index < numElements )
				{
					numElements--;
					
					// Destroy the element to be removed.
					T* destination = array + index;
					destination->~T();
					
					// Replace it with the last element if necessary.
					if ( index != numElements )
					{
						T* source = array + numElements;
						new (destination) T(*source);
						source->~T();
					}
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the first element equal to the parameter element, ordered version.
			/** 
			  * If this element is found, then it is removed and TRUE 
			  * is returned. Otherwise, FALSE is returned and the list is unaffected.
			  * The order of the list is unaffected, meaning that all of the elements after 
			  * the removed element must be copied one index towards the beginning 
			  * of the list. This gives the method an average case performance
			  * of O(n) where n is the number of elements in the array list. This
			  * method's complexity is worse than the ordered index remove method
			  * because it must search through the list for the element and then
			  * copy all subsequent elements one position nearer to the start of the
			  * list.
			  *
			  * @param object - the list element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool remove( const T& object )
			{
				const T* const end = array + numElements;
				
				for ( T* element = array; element != end; element++ )
				{
					if ( *element == object )
					{
						numElements--;
						
						// Destroy the element to be removed.
						element->~T();
						
						// Move the objects to fill the hole in the array.
						Allocator::moveArray( element, element + 1, end - element - 1 );
						
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove the first element equal to the parameter element, unordered version.
			/** 
			  * If this element is found, then it is removed and TRUE 
			  * is returned. Otherwise, FALSE is returned and the list is unaffected.
			  * The order of the list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the list. This 
			  * gives the method a worst case time complexity of O(n), that is
			  * much faster than the ordered remove methods (O(n^2)).
			  *
			  * @param object - the list element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeUnordered( const T& object )
			{
				const T* const end = array + numElements;
				
				for ( T* element = array; element != end; element++ )
				{
					if ( *element == object )
					{
						numElements--;
						
						// Destroy the element to be removed.
						element->~T();
						const T* last = array + numElements;
						
						// Replace it with the last element if possible.
						if ( element != last )
						{
							new (element) T(*last);
							last->~T();
						}
						
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove the last element in the array list.
			/** 
			  * If the array list has elements remaining in it, then
			  * the last element in the array list is removed and TRUE is returned.
			  * If the array list has no remaining elements, then FALSE is returned,
			  * indicating that the list was unchanged. This method has worst
			  * case O(1) time complexity.
			  * 
			  * @return whether or not the last element was successfully removed.
			  */
			OM_INLINE Bool removeLast()
			{
				if ( numElements != SizeType(0) )
				{
					numElements--;
					
					// destroy the last element.
					array[numElements].~T();
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the last N elements from the array list.
			/** 
			  * If the array list has at least N elements remaining in it, then
			  * the last N elements in the array list are removed and N is returned.
			  * If the array list has less than N elements, then the list will be
			  * completely cleared, resulting in an empty list. The method returns the
			  * number of elements successfully removed.
			  * 
			  * @return the number of elements removed from the end of the list.
			  */
			OM_INLINE SizeType removeLast( SizeType number )
			{
				number = numElements > number ? number : numElements;
				numElements -= number;
				
				// destroy the elements that were removed.
				Allocator::destructArray( array + numElements, number );
				
				return number;
			}
			
			
		//********************************************************************************
		//******	Clear Methods
			
			
			/// Clear the contents of this array list.
			/**
			  * This method calls the destructors of all elements in the array list
			  * and sets the number of elements to zero while maintaining the
			  * array's capacity.
			  */
			OM_INLINE void clear()
			{
				if ( array != NULL )
					Allocator::destructArray( array, numElements );
				
				numElements = SizeType(0);
			}
			
			
			/// Clear the contents of this array list and reclaim the allocated memory.
			/**
			  * This method performs the same function as the clear() method, but
			  * also deallocates the previously allocated internal array and reallocates
			  * it to an small initial starting size. Calling this method is equivalent
			  * to assigning a brand new array list instance to this one.
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
			
			
			/// Clear the contents of this array list and reclaim the allocated memory.
			/**
			  * This method performs the same function as the clear() method, but
			  * also deallocates the previously allocated internal array and reallocates
			  * it to a small initial starting size. Calling this method is equivalent
			  * to assigning a brand new array list instance to this one. This version of
			  * the reset() method allows the caller to specify the new starting
			  * capacity of the array list.
			  */
			OM_INLINE void reset( SizeType newCapacity )
			{
				if ( array != NULL )
				{
					Allocator::destructArray( array, numElements );
					AllocatorType::deallocate( array );
				}

				capacity = newCapacity;
				array = AllocatorType::template allocate<T>( capacity );
				numElements = SizeType(0);
			}
			
			
		//********************************************************************************
		//******	Contains Method
			
			
			/// Return whether or not the specified element is in this list.
			/**
			  * The method has average case O(n/2) time complexity, where
			  * n is the number of elements in the array list. This method
			  * is here for convenience. It just calls the array list's
			  * getSizeType() method, and tests to see if the return value is
			  * not equal to -1. It is recommended that if one wants the 
			  * index of the element as well as whether or not it is contained
			  * in the list, they should use the getSizeType() method exclusively,
			  * and check the return value to make sure that the element is in the
			  * list. This avoids the double O(n/2) lookup that would be performed
			  * one naively called this method and then that method.
			  * 
			  * @param object - the element to check to see if it is contained in the list.
			  * @return whether or not the specified element is in the list.
			  */
			OM_INLINE Bool contains( const T& object ) const
			{
				T* element = array;
				const T* const end = array + numElements;
				
				while ( element != end )
				{
					if ( *element == object )
						return true;
					
					element++;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Element Find Method
			
			
			/// Get the index of the list element equal to the parameter object.
			/** 
			  * If the specified element is not found within the list,
			  * then FALSE is returned. Otherwise, FALSE is returned and the index of
			  * the element in the list is placed in the output index parameter.
			  * 
			  * @param object - the object to find in the array list.
			  * @param index - a reference to the variable in which to place the index.
			  * @return TRUE if the object was found, FALSE otherwise.
			  */
			OM_INLINE Bool getIndex( const T& object, SizeType& index ) const
			{
				T* element = array;
				const T* const end = array + numElements;
				
				while ( element != end )
				{
					if ( *element == object )
					{
						index = element - array;
						return true;
					}
					
					element++;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the get() method, allowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& get( SizeType index )
			{
				OM_DEBUG_ASSERT_MESSAGE( index < numElements, "Cannot access invalid index in array list." );
				
				return array[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the get() method, disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& get( SizeType index ) const
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			/// Return a reference to the first element in the array list.
			OM_INLINE T& getFirst()
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *array;
			}
			
			
			/// Return a const reference to the first element in the array list.
			OM_INLINE const T& getFirst() const
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *array;
			}
			
			
			/// Return a reference to the last element in the array list.
			OM_INLINE T& getLast()
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *(array + numElements - 1);
			}
			
			
			/// Return a const reference to the last element in the array list.
			OM_INLINE const T& getLast() const
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *(array + numElements - 1);
			}
			
			
		//********************************************************************************
		//******	Element Accessor Operators
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& operator () ( SizeType index )
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& operator () ( SizeType index ) const
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& operator [] ( SizeType index )
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator [], disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& operator [] ( SizeType index ) const
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return array[index];
			}
			
			
			/// Return a const pointer to the beginning of the internal array.
			OM_INLINE const T* getPointer() const
			{
				return array;
			}
			
			
			/// Return a pointer to the beginning of the internal array.
			OM_INLINE T* getPointer()
			{
				return array;
			}
			
			
		//********************************************************************************
		//******	SizeType Accessor Methods
			
			
			/// Return whether or not the array list has any elements.
			/** 
			  * This method returns TRUE if the size of the array list
			  * is greater than zero, and FALSE otherwise.
			  * This method is here for convenience.
			  * 
			  * @return whether or not the array list has any elements.
			  */
			OM_INLINE Bool isEmpty() const
			{
				return numElements == SizeType(0);
			}
			
			
			/// Return the number of elements in the array list.
			OM_INLINE SizeType getSize() const
			{
				return numElements;
			}
			
			
			/// Return the current storage capacity of the array list.
			/**
			  * The capacity is the maximum number of elements that the
			  * array list can hold before it will have to resize its
			  * internal array.
			  * 
			  * @return the current capacity of the array list.
			  */
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
			/// Set the capacity of the array list.
			/**
			  * The capacity is the maximum number of elements that the
			  * array list can hold before it will have to resize its
			  * internal array. The capacity of the array list is set to
			  * the specified value unless this value is smaller than the
			  * number of elements in the array list. If so, the capacity
			  * remains unchanged.
			  * 
			  * @param newCapacity the desired capacity of the array list.
			  */
			OM_INLINE void setCapacity( SizeType newCapacity )
			{
				if ( newCapacity < numElements || newCapacity == 0 )
					return;
				else
					resize( newCapacity );
			}
			
			
		//********************************************************************************
		//******	Iterator Class
			
			
			// Forward-declare const iterator.
			class ConstIterator;
			
			
			/// Iterator class for an array list.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the array list, making changes as
			  * necessary to the elements.
			  */
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new ArrayList iterator from a reference to a list.
						OM_INLINE Iterator( ArrayList& newList )
							:	list( newList ),
								current( newList.array ),
								end( newList.array + newList.numElements )
						{
						}
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Prefix increment operator.
						OM_INLINE void operator ++ ()
						{
							OM_DEBUG_ASSERT_MESSAGE( current < end, "Cannot increment array list iterator past end of list." );
							
							current++;
						}
						
						
						
						
						/// Postfix increment operator.
						OM_INLINE void operator ++ ( int )
						{
							OM_DEBUG_ASSERT_MESSAGE( current < end, "Cannot increment array list iterator past end of list." );
							
							current++;
						}
						
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE, indicating that there are more
						  * elements to iterate over.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE.
						  */
						OM_INLINE operator Bool () const
						{
							return current < end;
						}
						
						
						
						
						/// Return a reference to the current iterator element.
						OM_INLINE T& operator * () const
						{
							return *current;
						}
						
						
						
						
						/// Access the current iterator element.
						OM_INLINE T* operator -> () const
						{
							return current;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtSizeType() method of the 
						  * iterated array list, and therefore has an average
						  * time complexity of O(n/2) where n is the size of the
						  * array list.
						  */
						OM_INLINE void remove()
						{
							list.removeAtIndex( getIndex() );
							current = current == list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtSizeTypeUnordered() method of the 
						  * iterated array list, and therefore has an average
						  * time complexity of O(1).
						  */
						OM_INLINE void removeUnordered()
						{
							list.removeAtIndexUnordered( getIndex() );
							current = current == list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						OM_INLINE void reset()
						{
							current = list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Return the index of the next element to be iterated over.
						OM_INLINE SizeType getIndex() const
						{
							return current - list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						ArrayList& list;
						
						
						
						
						/// Make the const iterator class a friend.
						friend class ArrayList::ConstIterator;
						
						
						
						
			};
			
			
		//********************************************************************************
		//******	ConstIterator Class
			
			
			/// An iterator class for an array list which can't modify it.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the array list.
			  */
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new ArrayList iterator from a reference to a list.
						OM_INLINE ConstIterator( const ArrayList& newList )
							:	list( newList ),
								current( newList.array ),
								end( newList.array + newList.numElements )
						{
						}
						
						
						
						/// Create a new const array list iterator from a non-const iterator.
						OM_INLINE ConstIterator( const Iterator& iterator )
							:	list( iterator.list ),
								current( iterator.current ),
								end( iterator.end )
						{
						}
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Prefix increment operator.
						OM_INLINE void operator ++ ()
						{
							current++;
						}
						
						
						
						
						/// Postfix increment operator.
						OM_INLINE void operator ++ ( int )
						{
							current++;
						}
						
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE, indicating that there are more
						  * elements to iterate over.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE.
						  */
						OM_INLINE operator Bool () const
						{
							return current < end;
						}
						
						
						
						
						/// Return a const-reference to the current iterator element.
						OM_INLINE const T& operator * () const
						{
							return *current;
						}
						
						
						
						
						/// Access the current iterator element.
						OM_INLINE const T* operator -> () const
						{
							return current;
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						OM_INLINE void reset()
						{
							current = list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Return the index of the next element to be iterated over.
						OM_INLINE SizeType getIndex() const
						{
							return current - list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						const T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						const ArrayList& list;
						
						
						
			};
			
			
		//********************************************************************************
		//******	Iterator Creation Methods
			
			
			/// Return an iterator for the array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the array list.
			  */
			OM_INLINE Iterator getIterator()
			{ 
				return Iterator(*this);
			}
			
			
			/// Return a const iterator for the array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the array list.
			  */
			OM_INLINE ConstIterator getIterator() const
			{ 
				return ConstIterator(*this);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Double the capacity of this array list's internal array of objects.
			OM_INLINE void doubleCapacity()
			{
				resize( capacity == 0 ? DEFAULT_INITIAL_CAPACITY : capacity << 1 );
			}
			
			
			/// Double the capacity of this array list's internal array until it is larger than the specified capacity.
			OM_INLINE void doubleCapacityUpTo( SizeType minimumCapacity )
			{
				SizeType newCapacity = capacity == 0 ? DEFAULT_INITIAL_CAPACITY : capacity;
				
				while ( newCapacity < minimumCapacity )
					newCapacity <<= 1;
				
				resize( newCapacity );
			}
			
			
			OM_NO_INLINE void resize( SizeType newCapacity );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the array containing all elements in the array list.
			T* array;
			
			
			/// The number of elements in the array list.
			SizeType numElements;
			
			
			/// The size of the array in this array list.
			SizeType capacity;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default initial capacity of an array list.
			/**
			  * This value is used whenever the creator of the list does
			  * not specify the initial capacity of the array list.
			  */
			static const SizeType DEFAULT_INITIAL_CAPACITY;
			
			
};




template < typename T, typename SizeType, typename AllocatorType >
const SizeType ArrayList<T,SizeType,AllocatorType>:: DEFAULT_INITIAL_CAPACITY = 8;




template < typename T, typename SizeType, typename AllocatorType >
Bool ArrayList<T,SizeType,AllocatorType>:: insert( SizeType index, const T& newElement )
{
	if ( index <= numElements )
	{
		if ( numElements == capacity )
			doubleCapacity();
		
		T* destination = array + numElements;
		const T* source = array + numElements - 1;
		const T* const sourceEnd = array + index - 1;
		
		while ( source != sourceEnd )
		{
			new (destination) T(*source);
			source->~T();
			
			source--;
			destination--;
		}
		
		new (array + index) T( newElement );
		numElements++;
		
		return true;
	}
	else
		return false;
}




template < typename T, typename SizeType, typename AllocatorType >
void ArrayList<T,SizeType,AllocatorType>:: resize( SizeType newCapacity )
{
	// Don't bother reallocating if the capacity decreases or stays the same.
	if ( newCapacity > capacity )
	{
		T* newArray = AllocatorType::template allocate<T>( newCapacity );
		
		// Copy objects from the old array if it has been allocated.
		if ( capacity > 0 )
		{
			// Move the elements from the old array to the new array.
			Allocator::moveArray( newArray, array, numElements );
			
			// Deallocate the old array.
			AllocatorType::deallocate( array );
		}
		
		array = newArray;
	}
	
	capacity = newCapacity;
}




template < typename T, typename SizeType, typename AllocatorType >
ArrayList<T,SizeType,AllocatorType>& ArrayList<T,SizeType,AllocatorType>:: operator = ( const ArrayList& other )
{
	if ( this != &other )
	{
		// Call the destructors of all objects that were previously constructed.
		if ( array != NULL )
			Allocator::destructArray( array, numElements );
		
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
			
			// copy the elements from the other ArrayList.
			Allocator::copyArray( array, other.array, numElements );
		}
	}
	
	return *this;
}




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ARRAY_LIST_H
