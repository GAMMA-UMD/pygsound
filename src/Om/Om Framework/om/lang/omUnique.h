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

#ifndef INCLUDE_OM_UNIQUE_H
#define INCLUDE_OM_UNIQUE_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that implements a unique pointer that has a single owner.
/**
  * A unique pointer is used to encapsulate objects that should be
  * allocated on the heap and have definite lifetime determined by a single owner.
  * The object pointed to by the unique pointer is destructed when the unique pointer is
  * destroyed. The unique pointer cannot be directly copied, since that
  * would violate the semantics. The unique pointer does not add any overhead
  * over a raw pointer.
  */
template < class T >
class Unique
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a unique pointer object that is NULL.
			OM_INLINE Unique()
				:	pointer( NULL )
			{
			}
			
			
			/// Create a unique pointer object that wraps the specified raw pointer.
			/**
			  * By calling this constructor, the user acknowledges that the Unique object now
			  * owns the object pointed to by the raw pointer and will destroy it
			  * when the unique pointer object is destroyed.
			  * 
			  * @param newPointer - the raw pointer which this Unique object should wrap.
			  */
			OM_INLINE explicit Unique( T* newPointer )
				:	pointer( newPointer )
			{
			}
			
			
			/// Create a unique pointer object that wraps the specified raw pointer of a different type.
			/**
			  * By calling this constructor, the user acknowledges that the Unique object now
			  * owns the object pointed to by the raw pointer and will destroy it
			  * when the unique pointer object is destroyed.
			  * 
			  * @param newPointer - the raw pointer which this Unique object should wrap.
			  */
			template < typename U >
			OM_INLINE explicit Unique( U* newPointer )
				:	pointer( newPointer )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Release this Unique's reference to the object.
			OM_INLINE ~Unique()
			{
				this->release();
			}
			
			
		//********************************************************************************
		//******	Cast Operators
			
			
			/// Cast this Unique object to a raw pointer.
			OM_INLINE operator T* () const
			{
				return pointer;
			}
			
			
			/// Cast this Unique object to a raw pointer of a different type to allow polymorphism.
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
			OM_INLINE Bool operator == ( const Unique& other ) const
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
			OM_INLINE Bool operator == ( const Unique<U>& other ) const
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
			OM_INLINE Bool operator != ( const Unique& other ) const
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
			OM_INLINE Bool operator != ( const Unique<U>& other ) const
			{
				return pointer != other.pointer;
			}
			
			
		//********************************************************************************
		//******	Dereference Operators
			
			
			/// Dereference the object referenced by this Unique.
			/**
			  * If the pointer is NULL, a debug assertion is raised.
			  * 
			  * @return a reference to the object that this Unique has a reference to.
			  */
			OM_INLINE T& operator * () const
			{
				OM_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot access the contents of a null pointer" );
				
				return *pointer;
			}
			
			
		//********************************************************************************
		//******	Unique Accessor Methods
			
			
			/// Overload the indirection operator so that this Unique object behaves like a raw pointer.
			/**
			  * If the pointer is NULL, a debug assertion is raised.
			  * 
			  * @return the raw pointer that this Unique object has a reference to.
			  */
			OM_INLINE T* operator->() const
			{
				OM_DEBUG_ASSERT_MESSAGE( pointer != NULL, "Cannot access the contents of a null pointer" );
				
				return pointer;
			}
			
			
			/// Return a raw pointer to the object that this unique pointer references.
			OM_INLINE T* getPointer() const
			{
				return pointer;
			}
			
			
		//********************************************************************************
		//******	Unique Trait Accessor Methods
			
			
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
			
			
		//********************************************************************************
		//******	Release Method
			
			
			/// Destruct this pointer's object, resulting in a NULL unique pointer.
			OM_INLINE void release()
			{
				if ( pointer != NULL )
				{
					util::destruct( pointer );
					pointer = NULL;
				}
			}
			
			
			/// Transfer the ownership of this unique pointer to another one.
			OM_INLINE void move( Unique<T>& unique )
			{
				// Destruct the other pointer.
				if ( unique.pointer != NULL )
					util::destruct( unique.pointer );
				
				// Copy the pointer to the other unique pointer.
				unique.pointer = pointer;
				
				// Release this unique pointer's reference to the pointer.
				pointer = NULL;
			}
			
			
		//********************************************************************************
		//******	Casting Methods
			
			
			/// Cast this pointer to the template type using static_cast.
			template < typename U >
			OM_INLINE U* cast() const
			{
				return static_cast<U*>(pointer);
			}
			
			
			/// Cast this pointer to the template type using dynamic_cast.
			/**
			  * If the cast was invalid and not able to performed, a NULL
			  * pointer is returned.
			  */
			template < typename U >
			OM_INLINE U* dynamicCast() const
			{
				return dynamic_cast<U*>(pointer);
			}
			
			
		//********************************************************************************
		//******	Construction Helper Methods
			
			/*
			/// Construct an object of the templated type with the specified arguments for its constructor.
			OM_INLINE static Unique construct()
			{
				return Unique( util::construct<T>() );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1 >
			OM_INLINE static Unique construct( P1 p1 )
			{
				return Unique( util::construct<T>( p1 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2 >
			OM_INLINE static Unique construct( P1 p1, P2 p2 )
			{
				return Unique( util::construct<T>( p1, p2 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE static Unique construct( P1 p1, P2 p2, P3 p3 )
			{
				return Unique( util::construct<T>( p1, p2, p3 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE static Unique construct( P1 p1, P2 p2, P3 p3, P4 p4 )
			{
				return Unique( util::construct<T>( p1, p2, p3, p4 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE static Unique construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5 )
			{
				return Unique( util::construct<T>( p1, p2, p3, p4, p5 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			OM_INLINE static Unique construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6 )
			{
				return Unique( util::construct<T>( p1, p2, p3, p4, p5, p6 ) );
			}
			
			
			/// Construct an object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			OM_INLINE static Unique construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7 )
			{
				return Unique( util::construct<T>( p1, p2, p3, p4, p5, p6, p7 ) );
			}
			*/
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Declared private to prevent unintended copying.
			template < typename U >
			OM_INLINE Unique( const Unique<U>& other )
			{
			}
			
			
			/// Declared private to prevent unintended copying.
			OM_INLINE Unique& operator = ( const Unique& other )
			{
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the object that this Unique has a reference to.
			T* pointer;
			
			
			
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_UNIQUE_H
