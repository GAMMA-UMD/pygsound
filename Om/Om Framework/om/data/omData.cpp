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

#include "omData.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Data:: Data( const UByte* newData, Size number )
	:	data( util::allocate<UByte>(number) ),
		size( number ),
		referenceCount( util::construct< threads::Atomic<Size> >(1) ),
		haveCalculatedHash( false )
{
	util::copy( data, newData, number );
}




Data:: Data( const util::Array<UByte>& array )
	:	data( util::allocate<UByte>(array.getSize()) ),
		size( array.getSize() ),
		referenceCount( util::construct< threads::Atomic<Size> >(1) ),
		haveCalculatedHash( false )
{
	util::copy( data, array.getPointer(), array.getSize() );
}




Data:: Data( const util::Array<UByte>& array, Size number )
	:	size( math::min( array.getSize(), number ) ),
		referenceCount( util::construct< threads::Atomic<Size> >(1) ),
		haveCalculatedHash( false )
{
	data = util::allocate<UByte>(size);
	util::copy( data, array.getPointer(), size );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Data:: ~Data()
{
	if ( referenceCount != NULL )
	{
		if ( *referenceCount > 1 )
			(*referenceCount)--;
		else
		{
			util::deallocate( data );
			util::deallocate( referenceCount );
		}
	}
}





//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Data& Data:: operator = ( const Data& other )
{
	if ( this != &other )
	{
		if ( referenceCount != NULL )
		{
			if ( *referenceCount > 1 )
				(*referenceCount)--;
			else
			{
				util::deallocate( data );
				util::deallocate( referenceCount );
			}
		}
		
		data = other.data;
		size = other.size;
		referenceCount = other.referenceCount;
		hashCode = other.hashCode;
		haveCalculatedHash = other.haveCalculatedHash;
		
		if ( referenceCount )
			(*referenceCount)++;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Equality Comparison Method
//############		
//##########################################################################################
//##########################################################################################




Bool Data:: equals( const Data& other ) const
{
	if ( data == other.data )
		return true;
	else if ( size != other.size )
		return false;
	else if ( haveCalculatedHash && other.haveCalculatedHash && hashCode == other.hashCode )
		return true;
	else
	{
		const UByte* const end = data + size;
		const UByte* byte1 = data;
		const UByte* byte2 = data;
		
		while ( byte1 != end )
		{
			if ( *byte1 != *byte2 )
				return false;
			
			byte1++;
			byte2++;
		}
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Concatenation Method
//############		
//##########################################################################################
//##########################################################################################




Data Data:: concatenate( const Data& other ) const
{
	Size finalSize = size + other.size;
	UByte* finalData = util::allocate<UByte>( finalSize );
	
	util::copy( finalData, data, size );
	util::copy( finalData + size, other.data, other.size );
	
	return Data( finalData, finalSize );
}




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
