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

#include "omSoundChannelType.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Channel Index Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Index ChannelType:: getChannelIndex() const
{
	switch ( type )
	{
		case LEFT:						return 0;
		case RIGHT:						return 1;
		case CENTER:					return 2;
		case LOW_FREQUENCY:				return 3;
		case SURROUND_LEFT:				return 4;
		case SURROUND_RIGHT:			return 5;
		case BACK_LEFT:					return 6;
		case BACK_RIGHT:				return 7;
		case BACK_CENTER:				return 8;
		case FRONT_LEFT_OF_CENTER:		return 9;
		case FRONT_RIGHT_OF_CENTER:		return 10;
		case MONO:						return 0;
	}
	
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Direction Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Vector3f ChannelType:: getPosition() const
{
	switch ( type )
	{
		case LEFT:					return polarXZ( math::degreesToRadians(-30.0f) );
		case RIGHT:					return polarXZ( math::degreesToRadians(30.0f) );
		case CENTER:				return Vector3f( 0, 0, -1 );
		case LOW_FREQUENCY:			return Vector3f( 0, 0, 0 );
		case SURROUND_LEFT:			return polarXZ( math::degreesToRadians(-105.0f) );
		case SURROUND_RIGHT:		return polarXZ( math::degreesToRadians(105.0f) );
		case BACK_LEFT:				return polarXZ( math::degreesToRadians(-135.0f) );
		case BACK_RIGHT:			return polarXZ( math::degreesToRadians(135.0f) );
		case FRONT_LEFT_OF_CENTER:	return polarXZ( math::degreesToRadians(-15.0f) );
		case FRONT_RIGHT_OF_CENTER:	return polarXZ( math::degreesToRadians(15.0f) );
		case BACK_CENTER:			return Vector3f( 0, 0, 1 );
		case MONO:					return Vector3f( 0, 0, -1 );
	}
	
	return Vector3f( 0, 0, 0 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Abbreviation Accessor Method
//############		
//##########################################################################################
//##########################################################################################




String ChannelType:: getAbbreviation() const
{
	const Char* result = "";
	
	switch ( type )
	{
		case LEFT:					result = "L";	break;
		case RIGHT:					result = "R";	break;
		case CENTER:				result = "C";	break;
		case LOW_FREQUENCY:			result = "Lf";	break;
		case SURROUND_LEFT:			result = "Sl";	break;
		case SURROUND_RIGHT:		result = "Sr";	break;
		case BACK_LEFT:				result = "Bl";	break;
		case BACK_RIGHT:			result = "Br";	break;
		case FRONT_LEFT_OF_CENTER:	result = "Fl";	break;
		case FRONT_RIGHT_OF_CENTER:	result = "Fr";	break;
		case BACK_CENTER:			result = "Bc";	break;
		case MONO:					result = "M";	break;
	}
	
	return String( result );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Name Accessor Method
//############		
//##########################################################################################
//##########################################################################################




String ChannelType:: toString() const
{
	const Char* result = "Undefined";
	
	switch ( type )
	{
		case LEFT:					result = "Left";					break;
		case RIGHT:					result = "Right";					break;
		case CENTER:				result = "Center";					break;
		case LOW_FREQUENCY:			result = "Low Frequency";			break;
		case SURROUND_LEFT:			result = "Left Surround";			break;
		case SURROUND_RIGHT:		result = "Right Surround";			break;
		case BACK_LEFT:				result = "Back Left";				break;
		case BACK_RIGHT:			result = "Back Right";				break;
		case FRONT_LEFT_OF_CENTER:	result = "Front Left-of-Center";	break;
		case FRONT_RIGHT_OF_CENTER:	result = "Front Right-of-Center";	break;
		case BACK_CENTER:			result = "Back Center";				break;
		case MONO:					result = "Mono";					break;
	}
	
	return String( result );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
