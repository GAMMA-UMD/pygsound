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

#include "omTensor.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Tensor:: Tensor( const TensorType& newType )
	:	type( newType )
{
	const Size typeSize = type.getSizeInBytes();
	
	// Check to see if we should allocate a buffer for the value or use the local buffer.
	if ( typeSize > LOCAL_BUFFER_SIZE )
	{
		value = util::allocate<UByte>( typeSize );
		capacityInBytes = typeSize;
	}
	else
	{
		value = localBuffer;
		capacityInBytes = LOCAL_BUFFER_SIZE;
	}
}




Tensor:: Tensor( const TensorType& newType, const UByte* valueData )
	:	type( newType )
{
	const Size typeSize = type.getSizeInBytes();
	
	// Check to see if we should allocate a buffer for the value or use the local buffer.
	if ( typeSize > LOCAL_BUFFER_SIZE )
	{
		value = util::allocate<UByte>( typeSize );
		capacityInBytes = typeSize;
	}
	else
	{
		value = localBuffer;
		capacityInBytes = LOCAL_BUFFER_SIZE;
	}
	
	if ( valueData )
		util::copyPOD( value, valueData, typeSize );
}




Tensor:: Tensor( const Tensor& other )
	:	type( other.type )
{
	if ( other.isSet() )
	{
		const Size typeSize = type.getSizeInBytes();
		
		if ( typeSize > LOCAL_BUFFER_SIZE )
		{
			value = util::allocate<UByte>( typeSize );
			capacityInBytes = typeSize;
		}
		else
		{
			value = localBuffer;
			capacityInBytes = LOCAL_BUFFER_SIZE;
		}
		
		util::copyPOD( value, other.value, typeSize );
	}
	else
	{
		value = localBuffer;
		capacityInBytes = LOCAL_BUFFER_SIZE;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Tensor& Tensor:: operator = ( const Tensor& other )
{
	if ( this != &other )
	{
		type = other.type;
		
		if ( other.isSet() )
		{
			const Size typeSize = type.getSizeInBytes();
			
			if ( typeSize > capacityInBytes )
			{
				if ( value != localBuffer )
					util::deallocate( value );
				
				value = util::allocate<UByte>( typeSize );
				capacityInBytes = typeSize;
			}
			
			util::copyPOD( value, other.value, typeSize );
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Attribute Type Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Tensor:: set( const TensorType& newType, const UByte* valueData )
{
	this->setType( newType );
	const Size typeSize = newType.getSizeInBytes();
	
	if ( valueData )
		util::copyPOD( value, valueData, typeSize );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Attribute Type Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Tensor:: setType( const TensorType& newType )
{
	const Size oldTypeSize = type.getSizeInBytes();
	const Size typeSize = newType.getSizeInBytes();
	
	// Resize the internal buffer if needed.
	if ( typeSize > capacityInBytes )
	{
		UByte* oldValue = value;
		value = util::allocate<UByte>( typeSize );
		util::copy( (UByte*)value, oldValue, oldTypeSize );
		
		if ( oldValue != localBuffer )
			util::deallocate( oldValue );
		
		capacityInBytes = typeSize;
	}
	
	type = newType;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reallocation Method
//############		
//##########################################################################################
//##########################################################################################




void Tensor:: reallocate( Size newCapacity )
{
	if ( value != localBuffer )
		util::deallocate( value );
	
	value = util::allocate<UByte>( newCapacity );
	capacityInBytes = newCapacity;
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




data::String Tensor:: toString() const
{
	switch ( type.getScalarType() )
	{
		case ScalarType::BOOLEAN:
			return convertToString( (const Bool*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::INT8:
			return convertToString( (const Byte*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::UINT8:
			return convertToString( (const UByte*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::INT16:
			return convertToString( (const Short*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::UINT16:
			return convertToString( (const UShort*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::INT32:
			return convertToString( (const Int*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::UINT32:
			return convertToString( (const UInt*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::FLOAT16:
			return convertToString( (const lang::HalfFloat*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::FLOAT32:
			return convertToString( (const Float*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::FLOAT64:
			return convertToString( (const Double*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::COMPLEX_FLOAT32:
			return convertToString( (const Float*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		case ScalarType::COMPLEX_FLOAT64:
			return convertToString( (const Double*)value, type.getSize(0), type.getSize(1), type.getSize(2) );
		default:
			return data::String("Undefined");
	}
}




template < typename T >
data::String Tensor:: convertToString( const T* scalars, Size size0, Size size1, Size size2 )
{
	if ( size2 > 1 )
	{
		// This is a 3D matrix, just print the type instead of the value.
		return TensorType( ScalarType::of<T>(), size0, size1, size2 ).toString();
	}
	else if ( size1 > 1 )
	{
		data::StringBuffer buffer( size0*size1*10 );
		const Index last0 = size0 - 1;
		const Index last1 = size1 - 1;
		
		// This is a matrix.
		for ( Index i = 0; i < size0; i++ )
		{
			buffer << '[' << ' ';
			
			for ( Index j = 0; j < size1; j++ )
			{
				buffer << scalars[j*size0 + i];
				
				if ( j != last1 )
					buffer << ',' << ' ';
			}
			
			buffer << ' ' << ']';
			
			if ( i != last0 )
				buffer << '\n';
		}
		
		return buffer.toString();
	}
	else if ( size0 > 1 )
	{
		data::StringBuffer buffer( size0*10 );
		const Index last0 = size0 - 1;
		
		// This is a vector. Print horizontally.
		buffer << '<' << ' ';
		
		for ( Index i = 0; i < size0; i++ )
		{
			buffer << scalars[i];
			
			if ( i != last0 )
				buffer << ',' << ' ';
		}
		
		buffer << ' ' << '>';
		
		return buffer.toString();
	}
	else
		return data::String( *scalars );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
