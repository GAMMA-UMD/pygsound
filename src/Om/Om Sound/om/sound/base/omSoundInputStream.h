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

#ifndef INCLUDE_OM_SOUND_INPUT_STREAM_H
#define INCLUDE_OM_SOUND_INPUT_STREAM_H


#include "omSoundBaseConfig.h"


#include "omSoundBuffer.h"
#include "omSoundResult.h"
#include "omSoundSampleType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that abstracts a read-only source of sound samples.
/**
  * This class serves as an interface for things like sound file input,
  * streaming input, etc. Reading from a SoundInputStream is very similar to reading
  * from a file. A stream can be either bounded, where it has a defined length
  * and may be seekable, or a stream can be unbounded where its length is unknown
  * and is not seekable.
  */
class SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound input stream and release any resources associated with it.
			virtual ~SoundInputStream();
			
			
		//********************************************************************************
		//******	Read Method
			
			
			/// Read the specified number of samples from the input stream into a sound buffer.
			/**
			  * This method attempts to read the specified number of samples from the stream
			  * into the buffer, and returns the number of samples actually read from the
			  * stream. The samples are converted to the format stored in the buffer
			  * (Sample32f). The input position in the stream is advanced by the number
			  * of samples that are read.
			  *
			  * This method enlarges the buffer to be at least as large as the number of requested
			  * samples, as well as makes sure it has as many channels as the stream has. The
			  * resulting buffer takes on the same sample rate as the stream.
			  */
			SoundResult read( SoundBuffer& buffer, Size numSamples );
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			/**
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this input stream's current position can be moved by the specified signed sample offset.
			/**
			  * This sample offset is specified as the number of sample frames to move
			  * in the stream - a frame is equal to one sample for each channel in the stream.
			  *
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Bool canSeek( Int64 relativeSampleOffset ) const;
			
			
			/// Move the current sample frame position in the stream by the specified signed amount.
			/**
			  * This method attempts to seek the position in the stream by the specified amount.
			  * The method returns the signed amount that the position in the stream was changed
			  * by. Thus, if seeking is not allowed, 0 is returned. Otherwise, the stream should
			  * seek as far as possible in the specified direction and return the actual change
			  * in position.
			  *
			  * The defaut implementation returns 0, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	Stream Size Accessor Methods
			
			
			/// Return the number of samples remaining in the sound input stream.
			/**
			  * The value returned must only be a lower bound on the total number of sample
			  * frames in the stream. For instance, if there are samples remaining, the method
			  * should return at least 1. If there are no samples remaining, the method should
			  * return 0. This behavior is allowed in order to support unbounded streams
			  * which might not have a defined endpoint.
			  */
			virtual SoundSize getSamplesRemaining() const = 0;
			
			
			/// Return whether or not this sound input stream has any samples remaining in the stream.
			OM_INLINE Bool hasSamplesRemaining() const
			{
				return this->getSamplesRemaining() > SoundSize(0);
			}
			
			
			/// Return the current position of the stream in samples relative to the start of the stream.
			/**
			  * The returned value indicates the sample index of the current read
			  * position within the sound stream. For unbounded streams, this value should indicate
			  * the number of samples that have been read by the stream since it was opened.
			  */
			virtual SampleIndex getPosition() const = 0;
			
			
		//********************************************************************************
		//******	Stream Format Accessor Methods
			
			
			/// Return the number of channels that are in the sound input stream.
			/**
			  * This is the number of channels of sound data that will be read with each read call
			  * to the stream's read() method.
			  */
			virtual Size getChannelCount() const = 0;
			
			
			/// Return the sample rate of the sound input stream's source audio data.
			/**
			  * Since some types of streams support variable sampling rates, this value
			  * is not necessarily the sample rate of all audio that is read from the stream.
			  * However, for most streams, this value represents the sample rate of the entire
			  * stream. One should always test the sample rate of the buffers returned by the
			  * stream to see what their sample rates are before doing any operations that assume
			  * a sampling rate.
			  */
			virtual SampleRate getSampleRate() const = 0;
			
			
			/// Return the actual sample type used in the stream.
			/**
			  * This is the format of the stream's source data. For instance, a file
			  * might be encoded with 8-bit, 16-bit or 24-bit samples. This value
			  * indicates that sample type. For formats that don't have a native sample type,
			  * such as those which use frequency domain encoding, this function should
			  * return SampleType::SAMPLE_32F, indicating that the stream's native format
			  * is 32-bit floating point samples.
			  */
			virtual SampleType getNativeSampleType() const = 0;
			
			
		//********************************************************************************
		//******	Stream Status Accessor Method
			
			
			/// Return whether or not the stream has a valid source of sound data.
			/**
			  * This method should return TRUE if everything is OK, but might return
			  * FALSE if the input stream is not valid (file not found, etc) or
			  * if the stream's data has improper format.
			  */
			virtual Bool isValid() const = 0;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Sound Input Method
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			/**
			  * This method attempts to read the specified number of samples from the stream
			  * into the input buffer. It then returns the total number of valid samples which
			  * were read into the output buffer. The samples are converted to the format
			  * stored in the input buffer (Sample32f). The input position in the stream
			  * is advanced by the number of samples that are read.
			  *
			  * The implementor of this method should make sure to set the sample rate of the
			  * buffer to be the correct output sample rate.
			  */
			virtual SoundResult readSamples( SoundBuffer& inputBuffer, Size numSamples ) = 0;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_INPUT_STREAM_H
