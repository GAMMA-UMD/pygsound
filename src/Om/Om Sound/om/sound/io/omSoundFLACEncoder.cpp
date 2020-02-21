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

#include "omSoundFLACEncoder.h"


//#include "FLAC/stream_encoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		FLAC Decoder Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class FLACEncoder:: FLACEncoderWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE FLACEncoderWrapper()
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The current reading position in bytes within the data stream, relative to the starting position.
			LargeIndex currentStreamPosition;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FLACEncoder:: FLACEncoder( DataOutputStream* outputStream, Size newNumChannels, SampleRate newSampleRate,
						const SoundFormatSettings& newFormatSettings )
	:	wrapper( om::util::construct<FLACEncoderWrapper>() ),
		numChannels( math::max( newNumChannels, Size(1) ) ),
		formatSettings( newFormatSettings ),
		sampleRate( math::max( newSampleRate, SampleRate(0) ) ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		valid( false ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream != NULL )
		valid = true;
}




FLACEncoder:: FLACEncoder( const FLACEncoder& other )
{
	// Empty because it is declared private and shouldn't be called.
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




FLACEncoder:: ~FLACEncoder()
{
	// Clean up the ogg stuff.
	this->flush();
	
	if ( valid )
	{
		// Clean up the FLAC encoder.
		
	}

	// Destroy the wrapper object.
	om::util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




FLACEncoder& FLACEncoder:: operator = ( const FLACEncoder& other )
{
	// Empty because it is declared private and shouldn't be called.
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Method
//############		
//##########################################################################################
//##########################################################################################




void FLACEncoder:: flush()
{
	
	
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FLACEncoder:: canSeek() const
{
	return false;
}




Bool FLACEncoder:: canSeek( Int64 relativeSampleOffset ) const
{
	return false;
}




Int64 FLACEncoder:: seek( Int64 relativeSampleOffset )
{
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoder Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size FLACEncoder:: getChannelCount() const
{
	return numChannels;
}




SampleRate FLACEncoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType FLACEncoder:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Reading Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FLACEncoder:: writeSamples( const SoundBuffer& inputBuffer, Size numSamples )
{
	// Acquire the mutex which synchronizes decoding with other class access.
	encodingMutex.lock();
	
	// If the encoder is not valid or there are no samples to write, return that
	// no samples were encoded.
	if ( !valid )
	{
		encodingMutex.unlock();
		return SoundResult::ERROR;
	}
	
	if ( numSamples == 0 )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	//***************************************************************************
	// Detect the input sample rate if the output sample rate hasn't been initialized.
	
	if ( sampleRate == SampleRate(0) )
		sampleRate = (SampleRate)inputBuffer.getSampleRate();
	
	//***************************************************************************
	// Write the ogg file's header if we haven't already.
	
	if ( !writtenHeader )
	{
		// If there is an error in writing the header, return that no samples were written.
		if ( !this->writeHeader() )
		{
			valid = false;
			encodingMutex.unlock();
			return 0;
		}
	}
	
	//***************************************************************************
	// Do sample rate conversion if necessary.
	
	const SoundBuffer* finalInputBuffer;
	
	// Test to see if the input buffer should be sample rate converted.
	if ( inputBuffer.getSampleRate() != sampleRate )
	{
		resampler.setOutputSampleRate( sampleRate );
		numSamples = resampler.process( inputBuffer, sampleRateConversionBuffer, numSamples );
		finalInputBuffer = &sampleRateConversionBuffer;
	}
	else
		finalInputBuffer = &inputBuffer;
	
	
	//***************************************************************************
	// Submit sound data to the encoding library.
	
	
	
	//***************************************************************************
	// Encode the data.
	
	
	//**********************************************************************************
	// Update the total length of the audio file.
	
	currentSampleIndex += numSamples;
	lengthInSamples = math::max( currentSampleIndex, lengthInSamples );
	
	encodingMutex.unlock();
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		.ogg Header Read Method
//############		
//##########################################################################################
//##########################################################################################




Bool FLACEncoder:: writeHeader()
{
	
	return true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
