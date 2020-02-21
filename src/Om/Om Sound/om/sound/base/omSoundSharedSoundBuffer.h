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

#ifndef INCLUDE_OM_SOUND_SHARED_SOUND_BUFFER_H
#define INCLUDE_OM_SOUND_SHARED_SOUND_BUFFER_H


#include "omSoundBaseConfig.h"


#include "omSoundSharedBufferInfo.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a temporary handle to a shared SoundBuffer.
/**
  * When this class is created, it obtains a handle to a shared sound buffer and
  * locks that buffer from being used elsewhere. Once this object goes out of scope
  * and is destructed, it releases its handle to that SoundBuffer, allowing its reuse.
  *
  * Therefore, it is an illegal operation to retain a pointer to the shared SoundBuffer
  * and use it after the associated SharedSoundBuffer instance has been destructed.
  */
class SharedSoundBuffer
{
	public:
		
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Declare the copy constructor private so that instances of this class cannot be copied.
			OM_INLINE SharedSoundBuffer( const SharedSoundBuffer& other )
				:	bufferInfo( other.bufferInfo )
			{
				bufferInfo->referenceCount++;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this handle to a shared sound buffer, releasing it back to its pool.
			OM_INLINE ~SharedSoundBuffer()
			{
				bufferInfo->referenceCount--;
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Declare the assignment operator private so that instances of this class cannot be copied.
			OM_INLINE SharedSoundBuffer& operator = ( const SharedSoundBuffer& other )
			{
				if ( this != &other )
				{
					bufferInfo->referenceCount--;
					bufferInfo = other.bufferInfo;
					bufferInfo->referenceCount++;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Buffer Accessor Method
			
			
			/// Return a reference to the underlying sound buffer that is being shared.
			OM_INLINE SoundBuffer& getBuffer()
			{
				return bufferInfo->buffer;
			}
			
			
			/// Return a reference to the underlying sound buffer that is being shared.
			OM_INLINE const SoundBuffer& getBuffer() const
			{
				return bufferInfo->buffer;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructors
			
			
			/// Create a new shared sound buffer for the specified sound buffer and buffer information structure.
			OM_INLINE SharedSoundBuffer( SharedBufferInfo* newBufferInfo )
				:	bufferInfo( newBufferInfo )
			{
				bufferInfo->referenceCount++;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to private information about the shared sound buffer.
			SharedBufferInfo* bufferInfo;
			
			
		//********************************************************************************
		//******	Private Friend Declarations
			
			
			/// Declare the SharedBufferPool class as a friend so that it can create instances of this class.
			friend class SharedBufferPool;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SHARED_SOUND_BUFFER_H
