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

#ifndef INCLUDE_OM_SOUND_CHANNEL_LAYOUT_TYPE_H
#define INCLUDE_OM_SOUND_CHANNEL_LAYOUT_TYPE_H


#include "omSoundBaseConfig.h"


#include "omSoundChannelLayout.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum wrapper class that specifies various predefined types of channel layouts.
/**
  * See the ChannelLayout::Type declaration for more information on the predefined layout types.
  */
class ChannelLayoutType
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new channel layout type object with the undefined channel layout type enum value.
			OM_INLINE ChannelLayoutType()
				:	type( ChannelLayout::UNDEFINED )
			{
			}
			
			
			/// Create a new channel layout type object for the specified number of channels.
			/**
			  * This constructor creates a layout type that is the cannonical semantic
			  * type for the given number of channels. For example, 1 = MONO, 2 = STEREO, 4 = QUAD, etc.
			  */
			ChannelLayoutType( Size numChannels );
			
			
			/// Create a new channel layout type object with the specified channel layout type enum value.
			OM_INLINE ChannelLayoutType( ChannelLayout::Type newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Channel Count Accessor Method
			
			
			/// Return the number of channels of audio that are part of a stream with this channel layout type.
			/**
			  * If the channel layout type is ChannelType::CUSTOM or ChannelType::UNDEFINED, the
			  * method returns 0.
			  */
			Size getChannelCount() const;
			
			
			/// Return the cannonical channel type for the specified channel index in this layout.
			/**
			  * If the index is invalid or the layout type has no channel type for that
			  * for that index, ChannelType::UNDEFINED is returned.
			  */
			ChannelType getChannelType( Index channelIndex ) const;
			
			
			/// Return the cannonical 3D position for the specified channel index in this layout.
			/**
			  * The direction returned is in the right-handed 3D coordinate system
			  * where -z is forward, +x is to the right, and +y is up. The direction
			  * may not be accurate for all kinds of speaker layouts, but points to the
			  * general position where the speaker should be located.
			  *
			  * Some channel types may have a length-zero position vector, indicating they
			  * are not positional, like MONO and LOW_FREQUENCY channels. 
			  */
			Vector3f getChannelPosition( Index channelIndex ) const;
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this channel layout type to an enum value.
			/**
			  * This operator is provided so that the ChannelLayoutType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this channel layout type.
			  */
			OM_INLINE operator ChannelLayout::Type () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the channel layout type.
			data::String toString() const;
			
			
			/// Return a string representing the name of this channel layout type.
			OM_INLINE data::String getName() const
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
			
			
			/// The underlying enum representing the type of layout for this ChannelLayoutType object.
			ChannelLayout::Type type;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CHANNEL_LAYOUT_TYPE_H
