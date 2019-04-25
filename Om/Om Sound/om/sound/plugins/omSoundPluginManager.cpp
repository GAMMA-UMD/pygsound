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

#include "omSoundPluginManager.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




PluginManager:: PluginManager()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Type Accesor Methods
//############		
//##########################################################################################
//##########################################################################################




Size PluginManager:: getPluginTypeCount() const
{
	return pluginTypes.getSize();
}




const PluginType& PluginManager:: getPluginType( Index pluginTypeIndex ) const
{
	if ( pluginTypeIndex < pluginTypes.getSize() )
		return pluginTypes[pluginTypeIndex]->getType();
	else
		return PluginType::UNDEFINED;
}




PluginTypeManager* PluginManager:: getPluginTypeManager( Index pluginTypeIndex ) const
{
	if ( pluginTypeIndex < pluginTypes.getSize() )
		return pluginTypes[pluginTypeIndex];
	else
		return NULL;
}




PluginTypeManager* PluginManager:: getPluginTypeManager( const PluginType& type ) const
{
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == type )
			return pluginTypes[i];
	}
	
	return NULL;
}




Bool PluginManager:: supportsPluginType( const PluginType& type ) const
{
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == type )
			return true;
	}
	
	return false;
}




Bool PluginManager:: addPluginType( PluginTypeManager* newTypeManager )
{
	// Make sure the new type manager isn't NULL.
	if ( newTypeManager == NULL )
		return false;
	
	// Check to see if the type manager already exists.
	const Size numTypes = pluginTypes.getSize();
	const PluginType newType = newTypeManager->getType();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == newType )
		{
			// Replace the previous one for this plugin type.
			pluginTypes[i] = newTypeManager;
			
			return true;
		}
	}
	
	// Otherwise add the type to the end of the list.
	pluginTypes.add( newTypeManager );
	
	return true;
}




Bool PluginManager:: removePluginType( const PluginType& type )
{
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == type )
		{
			pluginTypes.removeAtIndexUnordered(i);
			return true;
		}
	}
	
	return false;
}




void PluginManager:: clearPluginTypes()
{
	pluginTypes.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Accesor Methods
//############		
//##########################################################################################
//##########################################################################################




Size PluginManager:: getPluginCount() const
{
	// Count the total number of plugins.
	Size totalNumPlugins = 0;
	
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
		totalNumPlugins += pluginTypes[i]->getPluginCount();
	
	return totalNumPlugins;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Plugin Creation Methods
//############		
//##########################################################################################
//##########################################################################################




SoundFilter* PluginManager:: createPlugin( const PluginID& pluginID ) const
{
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == pluginID.getType() )
			return pluginTypes[i]->createPlugin( pluginID );
	}
	
	return NULL;
}




SoundFilterView* PluginManager:: createPluginView( const PluginID& pluginID, SoundFilter* filter ) const
{
	const Size numTypes = pluginTypes.getSize();
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		if ( pluginTypes[i]->getType() == pluginID.getType() )
			return pluginTypes[i]->createPluginView( pluginID, filter );
	}
	
	return NULL;
}




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
