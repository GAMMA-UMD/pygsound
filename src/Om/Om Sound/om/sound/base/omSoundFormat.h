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

#ifndef INCLUDE_OM_SOUND_FORMAT_H
#define INCLUDE_OM_SOUND_FORMAT_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class representing the different kinds of sound encoding formats.
class SoundFormat
{
	public:
		
		//********************************************************************************
		//******	Sound Format Enum Declaration
			
			
			/// An enum type representing the different kinds of sound encoding formats.
			enum Enum
			{
				/// An undefined sound format.
				UNDEFINED,
				
				/// The WAVE sound format.
				WAVE,
				
				/// The Audio Interchange File Format (AIFF) sound format.
				AIFF,				
				
				/// The compressed MPEG-3 sound format.
				MP3,
				
				/// The MPEG-4 audio-only sound format.
				M4A,
				
				/// The Free Lossless Audio Codec (FLAC) sound format.
				FLAC,
				
				/// The Core Audio Format (CAF) sound format.
				CAF
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an sound format object with an UNDEFINED sound format.
			OM_INLINE SoundFormat()
				:	format( UNDEFINED )
			{
			}
			
			
			/// Create a sound format object from the specified sound format Enum.
			OM_INLINE SoundFormat( SoundFormat::Enum newFormat )
				:	format( newFormat )
			{
			}
			
			
			/// Create a sound format object from the specified resource format.
			SoundFormat( const om::resources::ResourceFormat& newFormat );
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this sound format to an enum value.
			OM_INLINE operator Enum () const
			{
				return format;
			}
			
			
		//********************************************************************************
		//******	Sound Format Attribute Methods
			
			
			/// Return whether or not this sound format is lossy (loses information).
			/**
			  * Lossy formats will suffer from generation loss each time they are
			  * re-encoded.
			  */
			Bool isLossy() const;
			
			
		//********************************************************************************
		//******	Sound Format Extension Accessor Method
			
			
			/// Return the standard file extension used for this sound format.
			UTF8String getExtension() const;
			
			
			/// Return a sound format which corresponds to the format with the given extension string.
			static SoundFormat getFormatForExtension( const UTF8String& extension );
			
			
		//********************************************************************************
		//******	Sound Format String Conversion Methods
			
			
			/// Return a string representation of the sound format.
			String toString() const;
			
			
			/// Convert this sound format into a string representation.
			OM_INLINE operator String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The resource format for WAVE sound files.
			static const om::resources::ResourceFormat WAVE_FORMAT;
			
			/// The resource format for AIFF sound files.
			static const om::resources::ResourceFormat AIFF_FORMAT;			
			
			/// The resource format for MP3 sound files.
			static const om::resources::ResourceFormat MP3_FORMAT;
			
			/// The resource format for M4A sound files.
			static const om::resources::ResourceFormat M4A_FORMAT;
			
			/// The resource format for FLAC sound files.
			static const om::resources::ResourceFormat FLAC_FORMAT;
			
			/// The resource format for CAF sound files.
			static const om::resources::ResourceFormat CAF_FORMAT;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value specifying the sound format.
			Enum format;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FORMAT_H
