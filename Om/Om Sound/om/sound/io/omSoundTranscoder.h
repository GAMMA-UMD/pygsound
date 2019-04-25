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

#ifndef INCLUDE_OM_SOUND_TRANSCODER_H
#define INCLUDE_OM_SOUND_TRANSCODER_H


#include "omSoundIOConfig.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides the interface for objects that encode and decode sound data.
class SoundTranscoder : public ResourceTypeTranscoder<Sound>
{
	public:
		
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return an object that represents the sound format that this transcoder can encode and decode.
			virtual SoundFormat getSoundFormat() const = 0;
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Save the specified sound resource at the specified ID location.
			virtual Bool encode( const ResourceID& identifier, const Sound& sound,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
			/// Encode a sound to the specified output stream.
			virtual Bool encode( DataOutputStream& stream, const Sound& sound ) = 0;
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Load the sound pointed to by the specified identifier.
			virtual Sound* decode( ResourceID& identifier, ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
			/// Decode a sound from the specified output stream.
			virtual Bool decode( DataInputStream& stream, Sound& sound ) = 0;
			
			
			/// Decode a sound from the specified persistent output stream.
			virtual Bool decode( const Shared<DataInputStream>& stream, Sound& sound ) = 0;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Methods
			
			
			/// Encode a sound to the given sound output stream.
			static Bool encodeToStream( const Sound& sound, SoundOutputStream& encoder );
			
			
			/// Decode a sound from the given sound output stream.
			static Bool decodeFromStream( Sound& sound, SoundInputStream& decoder );
			
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_TRANSCODER_H
