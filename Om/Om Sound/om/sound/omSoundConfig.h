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

#ifndef INCLUDE_OM_SOUND_CONFIG_H
#define INCLUDE_OM_SOUND_CONFIG_H


#include "om/omFramework.h"
#include "om/omResources.h"


//##########################################################################################
//##########################################################################################
//############		
//############		Library Namespace Configuration
//############		
//##########################################################################################
//##########################################################################################




/// The enclosing namespace for the entire Om library.
/**
  * @namespace om
  */
namespace om
{
	#define OM_SOUND_NAMESPACE_START		namespace om { namespace sound {
	#define OM_SOUND_NAMESPACE_END			}; };
	
	/// A namespace that handles sound input, output, and processing.
	/**
	  * @namespace om::sound
	  */
	namespace sound
	{
		/// A namespace containing basic sound classes for samples, buffers, and others types.
		/**
		  * @namespace om::sound::base
		  */
		namespace base {};
		#define OM_SOUND_BASE_NAMESPACE_START		namespace om { namespace sound { namespace base {
		#define OM_SOUND_BASE_NAMESPACE_END		}; }; };
		
		/// A namespace containing classes handling input and output from audio devices.
		/**
		  * @namespace om::sound::devices
		  */
		namespace devices {};
		#define OM_SOUND_DEVICES_NAMESPACE_START		namespace om { namespace sound { namespace devices {
		#define OM_SOUND_DEVICES_NAMESPACE_END			}; }; };
		
		
		/// A namespace containing classes that encode and decode sound from various sound file types.
		/**
		  * @namespace om::sound::io
		  */
		namespace io {};
		#define OM_SOUND_IO_NAMESPACE_START		namespace om { namespace sound { namespace io {
		#define OM_SOUND_IO_NAMESPACE_END			}; }; };
		
		
		/// A namespace containing classes that do processing on buffers of audio and MIDI data.
		/**
		  * @namespace om::sound::filters
		  */
		namespace filters
		{
			class SoundFilter;
		};
		#define OM_SOUND_FILTERS_NAMESPACE_START		namespace om { namespace sound { namespace filters {
		#define OM_SOUND_FILTERS_NAMESPACE_END			}; }; };
		
		
		/// A namespace containing classes that manage different types of audio processing plugins (filters, AU, VST, etc).
		/**
		  * @namespace om::sound::plugins
		  */
		namespace plugins
		{
			class SoundPlugin;
		};
		#define OM_SOUND_PLUGINS_NAMESPACE_START		namespace om { namespace sound { namespace plugins {
		#define OM_SOUND_PLUGINS_NAMESPACE_END			}; }; };
		
		
		/// A namespace containing classes that encode and decode sound data to/from the resource format.
		/**
		  * @namespace om::sound::resources
		  */
		namespace resources
		{
			class DirectionalIRResourceTranscoder;
		};
		#define OM_SOUND_RESOURCES_NAMESPACE_START		namespace om { namespace sound { namespace resources {
		#define OM_SOUND_RESOURCES_NAMESPACE_END		}; }; };
		
		
		/// A namespace containing classes that provide graphical user interfaces for SoundFilter classes.
		/**
		  * @namespace om::sound::views
		  */
		namespace views
		{
			class SoundFilterView;
		};
		#define OM_SOUND_VIEWS_NAMESPACE_START		namespace om { namespace sound { namespace views {
		#define OM_SOUND_VIEWS_NAMESPACE_END		}; }; };
	};
};




//##########################################################################################
//*****************************  Start Om Sound Namespace  *********************************
OM_SOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Define the type to represent the sampling rate of sound data.
typedef Double SampleRate;


/// Define the type used to indicate the index of a sample within a sound.
typedef UInt64 SampleIndex;


/// Define the type used to indicate the size in samples of a sound.
typedef UInt64 SoundSize;




using om::math::AABB1f;
using om::math::Vector2f;
using om::math::Vector3f;
using om::math::Matrix3f;
using om::math::FFTReal;
using om::math::FFTComplex;




//##########################################################################################
//*****************************  End Om Sound Namespace  ***********************************
OM_SOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CONFIG_H
