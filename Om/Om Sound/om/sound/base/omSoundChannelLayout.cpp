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

#include "omSoundChannelLayout.h"


#include <algorithm>


#include "omSoundChannelLayoutType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ChannelInfo ChannelLayout:: UNDEFINED_CHANNEL;


//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Angle Class Definition
//############		
//##########################################################################################
//##########################################################################################




class ChannelLayout:: SpeakerAngle
{
	public:
		
		/// Create a new speaker angle object with the specified channel index and angle.
		OM_INLINE SpeakerAngle( Index newChannelIndex, Float newAngle )
			:	channelIndex( newChannelIndex ),
				angle( newAngle )
		{
		}
		
		
		/// Return whether or not this speaker angle is less than another, meaning it is more clockwise.
		OM_INLINE Bool operator < ( const SpeakerAngle& other ) const
		{
			return angle < other.angle;
		}
		
		
		/// The index of this speaker's channel within the channel layout.
		Index channelIndex;
		
		
		/// The angle within the XZ plane relative to the origin where this speaker is located.
		Float angle;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ChannelLayout:: ChannelLayout()
	:	type( UNDEFINED ),
		numChannels( 0 ),
		speakerAngles( NULL ),
		numSpeakerAngles( 0 ),
		speakerAngleCapacity( 0 ),
		speakerAnglesNeedUpdate( true )
{
}




ChannelLayout:: ChannelLayout( Size newNumChannels )
	:	type( UNDEFINED ),
		numChannels( 0 ),
		speakerAngles( NULL ),
		numSpeakerAngles( 0 ),
		speakerAngleCapacity( 0 ),
		speakerAnglesNeedUpdate( true )
{
	this->setChannelCount( newNumChannels );
}




ChannelLayout:: ChannelLayout( Type newType )
	:	type( newType ),
		numChannels( 0 ),
		speakerAngles( NULL ),
		numSpeakerAngles( 0 ),
		speakerAngleCapacity( 0 ),
		speakerAnglesNeedUpdate( true )
{
	this->setType( newType );
}




ChannelLayout:: ChannelLayout( const ChannelLayout& other )
	:	type( other.type ),
		numChannels( other.numChannels ),
		channels( other.channels )
{
	// Copy the speaker angle data structure if it is valid.
	if ( !other.speakerAnglesNeedUpdate )
	{
		speakerAngleCapacity = numSpeakerAngles = other.numSpeakerAngles;
		
		// Allocate speace for the new speaker angles.
		speakerAngles = om::util::allocate<SpeakerAngle>( numSpeakerAngles );
		
		// Copy the speaker angles.
		om::util::copy( speakerAngles, other.speakerAngles, numSpeakerAngles );
		
		speakerAnglesNeedUpdate = false;
	}
	else
	{
		speakerAngleCapacity = numSpeakerAngles = 0;
		speakerAngles = NULL;
		speakerAnglesNeedUpdate = true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ChannelLayout:: ~ChannelLayout()
{
	if ( speakerAngles != NULL )
		om::util::deallocate( speakerAngles );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ChannelLayout& ChannelLayout:: operator = ( const ChannelLayout& other )
{
	if ( this != &other )
	{
		type = other.type;
		numChannels = other.numChannels;
		channels = other.channels;
		speakerAnglesNeedUpdate = true;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Equality Comparison Operator
//############		
//##########################################################################################
//##########################################################################################




Bool ChannelLayout:: operator == ( const ChannelLayout& other ) const
{
	if ( type != other.type || numChannels != other.numChannels )
		return false;
	
	// Compare the channels.
	if ( channels.getSize() == numChannels && other.channels.getSize() == numChannels )
	{
		for ( Index c = 0; c < numChannels; c++ )
		{
			if ( channels[c] != other.channels[c] )
				return false;
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Layout Type Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelLayout:: setType( Type newType )
{
	type = newType;
	ChannelLayoutType layoutType( newType );
	
	const Size newNumChannels = layoutType.getChannelCount();
	
	if ( newNumChannels != numChannels )
		ChannelLayout::setChannelCount( newNumChannels );
	
	// Initialize the channels with the canonical channel data.
	for ( Index c = 0; c < newNumChannels; c++ )
	{
		channels[c].setType( layoutType.getChannelType(c) );
		channels[c].setPosition( layoutType.getChannelPosition(c) );
	}
	speakerAnglesNeedUpdate = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Count Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelLayout:: setChannelCount( Size newNumChannels )
{
	if ( channels.getSize() == numChannels && newNumChannels > channels.getSize() )
		channels.setSize( newNumChannels );
	
	// Store the new number of channels.
	numChannels = newNumChannels;
	speakerAnglesNeedUpdate = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Info Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelLayout:: setChannelInfo( Index channelIndex, const ChannelInfo& newInfo )
{
	// Don't do anything if the channel index is invalid.
	if ( channelIndex >= numChannels )
		return;
	
	// Make sure the channel array is properly initialized.
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Determine if the channel was previously a positional channel.
	Bool wasPositional = channels[channelIndex].isPositional();
	
	// Set the new channel position.
	channels[channelIndex] = newInfo;
	
	// Determine if the speaker angles should be updated.
	if ( wasPositional || channels[channelIndex].isPositional() )
		speakerAnglesNeedUpdate = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Vector3f ChannelLayout:: getChannelPosition( Index channelIndex ) const
{
	// Make sure the channel array is properly initialized.
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	if ( channelIndex < numChannels )
		return channels[channelIndex].getPosition();
	else
		return Vector3f();
}




void ChannelLayout:: setChannelPosition( Index channelIndex, const Vector2f& newPosition )
{
	// Don't do anything if the channel index is invalid.
	if ( channelIndex >= numChannels )
		return;
	
	// Make sure the channel array is properly initialized.
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Determine if the channel was previously a positional channel.
	Bool wasPositional = channels[channelIndex].isPositional();
	
	// Set the new channel position.
	channels[channelIndex].setPosition( newPosition );
	
	// Determine if the speaker angles should be updated.
	if ( wasPositional || channels[channelIndex].isPositional() )
		speakerAnglesNeedUpdate = true;
}




void ChannelLayout:: setChannelPosition( Index channelIndex, const Vector3f& newPosition )
{
	// Don't do anything if the channel index is invalid.
	if ( channelIndex >= numChannels )
		return;
	
	// Make sure the channel array is properly initialized.
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Determine if the channel was previously a positional channel.
	Bool wasPositional = channels[channelIndex].isPositional();
	
	// Set the new channel position.
	channels[channelIndex].setPosition( newPosition );
	
	// Determine if the speaker angles should be updated.
	if ( wasPositional || channels[channelIndex].isPositional() )
		speakerAnglesNeedUpdate = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Spatialization Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ChannelLayout:: panDirection( const Vector3f& direction, Array<Gain>& channelGains ) const
{
	// If the layout has 0 channels, return that there was an error.
	if ( numChannels == 0 )
		return false;
	
	// Make sure the output channel gain array is properly initialized.
	if ( channelGains.getSize() < numChannels )
		channelGains.setSize( numChannels );
	
	// Make sure the channel array is properly initialized.
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Update the speaker angles if they need it.
	if ( speakerAnglesNeedUpdate )
		updateSpeakerAngles();
	
	//****************************************************************************
	
	if ( numChannels == Size(1) )
	{
		// This is a monophonic channel configuration, the only channel's gain must be 1.
		channelGains[0] = Gain(1);
		return true;
	}
	else
	{
		// Zero-out all of the output channel gains to start.
		channelGains.setAll( Gain(0) );
		
		Index speakerIndex1, speakerIndex2;
		Float gain1, gain2;
		
		// Find the speaker pair which this direction vector bisects and compute the gain values.
		if ( panAngle( getAngle(direction), speakerIndex1, speakerIndex2, gain1, gain2 ) )
		{
			channelGains[speakerIndex1] = gain1;
			channelGains[speakerIndex2] = gain2;
			return true;
		}
		
		return false;
	}
}




Bool ChannelLayout:: mapLayout( const ChannelLayout& layout, ChannelMixMatrix& mixMatrix ) const
{
	const Size numInputChannels = layout.numChannels;
	
	// If either layout has 0 channels, return that there was an error.
	if ( numChannels == 0 || numInputChannels == 0 )
		return false;
	
	// Make sure the output mix matrix has the correct number of input and output channels.
	if ( mixMatrix.getInputCount() != numInputChannels || mixMatrix.getOutputCount() != numChannels )
		mixMatrix.setSize( numInputChannels, numChannels );
	
	// Make sure the channel arrays are properly initialized.
	if ( layout.channels.getSize() < numInputChannels )
		layout.initializeChannels();
	
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Update the speaker angles if they need it.
	if ( speakerAnglesNeedUpdate )
		updateSpeakerAngles();
	
	if ( layout.speakerAnglesNeedUpdate )
		layout.updateSpeakerAngles();
	
	//****************************************************************************
	
	// Zero the output mix matrix.
	mixMatrix.zero();
	
	// For each speaker in the input speaker layout, pan that speaker to the output layout.
	for ( Index i = 0; i < numInputChannels; i++ )
	{
		const ChannelInfo& inputChannel = layout.channels[i];
		
		// If this channel is positional, map it to this layout.
		if ( inputChannel.isPositional() )
		{
			if ( numSpeakerAngles == 1 )
				mixMatrix.setGain( i, speakerAngles[0].channelIndex, (Float(1)/Float(numInputChannels)) );
			else
			{
				const Vector3f& position = inputChannel.getPosition();
				
				Index channelIndex1, channelIndex2;
				Float gain1, gain2;
				
				// Find the speaker pair which this speaker's vector bisects and compute the gain values.
				if ( panAngle( getAngle( position ), channelIndex1, channelIndex2, gain1, gain2 ) )
				{
					mixMatrix.setGain( i, channelIndex1, gain1 );
					mixMatrix.setGain( i, channelIndex2, gain2 );
				}
			}
		}
		else
		{
			Bool foundChannel = false;
			
			if ( inputChannel.getType() != ChannelType::UNDEFINED )
			{
				Bool foundChannel = false;
				
				// Try to find a channel with the same channel type label.
				for ( Index c = 0; c < numChannels; c++ )
				{
					if ( channels[c].getType() == inputChannel.getType() )
					{
						mixMatrix.setGain( i, c, Gain(1) );
						foundChannel = true;
						break;
					}
				}
			}
			
			// Try to use the channel at the current index if it has undefined output channel type
			// or if this index corresponds to the cannonical index for the output channel type.
			if ( !foundChannel && i < numChannels &&
				(channels[i].getType() == ChannelType::UNDEFINED ||
				channels[i].getType().getChannelIndex() == i) )
			{
				mixMatrix.setGain( i, i, Gain(1) );
			}
		}
	}
	
	return true;
}




Bool ChannelLayout:: panLayout( const ChannelLayout& layout, const PanDirection& pan,
								ChannelMixMatrix& mixMatrix ) const
{
	const Size numInputChannels = layout.numChannels;
	
	// If either layout has 0 channels, return that there was an error.
	if ( numChannels == 0 || numInputChannels == 0 )
		return false;
	
	// Make sure the output mix matrix has the correct number of input and output channels.
	if ( mixMatrix.getInputCount() != numInputChannels || mixMatrix.getOutputCount() != numChannels )
		mixMatrix.setSize( numInputChannels, numChannels );
	
	// Make sure the channel arrays are properly initialized.
	if ( layout.channels.getSize() < numInputChannels )
		layout.initializeChannels();
	
	if ( channels.getSize() < numChannels )
		initializeChannels();
	
	// Update the speaker angles if they need it.
	if ( speakerAnglesNeedUpdate )
		updateSpeakerAngles();
	
	if ( layout.speakerAnglesNeedUpdate )
		layout.updateSpeakerAngles();
	
	//****************************************************************************
	
	// Zero the output mix matrix.
	mixMatrix.zero();
	
	// Compute the panning angle offset for all speaker channels.
	const Float panAngleOffset = getAngle( pan.getDirection() );
	const Float directivityConstant = Float(1) - pan.getDirectivity();
	
	// For each speaker in the input speaker layout, pan that speaker to the output layout.
	for ( Index i = 0; i < numInputChannels; i++ )
	{
		const ChannelInfo& inputChannel = layout.channels[i];
		
		// If this channel is positional, map it to this layout.
		if ( inputChannel.isPositional() )
		{
			if ( numSpeakerAngles == 1 )
				mixMatrix.setGain( i, speakerAngles[0].channelIndex, (Float(1)/Float(numInputChannels)) );
			else
			{
				Float inputAngle = getAngle( inputChannel.getPosition() );
				Float inputOffsetAngle;
				
				if ( inputAngle >= math::pi<Float>() )
				{
					inputOffsetAngle = Float(2)*math::pi<Float>() -
										directivityConstant*(Float(2)*math::pi<Float>() - inputAngle);
				}
				else
					inputOffsetAngle = directivityConstant*inputAngle;
				
				// Compute the final panned angle for the speaker.
				Float finalAngle = panAngleOffset + inputOffsetAngle;
				
				if ( finalAngle >= Float(2)*math::pi<Float>() )
					finalAngle -= Float(2)*math::pi<Float>();
				
				Index channelIndex1, channelIndex2;
				Float gain1, gain2;
				
				// Find the speaker pair which this speaker's vector bisects and compute the gain values.
				if ( panAngle( finalAngle, channelIndex1, channelIndex2, gain1, gain2 ) )
				{
					mixMatrix.setGain( i, channelIndex1, gain1 );
					mixMatrix.setGain( i, channelIndex2, gain2 );
				}
			}
		}
		else
		{
			Bool foundChannel = false;
			
			if ( inputChannel.getType() != ChannelType::UNDEFINED )
			{
				// Try to find a channel with the same channel type label.
				for ( Index c = 0; c < numChannels; c++ )
				{
					if ( channels[c].getType() == inputChannel.getType() )
					{
						mixMatrix.setGain( i, c, Gain(1) );
						foundChannel = true;
						break;
					}
				}
			}
			
			// Try to use the channel at the current index if it has undefined output channel type
			// or if this index corresponds to the cannonical index for the output channel type.
			if ( !foundChannel && i < numChannels &&
				(channels[i].getType() == ChannelType::UNDEFINED ||
				channels[i].getType().getChannelIndex() == i) )
			{
				mixMatrix.setGain( i, i, Gain(1) );
			}
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Angle Panning Helper Method
//############		
//##########################################################################################
//##########################################################################################




Bool ChannelLayout:: panAngle( Float angle, Index& channelIndex1, Index& channelIndex2,
								Float& gain1, Float& gain2 ) const
{
	const SpeakerAngle* lastSpeakerAngle = speakerAngles + numSpeakerAngles - 1;
	Float lastAngle = lastSpeakerAngle->angle - Float(2)*math::pi<Float>();
	
	if ( angle >= lastSpeakerAngle->angle )
		angle -= Float(2)*math::pi<Float>();
	
	// Find the speaker pair between which the angle lies.
	for ( Index i = 0; i < numSpeakerAngles; i++ )
	{
		const SpeakerAngle* speakerAngle = speakerAngles + i;
		
		if ( angle >= lastAngle && angle < speakerAngle->angle )
		{
			Float angle1 = lastAngle;
			Float angle2 = speakerAngle->angle;
			
			// Compute the adjusted angle which pans between the speakers (between 0 and 90 degrees).
			Float adjustedAngle = Float(0.5)*math::pi<Float>()*((angle - angle1)
										/ (angle2 - angle1));
			
			// Compute a sine-cosine-based panning value.
			gain1 = math::cos( adjustedAngle );
			gain2 = math::sin( adjustedAngle );
			channelIndex1 = lastSpeakerAngle->channelIndex;
			channelIndex2 = speakerAngle->channelIndex;
			
			return true;
		}
		
		lastSpeakerAngle = speakerAngle;
		lastAngle = speakerAngle->angle;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelLayout:: initializeChannels() const
{
	// Make sure the channels are initialized.
	if ( channels.getSize() < numChannels )
		channels.setSize( numChannels );
	
	if ( type != CUSTOM && type != UNDEFINED )
	{
		// Initialize the channels with the cannonical channel information.
		ChannelLayoutType layoutType( type );
		const Size numTypeChannels = math::min( layoutType.getChannelCount(), numChannels );
		
		for ( Index c = 0; c < numTypeChannels; c++ )
		{
			channels[c].setType( layoutType.getChannelType(c) );
			channels[c].setPosition( layoutType.getChannelPosition(c) );
		}
	}
	
	// Indicate that the speaker angles need to be recomputed.
	speakerAnglesNeedUpdate = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Speaker Angle Update Methods
//############		
//##########################################################################################
//##########################################################################################




void ChannelLayout:: updateSpeakerAngles() const
{
	// Clear the speaker angle array.
	numSpeakerAngles = 0;
	
	// Make sure the speaker angle array has a large enough capacity.
	if ( speakerAngleCapacity < numChannels )
	{
		// Clean up the old speaker angle array.
		if ( speakerAngles != NULL )
			om::util::deallocate( speakerAngles );
		
		// Allocate a new speaker angle array.
		speakerAngles = om::util::allocate<SpeakerAngle>( numChannels );
		speakerAngleCapacity = numChannels;
	}
	
	// Compute the iteration limits for the channels.
	const ChannelInfo* channel = channels;
	const ChannelInfo* const channelsEnd = channels + numChannels;
	
	while ( channel != channelsEnd )
	{
		// If this channel is positional, add it to the speaker angle array.
		if ( channel->isPositional() )
		{
			Index channelIndex = channel - channels;
			
			speakerAngles[numSpeakerAngles] = SpeakerAngle( channelIndex, getAngle( channel->getPosition() ) );
			numSpeakerAngles++;
		}
		
		channel++;
	}
	
	// Sort the new array of speaker angles.
	std::sort( speakerAngles, speakerAngles + numSpeakerAngles );
	
	speakerAnglesNeedUpdate = false;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
