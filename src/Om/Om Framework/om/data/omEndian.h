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

#ifndef INCLUDE_OM_ENDIAN_H
#define INCLUDE_OM_ENDIAN_H


#include "omDataConfig.h"


#include "omString.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




namespace endian
{
	/// Reverse the byte order of a signed 16-bit integer.
	OM_INLINE Int16 swap( Int16 value )
	{
		return (value << 8) | (value >> 8);
	}
	
	/// Reverse the byte order of an unsigned 16-bit integer.
	OM_INLINE UInt16 swap( UInt16 value )
	{
		return (value << 8) | (value >> 8);
	}
	
	/// Reverse the byte order of a signed 32-bit integer.
	OM_INLINE Int32 swap( Int32 value )
	{
		return	(value << 24) | ((value << 8) & 0x00FF0000) |
				((value >> 8) & 0x0000FF00) | (value >> 24);
	}
	
	/// Reverse the byte order of an unsigned 32-bit integer.
	OM_INLINE UInt32 swap( UInt32 value )
	{
		return	(value << 24) | ((value << 8) & 0x00FF0000) |
				((value >> 8) & 0x0000FF00) | (value >> 24);
	}
	
	/// Reverse the byte order of a signed 64-bit integer.
	OM_INLINE Int64 swap( Int64 value )
	{
		return (value >> 56) | 
				((value << 40) & 0x00FF000000000000ll) |
				((value << 24) & 0x0000FF0000000000ll) |
				((value << 8)  & 0x000000FF00000000ll) |
				((value >> 8)  & 0x00000000FF000000ll) |
				((value >> 24) & 0x0000000000FF0000ll) |
				((value >> 40) & 0x000000000000FF00ll) |
				(value << 56);
	}
	
	/// Reverse the byte order of an unsigned 64-bit integer.
	OM_INLINE UInt64 swap( UInt64 value )
	{
		return (value >> 56) | 
				((value << 40) & 0x00FF000000000000ull) |
				((value << 24) & 0x0000FF0000000000ull) |
				((value << 8)  & 0x000000FF00000000ull) |
				((value >> 8)  & 0x00000000FF000000ull) |
				((value >> 24) & 0x0000000000FF0000ull) |
				((value >> 40) & 0x000000000000FF00ull) |
				(value << 56);
	}
	
	/// Reverse the byte order of a 16-bit float.
	OM_INLINE lang::Float16 swap( lang::Float16 value )
	{
		UInt16 result = swap( *((UInt16*)&value) );
		
		return *((UInt16*)&result);
	}
	
	/// Reverse the byte order of a 32-bit float.
	OM_INLINE Float32 swap( Float32 value )
	{
		UInt32 result = swap( *((UInt32*)&value) );
		
		return *((Float32*)&result);
	}
	
	/// Reverse the byte order of a 64-bit float.
	OM_INLINE Float64 swap( Float64 value )
	{
		UInt64 result = swap( *((UInt64*)&value) );
		
		return *((Float64*)&result);
	}
	
	
#ifdef OM_LITTLE_ENDIAN
	
	
	template < typename T >
	OM_INLINE T fromBigEndian( T value )
	{
		return swap(value);
	}
	
	template < typename T >
	OM_INLINE T fromLittleEndian( T value )
	{
		return value;
	}
	
	template < typename T >
	OM_INLINE T toBigEndian( T value )
	{
		return swap(value);
	}
	
	template < typename T >
	OM_INLINE T toLittleEndian( T value )
	{
		return value;
	}
	
	OM_INLINE Bool isBigEndian()
	{
		return false;
	}
	
	OM_INLINE Bool isLittleEndian()
	{
		return true;
	}
	
#else
#ifdef OM_BIG_ENDIAN
	
	template < typename T >
	OM_INLINE T fromBigEndian( T value )
	{
		return value;
	}
	
	template < typename T >
	OM_INLINE T fromLittleEndian( T value )
	{
		return swap(value);
	}
	
	template < typename T >
	OM_INLINE T toBigEndian( T value )
	{
		return value;
	}
	
	template < typename T >
	OM_INLINE T toLittleEndian( T value )
	{
		return swap(value);
	}
	
	OM_INLINE Bool isBigEndian()
	{
		return true;
	}
	
	OM_INLINE Bool isLittleEndian()
	{
		return false;
	}
	
#endif
#endif
	
};




//********************************************************************************
/// A class that describes a byte ordering format and provides endian conversion to/from that format.
class Endianness
{
	public:
		
		//********************************************************************************
		//******	Endianness Type Enum Definition
			
			
			/// An enum that specifies the different possible endian types.
			enum Enum
			{
				/// The highest-order byte comes first in the byte order.
				BIG,
				
				/// The lowest-order byte comes first in the byte order.
				LITTLE
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a endianness type that represents the standard endianness for the current platform.
			OM_INLINE Endianness()
			{
				#if defined(OM_BIG_ENDIAN)
					type = BIG;
				#elif defined(OM_LITTLE_ENDIAN)
					type = LITTLE;
				#endif
			}
			
			
			/// Create a endianness type with the specified endianness type enum value.
			OM_INLINE Endianness( Enum newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this endianness type to an enum value.
			OM_INLINE operator Enum () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	Endianness Conversion Methods
			
			
			/// Convert the specified value, assumed to be in this endianness, to native endianness.
			template < typename T >
			OM_INLINE T convertToNative( T value ) const
			{
				if ( type == BIG )
					return endian::fromBigEndian( value );
				else
					return endian::fromLittleEndian( value );
			}
			
			
			/// Convert the specified value, assumed to be in native endianness, to this endianness.
			template < typename T >
			OM_INLINE T convertFromNative( T value ) const
			{
				if ( type == BIG )
					return endian::toBigEndian( value );
				else
					return endian::toLittleEndian( value );
			}
			
			
		//********************************************************************************
		//******	Is Native Accessor Method
			
			
			/// Return whether or not this Endianness is the native endianness of the current platform.
			OM_INLINE Bool isNative() const
			{
				#if defined(OM_BIG_ENDIAN)
				return type == BIG;
				#elif defined(OM_LITTLE_ENDIAN)
				return type == LITTLE;
				#endif
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the endianness type.
			OM_INLINE String toString() const
			{
				switch ( type )
				{
					case BIG:
						return String("Big Endian");
					case LITTLE:
						return String("Little Endian");
					default:
						return String("Undefined");
				}
			}
			
			
			/// Convert this endianness type into a string representation.
			OM_INLINE operator String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum for the endianness type.
			Enum type;
			
			
};




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ENDIAN_H
