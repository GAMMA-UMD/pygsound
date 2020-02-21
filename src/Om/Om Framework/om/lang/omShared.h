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

#ifndef INCLUDE_OM_SHARED_H
#define INCLUDE_OM_SHARED_H


#include "omLanguageConfig.h"


#include "../threads/omAtomics.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that implements a reference-counted thread-safe shared pointer.
/**
  * A shared pointer should be used to encapsulate objects that have a lifetime
  * that is not easily known or that need to be shared by more than one system.
  * The internal pointer is owned by the shared pointer and is destructed and
  * freed when its reference count reaches 0.
  *
  * An atomic reference count is used to make the pointer thread-safe.
  */
template < class T >
class Shared
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a Shared object that is NULL.
			OM_INLINE Shared()
				:	pointer( NULL ),
					count( NULL )
			{
			}
			
			
			/// Create a Shared object that wraps the specified raw pointer.
			/**
			  * If the specified raw pointer is equal to NULL, the reference count for the
			  * pointer is set to 0. Otherwise, the reference count for the Shared is 1.
			  * By calling this constructor, the user acknowledges that the Shared object now
			  * owns the object pointed to by the raw pointer and retains the right to destroy it
			  * when the reference count reaches 0.
			  * 
			  * @param newShared - the raw pointer which this Shared object should wrap.
			  */
			OM_INLINE explicit Shared( T* newShared )
				:	pointer( newShared )
			{
				// allocate a new counter if needed
				if ( pointer != NULL )
					count = util::construct< threads::Atomic<Size> >(1);
				else
					count = NULL;
			}
			
			
			/// Create a Shared object that wraps the specified raw pointer of a different type.
			/**
			  * If the specified raw pointer is equal to NULL, the reference count for the
			  * pointer is set to 0. Otherwise, the reference count for the Shared is 1.
			  * By calling this constructor, the user acknowledges that the Shared object now
			  * owns the object pointed to by the raw pointer and retains the right to destroy it
			  * when the reference count reaches 0.
			  * 
			  * @param newShared - the raw pointer which this Shared object should wrap.
			  */
			template < typename U >
			OM_INLINE explicit Shared( U* newShared )
				:	pointer( newShared )
			{
				// allocate a new counter if needed
				if ( pointer != NULL )
					count = util::construct< threads::Atomic<Size> >(1);
				else
					count = NULL;
			}
			
			
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Create a copy of the specified Shared object, increasing its reference count by 1.
			OM_INLINE Shared( const Shared& other )
				:	pointer( other.pointer ),
					count( other.count )
			{
				if ( count != NULL )
					(*count)++;
			}
			
			
			/// Create a copy of the specified Shared object, increasing its reference count by 1.
			template < typename U >
			OM_INLINE Shared( const Shared<U>& other )
				:	pointer( other.pointer ),
					count( other.count )
			{
				if ( count != NULL )
					(*count)++;
			}
			
			
		//********************************************************************************
		//******	Assignment Operators
			
			
			/// Assign the pointer stored by another Shared object to this object.
			/**
			  * The reference count of the old pointer stored in this object is reduced by 1
			  * and the pointer is copied from the other Shared object. The reference count
			  * for the new pointer is increased by 1.
			  * 
			  * @param other - the Shared object whose pointer should be copied.
			  * @return a reference to this Shared object to allow assignment chaining.
			  */
			OM_INLINE Shared& operator = ( const Shared& other )
			{
				if ( this != &other )
				{
					this->decrementCount();
					
					pointer = other.pointer;
					count = other.count;
					
					if ( count != NULL )
						(*count)++;
				}
				
				return *this;
			}
			
			
			/// Assign the pointer stored by another Shared object of different templated type to this object.
			/**
			  * The reference count of the old pointer stored in this object is reduced by 1
			  * and the pointer is copied from the other Shared object. The reference count
			  * for the new pointer is increased by 1.
			  * 
			  * @param other - the Shared object whose pointer should be copied.
			  * @return a reference to this Shared object to allow assignment chaining.
			  */
			template < typename U >
			OM_INLINE Shared& operator = ( const Shared<U>& other )
			{
				this->decrementCount();
				
				pointer = other.pointer;
				count = other.count;
				
				if ( count != NULL )
					(*count)++;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Release this Shared's reference to the object.
			OM_INLINE ~Shared()
			{
				this->decrementCount();
			}
			
			
		//********************************************************************************
		//******	Cast Operators
			
			
			/// Cast this Shared object to a raw pointer.
			OM_INLINE operator T* () const
			{
				return pointer;
			}
			
			
			/// Cast this Shared object to a raw pointer of a different type to allow polymorphism.
			template < typename U >
			OM_INLINE operator U* () const
			{
				return pointer;
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this pointer is equal to another pointer.
			/**
			  * This method compares the pointers themselves, not the objects pointed
			  * to by the pointers.
			  * 
			  * @param other - the pointer to compare for equality.
			  * @return whether or not this pointer is equal the other.
			  */
			OM_INLINE Bool operator == ( const Shared& other ) const
			{
				return pointer == other.pointer;
			}
			
			
			/// Return whether or not this pointer is equal to another pointer.
			/**
			  * This method compares the pointers themselves, not the objects pointed
			  * to by the pointers.
			  * 
			  * @param other - the pointer to compare for equality.
			  * @return whether or not this pointer is equal the other.
			  */
			template < typename U >
			OM_INLINE Bool operator == ( const Shared<U>& other ) const
			{
				return pointer == other.pointer;
			}
			
			
		//********************************************************************************
		//******	Inequality Comparison Operators
			
			
			/// Return whether or not this pointer is not equal to another pointer.
			/**
			  * This method compares the pointers themselves, not the objects pointed
			  * to by the pointers.
			  * 
			  * @param other - the pointer to compare for inequality.
			  * @return whether or not this pointer is not equal the other.
			  */
			OM_INLINE Bool operator != ( const Shared& other ) const
			{
				return pointer != other.pointer;
			}
			
			
			/// Return whether or not this pointer is not equal to another pointer.
			/**
			  * This method compares the pointers themselves, not the objects pointed
			  * to by the pointers.
			  * 
			  * @param other - the pointer to compare for inequality.
			  * @return whether or not this pointer is not equal the other.
			  */
			template < typename U >
			OM_INLINE Bool operator != ( const Shared<U>& other ) const
			{
				return pointer != other.pointer;
			}
			
			
		//********************************************************************************
		//******	Dereference Operators
			
			
			/// Dereference the object referenced by this Shared.
			/**
			  * If the pointer is NULL, a debug assertion is raised.
			  * 
			  * @return a reference to the object that this Shared has a reference to.
			  */
			OM_INLINE T& operator * () const
			{
				OM_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot access the contents of a null smart pointer" );
				
				return *pointer;
			}
			
			
		//********************************************************************************
		//******	Shared Accessor Methods
			
			
			/// Overload the indirection operator so that this Shared object behaves like a raw pointer.
			/**
			  * If the pointer is NULL, a debug assertion is raised.
			  * 
			  * @return the raw pointer that this Shared object has a reference to.
			  */
			OM_INLINE T* operator->() const
			{
				OM_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot access the contents of a null smart pointer" );
				
				return pointer;
			}
			
			
			/// Get a raw pointer to the object that this Shared references.
			OM_INLINE T* getPointer() const
			{
				return pointer;
			}
			
			
		//********************************************************************************
		//******	Shared Trait Accessor Methods
			
			
			/// Return whether or not if the object that this Shared references is not referenced by another Shared.
			/**
			  * The method returns TRUE if the reference count for the Shared is 1 or if the pointer
			  * is equal to NULL.
			  * 
			  * @return whether or not the reference count for the Shared is equal to 1.
			  */
			OM_INLINE Bool isUnique() const
			{
				if ( count == NULL )
					return true;
				else
					return *count == Size(1);
			}
			
			
			/// Return whether or not this pointer is equal to NULL.
			OM_INLINE Bool isNull() const
			{
				return pointer == NULL;
			}
			
			
			/// Return whether or not this pointer is not equal to NULL.
			OM_INLINE Bool isSet() const
			{
				return pointer != NULL;
			}
			
			
			/// Cast this pointer to a boolean value, indicating whether or not the pointer is NULL.
			OM_INLINE operator Bool () const
			{
				return pointer != NULL;
			}
			
			
			/// Get the number of references there are to this Shared's object.
			OM_INLINE Size getReferenceCount() const
			{
				if ( count == NULL )
					return Size(0);
				else
					return *count;
			}
			
			
		//********************************************************************************
		//******	Release Method
			
			
			/// Release this pointer's reference to its object, resulting in a NULL pointer with 0 reference count.
			OM_INLINE void release()
			{
				// Decrement the count, delete if it is 0
				if ( count != NULL )
				{
					if ( --(*count) == 0 )
					{
						// Delete the pointers.
						util::destruct( pointer );
						util::deallocate( count );
					}
					
					// Reset the pointer to NULL.
					pointer = NULL;
					count = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Casting Methods
			
			
			/// Cast this pointer to the template type using static_cast.
			template < typename U >
			OM_INLINE Shared<U> cast() const
			{
				return Shared<U>( static_cast<U*>(pointer), count );
			}
			
			
			/// Cast this pointer to the template type using dynamic_cast.
			/**
			  * If the cast was invalid and not able to performed, a NULL
			  * pointer is returned.
			  */
			template < typename U >
			OM_INLINE Shared<U> dynamicCast() const
			{
				U* newShared = dynamic_cast<U*>(pointer);
				
				if ( newShared )
					return Shared<U>( newShared, count );
				else
					return Shared<U>();
			}
			
			
		//********************************************************************************
		//******	Construction Helper Methods
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			OM_INLINE static Shared construct()
			{
				return Shared( util::construct<T>() );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1 >
			OM_INLINE static Shared construct( P1 p1 )
			{
				return Shared( util::construct<T>( p1 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2 >
			OM_INLINE static Shared construct( P1 p1, P2 p2 )
			{
				return Shared( util::construct<T>( p1, p2 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE static Shared construct( P1 p1, P2 p2, P3 p3 )
			{
				return Shared( util::construct<T>( p1, p2, p3 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE static Shared construct( P1 p1, P2 p2, P3 p3, P4 p4 )
			{
				return Shared( util::construct<T>( p1, p2, p3, p4 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE static Shared construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5 )
			{
				return Shared( util::construct<T>( p1, p2, p3, p4, p5 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			OM_INLINE static Shared construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6 )
			{
				return Shared( util::construct<T>( p1, p2, p3, p4, p5, p6 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			OM_INLINE static Shared construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7 )
			{
				return Shared( util::construct<T>( p1, p2, p3, p4, p5, p6, p7 ) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a new smart pointer which directly uses the specified pointer and reference count.
			OM_INLINE Shared( T* newShared, threads::Atomic<Size>* newCount )
				:	pointer( newShared ),
					count( newCount )
			{
				(*count)++;
			}
			
			
		//********************************************************************************
		//******	Private Methods
			
			
			/// Release this Shared's reference to the object that it was pointing to.
			/**
			  * If the reference count reaches zero, the object is destroyed.
			  */
			OM_INLINE void decrementCount()
			{
				// Decrement the count, delete if it is 0
				if ( count != NULL )
				{
					if ( --(*count) == 0 )
					{
						util::destruct( pointer );
						util::deallocate( count );
					}
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the object that this Shared has a reference to.
			T* pointer;
			
			
			/// A pointer to the reference count of this Shared.
			threads::Atomic<Size>* count;
			
			
		//********************************************************************************
		//******	Private Friend Class
			
			
			/// Allow pointer types with a different template parameter to access this class's internals.
			template < typename U >
			friend class Shared;
			
			
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SHARED_H
