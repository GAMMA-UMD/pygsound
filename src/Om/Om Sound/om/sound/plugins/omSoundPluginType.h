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

#ifndef INCLUDE_OM_SOUND_PLUGIN_TYPE_H
#define INCLUDE_OM_SOUND_PLUGIN_TYPE_H


#include "omSoundPluginsConfig.h"


//##########################################################################################
//**************************  Start Om Sound Plugins Namespace  ****************************
OM_SOUND_PLUGINS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents of a particular type of plugin, such as AU, VST, or filter.
class PluginType
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new plugin type with an undefined type string, the empty string.
			OM_INLINE PluginType()
				:	name()
			{
			}
			
			
			/// Create a new plugin type with the specified type name string.
			OM_INLINE PluginType( const UTF8String& newName )
				:	name( newName )
			{
			}
			
			
			/// Create a new plugin type with the specified constant type string.
			OM_INLINE PluginType( const char* newName )
				:	name( newName )
			{
			}
			
			
		//********************************************************************************
		//******	Type String Accessor Methods
			
			
			/// Return a reference to a string which uniquely represents the name of this plugin type.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set a reference to a string which uniquely represents the name of this plugin type.
			OM_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash-code value for this plugin type.
			OM_INLINE Hash getHashCode() const
			{
				return name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this plugin type is equivalent to another.
			/**
			  * The plugin types are equal if the type name strings are equal. The
			  * string are compared in a case-insensitive manner.
			  */
			OM_INLINE Bool operator == ( const PluginType& other ) const
			{
				return name.equalsIgnoreCase( other.name );
			}
			
			
			/// Return whether or not this plugin type is not equivalent to another.
			/**
			  * The plugin types are not equal if the type name strings are not equal. The
			  * string are compared in a case-insensitive manner.
			  */
			OM_INLINE Bool operator != ( const PluginType& other ) const
			{
				return !name.equalsIgnoreCase( other.name );
			}
			
			
		//********************************************************************************
		//******	Public Static Type Objects
			
			
			/// A plugin type object that represents an undefined plugin architecture.
			static const PluginType UNDEFINED;
			
			
			/// A plugin type object that represents the built-in SoundFilter plugin architecture.
			static const PluginType FILTER;
			
			
			/// A plugin type object that represents Apple's Audio Unit (AU) plugin architecture.
			static const PluginType AU;
			
			
			/// A plugin type object that represents Steinberg's Virtual Studio Technology (VST) plugin architecture.
			static const PluginType VST;
			
			
			/// A plugin type object that represents Avid's Real Time AudioSuite (RTAS) plugin architecture.
			static const PluginType RTAS;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string which holds the name of this plugin type.
			UTF8String name;
			
			
			
};




//##########################################################################################
//**************************  End Om Sound Plugins Namespace  ******************************
OM_SOUND_PLUGINS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PLUGIN_TYPE_H
