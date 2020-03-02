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

#include "omSoundFormat.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceFormat SoundFormat:: WAVE_FORMAT( "WAVE", "wav", "wave" );
const om::resources::ResourceFormat SoundFormat:: AIFF_FORMAT( "AIFF", "aif", "aiff", "aifc" );
const om::resources::ResourceFormat SoundFormat:: MP3_FORMAT( "MP3", "mp3" );
const om::resources::ResourceFormat SoundFormat:: M4A_FORMAT( "M4A", "m4a" );
const om::resources::ResourceFormat SoundFormat:: FLAC_FORMAT( "FLAC", "flac" );
const om::resources::ResourceFormat SoundFormat:: CAF_FORMAT( "CAF", "caf" );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundFormat:: SoundFormat( const om::resources::ResourceFormat& newFormat )
	:	format( UNDEFINED )
{
	if ( newFormat == WAVE_FORMAT )
		format = WAVE;
	else if ( newFormat == AIFF_FORMAT )
		format = AIFF;
	else if ( newFormat == MP3_FORMAT )
		format = MP3;
	else if ( newFormat == M4A_FORMAT )
		format = M4A;
	else if ( newFormat == FLAC_FORMAT )
		format = FLAC;
	else if ( newFormat == CAF_FORMAT )
		format = CAF;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Lossy Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundFormat:: isLossy() const
{
	switch ( format )
	{
		case MP3:
		case M4A:
			return true;
		default:
			return false;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Format Extension Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundFormat:: getExtension() const
{
	const Char* extension;
	
	switch ( format )
	{
		case WAVE:
			extension ="wav";
			break;
		case AIFF:
			extension = "aif";
			break;
		case MP3:
			extension = "mp3";
			break;
		case M4A:
			extension = "m4a";
			break;
		case FLAC:
			extension = "flac";
			break;
		case CAF:
			extension = "caf";
			break;
		default:
			extension = "";
			break;
	}
	
	return UTF8String( extension );
}




SoundFormat SoundFormat:: getFormatForExtension( const UTF8String& extension )
{
	UTF8String e = extension.toLowerCase();
	
	if ( e == "wav" || e == "wave" )
		return WAVE;
	else if ( e == "aif" || e == "aiff" )
		return AIFF;
	else if ( e == "mp3" )
		return MP3;
	else if ( e == "m4a" )
		return M4A;
	else if ( e == "flac" )
		return FLAC;
	else if ( e == "caf" )
		return CAF;
	else
		return UNDEFINED;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Format String Representation Method
//############		
//##########################################################################################
//##########################################################################################




String SoundFormat:: toString() const
{
	const Char* typeString;
	
	switch ( format )
	{
		case WAVE:
			typeString ="WAVE";
			break;
		case AIFF:
			typeString = "AIFF";
			break;
		case MP3:
			typeString = "MP3";
			break;
		case M4A:
			typeString = "M4A";
			break;
		case FLAC:
			typeString = "FLAC";
			break;
		case CAF:
			typeString = "CAF";
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
					
