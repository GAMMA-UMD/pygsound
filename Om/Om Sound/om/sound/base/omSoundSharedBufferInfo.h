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

#ifndef INCLUDE_OM_SOUND_SHARED_BUFFER_INFO_H
#define INCLUDE_OM_SOUND_SHARED_BUFFER_INFO_H


#include "omSoundBaseConfig.h"


#include "omSoundBuffer.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that holds information about a shared sound buffer buffer.
/**
  * This class is an internal class and shouldn't be used directly.
  */
class SharedBufferInfo
{
	public:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a new shared sound buffer information structure with the specified buffer attributes.
			OM_INLINE SharedBufferInfo( Size numChannels, Size numSamples, SampleRate sampleRate )
				:	buffer( numChannels, numSamples, sampleRate ),
					referenceCount( 0 )
			{
			}
			
			
	private:
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The sound buffer that is being shared.
			SoundBuffer buffer;
			
			
			/// A integer value indicating whether or not this shared buffer is currently in use.
			/**
			  * If the value is 0, there are no outstanding references to the shared buffer,
			  * otherwise this value indicates the number of shared references to the buffer.
			  */
			Index referenceCount;
			
			
		//********************************************************************************
		//******	Friend Class Declarations
			
			
			/// Declare the SharedBufferPool class as a friend so that it can create instances of this class.
			friend class SharedBufferPool;
			
			
			/// Declare the SharedSoundBuffer class as a friend so that it can modify private data of this class.
			friend class SharedSoundBuffer;
			
			
			
};





//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SHARED_BUFFER_INFO_H
