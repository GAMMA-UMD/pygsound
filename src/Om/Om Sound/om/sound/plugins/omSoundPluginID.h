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

#ifndef INCLUDE_OM_SOUND_PLUGIN_ID_H
#define INCLUDE_OM_SOUND_PLUGIN_ID_H


#include "omSoundPluginsConfig.h"


#include "omSoundPluginType.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents that unique identifier for an external sound filter plugin.
class PluginID
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default empty plugin ID.
			PluginID();
			
			
			/// Create a new plugin ID with the specified type, manufacturer, and name.
			PluginID( const PluginType& newType, const UTF8String& newManufacturer, const UTF8String& newName );
			
			
		//********************************************************************************
		//******	Type Accessor Methods
			
			
			/// Return a string that uniquely represents this plugin's type identifier.
			OM_INLINE const PluginType& getType() const
			{
				return type;
			}
			
			
			/// Set a string that uniquely represents this plugin's type identifier.
			OM_INLINE void setType( const PluginType& newType )
			{
				type = newType;
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a string that uniquely represents this plugin's name identifier.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set a string that uniquely represents this plugin's name identifier.
			OM_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Manufacturer Accessor Methods
			
			
			/// Return a string that uniquely represents the name of this plugin's manufacturer ID.
			OM_INLINE const UTF8String& getManufacturer() const
			{
				return manufacturer;
			}
			
			
			/// Set a string that uniquely represents the name of this plugin's manufacturer ID.
			OM_INLINE void setManufacturer( const UTF8String& newManufacturer )
			{
				manufacturer = newManufacturer;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this plugin ID is equivalent to another.
			/**
			  * The type, manufacturer and plugin name comparisons are case-insensitive.
			  */
			OM_INLINE Bool operator == ( const PluginID& other ) const
			{
				return type == other.type && manufacturer.equalsIgnoreCase( other.manufacturer ) &&
						name.equalsIgnoreCase( other.name );
			}
			
			
			/// Return whether or not this plugin ID is not equivalent to another.
			/**
			  * The type, manufacturer and plugin name comparisons are case-insensitive.
			  */
			OM_INLINE Bool operator != ( const PluginID& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash-code value for this plugin ID.
			OM_INLINE Hash getHashCode() const
			{
				return type.getHashCode() ^ manufacturer.getHashCode() ^ name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// An object that represents an invalid plugin ID.
			static const PluginID INVALID;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string representing the plugin type of this plugin ID.
			PluginType type;
			
			
			/// A string representing the manufacturer of this plugin ID.
			UTF8String manufacturer;
			
			
			/// A string representing the name of this plugin ID.
			UTF8String name;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PLUGIN_ID_H
