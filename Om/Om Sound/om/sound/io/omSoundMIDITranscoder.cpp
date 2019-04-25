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

#include "omSoundMIDITranscoder.h"


#include "omSoundMIDIDecoder.h"
#include "omSoundMIDIEncoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceFormat MIDITranscoder:: FORMAT("MIDI","mid","midi");


//##########################################################################################
//##########################################################################################
//############		
//############		Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const ResourceFormat& MIDITranscoder:: getResourceFormat() const
{
	return FORMAT;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool MIDITranscoder:: canEncode( const MIDI& midi ) const
{
	return true;
}




Bool MIDITranscoder:: encode( const ResourceID& identifier, const MIDI& midi,
								const ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canEncode( midi ) )
		return false;
	
	//const SoundBuffer& buffer = *sound.getBuffer();
	
	//MIDIEncoder encoder( identifier.getURL() );
	
	//encoder.write( buffer, buffer.getSize() );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool MIDITranscoder:: canDecode( const ResourceID& resourceID ) const
{
	return resourceID.getType() == ResourceType::of<MIDI>() && 
			(resourceID.getFormat() == ResourceFormat::UNDEFINED || resourceID.getFormat() == FORMAT);
}




MIDI* MIDITranscoder:: decode( ResourceID& identifier, ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canDecode( identifier ) )
		return NULL;
	
	
	
	return NULL;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
