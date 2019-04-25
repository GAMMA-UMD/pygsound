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

#ifndef INCLUDE_OM_SOUND_OUTPUT_STREAM_H
#define INCLUDE_OM_SOUND_OUTPUT_STREAM_H


#include "omSoundBaseConfig.h"


#include "omSoundSampleType.h"
#include "omSoundBuffer.h"
#include "omSoundResult.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that abstracts a write-only destination of sound samples.
/**
  * This class serves as an interface for things like sound file output,
  * streaming output, etc. Writing to a SoundOutputStream is very similar to writing
  * to a file.
  */
class SoundOutputStream
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound output stream and release any resources associated with it.
			virtual ~SoundOutputStream();
			
			
		//********************************************************************************
		//******	Sound Writing Methods
			
			
			/// Write the specified number of samples from a sound buffer to the output stream.
			/**
			  * This method attempts to write the specified number of samples to the stream
			  * from the buffer. It then returns the total number of valid samples which
			  * were written to the output stream. The current write position in the stream
			  * is advanced by the number of samples that are written.
			  */
			OM_INLINE SoundResult write( const SoundBuffer& buffer, Size numSamples )
			{
				numSamples = math::min( buffer.getSize(), numSamples );
				
				return this->writeSamples( buffer, numSamples );
			}
			
			
			/// Flush the sound output stream, sending all internally buffered samples to the destination.
			/** 
			  * This method causes all currently pending output audio data to be sent to it's
			  * final destination. This method blocks the current thread until it ensures that
			  * this is done and that all internal data buffers are emptied if they have any contents.
			  */
			virtual void flush() = 0;
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed in this output stream.
			/**
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this output stream's current position can be moved by the specified signed sample offset.
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
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	Stream Format Accessor Methods
			
			
			/// Return the number of channels that are being written by the sound output stream.
			/**
			  * This is the number of channels that should be provided to the write() method.
			  * If less than this number of channels is provided, silence is written for the
			  * other channels.
			  */
			virtual Size getChannelCount() const = 0;
			
			
			/// Return the sample rate of the sound output stream's destination audio data.
			/**
			  * This is the sampling rate at which the output stream is writing its audio data.
			  * The user should call this method to determine the output sample rate and provide
			  * all of the output sample data in that sampling rate. If sample data with a different
			  * sampling rate is provided, it is converted to the destination sample rate
			  * if deemed necessary by the output stream.
			  */
			virtual SampleRate getSampleRate() const = 0;
			
			
			/// Return the actual sample type used in the stream.
			/**
			  * This is the format of the stream's destination data. For instance, a file
			  * might be encoded with 8-bit, 16-bit or 24-bit samples. This value
			  * indicates that sample type. For formats that don't have a native sample type,
			  * such as those which use frequency domain encoding, this function should
			  * return SampleType::SAMPLE_32F, indicating that the stream's native format
			  * is undefined and can use 32-bit floating point samples.
			  */
			virtual SampleType getNativeSampleType() const = 0;
			
			
		//********************************************************************************
		//******	Stream Status Accessor Method
			
			
			/// Return whether or not the stream has a valid destination for sound data.
			/**
			  * This method should return TRUE if everything is OK, but might return
			  * FALSE if the data output stream is not valid (file not found, etc) or
			  * if the input data has improper format.
			  */
			virtual Bool isValid() const = 0;
			
			
	protected:
			
		//********************************************************************************
		//******	Protected Sound Output Method
			
			
			/// Write the specified number of samples from the output buffer to the output stream.
			/**
			  * This method attempts to write the specified number of samples to the stream
			  * from the output buffer. It then returns the total number of valid samples which
			  * were written to the output stream. The current write position in the stream
			  * is advanced by the number of samples that are written.
			  */
			virtual SoundResult writeSamples( const SoundBuffer& outputBuffer, Size numSamples ) = 0;
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_OUTPUT_STREAM_H
