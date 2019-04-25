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

#ifndef INCLUDE_OM_SOUND_FILTER_INPUT_STREAM_H
#define INCLUDE_OM_SOUND_FILTER_INPUT_STREAM_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that wraps a SoundFilter in a SoundInputStream.
/**
  * The filter input stream treats the SoundFilter as a read-only source of
  * sound samples. This allows an output-only filter (e.g. a ToneGenerator, Sampler)
  * to be used anywhere an input stream is required. The resulting stream is
  * not seekable.
  *
  * The filter stream does not own the filter pointer and does not destruct
  * it upon its own destruction.
  */
class FilterInputStream : public base::SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a filter input stream that doesn't have a SoundFilter to wrap.
			FilterInputStream();
			
			
			/// Create a filter input stream that doesn't have a SoundFilter to wrap.
			FilterInputStream( SoundFilter* filter );
			
			
		//********************************************************************************
		//******	Filter Accessor Methods
			
			
			/// Return a pointer to the sound filter that this filter input stream is currently wrapping.
			/**
			  * This pointer can be NULL if the filter input stream has not been
			  * provided a sound filter.
			  */
			OM_INLINE SoundFilter* getFilter() const
			{
				return filter;
			}
			
			
			/// Set the sound filter that this filter input stream should use a source of sound data.
			/**
			  * The filter input stream stores the pointer and uses the filter to
			  * read samples as needed. Calling this method resets the stream so that
			  * it matches the characteristics of the specified filter. The filter stream
			  * does not own the filter pointer and does not destruct it upon its own destruction.
			  */
			void setFilter( SoundFilter* newFilter );
			
			
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
			virtual SoundResult readSamples( SoundBuffer& inputBuffer, Size numSamples );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the filter that this filter input stream is wrapping.
			SoundFilter* filter;
			
			
			/// The number of samples that have been read so far from the filter.
			SoundSize samplesRead;
			
			
			/// The channel count that was produced by the filter on the last frame of output.
			Size lastChannelCount;
			
			
			/// The sample rate that was produced by the filter on the last frame of output.
			SampleRate lastSampleRate;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_INPUT_STREAM_H
