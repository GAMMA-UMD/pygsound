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

#ifndef INCLUDE_OM_SOUND_FORMAT_SETTINGS_H
#define INCLUDE_OM_SOUND_FORMAT_SETTINGS_H


#include "omSoundBaseConfig.h"


#include "omSoundFormatFlags.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies common settings for sound formats.
class SoundFormatSettings
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sound format settings object.
			OM_INLINE SoundFormatSettings()
				:	flags( SoundFormatFlags::DEFAULT ),
					sampleType( SampleType::SAMPLE_32F ),
					bitRate( 192 ),
					minBitRate( 32 ),
					maxBitRate( 256 )
			{
			}
			
			
		//********************************************************************************
		//******	Flags Accessor Methods
			
			
			/// Return a reference to an object that contains boolean parameters of the sound format setting.
			OM_INLINE SoundFormatFlags& getFlags()
			{
				return flags;
			}
			
			
			/// Return an object that contains boolean parameters of the sound format setting.
			OM_INLINE const SoundFormatFlags& getFlags() const
			{
				return flags;
			}
			
			
			/// Set an object that contains boolean parameters of the sound format setting.
			OM_INLINE void setFlags( const SoundFormatFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
			/// Return whether or not the specified boolan flag is set for this sound format setting.
			OM_INLINE Bool flagIsSet( SoundFormatFlags::Flag flag ) const
			{
				return flags.isSet( flag );
			}
			
			
			/// Return whether or not all of the specified boolan flags are set for this sound format setting.
			OM_INLINE Bool flagsAreSet( const SoundFormatFlags& otherFlags ) const
			{
				return (flags & otherFlags) == otherFlags;
			}
			
			
			/// Set whether or not the specified boolan flag is set for this sound format setting.
			OM_INLINE void setFlag( SoundFormatFlags::Flag flag, Bool newIsSet = true )
			{
				flags.set( flag, newIsSet );
			}
			
			
		//********************************************************************************
		//******	Sample Type Accessor Methods
			
			
			/// Return the type that is used for sound samples in this format.
			OM_INLINE const SampleType& getSampleType() const
			{
				return sampleType;
			}
			
			
			/// Set the type that is used for sound samples in this format.
			OM_INLINE void setSampleType( const SampleType& newSampleType )
			{
				sampleType = newSampleType;
			}
			
			
		//********************************************************************************
		//******	Bit Rate Accessor Methods
			
			
			/// Return the target bitrate in kilobits/second for encoding sound.
			OM_INLINE Float getBitRate() const
			{
				return bitRate;
			}
			
			
			/// Set the target bitrate in kilobits/second for encoding sound.
			/**
			  * The bit rate is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setBitRate( Float newBitRate )
			{
				bitRate = math::max( newBitRate, Float(0) );
			}
			
			
		//********************************************************************************
		//******	Minimum Bit Rate Accessor Methods
			
			
			/// Return the minimum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			OM_INLINE Float getMinBitRate() const
			{
				return minBitRate;
			}
			
			
			/// Set the minimum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			/**
			  * The bit rate is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setMinBitRate( Float newMinBitRate )
			{
				minBitRate = math::max( newMinBitRate, Float(0) );
			}
			
			
		//********************************************************************************
		//******	Maximum Bit Rate Accessor Methods
			
			
			/// Return the maximum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			OM_INLINE Float getMaxBitRate() const
			{
				return maxBitRate;
			}
			
			
			/// Set the maximum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			/**
			  * The bit rate is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setMaxBitRate( Float newMaxBitRate )
			{
				maxBitRate = math::max( newMaxBitRate, Float(0) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that contains boolean configuration flags for sound formats.
			SoundFormatFlags flags;
			
			
			/// The type that is used for samples in this format.
			SampleType sampleType;
			
			
			/// The target bitrate in kilobits/second for encoding sound.
			Float bitRate;
			
			
			/// The minimum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			Float minBitRate;
			
			
			/// The maximum allowed bitrate in kilobits/second for encoding sound with a variable bitrate.
			Float maxBitRate;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FORMAT_SETTINGS_H
