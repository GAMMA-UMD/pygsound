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

#ifndef INCLUDE_OM_SOUND_CHANNEL_LAYOUT_H
#define INCLUDE_OM_SOUND_CHANNEL_LAYOUT_H


#include "omSoundBaseConfig.h"


#include "omSoundChannelInfo.h"
#include "omSoundPanDirection.h"
#include "omSoundChannelMixMatrix.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies the configuration and usage of a set of channels of audio.
/**
  * A ChannelLayout object contains an enum value indicating the type of the layout.
  * This value either indicates a predefined layout type for standard channel
  * layout formats, or a custom or undefined layout. A custom layout indicates that
  * a custom SpeakerLayout has been defined and is stored by the ChannelLayout object.
  */
class ChannelLayout
{
	public:
		
		//********************************************************************************
		//******	Channel Layout Type Enum Declaration
			
			
			/// An enum that is used to specify various pre-defined types of channel layouts.
			enum Type
			{
				/// An enum value describing a ChannelLayout with only one speaker.
				MONO,
				
				/// An enum value describing a ChannelLayout for a basic stereo speaker system.
				/**
				  * The left and right speakers are placed 90 degrees left and right of front center.
				  * This doesn't correspond to the actual placement of stereo speakers but it is used
				  * to provide proper panning - stereo speakers at the normal placement  of 30 degrees off
				  * center axis wouldn't produce as wide of a stereo image.
				  * Another benefit is that this corresponds to headphone positioning, where it will produce
				  * realistic panning.
				  */
				STEREO,
				
				/// An enum value indicating a 2-channel binaural HRTF-based output format.
				BINAURAL,
				
				/// An enum value describing a ChannelLayout for a 4-channel quadraphonic speaker system.
				/**
				  * Front left and right speakers are placed 30 degrees off of front center. Back left
				  * and right speakers are placed 95 degrees off of front center.
				  */
				QUAD,
				
				/// An alias for the QUAD speaker configuration.
				SURROUND_4 = QUAD,
				
				/// An enum value describing a ChannelLayout for a 5.1 surround speaker system.
				/**
				  * Front left and right speakers are placed 30 degrees off of front center. Back left
				  * and right speakers are placed 110 degrees off of front center. The low frequency
				  * channel's positioning is undefined.
				  */
				SURROUND_5_1,
				
				/// An enum value describing a ChannelLayout for a 7.1 surround speaker system.
				/**
				  * Front left and right speakers are placed 30 degrees off of front center. Back left
				  * and right speakers are placed 135 degrees off of front center. Side left and right
				  * speakers are placed 90 degrees off of front center. The low frequency channel's
				  * positioning is undefined.
				  */
				SURROUND_7_1,

				/// An enum value describing a ChannelLayout for an ideal ambisonic microphone.
				/**
				  * The four channels are the 1st order ambisonic in B-format, in the ordering of WXYZ.
				  * The W channel is omnidirectional with X being forward facing, Y being right facing,
				  * and Z being up facing figure-of-eight microphones. In theory this type can be extended
				  * to have arbitrary numbers of higher order ambisonics.
				  */
				AMBISONIC_B,
				
				/// An enum value indicating a custom channel layout.
				CUSTOM,
				
