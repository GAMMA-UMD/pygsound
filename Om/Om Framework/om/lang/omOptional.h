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

#ifndef INCLUDE_OM_OPTIONAL_H
#define INCLUDE_OM_OPTIONAL_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to store a value that may or may not be set.
/**
  * The Optional class is implemented using a pointer to a value that can be
  * optionally NULL. When setting the value of an Optional object, the provided
  * value is copy-constructed and stored internally. Accessing the contents of an
  * Optional object that does not have a value will result in an assertion being raised.
  */
template < typename T >
class Optional
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an optional object whose value is not set.
			OM_INLINE Optional()
				:	value( NULL )
			{
			}
			
			
			/// Create an optional object with the specified value.
			/**
			  * @param newValue - the value to use for this Optional object.
			  */
			OM_INLINE Optional( const T& newValue )
				:	value( util::construct<T>( newValue ) )
			{
			}
			
			
			/// Create a copy of another Optional object.
			OM_INLINE Optional( const Optional& other )
			{
				if ( other.value != NULL )
					value = util::construct<T>( *other.value );
				else
					value = NULL;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an Optional object, destroying any value stored within.
			OM_INLINE ~Optional()
			{
				if ( value != NULL )
					util::destruct( value );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the value of another Optional object to this object.
			/**
			  * This method creates a copy of the value stored in the other object.
			  * 
			  * @param other - the Optional object whose value should be copied.
			  * @return a reference to this Optional object to allow assignment chaining.
			  */
			OM_INLINE Optional& operator = ( const Optional& other )
			{
				if ( this != &other )
				{
					if ( value != NULL )
						util::destruct( value );
					
					if ( other.value != NULL )
						value = util::construct<T>( *other.value );
					else
						value = NULL;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Cast Operators
			
			
			/// Get a reference to the value contained by this Optional object.
			/**
			  * If the value is not set, a debug assertion is raised.
			  * 
			  * @return a reference to the value contained by this Optional object.
			  */
			OM_INLINE operator T& ()
			{
				OM_DEBUG_ASSERT_MESSAGE( value != NULL, "Cannot retrieve optional value that is not set." );
				
				return *value;
			}
			
			
			/// Get a const reference to the value contained by this Optional object.
			/**
			  * If the value is not set, a debug assertion is raised.
			  * 
			  * @return a const reference to the value contained by this Optional object.
			  */
			OM_INLINE operator const T& () const
			{
				OM_DEBUG_ASSERT_MESSAGE( value != NULL, "Cannot retrieve optional value that is not set." );
				
				return *value;
			}
			
			
		//********************************************************************************
		//******	Value Accessor Methods
			
			
			/// Get a reference to the value contained by this Optional object.
			/**
			  * If the value is not set, a debug assertion is raised.
			  * 
			  * @return a reference to the value contained by this Optional object.
			  */
			OM_INLINE T& get()
			{
				OM_DEBUG_ASSERT_MESSAGE( value != NULL, "Cannot retrieve optional value that is not set." );
				
				return *value;
			}
			
			
			/// Get a const reference to the value contained by this Optional object.
			/**
			  * If the value is not set, a debug assertion is raised.
			  * 
			  * @return a const reference to the value contained by this Optional object.
			  */
			OM_INLINE const T& get() const
			{
				OM_DEBUG_ASSERT_MESSAGE( value != NULL, "Cannot retrieve optional value that is not set." );
				
				return *value;
			}
			
			
			/// Set the value contained by this Optional object.
			/**
			  * This method replaces any existing value with the new value. isSet() will
			  * always return TRUE after this method exits.
			  * 
			  * @param newValue - the value to which the Optional should be set.
			  */
			OM_INLINE void set( const T& newValue )
			{
				if ( value != NULL )
					util::destruct( value );
				
				value = util::construct<T>( newValue );
			}
			
			
			
		//********************************************************************************
		//******	Value-Is-Set State Accessor Method
			
			
			/// Return whether or not the optional value is set.
			/**
			  * If the value is set, TRUE is returned. Otherwise FALSE
			  * is returned.
			  * 
			  * @return whether or not the optional value is set.
			  */
			OM_INLINE Bool isSet() const
			{
				return value != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the optional value. NULL if the value is not set.
			T* value;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Optional Class Void Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A 'void' specialization for the Optional class that is provided so that the class works for void types.
template <>
class Optional<void>
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an Optional object that stores a 'void' type.
			OM_INLINE Optional()
			{
			}
			
			
		//********************************************************************************
		//******	Value Accessor Methods
			
			
			/// Get the 'void' value stored in this Optional object.
			OM_INLINE void get()
			{
			}
			
			
			/// Get the 'void' value stored in this Optional object.
			OM_INLINE void get() const
			{
			}
			
			
		//********************************************************************************
		//******	Value-Is-Set State Accessor Method
			
			
			/// Return whether or not the 'void' value in this Optional object is set.
			/**
			  * This method always returns TRUE.
			  */
			OM_INLINE Bool isSet() const
			{
				return true;
			}
			
			
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_OPTIONAL_H
