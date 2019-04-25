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

#ifndef INCLUDE_OM_SOUND_PLUGIN_MANAGER_H
#define INCLUDE_OM_SOUND_PLUGIN_MANAGER_H


#include "omSoundPluginsConfig.h"


#include "omSoundPluginType.h"
#include "omSoundPluginID.h"
#include "omSoundPluginTypeManager.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that manages generic access to of any kind of audio plugin.
/**
  * The plugin manager allows the user to register other types of plugins
  * so that the PluginManager can access filters, AU plugins, VST plugins, and others.
  */
class PluginManager
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default plugin manager which has access to plugins with the PluginType::FILTER type.
			PluginManager();
			
			
		//********************************************************************************
		//******	Plugin Type Accessor Methods
			
			
			/// Return the number of plugin types that this plugin manager has access to.
			Size getPluginTypeCount() const;
			
			
			/// Return the plugin type at the specified index that this manager has access to.
			/**
			  * If the specified plugin type index is invalid, the PluginType::UNDEFINED is returned.
			  */
			const PluginType& getPluginType( Index pluginTypeIndex ) const;
			
			
			/// Return a shared pointer to the plugin type manager at the specified index that this manager has access to.
			/**
			  * If the specified plugin type index is invalid, a NULL pointer is returned.
			  */
			PluginTypeManager* getPluginTypeManager( Index pluginTypeIndex ) const;
			
			
			/// Return a shared pointer to the plugin type manager for the given plugin type.
			/**
			  * If there is no plugin type manager for the given plugin type, a NULL
			  * pointer is returned.
			  */
			PluginTypeManager* getPluginTypeManager( const PluginType& pluginType ) const;
			
			
			/// Return whether or not this plugin manager supports the specified plugin type.
			Bool supportsPluginType( const PluginType& pluginType ) const;
			
			
			/// Add a new plugin type manager to this plugin manager.
			/**
			  * The method uses the new type manager's PluginType, as returned by PluginTypeManager::getType(),
			  * to store the plugin type manager. If there was already a manager for that type,
			  * it is replaced with the new one. Otherwise, if the plugin type is new, the
			  * manager is added as a new plugin type.
			  *
			  * If the method is successful and the new manager is added, TRUE is returned. Otherwise,
			  * if the new type manager pointer is NULL or if there is some other error, FALSE is
			  * returned.
			  */
			Bool addPluginType( PluginTypeManager* newTypeManager );
			
			
			/// Remove the specified plugin type from this plugin manager, along with the associated PluginTypeManager.
			/**
			  * The method returns whether or not the remove operation was successful.
			  */
			Bool removePluginType( const PluginType& type );
			
			
			/// Remove all previously registered plugin types from this plugin manager.
			void clearPluginTypes();
			
			
		//********************************************************************************
		//******	Plugin Accessor Methods
			
			
			/// Return the total number of plugins that are available to this plugin manager.
			Size getPluginCount() const;
			
			
		//********************************************************************************
		//******	Plugin Creation Methods
			
			
			/// Create a new plugin of the specified type with the given plugin ID.
			/**
			  * If there is an error in creating the new plugin, a NULL pointer is returned. This
			  * can happen if there is no plugin registered with the given ID, or if an error
			  * occurred within the plugin's factory function.
			  */
			SoundFilter* createPlugin( const PluginID& pluginID ) const;
			
			
			/// Create a new plugin view of the specified type with the given plugin ID.
			/**
			  * If there is an error in creating the new plugin view, a NULL pointer is returned. This
			  * can happen if there is no plugin registered with the given type, manufacturer and plugin
			  * name, or if an error occurred within the plugin's factory function.
			  * The type, manufacturer and plugin name comparisons are case-insensitive.
			  */
			SoundFilterView* createPluginView( const PluginID& pluginID, SoundFilter* filter ) const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of plugin managers for each type of plugin this manager support.
			ArrayList<PluginTypeManager*> pluginTypes;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PLUGIN_MANAGER_H
