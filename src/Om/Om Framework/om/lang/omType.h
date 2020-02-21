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

#ifndef INCLUDE_OM_TYPE_H
#define INCLUDE_OM_TYPE_H


#include <typeinfo>


#include "omLanguageConfig.h"


#include "../data/omString.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


using om::data::String;




//********************************************************************************
/// A class that represents a C++ type.
/**
  * The class consists of a string uniquely identifying the type
  * it represents. This string is retrieved from a C++ type_info
  * object. This allows a C++ type to be used as a first-class object.
  * Type objects can be created from both a statically defined type
  * via the templatized method Type::of<>() or from the dynamic type
  * of an object via a constructor or the Type::of() method. In order
  * for the dynamic type of an object to be determined, at least one class
  * in its inheritance hierarchy must have a virtual method.
  */
class Type
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty type object.
			OM_INLINE Type()
				:	typeString()
			{
			}
			
			
			/// Create a type object from the type of the parameter.
			template < typename T >
			OM_INLINE Type( const T& object )
				:	typeString( unmangle( typeid(object).name() ) )
			{
			}
			
			
		//********************************************************************************
		//******	Static Factory Methods
			
			
			/// Get a type object representing the templatized type of this method.
			template < typename T >
			OM_INLINE static Type of()
			{
				return Type( typeid(T).name() );
			}
			
			
			/// Get a type object representing the type of the parameter.
			template < typename T >
			OM_INLINE static Type of( const T& object )
			{
				return Type( object );
			}
			
			
			
		
		//********************************************************************************
		//******	Type String Accessor Methods
			
			
			/// Return a string representing the unique name of this type.
			OM_INLINE operator const String& () const
			{
				return typeString;
			}
			
			
			/// Return a string representing the unique name of this type.
			OM_INLINE const String& toString() const
			{
				return typeString;
			}
			
			
			/// Return a string representing the unique name of this type.
			OM_INLINE const String& getName() const
			{
				return typeString;
			}
			
			
		//********************************************************************************
		//******	ID Accessor Methods
			
			
			/// Return an unsigned integer for this type. Different types may have the same ID.
			OM_INLINE Hash getID() const
			{
				return typeString.getHashCode();
			}
			
			
			/// Return an integer hash code for this type.
			OM_INLINE Hash getHashCode() const
			{
				return typeString.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this type is equal to another.
			OM_INLINE Bool operator == ( const Type& type ) const
			{
				return typeString == type.typeString;
			}
			
			
			/// Return whether or not this type is not equal to another.
			OM_INLINE Bool operator != ( const Type& type ) const
			{
				return typeString != type.typeString;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a new type object from a type string and boolean attributes.
			OM_INLINE Type( const char* newTypeString )
				:	typeString( unmangle( newTypeString ) )
			{
			}
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Unmangle the specified raw type name.
			/**
			  * If the method fails, the original string may be returned.
			  */
			static String unmangle( const char* mangledName );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string representing the type, implementation defined.
			String typeString;
			
			
			
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TYPE_H
