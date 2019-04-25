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

#include "omSoundDataInputStream.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundDataInputStream:: SoundDataInputStream( const Shared<SoundInputStream>& newSoundStream,
											const Shared<DataInputStream>& newDataStream )
	:	soundStream( newSoundStream ),
		dataStream( newDataStream )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundDataInputStream:: ~SoundDataInputStream()
{
	soundStream.release();
	dataStream.release();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDataInputStream:: canSeek() const
{
	if ( soundStream.isNull() )
		return false;
	
	return soundStream->canSeek();
}




Bool SoundDataInputStream:: canSeek( Int64 relativeSampleOffset ) const
{
	if ( soundStream.isNull() )
		return false;
	
	return soundStream->canSeek( relativeSampleOffset );
}




Int64 SoundDataInputStream:: seek( Int64 relativeSampleOffset )
{
	if ( soundStream.isNull() )
		return false;
	
	return soundStream->seek( relativeSampleOffset );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize SoundDataInputStream:: getSamplesRemaining() const
{
	if ( soundStream.isNull() )
		return SoundSize(0);
	
	return soundStream->getSamplesRemaining();
}




SampleIndex SoundDataInputStream:: getPosition() const
{
	if ( soundStream.isNull() )
		return SampleIndex(0);
	
	return soundStream->getPosition();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundDataInputStream:: getChannelCount() const
{
	if ( soundStream.isNull() )
		return Size(0);
	
	return soundStream->getChannelCount();
}




SampleRate SoundDataInputStream:: getSampleRate() const
{
	if ( soundStream.isNull() )
		return SampleRate(0);
	
	return soundStream->getSampleRate();
}




SampleType SoundDataInputStream:: getNativeSampleType() const
{
	if ( soundStream.isNull() )
		return SampleType::UNDEFINED;
	
	return soundStream->getNativeSampleType();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Status Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDataInputStream:: isValid() const
{
	if ( soundStream.isNull() )
		return false;
	
	return soundStream->isValid();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Read Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundDataInputStream:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
{
	if ( soundStream.isNull() )
		return 0;
	
	return soundStream->read( inputBuffer, numSamples );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