				/// An enum value indicating an undefined type of channel layout.
				UNDEFINED
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default channel layout with no channels.
			ChannelLayout();
			
			
			/// Create a new channel layout with the specified number of channels.
			/**
			  * All channels in the layout are initialized to have the ChannelType::UNDEFINED
			  * type and are non-positional.
			  */
			ChannelLayout( Size newNumChannels );
			
			
			/// Create a ChannelLayout object that has the specified predefined layout type.
			/**
			  * The predefined layout type is used to determine the type and number of
			  * channels that should be in the layout.
			  */
			ChannelLayout( Type newType );
			
			
			/// Create a copy of the specified ChannelLayout object.
			ChannelLayout( const ChannelLayout& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy all state used by a ChannelLayout object.
			~ChannelLayout();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one ChannelLayout to this object.
			ChannelLayout& operator = ( const ChannelLayout& other );
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this channel layout is equal to another.
			Bool operator == ( const ChannelLayout& other ) const;
			
			
			/// Return whether or not this channel layout is not equal to another.
			OM_INLINE Bool operator != ( const ChannelLayout& other ) const
			{
				return !((*this) == other);
			}
			
			
		//********************************************************************************
		//******	Layout Type Accessor Methods
			
			
			/// Return the semantic kind of channel layout this layout corresponds to.
			OM_INLINE Type getType() const
			{
				return type;
			}
			
			
			/// Set the semantic kind of channel layout this layout corresponds to.
			/**
			  * If the number of channels for the specified layout type is different
			  * than the current number of channels, the channel layout is enlarged
			  * for the new number of channels.
			  *
			  * If the specified type is CUSTOM or UNDEFINED, the number and type of channels is
			  * unchanged.
			  */
			void setType( Type newType );
			
			
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			/// Return the total number of channels that are part of this channel layout.
			OM_INLINE Size getChannelCount() const
			{
				return numChannels;
			}
			
			
			/// Set the total number of channels that are part of this channel layout.
			/**
			  * If increasing the size of the layout, the new channels are initialized to
			  * have the ChannelType::UNDEFINED type and to be non-positional. If decreasing
			  * the size, the extra channels are just removed.
			  */
			void setChannelCount( Size newNumChannels );
			
			
		//********************************************************************************
		//******	Channel Information Accessor Methods
			
			
			/// Return a reference to the channel information object describing the channel at the given index.
			/**
			  * If the given channel index is invalid, a reference to a placeholder undefined
			  * channel is returned.
			  */
			OM_INLINE const ChannelInfo& getChannelInfo( Index channelIndex ) const
			{
				if ( channelIndex < numChannels )
					return channels[channelIndex];
				else
					return UNDEFINED_CHANNEL;
			}
			
			
			/// Set the information about the channel at the specified index in this layout.
			/**
			  * This method completely replaces the previous state of the channel at
			  * the given index. If the channel index is invalid, the method has no effect.
			  */
			void setChannelInfo( Index channelIndex, const ChannelInfo& newInfo );
			
			
		//********************************************************************************
		//******	Channel Type Accessor Methods
			
			
			/// Return an object representing the semantic type of the channel at the specified index.
			/**
			  * If the given channel index is invalid, the ChannelType::UNDEFINED type is
			  * returned.
			  */
			OM_INLINE ChannelType getChannelType( Index channelIndex ) const
			{
				if ( channelIndex < numChannels )
					return channels[channelIndex].getType();
				else
					return ChannelType::UNDEFINED;
			}
			
			
			/// Set the semantic type of the channel at the specified index.
			/**
			  * If the given channel index is invalid, the method has no effect.
			  */
			OM_INLINE void setChannelType( Index channelIndex, ChannelType newType )
			{
				if ( channelIndex < numChannels )
					return channels[channelIndex].setType( newType );
			}
			
			
		//********************************************************************************
		//******	Channel Position Accessor Methods
			
			
			/// Return a the channel position at the specified index.
			/**
			  * If the channel index is invalid or if the speaker is not positional,
			  * the vector (0,0,0) is returned.
			  */
			Vector3f getChannelPosition( Index channelIndex ) const;
			
			
			/// Set the position of the channel's speaker at the specified index.
			void setChannelPosition( Index channelIndex, const Vector2f& newPosition );
			
			
			/// Set the position of the channel's speaker at the specified index.
			void setChannelPosition( Index channelIndex, const Vector3f& newPosition );
			
			
		//********************************************************************************
		//******	Channel Spatialization Methods
			
			
			/// Get the channel gain coefficients for a sound source in the specified direction.
			/**
			  * This method uses a vector-based amplitude panning implementation to determine
			  * the speakers that are located nearest to the specified 3D direction. It then computes
			  * the gain coefficients for each of those speakers that will localize a sound
			  * source in the given direction. Those linear gain coefficients are placed in
			  * the output array of channel gains. The output array may be enlarged if necessary
			  * to hold all of this speaker layout's channels.
			  *
			  * The method returns TRUE if it succeeds and FALSE if there was an error.
			  * This can happen if the layout has no speakers.
			  */
			Bool panDirection( const Vector3f& direction, Array<Gain>& channelGains ) const;
			
			
			/// Get a channel mix matrix which maps the specified speaker layout to this one.
			/**
			  * This method determines the best mapping from the specified arbitrary speaker
			  * layout to this speaker layout. The method returns TRUE if a mapping was found and FALSE if
			  * no mapping was found between the layouts. This can happen if either layout
			  * has no speakers.
			  */
			Bool mapLayout( const ChannelLayout& layout, ChannelMixMatrix& mixMatrix ) const;
			
			
			/// Get a channel mix matrix which maps the specified speaker layout to this one using the specified panning.
			/**
			  * This method determines the best mapping from the specified arbitrary speaker
			  * layout to this speaker layout when the original layout is panned in the specified
			  * direction. The method returns TRUE if a mapping was found and FALSE if
			  * no mapping was found between the layouts. This can happen if either layout
			  * has no speakers.
			  */
			Bool panLayout( const ChannelLayout& layout, const PanDirection& pan,
							ChannelMixMatrix& mixMatrix ) const;
			
			
	private:
		
		//********************************************************************************
		//******	Speaker Class Definition
			
			
			/// A class that stores the angle of a single speaker within a speaker layout.
			class SpeakerAngle;
			
			
		//********************************************************************************
		//******	Private Panning Helper Methods
			
			
			/// Compute the angle within the XZ plane of the specified position, relative to the origin.
			OM_INLINE static Float getAngle( const Vector3f& position )
			{
				Float angle = math::atan2( -position.z, position.x ) - Float(0.5)*math::pi<Float>();
				
				// Make sure the angle is between 0 and 2*pi.
				if ( angle < Float(0) )
					angle += Float(2)*math::pi<Float>();
				
				return angle;
			}
			
			
			/// Find the speaker pair that the specified angle lies between and compute the panning gains.
			OM_FORCE_INLINE Bool panAngle( Float angle, Index& channelIndex1, Index& channelIndex2,
											Float& gain1, Float& gain2 ) const;
			
			
		//********************************************************************************
		//******	Private Speaker Angle Update Helper Methods
			
			
			/// Initialize the channel array in this layout based on the current layout type and channel count.
			void initializeChannels() const;
			
			
			/// Update and sort the speaker angles so that the speaker angles are in order from 0 to 2*pi.
			void updateSpeakerAngles() const;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// A static ChannelInfo object that is used to refer to an undefined channel.
			/**
			  * Declaring this here allows the channel layout to always return a reference
			  * to channel positions, etc even when invalid arguments are provided.
			  */
			static const ChannelInfo UNDEFINED_CHANNEL;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value that stores the semantic type of this channel layout.
			Type type;
			
			
			/// The total number of valid channels in this channel layout.
			Size numChannels;
			
			
			/// An array of information about the channels in this channel layout.
			mutable Array<ChannelInfo> channels;
			
			
			/// A pointer to an array of speaker angles which correspond to the positioned channels of this layout.
			mutable SpeakerAngle* speakerAngles;
			
			
			/// The total number of valid entries in the speaker angle array.
			mutable Size numSpeakerAngles;
			
			
			/// The capacity of the speaker angle array.
			mutable Size speakerAngleCapacity;
			
			
			/// A boolean value that acts as a dirty bit to determine when the speaker angle data needs to be updated.
			mutable Bool speakerAnglesNeedUpdate;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CHANNEL_LAYOUT_H
