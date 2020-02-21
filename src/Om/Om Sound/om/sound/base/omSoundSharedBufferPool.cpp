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

#include "omSoundSharedBufferPool.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




SharedBufferPool* SharedBufferPool:: staticPool = om::util::construct<SharedBufferPool>();




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SharedSoundBuffer SharedBufferPool:: getBuffer()
{
	bufferMutex.lock();
	
	for ( Index i = 0; i < buffers.getSize(); i++ )
	{
		SharedBufferInfo* bufferInfo = buffers[i];
		
		if ( bufferInfo->referenceCount == 0 )
		{
			SharedSoundBuffer sharedBuffer( bufferInfo );
			bufferMutex.unlock();
			
			return sharedBuffer;
		}
	}
	
	// Didn't find a suitable buffer, create a new one.
	buffers.add( om::util::construct<SharedBufferInfo>( 0, 0, 44100 ) );
	
	SharedSoundBuffer sharedBuffer( buffers.getLast() );
	
	bufferMutex.unlock();
	
	return sharedBuffer;
}




SharedSoundBuffer SharedBufferPool:: getBuffer( Size numChannels, Size numSamples, SampleRate sampleRate )
{
	bufferMutex.lock();
	
	for ( Index i = 0; i < buffers.getSize(); i++ )
	{
		SharedBufferInfo* bufferInfo = buffers[i];
		
		if ( bufferInfo->referenceCount == 0 )
		{
			if ( bufferInfo->buffer.getSize() < numSamples )
				bufferInfo->buffer.setSize( numSamples );
			
			if ( bufferInfo->buffer.getChannelCount() != numChannels )
				bufferInfo->buffer.setChannelCount( numChannels );
			
			bufferInfo->buffer.setSampleRate( sampleRate );
			
			SharedSoundBuffer sharedBuffer( bufferInfo );
			bufferMutex.unlock();
			
			return sharedBuffer;
		}
	}
	
	// Didn't find a suitable buffer, create a new one.
	buffers.add( om::util::construct<SharedBufferInfo>( numChannels, numSamples, sampleRate ) );
	
	SharedSoundBuffer sharedBuffer( buffers.getLast() );
	
	bufferMutex.unlock();
	
	return sharedBuffer;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Pool Reset Method
//############		
//##########################################################################################
//##########################################################################################




void SharedBufferPool:: reset()
{
	bufferMutex.lock();
	
	for ( Index i = 0; i < buffers.getSize(); )
	{
		if ( buffers[i]->referenceCount == 0 )
		{
			buffers.removeAtIndexUnordered(i);
			continue;
		}
		
		i++;
	}
	
	bufferMutex.unlock();
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
