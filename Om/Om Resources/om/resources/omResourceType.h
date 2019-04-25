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

#ifndef INCLUDE_OM_RESOURCE_TYPE_H
#define INCLUDE_OM_RESOURCE_TYPE_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents the type of a generic resource.
/**
  * An resource type is defined by a unique string.
  */
class ResourceType
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resource type with an UNDEFINED type string.
			OM_INLINE ResourceType()
				:	name(UNDEFINED.getName())
			{
			}
			
			
			/// Create a new resource type with the specified constant NULL-terminated type string.
			OM_INLINE ResourceType( const Char* newName )
				:	name( newName )
			{
			}
			
			
			/// Create a new resource type with the specified type string.
			OM_INLINE ResourceType( const data::UTF8String& newName )
				:	name( newName )
			{
			}

			
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a string representing the name of this resource type.
			OM_INLINE const data::UTF8String& getName() const
			{
				return name;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource type is equal to another.
			OM_INLINE Bool operator == ( const ResourceType& other ) const
			{
				return name == other.name;
			}
			
			
			/// Return whether or not this resource type is equal to another.
			OM_INLINE Bool operator != ( const ResourceType& other ) const
			{
				return name != other.name;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this resource type.
			OM_INLINE Hash getHashCode() const
			{
				return name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Resource Type Determination Method
			
			
			template < typename DataType >
			OM_INLINE static const ResourceType& of();/*
			{
				return UNDEFINED;
			}*/
			
			
		//********************************************************************************
		//******	Resource Copy Method
			
			
			template < typename DataType >
			OM_INLINE static DataType* copy( const DataType& data )
			{
				return util::construct<DataType>( data );
			}
			
			
		//********************************************************************************
		//******	Public Resource Type Declarations
			
			
			/// The resource type to use for an undefined object type.
			static const ResourceType UNDEFINED;
			
			/// The resource type to use for a 8-bit signed integer.
			static const ResourceType INT8;
			
			/// The resource type to use for a 8-bit unsigned integer.
			static const ResourceType UINT8;
			
			/// The resource type to use for a 16-bit signed integer.
			static const ResourceType INT16;
			
			/// The resource type to use for a 16-bit unsigned integer.
			static const ResourceType UINT16;
			
			/// The resource type to use for a 32-bit signed integer.
			static const ResourceType INT32;
			
			/// The resource type to use for a 32-bit unsigned integer.
			static const ResourceType UINT32;
			
			/// The resource type to use for a 64-bit signed integer.
			static const ResourceType INT64;
			
			/// The resource type to use for a 64-bit unsigned integer.
			static const ResourceType UINT64;
			
			/// The resource type to use for a 16-bit floating point number.
			static const ResourceType FLOAT16;
			
			/// The resource type to use for a 32-bit floating point number.
			static const ResourceType FLOAT32;
			
			/// The resource type to use for a 64-bit floating point number.
			static const ResourceType FLOAT64;
			
			/// The resource type to use for a 2-component 32-bit float vector.
			static const ResourceType VECTOR2_F32;
			
			/// The resource type to use for a 2-component 64-bit float vector.
			static const ResourceType VECTOR2_F64;
			
			/// The resource type to use for a 3-component 32-bit float vector.
			static const ResourceType VECTOR3_F32;
			
			/// The resource type to use for a 3-component 64-bit float vector.
			static const ResourceType VECTOR3_F64;
			
			/// The resource type to use for a 4-component 32-bit float vector.
			static const ResourceType VECTOR4_F32;
			
			/// The resource type to use for a 4-component 64-bit float vector.
			static const ResourceType VECTOR4_F64;
			
			/// The resource type to use for a 2x2 32-bit float matrix.
			static const ResourceType MATRIX2_F32;
			
			/// The resource type to use for a 2x2 64-bit float matrix.
			static const ResourceType MATRIX2_F64;
			
			/// The resource type to use for a 3x3 32-bit float matrix.
			static const ResourceType MATRIX3_F32;
			
			/// The resource type to use for a 3x3 64-bit float matrix.
			static const ResourceType MATRIX3_F64;
			
			/// The resource type to use for a 4x4 32-bit float matrix.
			static const ResourceType MATRIX4_F32;
			
			/// The resource type to use for a 4x4 64-bit float matrix.
			static const ResourceType MATRIX4_F64;
			
			/// The resource type to use for ASCII encoded strings.
			static const ResourceType STRING_ASCII;
			
			/// The resource type to use for UTF-8 encoded strings.
			static const ResourceType STRING_UTF8;
			
			/// The resource type to use for UTF-16 encoded strings.
			static const ResourceType STRING_UTF16;
			
			/// The resource type to use for UTF-32 encoded strings.
			static const ResourceType STRING_UTF32;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string which represents this resource type.
			data::UTF8String name;
			
			
			
};




/// A macro that defines a ResourceType for a fully-qualified concrete type.
#define OM_RESOURCE_TYPE( DataType, DataResourceType ) \
OM_RESOURCES_NAMESPACE_START \
template <>\
OM_INLINE const ResourceType& ResourceType::of< DataType >()\
{\
	return DataResourceType;\
}\
OM_RESOURCES_NAMESPACE_END




/// A macro that declares a function specialization for copying a resource data type.
#define OM_RESOURCE_TYPE_COPY( DataType, DataName, DataCopy ) \
OM_RESOURCES_NAMESPACE_START \
template <>\
OM_INLINE DataType* ResourceType::copy< DataType >( const DataType& DataName )\
{\
	DataCopy\
}\
OM_RESOURCES_NAMESPACE_END




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


// Primitive types.
OM_RESOURCE_TYPE( Int8, ResourceType::INT8 );
OM_RESOURCE_TYPE( UInt8, ResourceType::UINT8 );
OM_RESOURCE_TYPE( Int16, ResourceType::INT16 );
OM_RESOURCE_TYPE( UInt16, ResourceType::UINT16 );
OM_RESOURCE_TYPE( Int32, ResourceType::INT32 );
OM_RESOURCE_TYPE( UInt32, ResourceType::UINT32 );
OM_RESOURCE_TYPE( Int64, ResourceType::INT64 );
OM_RESOURCE_TYPE( UInt64, ResourceType::UINT64 );
OM_RESOURCE_TYPE( Float16, ResourceType::FLOAT16 );
OM_RESOURCE_TYPE( Float32, ResourceType::FLOAT32 );
OM_RESOURCE_TYPE( Float64, ResourceType::FLOAT64 );


// Vector types.
OM_RESOURCE_TYPE( math::Vector2f, ResourceType::VECTOR2_F32 );
OM_RESOURCE_TYPE( math::Vector2d, ResourceType::VECTOR2_F64 );
OM_RESOURCE_TYPE( math::Vector3f, ResourceType::VECTOR3_F32 );
OM_RESOURCE_TYPE( math::Vector3d, ResourceType::VECTOR3_F64 );
OM_RESOURCE_TYPE( math::Vector4f, ResourceType::VECTOR4_F32 );
OM_RESOURCE_TYPE( math::Vector4d, ResourceType::VECTOR4_F64 );


// Matrix types.
OM_RESOURCE_TYPE( math::Matrix2f, ResourceType::MATRIX2_F32 );
OM_RESOURCE_TYPE( math::Matrix2d, ResourceType::MATRIX2_F64 );
OM_RESOURCE_TYPE( math::Matrix3f, ResourceType::MATRIX3_F32 );
OM_RESOURCE_TYPE( math::Matrix3d, ResourceType::MATRIX3_F64 );
OM_RESOURCE_TYPE( math::Matrix4f, ResourceType::MATRIX4_F32 );
OM_RESOURCE_TYPE( math::Matrix4d, ResourceType::MATRIX4_F64 );


// String types.
OM_RESOURCE_TYPE( data::String, ResourceType::STRING_ASCII );
OM_RESOURCE_TYPE( data::UTF8String, ResourceType::STRING_UTF8 );
OM_RESOURCE_TYPE( data::UTF16String, ResourceType::STRING_UTF16 );
OM_RESOURCE_TYPE( data::UTF32String, ResourceType::STRING_UTF32 );


#endif // INCLUDE_OM_RESOURCE_TYPE_H
