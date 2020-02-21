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

#ifndef INCLUDE_OM_STATIC_ARRAY_LIST_H
#define INCLUDE_OM_STATIC_ARRAY_LIST_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An array-based list class with a static element capacity.
/**
  * The StaticArrayList class allows basic list operations: add(), remove(),
  * insert(), clear() and getSize(). Once the static capacity of the list is
  * reached, no more elements can be added to the list.
  */
template < typename T, Size capacity, typename SizeType = Size >
class StaticArrayList
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty static array list.
			OM_INLINE StaticArrayList()
				:	numElements( 0 )
			{
			}
			
			
			/// Create a new static array list with its internal array initialized with element from an external array.
			/** 
			  * The initial size of the static array list is set to the number of
			  * elements that are to be copied from the given array. If the number of
			  * elements to be copied is larger than the static capacity of the list,
			  * the maximum possible number of elements are copied.
			  * 
			  * @param elements - an array of contiguous element objects from which to initialize this static array list.
			  * @param newNumElements - the number of elements to copy from the element array.
			  */
			OM_INLINE StaticArrayList( const T* elements, SizeType newNumElements )
				:	numElements( newNumElements < capacity ? newNumElements : capacity )
			{
				copyObjects( (T*)array, (T*)elements, numElements );
			}
			
			
			/// Create a copy of another static array list, performing a deep copy.
			OM_INLINE StaticArrayList( const StaticArrayList& other )
				:	numElements( other.numElements )
			{
				copyObjects( (T*)array, (T*)other.array, other.numElements );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this static array list, releasing all internal state.
			OM_INLINE ~StaticArrayList()
			{
				// Call the destructors of all objects that were constructed.
				callDestructors( (T*)array, numElements );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents from another static array list to this one.
			OM_INLINE StaticArrayList& operator = ( const StaticArrayList& other )
			{
				if ( this != &other )
				{
					// Call the destructors of all objects that were constructed.
					callDestructors( (T*)array, numElements );
					
					// Copy the objects from the other array.
					numElements = other.numElements;
					copyObjects( (T*)array, (T*)other.array, other.numElements );
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Equality Operators
			
			
			/// Return whether or not whether every entry in this list is equal to another list's entries.
			OM_INLINE Bool operator == ( const StaticArrayList& other ) const
			{
				if ( numElements != other.numElements )
					return false;
				
				// Do an element-wise comparison otherwise.
				const T* a = (T*)array;
				const T* b = (T*)other.array;
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
			OM_INLINE Bool operator != ( const StaticArrayList& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Add Methods
			
			
			/// Add an element to the end of the static list.
			/** 
			  * If the capacity of the static array list is not great enough to hold
			  * the new element, then FALSE is returned and the static array list is
			  * unaffected. Otherwise, the element is successfully added and TRUE
			  * is returned.
			  * 
			  * @param newElement - the new element to add to the end of the static array list.
			  * @return whether or not the element was successfully added.
			  */
			OM_INLINE Bool add( const T& newElement )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( newElement );
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			/** 
			  * If the capacity of the array list is not great enough to hold
			  * the new element, then the internal array is reallocated to be 
			  * double the size and all elements are copied to the new array.
			  */
			OM_INLINE Bool addNew()
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T();
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1 >
			OM_INLINE Bool addNew( const P1& p1 )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( p1 );
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2 >
			OM_INLINE Bool addNew( const P1& p1, const P2& p2 )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( p1, p2 );
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE Bool addNew( const P1& p1, const P2& p2, const P3& p3 )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( p1, p2, p3 );
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE Bool addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( p1, p2, p3, p4 );
				numElements++;
				return true;
			}
			
			
			/// Construct a new element at the end of the list.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE Bool addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
			{
				if ( numElements == capacity )
					return false;
				
				new ((T*)array + numElements) T( p1, p2, p3, p4, p5 );
				numElements++;
				return true;
			}
			
			
			/// Add the contents of one static array list to another.
			/**
			  * This method has the effect of adding each element of
			  * the given list to the end of this array list in order.
			  * 
			  * @param list - the list to be added to the end of this list
			  */
			template < Size otherCapacity >
			Bool addAll( const StaticArrayList<T,otherCapacity,SizeType>& list )
			{
				if ( numElements + list.numElements > capacity )
					return false;
				
				StaticArrayList::copyObjects( (T*)array + numElements, (T*)list.array, list.numElements );
				numElements += list.numElements;
				return true;
			}
			
			
			/// Insert an element at the specified index of the static array list.
			/** 
			  * The method returns TRUE if the element was successfully inserted
			  * into the static array list. If the index is outside of the bounds of the
			  * static array list, then FALSE is returned, indicating that the element
			  * was not inserted. FALSE will also be returned if there is no
			  * more room in the static array list. This method has an average case
			  * time complexity of O(n/2) because all subsequent elements in
			  * the static array list have to be moved towards the end of the array by one
			  * index.
			  * 
			  * @param newElement - the new element to insert into the static array list.
			  * @param index - the index at which to insert the new element.
			  * @return whether or not the element was successfully inserted into the static array list.
			  */
			OM_INLINE Bool insert( SizeType index, const T& newElement )
			{
				if ( index >= 0 && index <= numElements && numElements != capacity )
				{
					T* destination = (T*)array + numElements;
					const T* source = (T*)array + numElements - 1;
					const T* const sourceEnd = (T*)array + index - 1;
					
					while ( source != sourceEnd )
					{
						new (destination) T(*source);
						source->~T();
						
						source--;
						destination--;
					}
					
					new ((T*)array + index) T( newElement );
					numElements++;
					
					return true;
				}
				else
					return false;
			}
			
			
		//********************************************************************************
		//******	Set Method
			
			
			/// Set an element at the specified index of the static array list to a new value.
			/** 
			  * This method returns TRUE if the specified index is within the bounds
			  * of the static array list, indicating that the element was successfully set
			  * at that position in the static array list. Otherwise, FALSE is returned, 
			  * indicating that the index was out of bounds of the static array list. This
			  * method has worst-case time complexity of O(1).
			  * 
			  * @param newElement - the new element to set in the static array list.
			  * @param index - the index at which to set the new element.
			  * @return whether or not the element was successfully set to the new value.
			  */
			OM_INLINE Bool set( SizeType index, const T& newElement )
			{
				if ( index < numElements )
				{
					// destroy the old element.
					T* element = (T*)array + index;
					element->~T();
					
					// replace it with the new element.
					new ( element ) T(newElement);
					
					return true;
				}
				else
					return false;
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the element at the specified index, ordered version.
			/** 
			  * If the index is within the bounds of the static array list ( >= 0 && < getSize() ),
			  * then the static array list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the static array list
			  * is unaffected. The order of the static array list is unaffected, meaning that 
			  * all of the elements after the removed element must be copied one 
			  * index towards the beginning of the static array list. This gives the method
			  * an average case performance of O(n/2) where n is the number of 
			  * elements in the static array list.
			  * 
			  * @param index - the index of the static array list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeAtIndex( SizeType index )
			{
				if ( index < numElements )
				{
					// shift all elements forward in the array one index.
					numElements--;
					
					// Destroy the element to be removed.
					T* element = (T*)array + index;
					element->~T();
					
					// Move the objects to fill the hole in the array.
					StaticArrayList::moveObjects( element, element + 1, numElements - index );
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the element at the specified index, unordered version.
			/** 
			  * If the index is within the bounds of the static array list ( >= 0 && < getSize() ),
			  * then the static array list element at that index is removed and TRUE is returned,
			  * indicating that the remove operation was successful.
			  * Otherwise, FALSE is returned and the static array list is unaffected. 
			  * The order of the static array list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the static array list. This 
			  * gives the method a worst case time complexity of O(1), that is
			  * much faster than the ordered remove methods.
			  * 
			  * @param index - the index of the static array list element to remove.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeAtIndexUnordered( SizeType index )
			{
				if ( index < numElements )
				{
					numElements--;
					
					// Destroy the element to be removed.
					T* destination = (T*)array + index;
					destination->~T();
					
					// Replace it with the last element if necessary.
					if ( index != numElements )
					{
						T* source = (T*)array + numElements;
						new (destination) T(*source);
						source->~T();
					}
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the first element equal to the parameter object, ordered version.
			/** 
			  * If this element is found, then it is removed and TRUE is returned.
			  * Otherwise, FALSE is returned and the static array list is unaffected.
			  * The order of the static array list is unaffected, meaning that all of the elements after 
			  * the removed element must be copied one index towards the beginning 
			  * of the static array list. This gives the method an average case performance
			  * of O(n) where n is the number of elements in the static array list. This
			  * method's complexity is worse than the ordered index remove method
			  * because it must search through the static array list for the element and then
			  * copy all subsequent elements one position nearer to the start of the
			  * list.
			  *
			  * @param object - the element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool remove( const T& object )
			{
				const T* const end = (T*)array + numElements;
				
				for ( T* element = (T*)array; element != end; element++ )
				{
					if ( *element == object )
					{
						numElements--;
						
						// Destroy the element to be removed.
						element->~T();
						
						// Move the objects to fill the hole in the array.
						StaticArrayList::moveObjects( element, element + 1, end - element - 1 );
						
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove the first element equal to the parameter object, unordered version.
			/** 
			  * If this element is found, then it is removed and TRUE is returned. 
			  * Otherwise, FALSE is returned and the static array list is unaffected.
			  * The order of the static array list is affected when this method
			  * successfully removes the element. It works by replacing the element
			  * at the index to be removed with the last element in the static array list. This 
			  * gives the method a worst case time complexity of O(1), that is
			  * much faster than the ordered remove methods.
			  *
			  * @param object - the static array list element to remove the first instance of.
			  * @return whether or not the element was successfully removed.
			  */
			OM_INLINE Bool removeUnordered( const T& object )
			{
				const T* const end = (T*)array + numElements;
				
				for ( T* element = (T*)array; element != end; element++ )
				{
					if ( *element == object )
					{
						numElements--;
						
						// Destroy the element to be removed.
						element->~T();
						const T* last = (T*)array + numElements;
						
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
			
			
			/// Remove the last element in the static array list.
			/** 
			  * If the static array list has elements remaining in it, then
			  * the last element in the array list is removed and TRUE is returned.
			  * If the static array list has no remaining elements, then FALSE is returned,
			  * indicating that the static array list was unaffected. This method has worst
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
					((T*)array + numElements)->~T();
					
					return true;
				}
				else
					return false;
			}
			
			
			/// Remove the last N elements from the static array list.
			/** 
			  * If the static array list has at least N elements remaining in it, then
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
				ArrayList<T>::callDestructors( (T*)array + numElements, number );
				
				return number;
			}
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Clear the contents of this static array list.
			/**
			  * This method calls the destructors of all elements in the static
			  * array and sets the number of elements to zero while maintaining the
			  * array's capacity.
			  */
			OM_INLINE void clear()
			{
				StaticArrayList::callDestructors( (T*)array, numElements );
				numElements = SizeType(0);
			}
			
			
		//********************************************************************************
		//******	Contains Method
			
			
			/// Return whether or not the specified object is in this static array list.
			/**
			  * The method has average case O(n/2) time complexity, where
			  * n is the number of elements in the static array list. This method
			  * is here for convenience. It just calls the static array list's
			  * getIndex() method, and tests to see if the return value is
			  * not equal to -1. It is recommended that if one wants the 
			  * index of the element as well as whether or not it is contained
			  * in the static array list, they should use the getIndex() method exclusively,
			  * and check the return value to make sure that the element is in the
			  * static array list. This avoids the double O(n/2) lookup that would be performed
			  * one naively called this method and then that method.
			  * 
			  * @param object - the element to check to see if it is contained in the static array list.
			  * @return whether or not the specified element is in the static array list.
			  */
			OM_INLINE Bool contains( const T& object ) const
			{
				T* element = (T*)array;
				const T* const end = (T*)array + numElements;
				
				while ( element != end )
				{
					if ( *element == object )
						return true;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Element Find Method
			
			
			/// Return the index of the array element equal to the parameter element.
			/** 
			  * If the specified element is not found within the static array list,
			  * then -1 is returned. Otherwise, the index of the element 
			  * equal to the parameter is returned.
			  * 
			  * @param object - the element to find in the static array list.
			  * @param index - a reference to where the index of the found object is returned.
			  * @return whether or not the object was found in the list.
			  */
			OM_INLINE Bool getIndex( const T& object, SizeType& index ) const
			{
				T* element = (T*)array;
				const T* const end = (T*)array + numElements;
				
				while ( element != end )
				{
					if ( *element == object )
					{
						index = element - (T*)array;
						return true;
					}
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the get() method, disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& get( SizeType index ) const
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return ((T*)array)[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the get() method, allowing modification of the element via the
			  * returned non-const reference.
			  * 
			  * @param index - the index of the desired element.
			  * @return a reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& get( SizeType index )
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return ((T*)array)[index];
			}
			
			
			/// Return a reference to the first element in the static array list.
			OM_INLINE T& getFirst()
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *(T*)array;
			}
			
			
			/// Return a const reference to the first element in the static array list.
			OM_INLINE const T& getFirst() const
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *(T*)array;
			}
			
			
			/// Return a reference to the last element in the static array list.
			OM_INLINE T& getLast()
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *((T*)array + numElements - 1);
			}
			
			
			/// Return a const reference to the last element in the static array list.
			OM_INLINE const T& getLast() const
			{
				OM_DEBUG_ASSERT( numElements != SizeType(0) );
				
				return *((T*)array + numElements - 1);
			}
			
			
		//********************************************************************************
		//******	Element Accessor Operators
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& operator () ( SizeType index ) const
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return ((T*)array)[index];
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the static array list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the operator (), allowing modification of the element via the
			  * returned non-const reference.
			  * 
			  * @param index - the index of the desired element.
			  * @return a reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& operator () ( SizeType index )
			{
				OM_DEBUG_ASSERT( index < numElements );
				
				return ((T*)array)[index];
			}
			
			
			/// Get a const pointer to the first element in the static array list.
			OM_INLINE operator const T* () const
			{
				return (T*)array;
			}
			
			
			/// Get a pointer to the first element in the static array list.
			OM_INLINE operator T* ()
			{
				return (T*)array;
			}
			
			
			/// Return a const pointer to the beginning of the internal array.
			OM_INLINE const T* getPointer() const
			{
				return (T*)array;
			}
			
			
			/// Return a pointer to the beginning of the internal array.
			OM_INLINE T* getPointer()
			{
				return (T*)array;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return whether or not the static array list has any elements.
			/** 
			  * This method returns TRUE if the size of the static array list
			  * is greater than zero, and FALSE otherwise.
			  * This method is here for convenience.
			  * 
			  * @return whether or not the static array list has any elements.
			  */
			OM_INLINE Bool isEmpty() const
			{
				return numElements == 0;
			}
			
			
			/// Get the number of elements in the static array list.
			/** 
			  * @return the number of elements in the static array list.
			  */
			OM_INLINE SizeType getSize() const
			{
				return numElements;
			}
			
			
			/// Get the capacity of the static array list.
			/**
			  * The capacity is the maximum number of elements that the
			  * static array list can hold. This value does not change during
			  * the lifetime of the static array list, hence the name.
			  * 
			  * @return the current capacity of the static array list.
			  */
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
		//********************************************************************************
		//******	Iterator Class
			
			
			// Forward-declare const iterator.
			class ConstIterator;
			
			
			/// Iterator class for an static array list.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the static array list, making changes as
			  * necessary to the elements.
			  */
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new static array list iterator from a reference to a list.
						OM_INLINE Iterator( StaticArrayList& newList )
							:	list( newList ),
								current( (T*)newList.array ),
								end( (T*)newList.array + newList.numElements )
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
						
						
						
						
						/// Return a reference to the current iterator element.
						OM_INLINE T& operator * ()
						{
							return *current;
						}
						
						
						
						
						/// Access the current iterator element.
						OM_INLINE T* operator -> ()
						{
							return current;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtIndex() method of the 
						  * iterated static array list, and therefore has an average
						  * time complexity of O(n/2) where n is the size of the
						  * array list.
						  */
						OM_INLINE void remove()
						{
							list.removeAtIndex( getIndex() );
							current = current == (T*)list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method calls the removeAtIndexUnordered() method of the 
						  * iterated static array list, and therefore has an average
						  * time complexity of O(1).
						  */
						OM_INLINE void removeUnordered()
						{
							list.removeAtIndexUnordered( getIndex() );
							current = current == (T*)list.array ? current : current - 1;
							end--;
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						OM_INLINE void reset()
						{
							current = (T*)list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Get the index of the next element to be iterated over.
						OM_INLINE SizeType getIndex()
						{
							return current - (T*)list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						StaticArrayList& list;
						
						
						
						
						/// Make the const iterator class a friend.
						friend class StaticArrayList::ConstIterator;
						
						
						
			};
			
			
		//********************************************************************************
		//******	ConstIterator Class
			
			
			/// An iterator class for a static array list which can't modify it.
			/** 
			  * The purpose of this class is to iterate through all
			  * or some of the elements in the static array list.
			  */
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new static array list iterator from a reference to a list.
						OM_INLINE ConstIterator( const StaticArrayList& newList )
							:	list( newList ),
								current( (const T*)newList.array ),
								end( (const T*)newList.array + newList.numElements )
						{
						}
						
						
						
						/// Create a new const static array list iterator from a non-const iterator.
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
							current = (const T*)list.array;
							end = current + list.numElements;
						}
						
						
						
						
						/// Get the index of the next element to be iterated over.
						OM_INLINE SizeType getIndex() const
						{
							return current - (const T*)list.array;
						}
						
						
						
					
				private:
					
					//********************************************
					//	Private Data Members
						
						
						
						/// The current position of the iterator
						const T* current;
						
						
						
						
						/// A pointer to one element past the end of the list.
						const T* end;
						
						
						
						
						/// The list that is being iterated over.
						const StaticArrayList& list;
						
						
						
			};
			
			
		//********************************************************************************
		//******	Iterator Creation Methods
			
			
			/// Return an iterator for the static array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the static array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the static array list.
			  */
			OM_INLINE Iterator getIterator()
			{ 
				return Iterator(*this);
			}
			
			
			/// Return a const iterator for the static array list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the static array list. It is more useful for
			  * a linked list type of data structure, but it is provided for
			  * uniformity among data structures.
			  * 
			  * @return an iterator for the static array list.
			  */
			OM_INLINE ConstIterator getIterator() const
			{ 
				return ConstIterator(*this);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			OM_INLINE static void callDestructors( T* array, SizeType number )
			{
				const T* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					array->~T();
					array++;
				}
			}
			
			
			OM_INLINE static void copyObjects( T* destination, const T* source, SizeType number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) T(*source);
					destination++;
					source++;
				}
			}
			
			
			OM_INLINE static void moveObjects( T* destination, const T* source, SizeType number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					// copy the object from the source to destination
					new (destination) T(*source);
					
					// call the destructors on the source
					source->~T();
					
					destination++;
					source++;
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The array of bytes used to allocate memory for the list.
			UByte array[capacity*sizeof(T)];
			
			
			/// The number of valid elements in the static array list.
			SizeType numElements;
			
			
			
};




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_STATIC_ARRAY_LIST_H
