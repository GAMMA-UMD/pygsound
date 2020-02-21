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

#ifndef INCLUDE_OM_SOUND_REGION_H
#define INCLUDE_OM_SOUND_REGION_H


#include "omSoundBaseConfig.h"


#include "omSoundPanDirection.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class Sound;




//********************************************************************************
/// A class that represents a region of a sound resource and its playback parameters.
class SoundRegion
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new default sound region without a valid sound resource.
			OM_INLINE SoundRegion()
				:	sound( NULL ),
					insert( NULL ),
					start(),
					length(),
					speed( 1 ),
					gain( 1 ),
					pan(),
					fadeInTime(),
					fadeOutTime()
			{
			}
			
			
			/// Create a new default sound region with the specified sound resource.
			OM_INLINE SoundRegion( Sound* newSound )
				:	sound( newSound ),
					insert( NULL ),
					start(),
					length(),
					speed( 1 ),
					gain( 1 ),
					pan(),
					fadeInTime(),
					fadeOutTime()
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the sound that this region uses for sound data.
			Sound* sound;
			
			
			/// A plugin that should be used to process the audio for the region.
			om::sound::plugins::SoundPlugin* insert;
			
			
			/// The starting playback position in the sound resource of this region.
			Time start;
			
			
			/// The length of time to play from the sound resource.
			/**
			  * A value of zero indicates the entire sound should be played.
			  */
			Time length;
			
			
			/// The length of time that should be used to fade in the sound resource to its full volume.
			/**
			  * A value of zero indicates there is no fading performed.
			  */
			Time fadeInTime;
			
			
			/// The length of time that should be used to fade out the sound resource from its full volume to silence.
			/**
			  * A value of zero indicates there is no fading performed.
			  */
			Time fadeOutTime;
			
			
			/// A scale factor for the playback speed, where 1 is the normal speed.
			Float speed;
			
			
			/// A linear gain factor for the sound from this region.
			Float gain;
			
			
			/// The panning direction that should be used when playing this region.
			PanDirection pan;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_REGION_H
