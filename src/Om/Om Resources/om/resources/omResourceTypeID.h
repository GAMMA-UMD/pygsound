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

#ifndef INCLUDE_OM_RESOURCE_TYPE_ID_H
#define INCLUDE_OM_RESOURCE_TYPE_ID_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the complete type of a resource object.
/**
  * A type ID consists of a tuple of resource category, resource type, and resource subtype.
  * If any of these is UNDEFINED, it reduces the specificity of the ID.
  */
class ResourceTypeID
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resource type ID with UNDEFINED category, type, and subtype.
			OM_INLINE ResourceTypeID()
				:	category( ResourceCategory::UNDEFINED ),
					type( ResourceType::UNDEFINED ),
					subType( ResourceSubType::UNDEFINED )
			{
			}
			
			
			/// Creata new resource type ID that selects any resource type belonging to the given category.
			OM_INLINE ResourceTypeID( const ResourceCategory& newCategory )
				:	category( newCategory ),
					type( ResourceType::UNDEFINED ),
					subType( ResourceSubType::UNDEFINED )
			{
			}
			
			
			/// Creata new resource type ID that selects any subtype of the given resource type.
			OM_INLINE ResourceTypeID( const ResourceType& newType )
				:	category( ResourceCategory::UNDEFINED ),
					type( newType ),
					subType( ResourceSubType::UNDEFINED )
			{
			}
			
			
			/// Creata new resource type ID that selects only a given subtype of the given resource type.
			OM_INLINE ResourceTypeID( const ResourceType& newType, const ResourceSubType& newSubType )
				:	category( ResourceCategory::UNDEFINED ),
					type( newType ),
					subType( newSubType )
			{
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource type is equal to another.
			OM_INLINE Bool operator == ( const ResourceTypeID& other ) const
			{
				return category == other.category && type == other.type && subType == other.subType;
			}
			
			
			/// Return whether or not this resource type is equal to another.
			OM_INLINE Bool operator != ( const ResourceTypeID& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this resource type ID.
			OM_INLINE Hash getHashCode() const
			{
				return HashCode( category.getHashCode(), type.getHashCode(), subType.getHashCode() );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The category that the resource type ID belongs to.
			ResourceCategory category;
			
			
			/// The resource type of the type ID.
			ResourceType type;
			
			
			/// The subtype of the resource type ID.
			ResourceSubType subType;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_TYPE_ID_H
