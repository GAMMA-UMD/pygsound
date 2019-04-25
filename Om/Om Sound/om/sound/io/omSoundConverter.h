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

#ifndef INCLUDE_OM_SOUND_CONVERTER_H
#define INCLUDE_OM_SOUND_CONVERTER_H


#include "omSoundIOConfig.h"


#include "omSoundTranscoder.h"
#include "omSoundAIFFTranscoder.h"
#include "omSoundWaveTranscoder.h"
#include "omSoundOggTranscoder.h"
#include "omSoundFLACTranscoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles encoding and decoding sound data to/from various formats.
class SoundConverter
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default sound converter which can encode or decode any of the supported sound formats.
			/**
			  * In order to encode unsupported sound formats, the user should create a class that
			  * inherits from SoundTranscoder and add it to this converter for the correct format.
			  */
			SoundConverter();
			
			
		//********************************************************************************
		//******	Transcoder Accessor Methods
			
			
			/// Return the total number of transcoders that this sound converter supports.
			OM_INLINE Size getTranscoderCount() const
			{
				return transcoders.getSize();
			}
			
			
			/// Return a pointer to the transcoder for the specified sound format.
			/**
			  * If that format is not supported by the converter, a NULL pointer is returned.
			  */
			SoundTranscoder* getTranscoder( SoundFormat format ) const;
			
			
			/// Add a new transcoder to this sound converter.
			/**
			  * If transcoder pointer is not NULL, the transcoder is added to the converter
			  * for its format (accessed by calling getFormat() on the transcoder).
			  * If there already exists a transcoder for that format, it is replaced.
			  * The method returns whether or not the new transcoder was successfully added.
			  */
			Bool addTranscoder( SoundTranscoder* newTranscoder );
			
			
			/// Remove the transcoder from this converter with the specified sound format.
			/**
			  * If there is a transcoder with that format, it is removed and TRUE is returned.
			  * Otherwise, the converter is unchanged and FALSE is returned.
			  */
			Bool removeTranscoder( SoundFormat format );
			
			
			/// Remove all transcoders from this sound converter.
			/**
			  * After this operation, the sound converter will not be able to decode or
			  * encode sounds in any format until new transcoder objects are added to it.
			  */
			void clearTranscoders();
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Return whether or not this transcoder is able to encode the specified sound format.
			Bool canEncode( const SoundFormat& format ) const;
			
			
			/// Return whether or not this transcoder is able to encode the specified sound.
			Bool canEncode( const SoundFormat& format, const Sound& sound ) const;
			
			
			/// Save the specified sound object at the specified ID location.
			/**
			  * The method returns whether or not the sound was successfully written.
			  */
			Bool encode( const SoundFormat& format, const UTF8String& filePath, const Sound& sound ) const;
			
			
			/// Save the specified sound object at the specified stream.
			Bool encode( const SoundFormat& format, DataOutputStream& stream, const Sound& sound ) const;
			
			
			/// Save the specified sound object at the specified file path.
			/**
			  * The output file type is determined by the file path's extension.
			  * The method returns whether or not the sound was successfully written.
			  */
			Bool encode( const UTF8String& filePath, const Sound& sound ) const;
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Return whether or not this converter can decode the specified sound format.
			Bool canDecode( const SoundFormat& format ) const;
			
			
			/// Return whether or not the specified identifier refers to a valid sound for this transcoder.
			/**
			  * If the identifier represents a valid sound, TRUE is returned. Otherwise,
			  * if the sound is not valid, FALSE is returned.
			  */
			Bool canDecode( const SoundFormat& format, const UTF8String& filePath ) const;
			
			
			/// Load the sound pointed to by the specified identifier.
			Bool decode( const SoundFormat& format, const UTF8String& filePath, Sound& sound ) const;
			
			
			/// Load the sound located at the current position in the given data stream..
			Bool decode( const SoundFormat& format, DataInputStream& stream, Sound& sound ) const;
			
			
			/// Load the sound located at the current position in the given data stream..
			Bool decode( const SoundFormat& format, const Shared<DataInputStream>& stream, Sound& sound ) const;
			
			
			/// Load the sound pointed to by the specified file path.
			/**
			  * The output file type is determined by the file path's extension.
			  */
			Bool decode( const UTF8String& filePath, Sound& sound ) const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A map from sound formats to sound transcoders associated with those formats.
			HashMap< SoundFormat, SoundTranscoder* > transcoders;
			
			
			/// An sound transcoder object for the AIFF format.
			AIFFTranscoder aiff;
			
			
			/// An sound transcoder object for the WAVE format.
			WaveTranscoder wave;
			
			
			/// An sound transcoder object for the OGG format.
			OggTranscoder ogg;
			
			
			/// An sound transcoder object for the FLAC format.
			FLACTranscoder flac;
			
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CONVERTER_H
