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

#ifndef INCLUDE_OM_SOUND_AIFF_TRANSCODER_H
#define INCLUDE_OM_SOUND_AIFF_TRANSCODER_H


#include "omSoundIOConfig.h"


#include "omSoundTranscoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class thats encode and decodes the PCM AIFF audio format.
class AIFFTranscoder : public SoundTranscoder
{
	public:
		
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return an object that represents the resource format that this transcoder can read and write.
			virtual const ResourceFormat& getResourceFormat() const;
			
			
			/// Return an object that represents the format that this transcoder can encode and decode.
			virtual SoundFormat getSoundFormat() const;
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Return whether or not this transcoder is able to encode the specified resource.
			virtual Bool canEncode( const Sound& resource ) const;
			
			
			/// Encode a sound to the specified output stream.
			virtual Bool encode( DataOutputStream& stream, const Sound& sound );
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Return whether or not the specified identifier refers to a valid resource for this transcoder.
			/**
			  * If the identifier represents a valid resource, TRUE is returned. Otherwise,
			  * if the resource is not valid, FALSE is returned.
			  */
			virtual Bool canDecode( const ResourceID& identifier ) const;
			
			
			/// Decode a sound from the specified output stream.
			virtual Bool decode( DataInputStream& stream, Sound& sound );
			
			
			/// Decode a sound from the specified persistent output stream.
			virtual Bool decode( const Shared<DataInputStream>& stream, Sound& sound );
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_AIFF_TRANSCODER_H
