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

#ifndef INCLUDE_OM_DATA_H
#define INCLUDE_OM_DATA_H


#include "omDataConfig.h"


#include "../util/omArray.h"
#include "../threads/omAtomics.h"


#include "omHashCode.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents an immutable array of unsigned byte data.
/**
  * The Data class is the data-oriented analogue to the GenericString class. It is
  * designed to hold an opaque block of data resident in memory. A Data object could
  * be used to hold anything: images, audio, even text. The internal array of data
  * is reference-counted in order to reduce unintended copying.
  */
class Data
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty Data object that doesn't hold any data.
			OM_INLINE Data()
				:	data( NULL ),
					size( 0 ),
					referenceCount( NULL ),
					haveCalculatedHash( false )
			{
			}
			
			
			/// Create a Data object by copying the specified number of bytes from the given data pointer.
			Data( const UByte* newData, Size number );
			
			
			/// Create a Data object by copying the specified data array.
			Data( const util::Array<UByte>& array );
			
			
			/// Create a Data object by copying the specified number of bytes from the given data array.
			Data( const util::Array<UByte>& array, Size number );
			
			
			OM_INLINE Data( const Data& other )
				:	data( other.data ),
					size( other.size ),
					referenceCount( other.referenceCount ),
					hashCode( other.hashCode ),
					haveCalculatedHash( other.haveCalculatedHash )
			{
				if ( referenceCount )
					(*referenceCount)++;
			}
			
			
		//********************************************************************************
		//******	Static Factory Methods
			
			
			/// Create a data object from the specified byte array with the specified size.
			/**
			  * Once this method completes, it is expected that the Data object now owns
			  * the data array pointer and will free it upon destruction.
			  */
			OM_INLINE static Data shallow( UByte* array, Size size )
			{
				return Data( array, size, 0 );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a Data object, deallocating the internal data array if the reference count reaches zero.
			~Data();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another Data object to this object.
			Data& operator = ( const Data& other );
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the number of bytes of data that this Data object holds.
			OM_INLINE Size getSize() const
			{
				return size;
			}
			
			
			/// Return the number of bytes of data that this Data object holds.
			OM_INLINE Size getSizeInBytes() const
			{
				return size;
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Get a pointer to the internal array of bytes that this Data object holds.
			OM_INLINE const UByte* getPointer() const
			{
				return data;
			}
			
			
			/// Get a pointer to the internal array of bytes that this Data object holds.
			OM_INLINE operator const UByte* () const
			{
				return data;
			}
			
			
			/// Access the byte at the specified index in the Data object's internal array.
			OM_INLINE const UByte& operator () ( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < size, "Cannot access invalid byte index in data array" );
				
				return data[index];
			}
			
			
		//********************************************************************************
		//******	Data Comparison Operators
			
			
			/// Return whether or not the data contained in this Data object is identical to another's.
			Bool equals( const Data& other ) const;
			
			
			/// Return whether or not the data contained in this Data object is identical to another's.
			OM_INLINE Bool operator == ( const Data& other ) const
			{
				return this->equals( other );
			}
			
			
			/// Return whether or not the data contained in this Data object is not identical to another's.
			OM_INLINE Bool operator != ( const Data& other ) const
			{
				return !this->equals( other );
			}
			
			
		//********************************************************************************
		//******	Data Concatentation Methods
			
			
			/// Return a Data object containing the concatenation of this data and the specified data.
			Data concatenate( const Data& other ) const;
			
			
			/// Return a Data object containing the concatenation of this data and the specified data.
			OM_INLINE Data operator + ( const Data& other ) const
			{
				return this->concatenate( other );
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Get a hash code for this Data object.
			OM_INLINE Hash getHashCode() const
			{
				if ( haveCalculatedHash )
					return hashCode;
				else
				{
					hashCode = HashCode( data, size );
					
					return hashCode;
				}
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Shallowly create a Data object from a raw pointer.
			OM_INLINE Data( UByte* newData, Size newSize, int )
				:	data( newData ),
					size( newSize ),
					referenceCount( util::construct< threads::Atomic<Size> >( 1 ) ),
					haveCalculatedHash( false )
			{
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the array of bytes that this Data object holds.
			UByte* data;
			
			
			/// The number of bytes of data that this Data object holds.
			Size size;
			
			
			/// A pointer to the location of the data object's reference count.
			threads::Atomic<Size>* referenceCount;
			
			
			/// The hash code of this data
			mutable Hash hashCode;
			
			
			/// Whether or not the hash code for this data has been calculated already.
			mutable Bool haveCalculatedHash;
			
			
			
};




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DATA_H
