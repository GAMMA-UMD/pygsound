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

#ifndef INCLUDE_OM_ANY_H
#define INCLUDE_OM_ANY_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that is able to store and retrieve a value of any arbitrary type in an opaque manner.
class Any
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an Any object that has a NULL value.
			OM_INLINE Any()
				:	value( NULL )
			{
			}
			
			
			/// Create an Any object that stores a copy of the specified templated type.
			template < typename T >
			OM_INLINE Any( const T& value )
				:	value( util::construct<Storage<T> >( value ) )
			{
			}
			
			
		//********************************************************************************
		//******	Copy Constructors
			
			
			/// Make a deep copy of the specified Any object, copying its internal value.
			OM_INLINE Any( const Any& other )
				:	value( other.value ? other.value->copy() : NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operators
			
			
			/// Copy the value from another Any object into this Any, replacing the current value.
			OM_INLINE Any& operator = ( const Any& other )
			{
				if ( this != &other )
				{
					if ( value != NULL )
						util::destruct( value );
					
					other.value ? other.value->copy() : NULL;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			/// Destroy an Any object and deallocate its internal storage.
			OM_INLINE ~Any()
			{
				if ( value )
					util::destruct( value );
			}
			
			
		//********************************************************************************
		//******	Value Accessor Methods
			
			
			/// Access this Any's value and place it in the output parameter.
			/**
			  * If the Any object has a NULL value, or if the value's type does not
			  * match that of the output parameter, FALSE is returned and no value is
			  * retrieved. Otherwise, the method succeeds and TRUE is returned.
			  */
			template < typename T >
			OM_INLINE Bool getValue( T& output ) const
			{
				const T* pointer;
				
				if ( !value || !(pointer = value->getValue<T>()) )
					return false;
				
				output = *pointer;
				return true;
			}
			
			
			/// Set this Any object to have a new value.
			template < typename T >
			OM_INLINE void setValue( const T& newValue )
			{
				if ( value )
					util::destruct( value );
				
				value = util::construct< Storage<T> >( newValue );
			}
			
			
		//********************************************************************************
		//******	Value Pointer Accessor Methods
			
			
			/// Return a pointer to the value stored by this Any object.
			/**
			  * If the specified template type is not the same as the stored value,
			  * NULL is returned.
			  */
			template < typename T >
			OM_INLINE T* getPointer()
			{
				return value ? value->getValue<T>() : NULL;
			}
			
			
			/// Return a const pointer to the value stored by this Any object.
			/**
			  * If the specified template type is not the same as the stored value,
			  * NULL is returned.
			  */
			template < typename T >
			OM_INLINE const T* getPointer() const
			{
				return value ? value->getValue<T>() : NULL;
			}
			
			
		//********************************************************************************
		//******	Equality Operators
			
			
			/// Compare this Any object to another for equality.
			/**
			  * If this Any object has the same opaque type and has
			  * an equal value, the method returns TRUE. Otherwise, the
			  * method returns FALSE indicating that the objects are not equal.
			  */
			OM_INLINE Bool operator == ( const Any& other ) const
			{
				if ( value == other.value )
					return true;
				
				return value->equals( *other.value );
			}
			
			
			/// Compare this Any object to another for inequality.
			/**
			  * If this Any object has a different opaque type or has
			  * an unequal value, the method returns FALSE. Otherwise, the
			  * method returns TRUE indicating that the objects are equal.
			  */
			OM_INLINE Bool operator != ( const Any& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Any Status Accessor Methods
			
			
			/// Return whether or not this Any object's internal value is NULL.
			OM_INLINE Bool isNull() const
			{
				return value == NULL;
			}
			
			
			/// Get whether or not this Any object's internal value is not NULL.
			OM_INLINE Bool isSet() const
			{
				return value != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// The base class for classes that store objects of an arbitrary type.
			class StorageBase
			{
				public:
					
					virtual ~StorageBase()
					{
					}
					
					
					/// Construct and return a copy of this storage object, creating a new value.
					virtual StorageBase* copy() const = 0;
					
					
					/// Return whether or not this object's value is equal to another.
					virtual Bool equals( const StorageBase& other ) const = 0;
					
					
					template < typename T >
					OM_INLINE T* getValue();
					
					
					template < typename T >
					OM_INLINE const T* getValue() const;
					
					
			};
			
			
			/// A class that is used to store arbitrarily-typed values in an opaque manner.
			template < typename T >
			class Storage : public StorageBase
			{
				public:
					
					/// Create a new storage object that copies the specified value.
					OM_FORCE_INLINE Storage( const T& newValue )
						:	value( newValue )
					{
					}
					
					
					/// Destroy this storage object.
					OM_INLINE ~Storage()
					{
					}
					
					
					/// Construct and return a copy of this storage object, creating a new value.
					virtual StorageBase* copy() const
					{
						return util::construct< Storage<T> >( value );
					}
					
					
					/// Return whether or not this object's value is equal to another.
					virtual Bool equals( const StorageBase& other ) const
					{
						const Storage<T>* concreteObject = dynamic_cast<const Storage<T>*>( &other );
						
						if ( concreteObject )
							return value == concreteObject->value;
						else
							return false;
					}
					
					
					/// The actual object that is stored by an Any object.
					T value;
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the object that stores this Any's value.
			StorageBase* value;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Storage Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
T* Any::StorageBase:: getValue()
{
	Storage<T>* derived = dynamic_cast<Storage<T>*>( this );
	
	if ( derived )
		return &derived->value;
	else
		return NULL;
}




template < typename T >
const T* Any::StorageBase:: getValue() const
{
	const Storage<T>* derived = dynamic_cast<const Storage<T>*>( this );
	
	if ( derived )
		return &derived->value;
	else
		return NULL;
}




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ANY_H
