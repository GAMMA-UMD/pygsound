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

#include "omSoundFilterInputStream.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FilterInputStream:: FilterInputStream()
	:	filter( NULL ),
		samplesRead( 0 ),
		lastChannelCount( 0 ),
		lastSampleRate( 0 )
{
}




FilterInputStream:: FilterInputStream( SoundFilter* newFilter )
	:	filter( newFilter ),
		samplesRead( 0 ),
		lastChannelCount( 0 ),
		lastSampleRate( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void FilterInputStream:: setFilter( SoundFilter* newFilter )
{
	filter = newFilter;
	samplesRead = 0;
	lastChannelCount = 0;
	lastSampleRate = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize FilterInputStream:: getSamplesRemaining() const
{
	return math::max<SoundSize>();
}




SampleIndex FilterInputStream:: getPosition() const
{
	return samplesRead;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size FilterInputStream:: getChannelCount() const
{
	return lastChannelCount;
}




SampleRate FilterInputStream:: getSampleRate() const
{
	return lastSampleRate;
}




SampleType FilterInputStream:: getNativeSampleType() const
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




Bool FilterInputStream:: isValid() const
{
	return filter != NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Read Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FilterInputStream:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
{
	if ( filter )
	{
		numSamples = filter->read( inputBuffer, numSamples );
		samplesRead += numSamples;
		lastChannelCount = inputBuffer.getChannelCount();
		lastSampleRate = inputBuffer.getSampleRate();
		
		return numSamples;
	}
	else
		return 0;
}





//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
