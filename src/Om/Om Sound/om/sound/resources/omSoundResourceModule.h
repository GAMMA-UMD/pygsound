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

#ifndef INCLUDE_OM_SOUND_RESOURCE_MODULE_H
#define INCLUDE_OM_SOUND_RESOURCE_MODULE_H


#include "omSoundResourcesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Resources Namespace  ***************************
OM_SOUND_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class that encapsulates a collection of resource set transcoders for sounds.
class SoundResourceModule : public ResourceModule
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new sound resource module with a new set of sound resource transcoders.
			SoundResourceModule();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an sound resource module and all of its resource type transcoders.
			/**
			  * Any ResourceSetTranscoder objects that are using the transcoders provided
			  * by this module should cease using those trancoders before the module is detroyed.
			  */
			~SoundResourceModule();
			
			
		//********************************************************************************
		//******	Resource Type Accessor Method
			
			
			/// Add transcoders to the specified resource manager for the resource formats this module supports.
			virtual void getFormats( ResourceManager& manager );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A resource transcoder for the HRTF format.
			io::HRTFTranscoder hrtf;
			
			/// A resource transcoder for the AIFF sound resource format.
			io::AIFFTranscoder aiff;
			
			/// A resource transcoder for the OGG sound resource format.
			io::OggTranscoder ogg;
			
			/// A resource transcoder for the WAVE sound resource format.
			io::WaveTranscoder wave;
			
			
};




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_RESOURCE_MODULE_H
