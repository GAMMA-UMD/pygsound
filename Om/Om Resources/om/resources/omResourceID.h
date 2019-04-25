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

#ifndef INCLUDE_OM_RESOURCE_ID_H
#define INCLUDE_OM_RESOURCE_ID_H


#include "omResourcesConfig.h"


#include "omResourceType.h"
#include "omResourceFormat.h"
#include "omResourceFlags.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// The integer type to use for file-local resource ID numbers.
typedef UInt64 ResourceLocalID;




//********************************************************************************
/// A class that represents a unique identifier for a resource.
/**
  * A resource is specified by a path to a file, an enum determining how that file
  * should be interpreted (its type), the type of resource that this ID corresponds to,
  * and an optional name that is used to identify the resource within the file.
  */
class ResourceID
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resource ID which doesn't point to a valid resource.
			OM_INLINE ResourceID()
				:	flags( ResourceFlags::DEFAULT )
			{
			}
			
			
			/// Create a new resource ID with the specified resource file path.
			OM_INLINE ResourceID( const data::UTF8String& newURL )
				:	url( newURL ),
					flags( ResourceFlags::DEFAULT )
			{
			}
			
			
			/// Create a new resource ID with the specified attributes.
			OM_INLINE ResourceID( const ResourceType& newType, const data::UTF8String& newURL )
				:	type( newType ),
					url( newURL ),
					flags( ResourceFlags::DEFAULT )
			{
			}
			
			
			/// Create a new resource ID with the specified attributes.
			OM_INLINE ResourceID( const ResourceType& newType, const ResourceFormat& newFormat,
									const data::UTF8String& newURL )
				:	type( newType ),
					format( newFormat ),
					url( newURL ),
					flags( ResourceFlags::DEFAULT )
			{
			}
			
			
			/// Create a new resource ID with the specified attributes.
			OM_INLINE ResourceID( const ResourceType& newType, const ResourceFormat& newFormat,
									const data::UTF8String& newURL, const data::UTF8String& newName,
									const ResourceFlags& newFlags = ResourceFlags::DEFAULT )
				:	type( newType ),
					format( newFormat ),
					url( newURL ),
					name( newName ),
					flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return an object describing the type of resource this ID refers to.
			OM_INLINE const ResourceType& getType() const
			{
				return type;
			}
			
			
			/// Set an object describing the type of resource this ID refers to.
			OM_INLINE void setType( const ResourceType& newType )
			{
				type = newType;
			}
			
			
		//********************************************************************************
		//******	Resource File Type Accessor Methods
			
			
			/// Return an object describing the format of file this ID refers to.
			OM_INLINE const ResourceFormat& getFormat() const
			{
				return format;
			}
			
			
			/// Set an object describing the format of file this ID refers to.
			OM_INLINE void setFormat( const ResourceFormat& newFormat )
			{
				format = newFormat;
			}
			
			
		//********************************************************************************
		//******	File Path String Accessor Methods
			
			
			/// Return a reference to the UTF-8 encoded string representing the path to this resource.
			OM_INLINE const data::UTF8String& getURL() const
			{
				return url;
			}
			
			
			/// Set a UTF-8 encoded string representing a path to this resource.
			OM_INLINE void setURL( const data::UTF8String& newURL )
			{
				url = newURL;
			}
			
			
		//********************************************************************************
		//******	Resource Name Accessor Methods
			
			
			/// Return a UTF-8 encoded string representing the name of the resource for this ID.
			OM_INLINE const data::UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set a UTF-8 encoded string representing the name of the resource for this ID.
			OM_INLINE void setName( const data::UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Flags Accessor Methods
			
			
			/// Return a reference to an object that contains boolean parameters of the resource.
			OM_INLINE ResourceFlags& getFlags()
			{
				return flags;
			}
			
			
			/// Return an object that contains boolean parameters of the resource.
			OM_INLINE const ResourceFlags& getFlags() const
			{
				return flags;
			}
			
			
			/// Set an object that contains boolean parameters of the resource.
			OM_INLINE void setFlags( const ResourceFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
			/// Return whether or not the specified boolan flag is set for this resource.
			OM_INLINE Bool flagIsSet( ResourceFlags::Flag flag ) const
			{
				return flags.isSet( flag );
			}
			
			
			/// Set whether or not the specified boolan flag is set for this resource.
			OM_INLINE void setFlag( ResourceFlags::Flag flag, Bool newIsSet = true )
			{
				flags.set( flag, newIsSet );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource ID is equal to another.
			OM_INLINE Bool operator == ( const ResourceID& other ) const
			{
				if ( url != other.url )
					return false;
				
				if ( type != ResourceType::UNDEFINED && other.type != ResourceType::UNDEFINED && type != other.type )
					return false;
				
				if ( name.getLength() != 0 && other.name.getLength() != 0 && name != other.name )
					return false;
				
				return true;
			}
			
			
			/// Return whether or not this resource ID is not equal to another.
			OM_INLINE Bool operator != ( const ResourceID& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Return an integer hash code for this resource ID.
			OM_INLINE Hash getHashCode() const
			{
				return type.getHashCode() ^ format.getHashCode() ^ url.getHashCode() ^ name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The invalid local ID that indicates the local ID is unused.
			static const ResourceLocalID INVALID_LOCAL_ID = ResourceLocalID(Int64(-1));
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object describing the type of this resource.
			ResourceType type;
			
			
			/// An object describing the format of this resource's file, how it should be interpreted.
			ResourceFormat format;
			
			
			/// A string representing a universal locator for the resource.
			data::UTF8String url;
			
			
			/// The optional name of the resource within the file.
			data::UTF8String name;
			
			
			/// An object containing boolean configuration flags for this resource.
			ResourceFlags flags;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_ID_H
