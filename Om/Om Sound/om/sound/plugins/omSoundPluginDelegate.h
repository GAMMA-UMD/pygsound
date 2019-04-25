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

#ifndef INCLUDE_OM_SOUND_PLUGIN_DELEGATE_H
#define INCLUDE_OM_SOUND_PLUGIN_DELEGATE_H


#include "omSoundPluginsConfig.h"


#include "omSoundPluginType.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// The function prototype to use as a factory method for creating new plugin objects.
/**
  * The factory method should create a SoundFilter object that encapsulates a plugin
  * with the given plugin type, manufacturer name, and plugin name. The function is given
  * an opaque pointer to user-defined data which was passed into the plugin registration function.
  *
  * If the plugin is unable to be created, the function should return a NULL pointer.
  */
typedef Function<SoundFilter* ( const PluginID& pluginID, void* userData )> PluginFactory;




/// The function prototype to use as a factory method for creating new plugin view objects.
/**
  * The factory method should create a SoundFilterView object that encapsulates a plugin view
  * with the given plugin type, manufacturer name, and plugin name. The returned filter view
  * should wrap the given filter. The function is given an opaque pointer to user-defined data
  * which was passed into the plugin registration function.
  *
  * If the plugin is unable to be created, the function should return a NULL pointer.
  */
typedef Function<SoundFilterView* ( const PluginID& pluginID, SoundFilter* filter, void* userData )> PluginViewFactory;




//********************************************************************************
/// A class that handles events related to a particular sound plugin, such as instance creation.
class PluginDelegate
{
	public:
		
		//********************************************************************************
		//******	Creation Methods
			
			
			/// A delegate method that is called whenever the plugin host wants to create an instance of this plugin.
			PluginFactory create;
			
			
			/// A delegate method that is called whenever the plugin host wants to create an instance of this plugin's view.
			PluginViewFactory createView;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PLUGIN_DELEGATE_H
