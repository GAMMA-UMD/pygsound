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

#ifndef INCLUDE_OM_SOUND_PLUGIN_TYPE_MANAGER_H
#define INCLUDE_OM_SOUND_PLUGIN_TYPE_MANAGER_H


#include "omSoundPluginsConfig.h"


#include "omSoundPluginType.h"
#include "omSoundPluginID.h"
#include "omSoundPluginDelegate.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An interface for a class that manages a collection of plugins of a particular type.
class PluginTypeManager
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new plugin type manager, initially empty without any plugins.
			PluginTypeManager();
			
			
			/// Create a new plugin type manager, copying all plugin information from the another manager.
			PluginTypeManager( const PluginTypeManager& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this plugin type manager and release all of its internal resources.
			virtual ~PluginTypeManager();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the entire state of one plugin type manager to this one, replacing all previous plugins.
			PluginTypeManager& operator = ( const PluginTypeManager& other );
			
			
		//********************************************************************************
		//******	Plugin Type Accessor Methods
			
			
			/// Return the type of plugin that this manager handles.
			/**
			  * Examples of plugin types include 'AU', 'VST', and 'Filter'. Subclasses
			  * should implement this method to return the type of plugin that they handle.
			  */
			virtual const PluginType& getType() const = 0;
			
			
		//********************************************************************************
		//******	Plugin Rescan Method
			
			
			/// Rescan the system for available plugins of this manager's type.
			/**
			  * This method has the effect of updating the internal list of plugins with any
			  * newly detected plugins while retaining all previously existing plugins, as
			  * long as their presence on the system is retained. If a plugin no longer exists,
			  * it is removed from the internal list of plugins.
			  *
			  * This method is called the first time that the user accesses the plugins for
			  * this plugin type manager, and can be called afterward by the user if necessary.
			  * Subclasses should implement this method to function properly for those situations.
			  *
			  * This method invalidates all plugin indices for this manager.
			  */
			virtual void rescan() = 0;
			
			
		//********************************************************************************
		//******	Plugin Accessor Methods
			
			
			/// Return the total number of plugins that are available to this plugin type manager.
			Size getPluginCount() const;
			
			
			/// Return the plugin ID for the plugin at the specified index.
			/**
			  * If the plugin index is invalid, an undefined ID is returned.
			  */
			const PluginID& getPluginID( Index pluginIndex ) const;
			
			
			/// Return a string representing the name for the plugin at the specified index.
			/**
			  * If the plugin index is invalid, the empty string is returned.
			  */
			const UTF8String& getPluginName( Index pluginIndex ) const;
			
			
			/// Get the index of the plugin in this manager with the specified ID.
			/**
			  * The plugin index is returned in the output index parameter, and a boolean value is returned
			  * indicating whether or not the index for the given plugin was successfully retrieved.
			  */
			Bool getPluginIndex( const PluginID& pluginID, Index& pluginIndex ) const;
			
			
			/// Return whether or not there exists a plugin in this plugin manager with the specified manufacturer and name.
			Bool hasPlugin( const PluginID& pluginID ) const;
			
			
			/// Add a new kind of plugin to this plugin manager.
			/**
			  * This method adds a new plugin kind with the specified ID
			  * and delegate to this plugin type manager. The method allows the user
			  * to provide a pointer to data that is passed into the factory function when a plugin
			  * with this description is created.
			  *
			  * If the new plugin was able to be registered, TRUE is returned. Otherwise, if
			  * there is an error, FALSE is returned and the method has no effect.
			  */
			Bool addPlugin( const PluginID& pluginID, const PluginDelegate& delegate, void* userData = NULL );
			
			
			/// Remove the plugin at the specified index from this plugin manager.
			/**
			  * The method returns whether or not the specified plugin was removed from this manager.
			  * If the method succeeds, it invalidates all plugin indices for this manager.
			  */
			Bool removePlugin( Index pluginIndex );
			
			
			/// Remove all registered plugins from this plugin type manager.
			/**
			  * This method invalidates all plugin indices for this manager.
			  */
			void clearPlugins();
			
			
		//********************************************************************************
		//******	Plugin Creation Methods
			
			
			/// Create a new plugin of this manager's type for the specified plugin kind index.
			/**
			  * If there is an error in creating the new plugin, a NULL pointer is returned. This
			  * can happen if the specified plugin index is invalid, or if an error occurred
			  * within the plugin's factory function.
			  */
			SoundFilter* createPlugin( Index pluginIndex ) const;
			
			
			/// Create a new plugin of this manager's type with the given manufacturer and plugin names.
			/**
			  * If there is an error in creating the new plugin, a NULL pointer is returned. This
			  * can happen if there is no plugin registered with the given manufacturer and plugin
			  * name, or if an error occurred within the plugin's factory function.
			  * The manufacturer and plugin name comparisons are case-insensitive.
			  */
			SoundFilter* createPlugin( const PluginID& pluginID ) const;
			
			
		//********************************************************************************
		//******	Plugin View Creation Methods
			
			
			/// Create a new plugin view of this manager's type for the specified plugin kind index.
			/**
			  * If there is an error in creating the new plugin view, a NULL pointer is returned. This
			  * can happen if the specified plugin index is invalid, or if an error occurred
			  * within the plugin's factory function.
			  */
			SoundFilterView* createPluginView( Index pluginIndex, SoundFilter* filter ) const;
			
			
			/// Create a new plugin view of this manager's type with the given manufacturer and plugin names.
			/**
			  * If there is an error in creating the new plugin view, a NULL pointer is returned. This
			  * can happen if there is no plugin registered with the given manufacturer and plugin
			  * name, or if an error occurred within the plugin's factory function.
			  * The manufacturer and plugin name comparisons are case-insensitive.
			  */
			SoundFilterView* createPluginView( const PluginID& pluginID, SoundFilter* filter ) const;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Helper Methods
			
			
			/// Add a new kind of plugin to this plugin manager.
			/**
			  * This method adds a new plugin kind with the specified manufacturer name, plugin name,
			  * and delegate to this plugin type manager. The method allows the user
			  * to provide a pointer to data that is passed into the factory function when a plugin
			  * with this description is created.
			  *
			  * If the new plugin was able to be registered, TRUE is returned. Otherwise, if
			  * there is an error, FALSE is returned and the method has no effect.
			  *
			  * Subclasses should use this method to register new plugins to the base class.
			  */
			Bool addPluginInternal( const PluginID& id, const PluginDelegate& delegate, void* userData = NULL );
			
			
			/// Remove the plugin at the specified index from this plugin manager.
			/**
			  * The method returns whether or not the specified plugin was removed from this manager.
			  * If the method succeeds, it invalidates all plugin indices for this manager.
			  *
			  * Subclasses should use this method to remove plugins from the base class when they
			  * are disconnected and unable to be used.
			  */
			Bool removePluginInternal( Index pluginIndex );
			
			
			/// Remove all registered plugins from this plugin type manager.
			/**
			  * This method invalidates all plugin indices for this manager.
			  *
			  * Subclasses should use this method to cheaply remove all previously present
			  * plugins from the plugin manager.
			  */
			void clearPluginsInternal();
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that holds information about a particular plugin.
			class PluginInfo;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the plugins that are registered with this filter plugin manager.
			ArrayList<PluginInfo*> plugins;
			
			
			/// A hash map from plugin IDs to plugin info pointers which allows efficient name lookup access for plugins.
			HashMap<PluginID,PluginInfo*> idMap;
			
			
			/// A boolean value indicating whether or not the plugin type manager has already called the subclasses's rescan() method.
			mutable Bool hasScannedPlugins;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PLUGIN_TYPE_MANAGER_H
