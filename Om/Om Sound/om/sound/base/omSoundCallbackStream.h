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

#ifndef INCLUDE_OM_SOUND_CALLBACK_STREAM_H
#define INCLUDE_OM_SOUND_CALLBACK_STREAM_H


#include "omSoundBaseConfig.h"


#include "omSoundInputStream.h"
#include "omSoundFrame.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


/// A function object that handles sound callback events that generated sound samples.
typedef Function< SoundResult ( SoundFrame& outputFrame, Size numSamples ) > SoundCallback;




//********************************************************************************
/// A class that wraps a function callback source of sound samples as a SoundInputStream.
/**
  * This class is provided so that external sound systems can easily interface with the
  * SoundInputStream interface. The callback stream does not support seeking within the stream.
  */
class SoundCallbackStream : public SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new sound data input stream that wraps the specified sound stream and data stream.
			OM_INLINE SoundCallbackStream( const SoundCallback& newCallback )
				:	callback( newCallback ),
					streamPosition( 0 ),
					lastSampleRate( 0 ),
					lastChannelCount( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Callback Accessor Methods
			
			
			/// Return a reference to the function callback object that this stream is wrapping.
			OM_INLINE const SoundCallback& getCallback() const
			{
				return callback;
			}
			
			
			/// Set a reference to the function callback object that this stream is wrapping.
			OM_INLINE void setCallback( const SoundCallback& newCallback )
			{
				callback = newCallback;
				streamPosition = 0;
			}
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream's current position can be moved by the specified signed sample offset.
			virtual Bool canSeek( Int64 relativeSampleOffset ) const;
			
			
			/// Move the current sample frame position in the stream by the specified signed amount.
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	Stream Size Accessor Methods
			
			
			/// Return the number of samples remaining in the sound input stream.
			virtual SoundSize getSamplesRemaining() const;
			
			
			/// Return the current position of the stream in samples relative to the start of the stream.
			virtual SampleIndex getPosition() const;
			
			
		//********************************************************************************
		//******	Stream Format Accessor Methods
			
			
			/// Return the number of channels that are in the sound input stream.
			virtual Size getChannelCount() const;
			
			
			/// Return the sample rate of the sound input stream's source audio data.
			virtual SampleRate getSampleRate() const;
			
			
			/// Return the actual sample type used in the stream.
			virtual SampleType getNativeSampleType() const;
			
			
		//********************************************************************************
		//******	Stream Status Accessor Method
			
			
			/// Return whether or not the stream has a valid source of sound data.
			virtual Bool isValid() const;
			
			
	protected:
		
		//********************************************************************************
		//******	Sample Read Method
			
			
			/// Read the specified number of samples from the callback into the output buffer.
			virtual SoundResult readSamples( SoundBuffer& inputBuffer, Size numSamples );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A function callback object that this stream is wrapping.
			SoundCallback callback;
			
			
			/// The number of samples that have been read by this stream from the callback function.
			SampleIndex streamPosition;
			
			
			/// The sample rate of the last buffer of audio that was generated by the callback function.
			SampleRate lastSampleRate;
			
			
			/// The number of channels of the last buffer of audio that was generated by the callback function.
			Size lastChannelCount;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CALLBACK_STREAM_H
