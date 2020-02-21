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

#ifndef INCLUDE_OM_OPAQUE_RESOURCE_H
#define INCLUDE_OM_OPAQUE_RESOURCE_H


#include "omResourcesConfig.h"


#include "omResourceType.h"
#include "omResource.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that opaquely stores a reference to a resource.
class OpaqueResource
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new opaque resource which doesn't point to a valid resource.
			OM_INLINE OpaqueResource()
				:	data( NULL ),
					resourceID( NULL )
			{
			}
			
			
			/// Create a new opaque resource for the specified data and resource type.
			OM_INLINE OpaqueResource( void* newData, ResourceID* newID )
				:	data( newData ),
					resourceID( newID )
			{
			}
			
			
			/// Create a new opaque resource for the specified resource.
			template < typename T >
			OM_INLINE OpaqueResource( const Resource<T>& newResource )
				:	data( newResource.getPointer() ),
					resourceID( newResource.getID() )
			{
			}
			
			
		//********************************************************************************
		//******	Resource Data Accessor Methods
			
			
			/// Return an opaque pointer to the resource object data.
			OM_INLINE void* getData() const
			{
				return data;
			}
			
			
			/// Set an opaque pointer to the resource object data.
			OM_INLINE void setData( void* newData )
			{
				data = newData;
			}
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return a pointer to an object describing the ID for the resource.
			OM_INLINE ResourceID* getID() const
			{
				return resourceID;
			}
			
			
			/// Set a pointer to an object describing the ID for the resource.
			OM_INLINE void setID( ResourceID* newID )
			{
				resourceID = newID;
			}
			
			
		//********************************************************************************
		//******	Status Accessor Methods
			
			
			/// Return whether or not this opaque resource's data and ID are both valid and non-NULL.
			OM_INLINE Bool isSet() const
			{
				return data != NULL && resourceID != NULL;
			}
			
			
			/// Return whether or not this opaque resource's data or ID are NULL.
			OM_INLINE Bool isNull() const
			{
				return data == NULL || resourceID != NULL;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this opaque resource is equal to another.
			OM_INLINE Bool operator == ( const OpaqueResource& other ) const
			{
				return data == other.data;
			}
			
			
			/// Return whether or not this opaque resource is not equal to another.
			OM_INLINE Bool operator != ( const OpaqueResource& other ) const
			{
				return !(*this == other);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An opaque pointer to the resource object data.
			void* data;
			
			
			/// A pointer to the identifier of this resource.
			ResourceID* resourceID;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_OPAQUE_RESOURCE_H
