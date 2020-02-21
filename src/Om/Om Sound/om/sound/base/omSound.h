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

#ifndef INCLUDE_OM_SOUND_SOUND_H
#define INCLUDE_OM_SOUND_SOUND_H


#include "omSoundBaseConfig.h"


#include "omSoundInputStream.h"
#include "omSoundFormat.h"
#include "omSoundFormatSettings.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents either a streaming or memory-resident sound resource.
/**
  * This class allows the user to reference a source of sound data without
  * knowing its storage type. The source could be a memory-resident SoundBuffer which contains
  * the referenced sound data or the source could be a streaming source of data (i.e.
  * from a file). Thus, this allows both streaming and non-streaming sounds to be treated
  * the same.
  */
class Sound : public SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound resource that has no sound data.
			Sound();
			
			
			/// Create a new sound resource for the specified memory-resident sound buffer.
			/**
			  * This constructor copies the contents of the specified buffer into a new
			  * internal buffer.
			  */
			Sound( const SoundBuffer& buffer );
			
			
			/// Create a new sound resource for the specified memory-resident sound buffer.
			/**
			  * This constructor copies the specified number of samples from the specified
			  * buffer into a new internal buffer.
			  */
			Sound( const SoundBuffer& buffer, Size numSamples );
			
			
			/// Create a new sound resource that shares the specified memory-resident sound buffer.
			Sound( const Shared<SoundBuffer>& buffer );
			
			
			/// Create a new sound resource for the specified sound input stream.
			/**
			  * This constructor allows the user to optionally specify if the data pointed
			  * to by the stream should be read entirely into memory, rather than being
			  * streamed in real time. The default is to use pure streaming.
			  */
			Sound( const Shared<SoundInputStream>& stream );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound and release its stream and/or buffer.
			~Sound();
			
			
		//********************************************************************************
		//******	Public Stream Accessor Methods
			
			
			/// Return whether or not this resource has a streaming source of sound data.
			OM_INLINE Bool hasStream() const
			{
				return stream.isSet();
			}
			
			
			/// Return a pointer to this sound resource's streaming source of sound data.
			OM_INLINE const Shared<SoundInputStream>& getStream() const
			{
				return stream;
			}
			
			
			/// Set a pointer to this sound resource's streaming source of sound data.
			/**
			  * The sound uses the current position within the sound stream as the start
			  * of the sound.
			  */
			Bool setStream( const Shared<SoundInputStream>& newStream );
			
			
		//********************************************************************************
		//******	Public Buffer Accessor Methods
			
			
			/// Return whether or not this resource has a memory-resident buffer of sound data.
			OM_INLINE Bool hasBuffer() const
			{
				return buffer.isSet();
			}
			
			
			/// Return a pointer to this sound resource's internal memory-resident buffer of sound data.
			OM_INLINE const Shared<SoundBuffer>& getBuffer() const
			{
				return buffer;
			}
			
			
			/// Set a pointer to this sound resource's internal memory-resident buffer of sound data.
			void setBuffer( const Shared<SoundBuffer>& newBuffer );
			
			
		//********************************************************************************
		//******	Resource Loading Methods
			
			
			/// Load this resource from its stream into a buffer if possible.
			/**
			  * The method returns whether or not the resource was able to be loaded.
			  * If the parameter is set to TRUE, the backing stream is released upon load.
			  */
			Bool loadBuffer( Bool releaseStream = false );
			
			
			/// Destroy the buffer for this resource, and revert to using the stream if possible.
			/**
			  * The method returns whether or not the resource was able to be unloaded.
			  * The method fails if there is no stream for the resource.
			  */
			Bool unloadBuffer();
			
			
		//********************************************************************************
		//******	Public Seek Status Accessor Methods
			
			
			/// Return whether or not seeking is allowed in this sound resource input stream.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this resource stream's current position can be moved by the specified signed sample offset.
			virtual Bool canSeek( Int64 relativeSampleOffset ) const;
			
			
			/// Move the current sample frame position in the resource stream by the specified signed amount.
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	Stream Size Accessor Methods
			
			
			/// Return the number of samples remaining in the sound resource input stream.
			virtual SoundSize getSamplesRemaining() const;
			
			
			/// Return the current position of the stream within itself.
			virtual SampleIndex getPosition() const;
			
			
		//********************************************************************************
		//******	Stream Format Accessor Methods
			
			
			/// Return the number of channels that are in the sound resource input stream.
			virtual Size getChannelCount() const;
			
			
			/// Return the sample rate of the sound resource input stream's source audio data.
			virtual SampleRate getSampleRate() const;
			
			
			/// Return the actual sample type used in the resource stream.
			virtual SampleType getNativeSampleType() const;
			
			
		//********************************************************************************
		//******	Stream Status Accessor Method
			
			
			/// Return whether or not the resource has a valid source of sound data.
			virtual Bool isValid() const;
			
			
		//********************************************************************************
		//******	Sound Format Accessor Method
			
			
			/// Return an object that describes the perferred encoded format of this sound.
			OM_FORCE_INLINE const SoundFormat& getFormat() const
			{
				return format;
			}
			
			
			/// Change the preferred encoded format of this sound.
			/**
			  * This call deallocates the sound's encoded data if it was allocated
			  * and the new format is not compatible with the previous encoded data format.
			  */
			void setFormat( const SoundFormat& newFormat );
			
			
		//********************************************************************************
		//******	Sound Format Settings Accessor Method
			
			
			/// Return the settings for the preferred encoded format of this sound.
			OM_INLINE SoundFormatSettings& getFormatSettings()
			{
				return formatSettings;
			}
			
			
			/// Return the settings for the preferred encoded format of this sound.
			OM_INLINE const SoundFormatSettings& getFormatSettings() const
			{
				return formatSettings;
			}
			
			
			/// Change the settings for the preferred encoded format of this sound.
			OM_INLINE void setFormatSettings( const SoundFormatSettings& newFormatSettings )
			{
				formatSettings = newFormatSettings;
			}
			
			
		//********************************************************************************
		//******	Encoded Data Accessor Methods
			
			
			/// Return whether or not this sound has any encoded data.
			OM_INLINE Bool hasEncodedData() const
			{
				return encodedData != NULL;
			}
			
			
			/// Return a pointer to the encoded data for this sound.
			/**
			  * If the returned pointer is NULL, it indicates the start of memory
			  * containing the sound's encoded data in its format.
			  */
			OM_INLINE UByte* getEncodedData()
			{
				return encodedData;
			}
			
			
			/// Return a pointer to the encoded data for this sound.
			/**
			  * If the returned pointer is NULL, it indicates the start of memory
			  * containing the sound's encoded data in its format.
			  */
			OM_INLINE const UByte* getEncodedData() const
			{
				return encodedData;
			}
			
			
			/// Return the size in bytes of the encoded data for this sound.
			OM_INLINE Size getEncodedDataSize() const
			{
				return encodedDataSize;
			}
			
			
			/// Set the size of the encoded data for this sound.
			/**
			  * The sound allocates internal space for the specified number of bytes of encoded sound
			  * data. The contents of the encoded data are undefined.
			  */
			Bool setEncodedDataSize( Size newEncodedDataSize );
			
			
			/// Set the encoded data for this sound.
			/**
			  * The sound copies the sound data to internal memory and the current sound format is
			  * used. The method returns whether or not the operation was successful.
			  */
			Bool setEncodedData( const UByte* newEncodedData, Size newEncodedDataSize );
			
			
			/// Set the encoded data for this sound.
			/**
			  * The sound copies the sound data to internal memory and the sound format is
			  * set to the specified format. The method returns whether or not the operation
			  * was successful.
			  */
			Bool setEncodedData( const UByte* newEncodedData, Size newEncodedDataSize, const SoundFormat& format );
			
			
			/// Deallocate any previous encoded sound data.
			/**
			  * This method is used to discard a previously encoded version of an
			  * sound so that will be re-encoded when it is next saved. This method should
			  * be called whenever an sound's pixel data is edited.
			  */
			void clearEncodedData();
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a sound.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Sound Input Method
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			/**
			  * This method attempts to read the specified number of samples from the stream
			  * into the input buffer. It then returns the total number of valid samples which
			  * were read into the output buffer. The samples are converted to the format
			  * stored in the input buffer (Sample32f). The input position in the stream
			  * is advanced by the number of samples that are read.
			  */
			virtual SoundResult readSamples( SoundBuffer& inputBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to a stream which this resource is wrapping.
			Shared<SoundInputStream> stream;
			
			
			/// The current position within the stream for this sound.
			SampleIndex streamPosition;
			
			
			/// The position in the stream where this sound starts.
			SampleIndex streamStartPosition;
			
			
			/// A pointer to a buffer which this resource is wrapping.
			Shared<SoundBuffer> buffer;
			
			
			/// The current sample read index within the sound buffer which this resource is wrapping.
			Index bufferPosition;
			
			
			/// An object that describes the preferred encoded format of this sound.
			SoundFormat format;
			
			
			/// The settings for the preferred encoded format of this sound.
			SoundFormatSettings formatSettings;
			
			
			/// A pointer to the encoded data for this sound or NULL if there is none.
			UByte* encodedData;
			
			
			/// The number of bytes of encoded data there are for this sound.
			Size encodedDataSize;
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::base::Sound, om::sound::base::Sound::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_SOUND_H
