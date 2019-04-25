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

#ifndef INCLUDE_OM_SOUND_CHANNEL_TYPE_H
#define INCLUDE_OM_SOUND_CHANNEL_TYPE_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum wrapper class that specifies the semantic usage of a channel of audio.
/**
  * This class allows channels of audio to be tagged with their semantic usage.
  * Classes that process that audio can then alter their processing depending on the
  * semantic usage of the channel.
  */
class ChannelType
{
	public:
		
		//********************************************************************************
		//******	Sample Type Enum Definition
			
			
			/// The underlying enum type that specifies the type of a sample of audio data.
			enum Enum
			{
				//********************************************************************************
				// Standard Stereo Channel Types.
				
				
				/// The main front left channel in headphone, 2-channel, quad, and surround setups.
				LEFT = 0,
				
				/// The main front left channel in headphone, 2-channel, quad, and surround setups.
				FRONT_LEFT = LEFT,
				
				/// The main front right channel in headphone, 2-channel, quad, and surround setups.
				RIGHT = 1,
				
				/// The main front right channel in headphone, 2-channel, quad, and surround setups.
				FRONT_RIGHT = RIGHT,
				
				
				//********************************************************************************
				// Quadrophonic, 5.1, 7.1 Surround Channel Types.
				
				
				/// The front center channel in surround setups.
				CENTER = 2,
				
				/// The front center channel in surround setups.
				FRONT_CENTER = CENTER,
				
				/// A channel which carries low-frequency audio information only.
				LOW_FREQUENCY = 3,
				
				/// The left surround channel in quad and surround setups.
				SURROUND_LEFT = 4,
				
				/// The right surround channel in quad and surround setups.
				SURROUND_RIGHT = 5,
				
				/// The back left channel in 7.1 and higher surround setups.
				BACK_LEFT = 6,
				
				/// The back right channel in 7.1 and higher surround setups.
				BACK_RIGHT = 7,
				
				
				//********************************************************************************
				// Other Surround Channel Types.
				
				
				/// The channel for a speaker positioned between the center speaker and the front left speaker.
				FRONT_LEFT_OF_CENTER = 8,
				
				/// The channel for a speaker positioned between the center speaker and the front right speaker.
				FRONT_RIGHT_OF_CENTER = 9,
				
				/// The channel for a speaker positioned directly behind the listener.
				BACK_CENTER = 10,
				
				
				//********************************************************************************
				// Other Channel Types.
				
				
				/// The channel type indicating a single channel speaker.
				MONO,
				
				/// An undefined channel type - the channel is used for an unknown purpose.
				UNDEFINED
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new channel type object with the undefined channel type enum value.
			OM_INLINE ChannelType()
				:	type( UNDEFINED )
			{
			}
			
			
			/// Create a new channel type object with the specified channel type enum value.
			OM_INLINE ChannelType( Enum newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this channel type to an enum value.
			/**
			  * This operator is provided so that the ChannelType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this channel type.
			  */
			OM_INLINE operator Enum () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	Channel Type Attribute Accessor Methods
			
			
			/// Return the cannonical channel index associated with this channel type.
			Index getChannelIndex() const;
			
			
			/// Return a 3D vector representing the cannonical position for this channel type's speaker.
			/**
			  * The direction returned is in the right-handed 3D coordinate system
			  * where -z is forward, +x is to the right, and +y is up. The direction
			  * may not be accurate for all kinds of speaker layouts, but points to the
			  * general position where the speaker should be located.
			  *
			  * Some channel types may have a length-zero position vector, indicating they
			  * are not positional, like MONO and LOW_FREQUENCY channels. 
			  */
			Vector3f getPosition() const;
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a short abbreviation string for this channel type's string representation.
			String getAbbreviation() const;
			
			
			/// Return a string representation of the channel type.
			String toString() const;
			
			
			/// Return a string representing a human-readable name of this channel type.
			OM_INLINE String getName() const
			{
				return this->toString();
			}
			
			
			/// Convert this channel type into a string representation.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Return the 3D normalized vector to a speaker located at the specified rotation relative to front center.
			OM_FORCE_INLINE static Vector3f polarXZ( Float degrees )
			{
				const Float radians = math::degreesToRadians( degrees );
				return Vector3f( math::sin(radians), 0, -math::cos(radians) );
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The underlying enum representing the type of channel for this ChannelType object.
			Enum type;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CHANNEL_TYPE_H
