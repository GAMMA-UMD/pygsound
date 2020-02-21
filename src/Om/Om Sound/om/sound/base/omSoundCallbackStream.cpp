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

#include "omSoundCallbackStream.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundCallbackStream:: canSeek() const
{
	return false;
}




Bool SoundCallbackStream:: canSeek( Int64 relativeSampleOffset ) const
{
	return false;
}




Int64 SoundCallbackStream:: seek( Int64 relativeSampleOffset )
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize SoundCallbackStream:: getSamplesRemaining() const
{
	if ( callback.isNull() )
		return SoundSize(0);
	
	return math::max<SoundSize>();
}




SampleIndex SoundCallbackStream:: getPosition() const
{
	if ( callback.isNull() )
		return SampleIndex(0);
	
	return streamPosition;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundCallbackStream:: getChannelCount() const
{
	if ( callback.isNull() )
		return Size(0);
	
	return lastChannelCount;
}




SampleRate SoundCallbackStream:: getSampleRate() const
{
	if ( callback.isNull() )
		return SampleRate(0);
	
	return lastSampleRate;
}




SampleType SoundCallbackStream:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Status Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundCallbackStream:: isValid() const
{
	return callback.isSet();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Read Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundCallbackStream:: readSamples( SoundBuffer& buffer, Size numSamples )
{
	if ( callback.isNull() )
		return SoundResult::ERROR;
	
	// Create a sound frame to hold the buffer.
	SoundFrame frame( &buffer );
	
	// Request the audio from the callback.
	SoundResult result = callback( frame, numSamples );
	
	// Remember the sample rate and channel count of the buffer.
	lastSampleRate = buffer.getSampleRate();
	lastChannelCount = buffer.getChannelCount();
	
	return result;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
