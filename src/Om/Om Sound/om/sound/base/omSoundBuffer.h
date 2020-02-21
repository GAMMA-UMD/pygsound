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

#ifndef INCLUDE_OM_SOUND_BUFFER_H
#define INCLUDE_OM_SOUND_BUFFER_H


#include "omSoundBaseConfig.h"


#include "omSoundSample.h"
#include "omSoundChannelLayout.h"
#include "omSoundChannelLayoutType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a buffer of multichannel audio data.
/**
  * A SoundBuffer class contains zero or more non-interleaved channels of audio data.
  * All channels are the same length. The class will not allocate any memory for audio
  * data unless the number of channels and length of the buffer are non-zero, saving
  * unnecessary allocations. The class also uses a fixed-size internal array of channel
  * pointers, avoiding channel array allocations for small numbers of channels.
  *
  * The sampling rate of the audio contained in a SoundBuffer is stored in each instance
  * of the class. This sampling rate may be changed without affecting the samples stored
  * in the buffer, resulting in a pitch shifting effect when interpreted at the new sample
  * rate.
  *
  * The starting samples for each channel in a buffer are guaranteed to be 16-byte aligned
  * which allows efficient SIMD processing of channel data.
  */
class SoundBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SoundBuffer object with zero channels of zero samples each.
			/**
			  * No memory is allocated until the number of channels is set to a non-zero value
			  * using the setChannelCount() method.
			  *
			  * The default sampling rate of 44.1 kHz is used.
			  *
			  * The channel layout for this buffer is by default ChannelLayout::UNDEFINED.
			  */
			OM_INLINE SoundBuffer()
				:	samples( NULL ),
					numChannels( 0 ),
					numSamples( 0 ),
					channelCapacity( 0 ),
					sampleCapacity( 0 ),
					totalCapacity( 0 ),
					sampleRate( 44100 ),
					layout()
			{
			}
			
			
			/// Create a SoundBuffer object with the specified number of channels.
			/**
			  * The buffer is initially of length 0, meaning that no channel data will be
			  * allocated until the size of the buffer is set to a value greater than 0.
			  *
			  * The default sampling rate of 44.1 kHz is used. The channel layout for this buffer
			  * is by default ChannelLayout::UNDEFINED.
			  *
			  * @param newNumChannels - the number of channels that this sound buffers should have.
			  */
			SoundBuffer( Size newNumChannels );
			
			
			/// Create a SoundBuffer object with the specified number of channels and buffer size.
			/**
			  * This constructor allows the user to specify the desired sample rate of the sound
			  * contained in the buffer, with a default value of 44.1 kHz.
			  *
			  * The channel layout for this buffer is by default ChannelLayout::UNDEFINED.
			  * 
			  * @param newNumChannels - the number of channels that this sound buffers should have.
			  * @param newNumSamples - the length in samples that this buffer should have.
			  * @param newSampleRate - the sample rate to use for the audio contained in this buffer.
			  */
			SoundBuffer( Size newNumChannels, Size newNumSamples, SampleRate newSampleRate = 44100 );
			
			
			/// Create a SoundBuffer object that uses the specified predefined channel layout type.
			/**
			  * The channel layout type determines the total number of channels in the buffer,
			  * as well as their types. The buffer is initially of length 0, meaning
			  * that no channel data will be allocated until the size of the buffer is set
			  * to a value greater than 0.
			  *
			  * The default sampling rate of 44.1 kHz is used.
			  */
			SoundBuffer( ChannelLayoutType newLayoutType );
			
			
			/// Create a SoundBuffer object that uses the specified channel layout type, size, and sample rate.
			/**
			  * The channel layout type determines the total number of channels in the buffer,
			  * as well as their types.
			  *
			  * The default sampling rate of 44.1 kHz is used.
			  */
			SoundBuffer( ChannelLayoutType newLayoutType, Size newNumSamples, SampleRate newSampleRate = 44100 );
			
			
			/// Create a SoundBuffer object that uses the specified channel layout.
			/**
			  * The channel layout determines the total number of channels in the buffer,
			  * as well as their types. The buffer is initially of length 0, meaning
			  * that no channel data will be allocated until the size of the buffer is set
			  * to a value greater than 0.
			  *
			  * The default sampling rate of 44.1 kHz is used.
			  */
			SoundBuffer( const ChannelLayout& newLayout );
			
			
			/// Create a SoundBuffer object that uses the specified channel layout, size, and sample rate.
			/**
			  * The channel layout determines the total number of channels in the buffer,
			  * as well as their types.
			  *
			  * The default sampling rate of 44.1 kHz is used.
			  */
			SoundBuffer( const ChannelLayout& newLayout, Size newNumSamples, SampleRate newSampleRate = 44100 );
			
			
			/// Create an exact copy of the specified SoundBuffer.
			/**
			  * @param other - the SoundBuffer object whose state is to be copied.
			  */
			SoundBuffer( const SoundBuffer& other );
			
			
			/// Create a copy of the specified SoundBuffer, using at most the specified number of samples.
			/**
			  * @param other - the SoundBuffer object whose state is to be copied.
			  * @param numSamples - the number of samples to copy from the specified sound buffer.
			  */
			SoundBuffer( const SoundBuffer& other, Size numSamples );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a SoundBuffer object.
			/**
			  * All internally allocated data is released.
			  */
			~SoundBuffer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents of one SoundBuffer object into this SoundBuffer object.
			/**
			  * All previously existing internal state is released and new state
			  * allocated to exactly replicate the specified SoundBuffer object.
			  *
			  * @param other - the SoundBuffer object whose state should replace this SoundBuffer's.
			  * @return a reference to this SoundBuffer object, allowing assignment chaining.
			  */
			SoundBuffer& operator = ( const SoundBuffer& other );
			
			
		//********************************************************************************
		//******	Channel Layout Accessor Methods
			
			
			/// Return a reference to an object describing this buffer's channel layout.
			OM_INLINE const ChannelLayout& getLayout() const
			{
				return layout;
			}
			
			
			/// Replace the current channel layout of this sound buffer with the specified one.
			/**
			  * This method may resize the number of channels in the buffer so that it
			  * is at least the number of channels in the specified layout.
			  */
			void setLayout( const ChannelLayout& newLayout );
			
			
			/// Replace the current channel layout of this sound buffer with a predefined layout of the given type.
			/**
			  * This method may resize the number of channels in the buffer so that it
			  * is at least the number of channels in the specified layout.
			  */
			void setLayout( ChannelLayoutType newLayoutType );
			
			
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			/// Get the number of channels that this SoundBuffer object has.
			/**
			  * @return the number of channels that this sound buffer has.
			  */
			OM_INLINE Size getChannelCount() const
			{
				return numChannels;
			}
			
			
			/// Set the number of channels that this SoundBuffer object has.
			/**
			  * Changing the number of channels entails reallocating the internal
			  * array of channels and is a potentially expensive operation.
			  * 
			  * @param newNumChannels - the new number of channels that this SoundBuffer object should have.
			  */
			void setChannelCount( Size newNumChannels );
			
			
		//********************************************************************************
		//******	Channel Type Accessor Methods
			
			
			/// Return an object representing the semantic type of the channel at the specified index.
			/**
			  * If the given channel index is invalid, the ChannelType::UNDEFINED type is
			  * returned.
			  */
			OM_INLINE ChannelType getChannelType( Index channelIndex ) const
			{
				return layout.getChannelType( channelIndex );
			}
			
			
			/// Set the semantic type of the channel at the specified index.
			/**
			  * If the given channel index is invalid, the method has no effect.
			  */
			OM_INLINE void setChannelType( Index channelIndex, ChannelType newType )
			{
				layout.setChannelType( channelIndex, newType );
			}
			
			
		//********************************************************************************
		//******	Channel Position Accessor Methods
			
			
			/// Return a the channel position at the specified index.
			/**
			  * If the channel index is invalid or if the speaker is not positional,
			  * the vector (0,0,0) is returned.
			  */
			OM_INLINE Vector3f getChannelPosition( Index channelIndex ) const
			{
				return layout.getChannelPosition( channelIndex );
			}
			
			
			/// Set the position of the channel's speaker at the specified index.
			OM_INLINE void setChannelPosition( Index channelIndex, const Vector2f& newPosition )
			{
				layout.setChannelPosition( channelIndex, newPosition );
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the number of samples that this SoundBuffer object is holding.
			/**
			  * @return the number of samples that this SoundBuffer object is holding.
			  */
			OM_INLINE Size getSize() const
			{
				return numSamples;
			}
			
			
			/// Set the number of samples that this SoundBuffer object is holding.
			/**
			  * This method reallocates the internal channel buffers if necessary
			  * to hold the specified number of samples. If the specified size is less
			  * than the current size, no memory may be reallocated if not necessary.
			  * The existing samples in the old buffer are coppied over to the new buffer
			  * if one is allocated. The remaining new samples are uninitialized and have
			  * undefined contents.
			  *
			  * @param newSize - the new number of samples that this SoundBuffer should hold.
			  */
			void setSize( Size newSize );
			
			
			/// Return the approximate number of bytes of memory allocated by this sound buffer.
			OM_INLINE Size getSizeInBytes() const
			{
				return sizeof(SoundBuffer) + channelCapacity*sizeof(Sample32f*) + numChannels*sampleCapacity*sizeof(Sample32f);
			}
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sampling rate of this sound buffer in samples per second.
			OM_INLINE SampleRate getSampleRate() const
			{
				return sampleRate;
			}
			
			
			/// Set the sampling rate of this sound buffer in samples per second.
			/**
			  * This method does not alter the channel count, the number of samples, or any of the
			  * samples in the buffer. It simply sets the sampling rate at which the buffer's
			  * samples should be interpreted.
			  */
			OM_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				sampleRate = newSampleRate;
			}
			
			
		//********************************************************************************
		//******	Channel Start/End Accessor Methods
			
			
			/// Get a pointer to the first sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose start pointer should be accessed.
			  * @return a pointer to the first sample of the specified channel within this SoundBuffer.
			  */
			OM_INLINE Sample32f* getChannel( Index channelIndex )
			{
				OM_DEBUG_ASSERT_MESSAGE( channelIndex < numChannels, "Cannot access channel at invalid index in sound buffer." );
				
				return samples + sampleCapacity*channelIndex;
			}
			
			
			/// Get a pointer to the first sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose start pointer should be accessed.
			  * @return a pointer to the first sample of the specified channel within this SoundBuffer.
			  */
			OM_INLINE const Sample32f* getChannel( Index channelIndex ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( channelIndex < numChannels, "Cannot access channel at invalid index in sound buffer." );
				
				return samples + sampleCapacity*channelIndex;
			}
			
			
			/// Get a pointer to just past the last sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose end pointer should be accessed.
			  * @return a pointer to the first sample after the end of the specified channel.
			  */
			OM_INLINE Sample32f* getChannelEnd( Index channelIndex )
			{
				OM_DEBUG_ASSERT_MESSAGE( channelIndex < numChannels, "Cannot access channel at invalid index in sound buffer." );
				
				return samples + sampleCapacity*channelIndex + numSamples;
			}
			
			
			/// Get a pointer to just past the last sample within the channel with the specified index.
			/**
			  * If the specified channel index is not within the valid bounds of channel
			  * indices, an assertion is raised.
			  * 
			  * @param channelIndex - the channel whose end pointer should be accessed.
			  * @return a pointer to the first sample after the end of the specified channel.
			  */
			OM_INLINE const Sample32f* getChannelEnd( Index channelIndex ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( channelIndex < numChannels, "Cannot access channel at invalid index in sound buffer." );
				
				return samples + sampleCapacity*channelIndex + numSamples;
			}
			
			
		//********************************************************************************
		//******	Stride Accessor Methods
			
			
			/// Return the advance (in samples) between successive samples of the same channel in this sound buffer.
			/**
			  * The stride is always 0 for non-interleaved buffer (the default). For interleaved
			  * buffers, the stride depends on the storage format of the buffer.
			  */
			OM_INLINE Size getStride() const
			{
				return 0;
			}
			
			
		//********************************************************************************
		//******	Buffer Zero Methods
			
			
			/// Zero-out the entire contents of this SoundBuffer.
			OM_INLINE void zero()
			{
				this->zero( 0, numSamples );
			}
			
			
			/// Zero-out a range of the contents of this SoundBuffer.
			/**
			  * This method writes zero to every sample of every channel in this SoundBuffer
			  * starting at the specified sample start index and ending numSamples past
			  * that index.
			  * 
			  * @param startIndex - the first sample to be zero-d out in the SoundBuffer object.
			  * @param numSamplesToZero - the number of samples to be zero-d out after the start index.
			  */
			void zero( Index startIndex, Size numSamplesToZero );
			
			
		//********************************************************************************
		//******	Buffer Copy Methods
			
			
			/// Copy the specified number of samples from each channel of this buffer to the output buffer.
			/**
			  * The method interleaves the channel samples in the output buffer. The output
			  * buffer must be at least as large as the number of requested samples multiplied
			  * by the number of channels in this SoundBuffer.
			  * 
			  * If the supplied buffer pointer is NULL, the method return immediately and has no effect.
			  * If the number of samples in this SoundBuffer is less than the specified
			  * number of requested samples, the number of requested samples is clamped to the
			  * size of the SoundBuffer.
			  *
			  * The method returns the total number of samples copied to the output buffer per channel.
			  */
			Size copyToInterleavedBuffer( Sample32f* output, Size numSamplesToCopy ) const;
			
			
			/// Copy the contents of this buffer to the other specified sound buffer.
			/**
			  * The method returns the total number of samples per channel actually copied
			  * from this buffer.
			  */
			OM_INLINE Size copyTo( SoundBuffer& other ) const
			{
				return this->copyTo( other, numSamples );
			}
			
			
			/// Copy the specified number of samples from this buffer to the other specified sound buffer.
			/**
			  * The other buffer is enlarged to hold the contents of this buffer if necessary
			  * and as many samples as possible, up to the specified number of samples, are copied
			  * to the other buffer. The method returns the total number of samples per channel
			  * actually copied from this buffer.
			  */
			Size copyTo( SoundBuffer& other, Size numSamplesToCopy ) const;
			
			
		//********************************************************************************
		//******	Buffer Mix Methods
			
			
			/// Mix the contents of this buffer with the other specified sound buffer.
			/**
			  * This method overwrites the contents of the other buffer with the mixed output.
			  * The method returns the total number of samples per channel actually mixed
			  * from this buffer.
			  *
			  * If the other buffer is smaller than the number of samples from this buffer,
			  * only that many samples are mixed (truncating the other samples).
			  */
			OM_INLINE Size mixTo( SoundBuffer& other ) const
			{
				return this->mixTo( other, numSamples );
			}
			
			
			/// Mix the specified number of samples from this buffer with the other specified sound buffer.
			/**
			  * This method overwrites the contents of the other buffer with the mixed output.
			  * The method returns the total number of samples per channel actually mixed
			  * from this buffer.
			  *
			  * If the other buffer is smaller than the number of samples from this buffer,
			  * only that many samples are mixed (truncating the other samples). If there
			  * is a mismatch in the number of buffer channels, as many channels as possible
			  * are mixed with the output buffer.
			  */
			Size mixTo( SoundBuffer& other, Size numSamples ) const;
			
			
			/// Mix the specified number of samples from this buffer with the other specified sound buffer after applying a gain factor.
			/**
			  * This method overwrites the contents of the other buffer with the mixed output.
			  * The method returns the total number of samples per channel actually mixed
			  * from this buffer.
			  *
			  * If the other buffer is smaller than the number of samples from this buffer,
			  * only that many samples are mixed (truncating the other samples). If there
			  * is a mismatch in the number of buffer channels, as many channels as possible
			  * are mixed with the output buffer.
			  */
			Size mixTo( SoundBuffer& other, Size numSamples, Gain gain ) const;
			
			
		//********************************************************************************
		//******	Buffer Gain Methods
			
			
			/// Apply the specified linear gain factor to all samples in this buffer, modifying it.
			/**
			  * This method has the effect of multiplying all samples in the buffer by
			  * the specified gain factor.
			  */
			OM_INLINE void applyGain( Gain linearGain )
			{
				this->applyGain( linearGain, numSamples );
			}
			
			
			/// Apply the specified linear gain factor to the specified number of samples in this buffer, modifying it.
			/**
			  * This method has the effect of multiplying the specified number of samples in the buffer by
			  * the specified gain factor.
			  */
			void applyGain( Gain linearGain, Size numSamples );
			
			
		//********************************************************************************
		//******	Buffer Polarity Methods
			
			
			/// Invert the polarity for the all of the samples in the buffer.
			OM_INLINE void invertPolarity()
			{
				this->invertPolarity( numSamples );
			}
			
			
			/// Invert the polarity for the specified number of samples in the buffer.
			void invertPolarity( Size numSamples );
			
			
		//********************************************************************************
		//******	Buffer Format Accessor Methods
			
			
			/// Copy the format of this buffer to the other specified buffer.
			/**
			  * This method ensures that the specified buffer has the same sample rate,
			  * channel layout, number of channels, and size as this buffer.
			  */
			OM_INLINE void copyFormatTo( SoundBuffer& other ) const
			{
				this->copyFormatTo( other, numSamples );
			}
			
			
			/// Copy the format of this buffer to the other specified buffer.
			/**
			  * This method ensures that the specified buffer has the same sample rate,
			  * channel layout, and number of channels as this buffer. In addition, the
			  * specified buffer is enlarged to be at least as large as the specified
			  * minimum number of samples.
			  */
			void copyFormatTo( SoundBuffer& other, Size minimumNumberOfSamples ) const;
			
			
			/// Set the format of this buffer, so that it has the specified number of channels and samples.
			void setFormat( Size newNumChannels, Size newNumSamples );
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a SoundBuffer resource.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Initialize the specified number of channels of the SoundBuffer to have the given size.
			OM_FORCE_INLINE void initializeChannels( Size newNumChannels, Size newNumSamples );
			
			
			/// Initialize this buffer using the contents of the specified buffer and number of samples.
			OM_FORCE_INLINE void initializeCopyBuffer( const SoundBuffer& buffer, Size newNumSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the samples in this buffer.
			Sample32f* samples;
			
			
			/// The number of channels currently in use in this SoundBuffer.
			Size numChannels;
			
			
			/// The number of valid samples in each channel of this SoundBuffer.
			Size numSamples;
			
			
			/// The total allocated capacity for this SoundBuffer's channels.
			Size channelCapacity;
			
			
			/// The total allocated number of samples for each channel of this SoundBuffer.
			Size sampleCapacity;
			
			
			/// The total number of samples allocated in this sound buffer.
			Size totalCapacity;
			
			
			/// The sampling rate of the audio contained in this SoundBuffer.
			SampleRate sampleRate;
			
			
			/// An object describing the channel layout of this SoundBuffer.
			ChannelLayout layout;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::base::SoundBuffer, om::sound::base::SoundBuffer::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_BUFFER_H
