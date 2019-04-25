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

#include "omSoundPluginTypeManager.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Info Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class PluginTypeManager:: PluginInfo
{
	public:
		
		/// Create a new plugin info structure
		OM_INLINE PluginInfo( const PluginID& newID, const PluginDelegate& newDelegate, void* newUserData )
			:	id( newID ),
				delegate( newDelegate ),
				userData( newUserData )
		{
		}
		
		
		
		
		/// The ID of the plugin for this plugin info.
		PluginID id;
		
		
		/// A delegate that is used to create instances of this plugin.
		PluginDelegate delegate;
		
		
		/// A pointer to user data that is passed into the plugin delegate functions.
		void* userData;
		
		
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




PluginTypeManager:: PluginTypeManager()
	:	plugins(),
		idMap(),
		hasScannedPlugins( false )
{
}




PluginTypeManager:: PluginTypeManager( const PluginTypeManager& other )
	:	plugins( other.plugins.getCapacity() ),
		idMap(),
		hasScannedPlugins( false )
{
	// Copy all plugin information.
	const Size numPlugins = other.plugins.getSize();
	
	for ( Index i = 0; i < numPlugins; i++ )
	{
		const PluginInfo* info = other.plugins[i];
		
		this->addPluginInternal( info->id, info->delegate, info->userData );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################





PluginTypeManager:: ~PluginTypeManager()
{
	// Iterate over the plugins and destroy all of the plugin info structures.
	const Size numPlugins = plugins.getSize();
	
	for ( Index i = 0; i < numPlugins; i++ )
		om::util::destruct( plugins[i] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




PluginTypeManager& PluginTypeManager:: operator = ( const PluginTypeManager& other )
{
	if ( this != &other )
	{
		this->clearPluginsInternal();
		
		// Copy all plugin information.
		const Size numPlugins = other.plugins.getSize();
		
		for ( Index i = 0; i < numPlugins; i++ )
		{
			const PluginInfo* info = other.plugins[i];
			
			this->addPluginInternal( info->id, info->delegate, info->userData );
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Accesor Methods
//############		
//##########################################################################################
//##########################################################################################




Size PluginTypeManager:: getPluginCount() const
{
	// Scan for plugins preset if it hasn't been done.
	if ( !hasScannedPlugins )
	{
		hasScannedPlugins = true;
		const_cast<PluginTypeManager*>(this)->rescan();
	}
	
	return plugins.getSize();
}




const PluginID& PluginTypeManager:: getPluginID( Index pluginIndex ) const
{
	// Scan for plugins preset if it hasn't been done.
	if ( !hasScannedPlugins )
	{
		hasScannedPlugins = true;
		const_cast<PluginTypeManager*>(this)->rescan();
	}
	
	if ( pluginIndex < plugins.getSize() )
		return plugins[pluginIndex]->id;
	else
		return PluginID::INVALID;
}




Bool PluginTypeManager:: getPluginIndex( const PluginID& pluginID, Index& pluginIndex ) const
{
	// Scan for plugins preset if it hasn't been done.
	if ( !hasScannedPlugins )
	{
		hasScannedPlugins = true;
		const_cast<PluginTypeManager*>(this)->rescan();
	}
	
	// Use the ID map lookup to avoid O(n) string comparisons.
	PluginInfo* const * pluginInfo;
	
	if ( !idMap.find( pluginID.getHashCode(), pluginID, pluginInfo ) )
		return false;
	
	// Find the plugin with the same info pointer in the list. This is still O(n) but much faster.
	const Size numPlugins = plugins.getSize();
	
	for ( Index i = 0; i < numPlugins; i++ )
	{
		if ( plugins[i] == *pluginInfo )
		{
			pluginIndex = i;
			return true;
		}
	}
	
	return false;
}




Bool PluginTypeManager:: hasPlugin( const PluginID& pluginID ) const
{
	Index pluginIndex;
	return this->getPluginIndex( pluginID, pluginIndex );
}




Bool PluginTypeManager:: addPlugin( const PluginID& pluginID, const PluginDelegate& delegate, void* userData )
{
	// Scan for plugins preset if it hasn't been done.
	if ( !hasScannedPlugins )
	{
		hasScannedPlugins = true;
		this->rescan();
	}
	
	// Make sure the filter isn't aready preset, If so, remove it.
	Index pluginIndex;
	
	if ( this->getPluginIndex( pluginID, pluginIndex ) )
		this->removePlugin( pluginIndex );
	
	return this->addPluginInternal( pluginID, delegate, userData );
}




Bool PluginTypeManager:: addPluginInternal( const PluginID& id, const PluginDelegate& delegate, void* userData )
{
	// Create the new plugin info object.
	PluginInfo* pluginInfo = om::util::construct<PluginInfo>( id, delegate, userData );
	
	// Add the plugin info object to the list of plugins.
	plugins.add( pluginInfo );
	
	// Add the ID to plugin info object mapping to the ID map.
	idMap.add( id.getHashCode(), id, pluginInfo );
	
	return true;
}




Bool PluginTypeManager:: removePlugin( Index pluginIndex )
{
	// Scan for plugins preset if it hasn't been done.
	if ( !hasScannedPlugins )
	{
		hasScannedPlugins = true;
		this->rescan();
	}
	
	return this->removePluginInternal( pluginIndex );
}




Bool PluginTypeManager:: removePluginInternal( Index pluginIndex )
{
	// Make sure the remove index is in the valid bounds, if not, return.
	if ( pluginIndex >= plugins.getSize() )
		return false;
	
	PluginInfo* pluginInfo = plugins[pluginIndex];
	
	// Remove the plugin info from the ID map.
	idMap.remove( pluginInfo->id.getHashCode(), pluginInfo->id );
	
	// Remove the plugin from the plugin list.
	plugins.removeAtIndexUnordered( pluginIndex );
	
	// Destory the plugin info object.
	om::util::destruct( pluginInfo );
	
	return true;
}




void PluginTypeManager:: clearPlugins()
{
	this->clearPluginsInternal();
}




void PluginTypeManager:: clearPluginsInternal()
{
	// Iterate over the plugins and destroy all of the plugin info structures.
	const Size numPlugins = plugins.getSize();
	
	for ( Index i = 0; i < numPlugins; i++ )
		om::util::destruct( plugins[i] );
	
	// Clear the list of plugins and the plugin ID map.
	plugins.clear();
	idMap.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Creation Methods
//############		
//##########################################################################################
//##########################################################################################




SoundFilter* PluginTypeManager:: createPlugin( Index pluginIndex ) const
{
	if ( pluginIndex >= plugins.getSize() )
		return NULL;
	
	const PluginInfo* pluginInfo = plugins[pluginIndex];
	
	// Make sure the plugin creation delegate function is set.
	if ( pluginInfo->delegate.create.isNull() )
		return NULL;
	
	return pluginInfo->delegate.create( pluginInfo->id, pluginInfo->userData );
}




SoundFilter* PluginTypeManager:: createPlugin( const PluginID& pluginID ) const
{
	// Get the index of the plugin with this ID.
	Index pluginIndex;
	
	if ( !this->getPluginIndex( pluginID, pluginIndex ) )
		return NULL;
	
	return this->createPlugin( pluginIndex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin View Creation Methods
//############		
//##########################################################################################
//##########################################################################################




SoundFilterView* PluginTypeManager:: createPluginView( Index pluginIndex, SoundFilter* filter ) const
{
	if ( pluginIndex >= plugins.getSize() || filter == NULL )
		return NULL;
	
	const PluginInfo* pluginInfo = plugins[pluginIndex];
	
	// Make sure the plugin view creation delegate function is set.
	if ( pluginInfo->delegate.createView.isNull() )
		return NULL;
	
	return pluginInfo->delegate.createView( pluginInfo->id, filter, pluginInfo->userData );
}




SoundFilterView* PluginTypeManager:: createPluginView( const PluginID& pluginID, SoundFilter* filter ) const
{
	if ( filter == NULL )
		return NULL;
	
	// Get the index of the plugin with this ID.
	Index pluginIndex;
	
	if ( !this->getPluginIndex( pluginID, pluginIndex ) )
		return NULL;
	
	return this->createPluginView( pluginIndex, filter );
}




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
