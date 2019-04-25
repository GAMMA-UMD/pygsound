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

#ifndef INCLUDE_OM_RESOURCE_CATEGORY_H
#define INCLUDE_OM_RESOURCE_CATEGORY_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents the category of a generic resource.
/**
  * An resource category is defined by a unique string.
  */
class ResourceCategory
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resource category with an UNDEFINED category string.
			OM_INLINE ResourceCategory()
				:	name(UNDEFINED.getName())
			{
			}
			
			
			/// Create a new resource category with the specified constant NULL-terminated category string.
			OM_INLINE ResourceCategory( const Char* newName )
				:	name( newName )
			{
			}
			
			
			/// Create a new resource category with the specified category string.
			OM_INLINE ResourceCategory( const data::UTF8String& newName )
				:	name( newName )
			{
			}

			
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a string representing the name of this resource category.
			OM_INLINE const data::UTF8String& getName() const
			{
				return name;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource category is equal to another.
			OM_INLINE Bool operator == ( const ResourceCategory& other ) const
			{
				return name == other.name;
			}
			
			
			/// Return whether or not this resource category is equal to another.
			OM_INLINE Bool operator != ( const ResourceCategory& other ) const
			{
				return name != other.name;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this resource category.
			OM_INLINE Hash getHashCode() const
			{
				return name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Resource Type Determination Method
			
			
			template < typename DataType >
			OM_INLINE static const ResourceCategory& of();/*
			{
				return UNDEFINED;
			}*/
			
			
		//********************************************************************************
		//******	Public Resource Type Declarations
			
			
			/// An undefined resource category for when no category is specified.
			static const ResourceCategory UNDEFINED;
			
			
			/// A resource category corresponding to graphics data types.
			static const ResourceCategory GRAPHICS;
			
			
			/// A resource category corresponding to physics data types.
			static const ResourceCategory PHYSICS;
			
			
			/// A resource category corresponding to sound data types.
			static const ResourceCategory SOUND;
			
			
			/// A resource category corresponding to textual or string data types.
			static const ResourceCategory TEXT;
			
			
			/// A resource category corresponding to math data types.
			static const ResourceCategory MATH;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string which represents this resource category.
			data::UTF8String name;
			
			
			
};




/// A macro that defines a ResourceCategory for a fully-qualified concrete type.
#define OM_RESOURCE_CATEGORY( DataType, DataResourceCategory ) \
OM_RESOURCES_NAMESPACE_START \
template <>\
OM_INLINE const ResourceCategory& ResourceCategory::of< DataType >()\
{\
	return DataResourceCategory;\
}\
OM_RESOURCES_NAMESPACE_END




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


// Primitive types.
OM_RESOURCE_CATEGORY( Int8, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( UInt8, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Int16, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( UInt16, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Int32, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( UInt32, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Int64, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( UInt64, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Float16, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Float32, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( Float64, ResourceCategory::MATH );


// Vector types.
OM_RESOURCE_CATEGORY( math::Vector2f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Vector2d, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Vector3f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Vector3d, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Vector4f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Vector4d, ResourceCategory::MATH );


// Matrix types.
OM_RESOURCE_CATEGORY( math::Matrix2f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Matrix2d, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Matrix3f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Matrix3d, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Matrix4f, ResourceCategory::MATH );
OM_RESOURCE_CATEGORY( math::Matrix4d, ResourceCategory::MATH );


// String types.
OM_RESOURCE_CATEGORY( data::String, ResourceCategory::TEXT );
OM_RESOURCE_CATEGORY( data::UTF8String, ResourceCategory::TEXT );
OM_RESOURCE_CATEGORY( data::UTF16String, ResourceCategory::TEXT );
OM_RESOURCE_CATEGORY( data::UTF32String, ResourceCategory::TEXT );


#endif // INCLUDE_OM_RESOURCE_CATEGORY_H
