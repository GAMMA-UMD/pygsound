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

#ifndef INCLUDE_OM_RESOURCE_FORMAT_H
#define INCLUDE_OM_RESOURCE_FORMAT_H


#include "omResourcesConfig.h"


#include "omResourceType.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class that specifies a storage format for a resource and its attributes.
class ResourceFormat
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an UNDEFINED resource format.
			ResourceFormat();
			
			
			/// Create a new resource format with the specified resource format name and no extensions.
			ResourceFormat( const data::UTF8String& newName );
			
			
			/// Create a new resource format with the specified resource format name and extension.
			ResourceFormat( const data::UTF8String& newName, const data::UTF8String& newExtension );
			
			
			/// Create a new resource format with the specified resource format name and extensions.
			ResourceFormat( const data::UTF8String& newName, const data::UTF8String& newExtension1,
							const data::UTF8String& newExtension2 );
			
			
			/// Create a new resource format with the specified resource format name and extensions.
			ResourceFormat( const data::UTF8String& newName, const data::UTF8String& newExtension1,
							const data::UTF8String& newExtension2, const data::UTF8String& newExtension3 );
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return the unique name string for this resource format.
			OM_INLINE const data::UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set the unique name string for this resource format.
			OM_INLINE void setName( const data::UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Format Extensions Accessor Methods
			
			
			/// Return the number of possible extensions that this resource format has.
			OM_INLINE Size getExtensionCount() const
			{
				return extensions.getSize();
			}
			
			
			/// Return the file extension at the given index that can be used for this format.
			OM_INLINE const data::UTF8String& getExtension( Index index ) const
			{
				return extensions[index];
			}
			
			
			/// Return whether or not the specified string is a valid extension for this format.
			Bool hasExtension( const data::UTF8String& newExtension ) const;
			
			
			/// Add a new extension to this resource format.
			void addExtension( const data::UTF8String& newExtension );
			
			
			/// Clear all extensions from this resource format.
			void clearExtensions();
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this resource format.
			OM_INLINE Hash getHashCode() const
			{
				return name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource format is equal to another.
			OM_INLINE Bool operator == ( const ResourceFormat& other ) const
			{
				return name == other.name;
			}
			
			
			/// Return whether or not this resource format is equal to another.
			OM_INLINE Bool operator != ( const ResourceFormat& other ) const
			{
				return name != other.name;
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The resource type to use for an undefined resource format.
			static const ResourceFormat UNDEFINED;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string that uniquely describes the name of this resource format.
			data::UTF8String name;
			
			
			/// A list of strings that are valid extensions for this format.
			util::ShortArrayList<data::UTF8String,1> extensions;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_FORMAT_H
