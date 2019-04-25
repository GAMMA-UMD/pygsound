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

#ifndef INCLUDE_OM_SOUND_PLUGIN_H
#define INCLUDE_OM_SOUND_PLUGIN_H


#include "omSoundPluginsConfig.h"


#include "omSoundPluginID.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that opaquely wraps an arbitrary sound effect that has a SoundFilter interface.
class SoundPlugin
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default empty invalid sound plugin.
			SoundPlugin();
			
			
			/// Create a new sound plugin for the given plugin ID, but with no filter.
			SoundPlugin( const PluginID& newID );
			
			
			/// Create a new sound plugin for the given plugin ID and sound filter.
			SoundPlugin( const PluginID& newID, const Shared<SoundFilter>& newFilter );
			
			
		//********************************************************************************
		//******	Filter Accessor Methods
			
			
			/// Return a pointer to the sound filter this plugin is wrapping.
			OM_INLINE const Shared<SoundFilter>& getFilter() const
			{
				return filter;
			}
			
			
			/// Set a pointer to the sound filter this plugin is wrapping.
			OM_INLINE void setFilter( const Shared<SoundFilter>& newFilter )
			{
				filter = newFilter;
			}
			
			
		//********************************************************************************
		//******	ID Accessor Methods
			
			
			/// Return an object that uniquely represents this plugin's identifier.
			OM_INLINE const PluginID& getID() const
			{
				return puginID;
			}
			
			
			/// Set an object that uniquely represents this plugin's identifier.
			OM_INLINE void setID( const PluginID& newID )
			{
				puginID = newID;
			}
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a sound plugin.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that represents the plugin's unique identifier.
			PluginID puginID;
			
			
			/// A pointer to the sound filter that this plugin is wrapping.
			Shared<SoundFilter> filter;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::plugins::SoundPlugin, om::sound::plugins::SoundPlugin::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_PLUGIN_H
