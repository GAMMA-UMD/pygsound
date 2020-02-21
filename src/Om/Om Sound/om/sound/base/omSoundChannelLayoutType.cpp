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

#include "omSoundChannelLayoutType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




ChannelLayoutType:: ChannelLayoutType( Size numChannels )
	:	type( ChannelLayout::UNDEFINED )
{
	switch ( numChannels )
	{
		case 1:		type = ChannelLayout::MONO;				break;
		case 2:		type = ChannelLayout::STEREO;			break;
		case 4:		type = ChannelLayout::QUAD;				break;
		case 6:		type = ChannelLayout::SURROUND_5_1;		break;
		case 8:		type = ChannelLayout::SURROUND_7_1;		break;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Count Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size ChannelLayoutType:: getChannelCount() const
{
	switch ( type )
	{
		case ChannelLayout::MONO:			return 1;
		case ChannelLayout::STEREO:			return 2;
		case ChannelLayout::BINAURAL:		return 2;
		case ChannelLayout::QUAD:			return 4;
		case ChannelLayout::SURROUND_5_1:	return 6;
		case ChannelLayout::SURROUND_7_1:	return 8;
		case ChannelLayout::AMBISONIC_B:	return 4;
		default:							return 0;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Type Accessor Method
//############		
//##########################################################################################
//##########################################################################################




ChannelType ChannelLayoutType:: getChannelType( Index channelIndex ) const
{
	switch ( type )
	{
		case ChannelLayout::MONO:
			if ( channelIndex == 0 )	return ChannelType::MONO;
			break;\
		case ChannelLayout::STEREO:
			switch ( channelIndex )
			{
				case 0:	return ChannelType::LEFT;
				case 1:	return ChannelType::RIGHT;
			}
			break;
		case ChannelLayout::BINAURAL:
			switch ( channelIndex )
			{
				case 0:	return ChannelType::LEFT;
				case 1:	return ChannelType::RIGHT;
			}
			break;
		case ChannelLayout::QUAD:
			switch ( channelIndex )
			{
				case 0:	return ChannelType::FRONT_LEFT;
				case 1:	return ChannelType::FRONT_RIGHT;
				case 2:	return ChannelType::SURROUND_LEFT;
				case 3:	return ChannelType::SURROUND_RIGHT;
			}
			break;
		case ChannelLayout::SURROUND_5_1:
			switch ( channelIndex )
			{
				case 0:	return ChannelType::FRONT_LEFT;
				case 1:	return ChannelType::FRONT_RIGHT;
				case 2:	return ChannelType::FRONT_CENTER;
				case 3:	return ChannelType::LOW_FREQUENCY;
				case 4:	return ChannelType::SURROUND_LEFT;
				case 5:	return ChannelType::SURROUND_RIGHT;
			}
			break;
		case ChannelLayout::SURROUND_7_1:
			switch ( channelIndex )
			{
				case 0:	return ChannelType::FRONT_LEFT;
				case 1:	return ChannelType::FRONT_RIGHT;
				case 2:	return ChannelType::FRONT_CENTER;
				case 3:	return ChannelType::LOW_FREQUENCY;
				case 4:	return ChannelType::SURROUND_LEFT;
				case 5:	return ChannelType::SURROUND_RIGHT;
				case 6:	return ChannelType::BACK_LEFT;
				case 7:	return ChannelType::BACK_RIGHT;
			}
			break;
	}
	
	return ChannelType::UNDEFINED;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Direction Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Vector3f ChannelLayoutType:: getChannelPosition( Index channelIndex ) const
{
	switch ( type )
	{
		case ChannelLayout::MONO:
			if ( channelIndex == 0 )	return Vector3f( Float(0), Float(0), Float(-1) );
			break;
		case ChannelLayout::STEREO:
			switch ( channelIndex )
			{
				case 0:	return Vector3f( Float(-1), Float(0), Float(0) ); // LEFT
				case 1:	return Vector3f( Float(1), Float(0), Float(0) ); // RIGHT
			}
			break;
		case ChannelLayout::BINAURAL:
			switch ( channelIndex )
			{
				case 0:	return Vector3f( Float(-1), Float(0), Float(0) ); // LEFT
				case 1:	return Vector3f( Float(1), Float(0), Float(0) ); // RIGHT
			}
			break;
		case ChannelLayout::QUAD:
			switch ( channelIndex )
			{
				case 0:	return polarXZ( math::degreesToRadians(-30.0f) ); // FRONT_LEFT
				case 1:	return polarXZ( math::degreesToRadians(30.0f) ); // FRONT_RIGHT
				case 2:	return polarXZ( math::degreesToRadians(-95.0f) ); // SURROUND_LEFT
				case 3:	return polarXZ( math::degreesToRadians(95.0f) ); // SURROUND_RIGHT
			}
			break;
		case ChannelLayout::SURROUND_5_1:
			switch ( channelIndex )
			{
				case 0:	return polarXZ( math::degreesToRadians(-30.0f) ); // FRONT_LEFT
				case 1:	return polarXZ( math::degreesToRadians(30.0f) ); // FRONT_RIGHT
				case 2:	return Vector3f( Float(0), Float(0), Float(-1) ); // FRONT_CENTER
				case 3:	return Vector3f(); // LOW_FREQUENCY
				case 4:	return polarXZ( math::degreesToRadians(-110.0f) ); // SURROUND_LEFT
				case 5:	return polarXZ( math::degreesToRadians(110.0f) ); // SURROUND_RIGHT
			}
			break;
		case ChannelLayout::SURROUND_7_1:
			switch ( channelIndex )
			{
				case 0:	return polarXZ( math::degreesToRadians(-30.0f) ); // FRONT_LEFT
				case 1:	return polarXZ( math::degreesToRadians(30.0f) ); // FRONT_RIGHT
				case 2:	return Vector3f( Float(0), Float(0), Float(-1) ); // FRONT_CENTER
				case 3:	return Vector3f(); // LOW_FREQUENCY
				case 4:	return Vector3f( Float(-1), Float(0), Float(0) ); // SURROUND_LEFT
				case 5:	return Vector3f( Float(1), Float(0), Float(0) ); // SURROUND_RIGHT
				case 6:	return polarXZ( math::degreesToRadians(-135.0f) ); // BACK_LEFT
				case 7:	return polarXZ( math::degreesToRadians(135.0f) ); // BACK_RIGHT
			}
			break;
		case ChannelLayout::AMBISONIC_B:
			switch ( channelIndex )
			{
				case 0:	return Vector3f( Float(0), Float(0), Float(0) ); // W
				case 1:	return Vector3f( Float(0), Float(0), Float(-1) ); // X
				case 2:	return Vector3f( Float(1), Float(0), Float(0) ); // Y
				case 3:	return Vector3f( Float(0), Float(1), Float(0) ); // Z
			}
			break;
	}
	
	return Vector3f();
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Conversion Method
//############		
//##########################################################################################
//##########################################################################################




String ChannelLayoutType:: toString() const
{
	const Char* typeString;
	
	switch ( type )
	{
		case ChannelLayout::MONO:
			typeString = "Mono";
			break;
		case ChannelLayout::STEREO:
			typeString = "Stereo";
			break;
		case ChannelLayout::BINAURAL:
			typeString = "Binaural";
			break;
		case ChannelLayout::QUAD:
			typeString = "Quadraphonic";
			break;
		case ChannelLayout::SURROUND_5_1:
			typeString = "5.1 Surround";
			break;
		case ChannelLayout::SURROUND_7_1:
			typeString = "7.1 Surround";
			break;
		case ChannelLayout::CUSTOM:
			typeString = "Custom";
			break;
		case ChannelLayout::AMBISONIC_B:
			typeString = "Ambisonic B-format";
			break;
		default:
			typeString = "Undefined";
			break;
	}
	
	return String( typeString );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
