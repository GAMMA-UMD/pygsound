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

#ifndef INCLUDE_OM_SOUND_INT_24_H
#define INCLUDE_OM_SOUND_INT_24_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class used to represent a 24-bit integer number.
/**
  * This class helps facilitate operations on 24-bit sound samples
  * by providing methods which pack and unpack a 24-bit sound sample
  * from/to a 32-bit integer sound sample. This class should be exactly
  * 24 bits wide, allowing its use in arrays of samples.
  */
class Int24
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a 24-bit sample equal to 0.
			OM_FORCE_INLINE Int24()
			{
				data[0] = data[1] = data[2] = 0;
			}
			
			
			/// Create a 24-bit sample from the specified 32-bit integer sample, discarding the lower bits.
			OM_FORCE_INLINE Int24( Int32 value )
			{
				pack( value );
			}
			
			
			/// Cast this 24-bit sample to a 32-bit integer sample.
			OM_FORCE_INLINE operator Int32 () const
			{
				return unpack();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Unpack this 24-bit sample into a 32-bit integer sample.
			OM_FORCE_INLINE Int32 unpack() const
			{
				return Int32(data[0] << 24) | Int32(data[1] << 16) | Int32(data[2] << 8);
			}
			
			
			/// Pack the specified 32-bit integer sample into this 24-bit sample, discarding the lower bits.
			OM_FORCE_INLINE void pack( Int32 value )
			{
				value = math::clamp( value, -Int32(1 << 23), Int32(1 << 23) - 1 );
				
				data[0] = (UInt8)((value & 0xFF000000) >> 24);
				data[1] = (UInt8)((value & 0x00FF0000) >> 16);
				data[2] = (UInt8)((value & 0x0000FF00) >> 8);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of 3 bytes representing the 3 bytes of a 24-bit word.
			UInt8 data[3];
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_INT_24_H
