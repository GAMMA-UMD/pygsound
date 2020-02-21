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

#ifndef INCLUDE_OM_LINKED_LIST_H
#define INCLUDE_OM_LINKED_LIST_H


#include "omUtilitiesConfig.h"


#include "omAllocator.h"


//##########################################################################################
//****************************  Start Om Framework Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A doubly-linked list class.
/** 
  * This class has an interface much like that of the class ArrayList,
  * but it's implementation differs in that this class stores it's
  * array elements non-contiguously in memory, which allows for 
  * faster insertion than an array-based list if the Iterator is used.
  *		However, the class is mostly useless other than that because
  * an array-based list outperforms it in almost every other comparison.
  * Essentially, it is not recommended that one uses this class unless
  * it's quick insertion time while using the Iterator will be taken
  * advantage of.
  */
template < typename T >
class LinkedList
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new empty linked list with no elements.
			OM_INLINE LinkedList()
				:	head(NULL),
					tail(NULL),
					numElements(0)
			{
			}
			
			
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Create a deep copy of an existing linked list.
			OM_INLINE LinkedList( const LinkedList<T>& linkedList ) 
			{
				numElements = linkedList.numElements;
				
				head = util::construct<Node>( linkedList.head->data, NULL, NULL );
				
				Node* thisCurrent = head;
				Node* otherCurrent = linkedList.head->next;
				
				while ( otherCurrent )
				{
					thisCurrent->next = util::construct<Node>( otherCurrent->data, thisCurrent, NULL );
					
					thisCurrent = thisCurrent->next;
					otherCurrent = otherCurrent->next;
				}
				
				tail = thisCurrent;
			}
			
			
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Assign this linked list with the contents of another, performing a deep copy.
			OM_INLINE LinkedList<T>& operator = ( const LinkedList<T>& linkedList )
			{
				if ( this != &linkedList )
				{
					numElements = linkedList.numElements;
					
					head = util::construct<Node>( linkedList.head->data, NULL, NULL );
					
					Node* thisCurrent = head;
					Node* otherCurrent = linkedList.head->next;
					
					while ( otherCurrent )
					{
						thisCurrent->next = util::construct<Node>( otherCurrent->data, thisCurrent, NULL );
						
						thisCurrent = thisCurrent->next;
						otherCurrent = otherCurrent->next;
					}
					
					tail = thisCurrent;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a linked list, deallocating all resourced used.
			OM_INLINE ~LinkedList()
			{
				clear();
			}
			
			
		//********************************************************************************
		//******	Add Methods
			
			
			/// Add an element to the end of the list.
			/** 
			  * @param newElement - the new element to add to the end of the list
			  */
			OM_INLINE void add( const T& newElement )
			{
				if ( !tail )
					tail = head = util::construct<Node>( newElement, NULL, NULL );
				else
					tail = tail->next = util::construct<Node>( newElement, tail, NULL );
				
				numElements++;
			}
			
			
			/// Insert an element at the specified index of the list.
			/** 
			  * If the index is out of the valid indices for the linked list, the
			  * method fails and FALSE is returned. Otherwise TRUE is returned.
			  * 
			  * @param newElement - the new element to insert in the list
			  * @param index - the index at which to insert the new element
			  */
			OM_INLINE Bool insert( Index index, const T& newElement )
			{
				if ( !tail )
					tail = head = util::construct<Node>( newElement, NULL, NULL );
				else
				{
					Node* node = getNodeAtIndex( index );
					
					if ( node == NULL )
						return false;
					
					insertBefore( node );
				}
				
				return true;
			}
			
			
			/// Set an element at the specified index of the list to a new value.
			/** 
			  * If the index is out of then valid indices for the linked list, 
			  * the method fails and FALSE is returned..
			  * Otherwise, the list element at the specified index
			  * is set to a new value, and TRUE is returned.
			  * 
			  * @param index - the index at which to set the new element
			  * @param newElement - the new element to set in the list
			  */
			OM_INLINE Bool set( Index index, const T& newElement )
			{
				Node* current = getNodeAtIndex( index );
				
				if ( current == NULL )
					return false;
				
				current->data = newElement;
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the element at the specified index from this list.
			/** 
			  * If the index is within the bounds of the list,
			  * then the list element at that index is removed and the method succeeds,
			  * returning TRUE. Otherwise, the method fails and FALSE is returned.
			  * 
			  * @param index - the index of the list element to remove
			  * @return whether or not the element was removed
			  */
			OM_INLINE Bool removeAtIndex( Index index )
			{
				Node* node = getNodeAtIndex( index );
				
				return removeNode( node );
			}
			
			
			/// Remove the first element equal to the parameter element.
			/** 
			  * If this element is found, then it is removed and TRUE 
			  * is returned. Otherwise, FALSE is returned and the list is unaffected.
			  * 
			  * @param element - the list element to remove the first instance of
			  * @return whether or not the element was removed
			  */
			OM_INLINE Bool remove( T element )
			{
				Node* node = getNodeWithData( element );
				
				return removeNode( node );
			}
			
			
			/// Remove the last element in the linked list.
			/** 
			  * If the linked list has elements remaining in it, then
			  * the last element in the linked list is removed and
			  * TRUE is returned. Otherwise the list is unmodified and
			  * FALSE is returned.
			  * 
			  * @return whether or not the remove operation was successful.
			  */
			OM_INLINE Bool removeLast()
			{
				return removeNode( tail );
			}
			
			
			/// Remove the first element in the linked list.
			/** 
			  * If the linked list has elements remaining in it, then
			  * the first element in the linked list is removed and
			  * TRUE is returned. Otherwise the list is unmodified and
			  * FALSE is returned.
			  * 
			  * @return whether or not the remove operation was successful.
			  */
			OM_INLINE Bool removeFirst()
			{
				return removeNode( head );
			}
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Remove all elements from the linked list.
			OM_INLINE void clear()
			{
				Node* current = head;
				Node* previous = NULL;
				
				while ( current )
				{
					previous = current;
					current = current->next;
					util::destruct( previous );
				}
				
				head = tail = NULL;
				numElements = 0;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the linked list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the non-const version
			  * of the get() method, allowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& get( Index index )
			{
				OM_DEBUG_ASSERT_MESSAGE( index < numElements, "Linked list index out-of-bounds" );
				
				return getNodeAtIndex( index )->data;
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
			OM_INLINE const T& get( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < numElements, "Linked list index out-of-bounds" );
				
				return getNodeAtIndex( index )->data;
			}
			
			
			/// Return a reference to the first element in the array list.
			OM_INLINE T& getFirst()
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != Size(0), "Cannot get first element from empty linked list." );
				
				return head->data;
			}
			
			
			/// Return a const reference to the first element in the array list.
			OM_INLINE const T& getFirst() const
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != Size(0), "Cannot get first element from empty linked list." );
				
				return head->data;
			}
			
			
			/// Return a reference to the last element in the array list.
			OM_INLINE T& getLast()
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != Size(0), "Cannot get last element from empty linked list." );
				
				return tail->data;
			}
			
			
			/// Return a const reference to the last element in the array list.
			OM_INLINE const T& getLast() const
			{
				OM_DEBUG_ASSERT_MESSAGE( numElements != Size(0), "Cannot get last element from empty linked list." );
				
				return tail->data;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Operators
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the linked list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE T& operator () ( Index index )
			{
				OM_DEBUG_ASSERT_MESSAGE( index < numElements, "Linked list index out-of-bounds" );
				
				return getNodeAtIndex( index )->data;
			}
			
			
			/// Return the element at the specified index.
			/** 
			  * If the specified index is not within the valid bounds
			  * of the linked list, then an exception is thrown indicating
			  * an index out of bounds error occurred. This is the const version
			  * of the operator (), disallowing modification of the element.
			  * 
			  * @param index - the index of the desired element.
			  * @return a const reference to the element at the index specified by the parameter.
			  */
			OM_INLINE const T& operator () ( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < numElements, "Linked list index out-of-bounds" );
				
				return getNodeAtIndex( index )->data;
			}
			
			
			
		//********************************************************************************
		//******	Contains Method
			
			
			/// Return whether or not the specified element is in this list.
			OM_INLINE Bool contains( const T& anElement ) const
			{
				return getNodeWithData( anElement ) != NULL;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return whether or not the linked list has any elements
			/** 
			  * @return whether or not the linked list has any elements
			  */
			OM_INLINE Bool isEmpty() const
			{
				return numElements == 0;
			}
			
			
			/// Get the number of elements in the linked list.
			/** 
			  * @return the number of elements in the linked list
			  */
			OM_INLINE Size getSize() const
			{
				return numElements;
			}
			
			
		//********************************************************************************
		//******	Linked List Node Class
			
			
			/// The class for a linked list node.
			/** 
			  * This class is used to implement a doubly-linked list,
			  * such that each node has a reference to it's previous node
			  * and it's next node. This makes adding and removing elements more
			  * efficient.
			  */
			class Node
			{
				public:
					
					//****************************************************
					//	Constructors
						
						
						
						/// Create a new Node.
						/** 
						  * The user must specifiy the node data, the previous node,
						  * and the next node.
						  * 
						  * @param newData - the data for the node
						  * @param newPrevious - the previous node in the list
						  * @param newNext - the next node in the list
						  */
						OM_INLINE Node( const T& newData, Node* newPrevious, Node* newNext )
							:	data( newData ),
								previous( newPrevious ),
								next( newNext )
						{
						}
						
						
						
						
					//****************************************************
					//	Data Members
						
						
						/// The data contained by the list node.
						T data;
						
						
						/// The previous node for the list node
						Node* previous;
						
						
						/// The next node for the list node
						Node* next;
						
						
			};
			
			
		//********************************************************************************
		//******	Linked List Iterator Class
			
			
			// Forward-declare const iterator.
			class ConstIterator;
			
			
			/// Iterator class for a linked list.
			/** 
			  * It's purpose is to efficiently iterate through all
			  * or some of the elements in the linked list, making changes as
			  * necessary. It avoids the O(n) time of removing or adding elements
			  * to the list under ordinary situations.
			  */
			class Iterator
			{
				public:
					
					//****************************************************
					//	Constructor
						
						
						
						/// create a new linked list iterator from a reference to a list.
						OM_INLINE Iterator( LinkedList<T>& newList )
							:	list( newList ),
								currentNode( newList.head )
						{
						}
						
						
						
					//****************************************************
					//	Public Methods
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE
						  */
						OM_INLINE operator Bool () const
						{
							return currentNode != NULL;
						}
						
						
						
						
						/// Prefix increment operator.
						OM_INLINE void operator ++ ()
						{
							currentNode = currentNode->next;
						}
						
						
						
						
						/// Postfix increment operator.
						OM_INLINE void operator ++ ( int )
						{
							currentNode = currentNode->next;
						}
						
						
						
						
						/// Return a reference to the current iterator element.
						OM_INLINE T& operator * ()
						{
							return currentNode->data;
						}
						
						
						
						
						/// Access the current iterator element.
						OM_INLINE T* operator -> ()
						{
							return &currentNode->data;
						}
						
						
						
						
						/// Remove the current element from the list.
						/**
						  * This method removes an element in constant time.
						  */
						OM_INLINE void remove()
						{
							list.removeNode( currentNode );
						}
						
						
						
						
						/// Insert a data element before the current iterator element.
						/**
						  * This method inserts an element in constant time.
						  */
						OM_INLINE void insertBefore( const T& data )
						{
							list.insertBefore( data, currentNode );
						}
						
						
						
						
						/// Insert a data element after the current iterator element.
						/**
						  * This method inserts an element in constant time.
						  */
						OM_INLINE void insertAfter( const T& data )
						{
							list.insertAfter( data, currentNode );
						}
						
						
						
						
						/// Reset the iterator to the beginning of the list.
						OM_INLINE void reset()
						{
							currentNode = list.head;
						}
						
						
						
				private:
					
					//****************************************************
					//	Private Data Members
						
						
						/// The list we are iterating through.
						LinkedList& list;
						
						
						
						
						/// The current node of the iterator.
						Node* currentNode;
						
						
						
						
						/// Make the const iterator class a friend.
						friend class LinkedList<T>::ConstIterator;
						
						
			};
			
			
		//********************************************************************************
		//******	Const Linked List Iterator Class
			
			
			/// Iterator class for a linked list which can't modify it.
			/** 
			  * It's purpose is to efficiently iterate through all
			  * or some of the elements in the linked list, making changes as
			  * necessary. It avoids the O(n) time of removing or adding elements
			  * to the list under ordinary situations.
			  */
			class ConstIterator
			{
				public:
					
					//****************************************************
					//	Constructor
						
						
						
						/// Create a new linked list iterator from a reference to a list.
						OM_INLINE ConstIterator( const LinkedList<T>& newList )
							:	list( newList ),
								currentNode( newList.head )
						{
						}
						
						
						
						/// Create a new const linked list iterator from a non-const iterator.
						OM_INLINE ConstIterator( const Iterator& iterator )
							:	list( iterator.list ),
								currentNode( iterator.currentNode )
						{
						}
						
						
						
					//****************************************************
					//	Public Methods
						
						
						
						/// Return whether or not the iterator is at the end of the list.
						/** 
						  * If the iterator is at the end of the list, return FALSE.
						  * Otherwise, return TRUE.
						  * 
						  * @return FALSE if at the end of list, otherwise TRUE
						  */
						OM_INLINE operator Bool () const
						{
							return currentNode != NULL;
						}
						
						
						
						
						/// Prefix increment operator.
						OM_INLINE void operator ++ ()
						{
							currentNode = currentNode->next;
						}
						
						
						
						
						/// Postfix increment operator.
						OM_INLINE void operator ++ ( int )
						{
							currentNode = currentNode->next;
						}
						
						
						
						
						/// Return a reference to the current iterator element.
						OM_INLINE const T& operator * ()
						{
							return currentNode->data;
						}
						
						
						
						
						/// Access the current iterator element.
						OM_INLINE const T* operator -> ()
						{
							return &currentNode->data;
						}
						
						
						
						/// Reset the iterator to the beginning of the list.
						OM_INLINE void reset()
						{
							currentNode = list.head;
						}
						
						
						
				private:
					
					//****************************************************
					//	Private Data Members
						
						
						/// The list we are iterating through.
						const LinkedList& list;
						
						
						/// The current node of the iterator.
						const Node* currentNode;
						
						
			};
			
			
		//********************************************************************************
		//******	Iterator Method
			
			
			/// Return an iterator for the linked list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the linked list and to perform modifications
			  * to the structure without incurring O(n) overhead for inserting or
			  * removing elements.
			  * 
			  * @return an iterator for the linked list.
			  */
			OM_INLINE Iterator getIterator()
			{
				return Iterator(*this);
			}
			
			
			/// Return a const iterator for the linked list.
			/**
			  * The iterator serves to provide a way to efficiently iterate
			  * over the elements of the linked list without incurring O(n) overhead.
			  * The returned iterator cannot modify the linked list.
			  * 
			  * @return an iterator for the linked list.
			  */
			OM_INLINE ConstIterator getIterator() const
			{
				return ConstIterator(*this);
			}
			
			
	private:
		
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Get the node at the specified index.
			/** 
			  * If index is out of bounds,
			  * then return NULL. Otherwise, return a pointer to the node
			  * at the specified index.
			  * 
			  * @param index - the index of the node to retrieve
			  * @return the node at the specified index, or NULL if index is out of bounds
			  */
			OM_INLINE Node* getNodeAtIndex( Index index )
			{
				if ( index < numElements )
				{
					Node* current = head;
					Index currentIndex = 0;
					
					while ( current != NULL && currentIndex < index )
					{
						current = current->next;
						currentIndex++;
					}
					
					return current;
				}
				else
					return NULL;
			}
			
			
			/// Get the first node whose data is equal to the specified element.
			/** 
			  * If a node meets these conditions, then a pointer to it is returned.
			  * Otherwise, NULL is returned.
			  * 
			  * @param element - the element to find in the list
			  * @return the first node with data equal to the specified element
			  */
			OM_INLINE Node* getNodeWithData( const T& element )
			{
				Node* current = head;
				
				while ( current != NULL && !(current->data == element) )
					current = current->next;
				
				return current;
			}
			
			
			/// Remove the specified node from the linked list.
			OM_INLINE Bool removeNode( Node* node )
			{
				if ( !node )
					return false;
				else
				{
					if ( node == head )
					{
						// We are removing the head of the list.
						if ( node == tail )
							tail = NULL;
						
						node = node->next;
						util::destruct( head );
						head = node;
					}
					else if ( node == tail )
					{
						// We are removing the tail of the list.
						node->previous->next = NULL;
						tail = node->previous;
						util::destruct( node );
					}
					else
					{
						// We are removing from the middle of the list.
						node->previous->next = node->next;
						node->next->previous = node->previous;
						util::destruct( node );
					}
					
					numElements--;
					
					return true;
				}
			}
			
			
			/// Insert a data element before the specified node.
			/**
			  * This method inserts an element in constant time.
			  */
			OM_INLINE void insertBefore( const T& data, Node* node )
			{
				if ( !node )
				{
					head = tail = util::construct<Node>( data, NULL, NULL );
				}
				else if ( node == head )
				{
					head = util::construct<Node>( data, NULL, node );
					node->previous = head;
				}
				else
				{
					node->previous->next = util::construct<Node>( data, node->previous, node );
					node->previous = node->previous->next;
				}
				
				numElements++;
			}
			
			
			/// Insert a data element after the specified node.
			/**
			  * This method inserts an element in constant time.
			  */
			OM_INLINE void insertAfter( const T& data, Node* node )
			{
				if ( !node )
				{
					head = tail = util::construct<Node>( data, NULL, NULL );
				}
				else if ( node == tail )
				{
					tail = util::construct<Node>( data, node, NULL );
					node->previous = tail;
				}
				else
				{
					node->next->previous = util::construct<Node>( data, node, node->next );
					node->next = node->next->previous;
				}
				
				numElements++;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The head of the linked list.
			Node* head;
			
			
			/// The tail of the linked list.
			Node* tail;
			
			
			/// The number of elements in the linked list.
			Size numElements;
			
			
};




//##########################################################################################
//****************************  End Om Framework Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_LINKED_LIST_H
