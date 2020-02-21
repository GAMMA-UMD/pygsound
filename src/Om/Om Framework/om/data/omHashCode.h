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

#ifndef INCLUDE_OM_HASH_CODE_H
#define INCLUDE_OM_HASH_CODE_H


#include "omDataConfig.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to represent and compute hash codes for arbitrary data.
class HashCode
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Compute a hash code for the specified array of data values.
			template < typename DataType >
			OM_INLINE HashCode( const DataType* data, Size number )
			{
				hashCode = computeHashCode( (const UByte*)data, sizeof(DataType)*number );
			}
			
			
			/// Compute a hash code for the specified pointer.
			template < typename DataType >
			OM_INLINE HashCode( const DataType* pointer )
				:	hashCode( Hash(PointerInt(pointer) >> 2)*Hash(2185031351ul) )
			{
			}
			
			
			/// Compute a hash code for the specified pair of pointers.
			template < typename DataType1, typename DataType2 >
			OM_INLINE HashCode( const DataType1* pointer1, const DataType2* pointer2 )
				:	hashCode( (Hash(PointerInt(pointer1) >> 2)*Hash(2185031351ul)) ^
							(Hash(PointerInt(pointer2) >> 2)*Hash(4232417593ul)) )
			{
			}
			
			
			/// Compute a combined hash code from the two given hash code values.
			OM_INLINE HashCode( Hash hash1, Hash hash2 )
				:	hashCode( (hash1*Hash(2185031351ul)) ^ (hash2*Hash(4232417593ul)) )
			{
			}
			
			
			/// Compute a combined hash code from the three given hash code values.
			OM_INLINE HashCode( Hash hash1, Hash hash2, Hash hash3 )
				:	hashCode( (hash1*Hash(0x8DA6B343)) ^ (hash2*Hash(0xD8163841)) ^ (hash3*Hash(0xCB1AB31F)) )
			{
			}
			
			
			
		//********************************************************************************
		//******	Cast Operator
			
			
			/// Return the integer hash code value.
			OM_INLINE operator Hash () const
			{
				return hashCode;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Compute and return the hash code for the specified array of arbitrary data.
			static Hash computeHashCode( const UByte* data, Size length );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The integer that contains the value of the hash code.
			Hash hashCode;
			
			
			
};




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_HASH_CODE_H
