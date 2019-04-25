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

#ifndef INCLUDE_OM_SOUND_CHANNEL_INFO_H
#define INCLUDE_OM_SOUND_CHANNEL_INFO_H


#include "omSoundBaseConfig.h"


#include "omSoundChannelType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores information about the 3D placement and semantic type of a channel's speaker.
class ChannelInfo
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new channel information object with undefined type and position (0,0,0).
			OM_INLINE ChannelInfo()
				:	type( ChannelType::UNDEFINED ),
					position( 0, 0, 0 )
			{
			}
			
			
			/// Create a new channel information object with the specified type that is not positional.
			OM_INLINE ChannelInfo( const ChannelType& newType )
				:	type( newType ),
					position( 0, 0, 0 )
			{
			}
			
			
			/// Create a new channel information object with the specified type and 2D speaker position.
			OM_INLINE ChannelInfo( const ChannelType& newType, const Vector2f& newPosition )
				:	type( newType ),
					position( newPosition.x, 0, newPosition.y )
			{
			}
			
			
			/// Create a new channel information object with the specified type and 3D speaker position.
			OM_INLINE ChannelInfo( const ChannelType& newType, const Vector3f& newPosition )
				:	type( newType ),
					position( newPosition )
			{
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this channel info object is equal to another.
			OM_INLINE Bool operator == ( const ChannelInfo& other ) const
			{
				return type == other.type && position == other.position;
			}
			
			
			/// Return whether or not this channel info object is not equal to another.
			OM_INLINE Bool operator != ( const ChannelInfo& other ) const
			{
				return type != other.type || position != other.position;
			}
			
			
		//********************************************************************************
		//******	Channel Type Accessor Methods
			
			
			/// Return an object representing the semantic type of this channel's speaker.
			OM_INLINE const ChannelType& getType() const
			{
				return type;
			}
			
			
			/// Set the semantic type of this channel's speaker.
			OM_INLINE void setType( const ChannelType& newType )
			{
				type = newType;
			}
			
			
		//********************************************************************************
		//******	Speaker Position Accessor Methods
			
			
			/// Return a reference to the 3D position of the channel's speaker relative to the listener.
			/**
			  * The position is specified in the right-handed coordinate system where -z
			  * is forward, +x is right, and +y is up. The distance to the speaker may
			  * optionally be encoded as the magnitude of the position vector.
			  */
			OM_INLINE const Vector3f& getPosition() const
			{
				return position;
			}
			
			
			/// Set the 2D XZ plane position of the channel's speaker relative to the listener.
			/**
			  * The position is specified in the right-handed coordinate system where -z
			  * is forward, +x is right, and +y is up. This method is designed for dealing
			  * with 2D arrays of speakers in the horizontal plane. The value 0 is inserted
			  * for the Y component of the 3D position vector. The distance to the speaker may
			  * optionally be encoded as the magnitude of the position vector.
			  */
			OM_INLINE void setPosition( const Vector2f& newPosition )
			{
				position = Vector3f( newPosition.x, 0, newPosition.y );
			}
			
			
			/// Set the 3D position of the channel's speaker relative to the listener.
			/**
			  * The position is specified in the right-handed coordinate system where -z
			  * is forward, +x is right, and +y is up. This method is designed for dealing
			  * with 3D arrays of speakers. The distance to the speaker may
			  * optionally be encoded as the magnitude of the position vector.
			  */
			OM_INLINE void setPosition( const Vector3f& newPosition )
			{
				position = newPosition;
			}
			
			
			/// Return whether or not this channel has a position that is non-zero.
			OM_INLINE Bool isPositional() const
			{
				return position.x != Float(0) || position.y != Float(0) || position.z != Float(0);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value indicating the type of the channel.
			ChannelType type;
			
			
			/// A 3D vector indicating the position of the channel's speaker relative to the listener.
			/**
			  * The position is specified in the right-handed coordinate system where -z
			  * is forward, +x is right, and +y is up. The distance to the speaker may
			  * optionally be encoded as the magnitude of the position vector.
			  */
			Vector3f position;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CHANNEL_INFO_H
