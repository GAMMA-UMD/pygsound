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

#ifndef INCLUDE_OM_RESOURCE_REFERENCE_PATCHER_H
#define INCLUDE_OM_RESOURCE_REFERENCE_PATCHER_H


#include "omResourcesConfig.h"


#include "omResourceSetTypeTranscoder.h"


//##########################################################################################
//*****************************  Start Om Resources Namespace  *****************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores resource references that need to be patched for a templated data type.
class ResourceReferencePatcher
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default empty resource reference patcher.
			OM_INLINE ResourceReferencePatcher()
				:	newObject( true ),
					newObjectStart( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Reference Patching Method
			
			
			/// Patch all of the missing references for the given transcoder using the loaded resource set.
			template < typename DataType >
			OM_INLINE void patchReferences( ResourceSetTypeTranscoder<DataType>& transcoder, const ResourceSet& resources )
			{
				const Size numObjects = referenceObjects.getSize();
				
				for ( Index i = 0; i < numObjects; i++ )
				{
					const ReferenceObject& object = referenceObjects[i];
					
					// Patch the reference for the object using the transcoder.
					transcoder.patchReferences( references.getPointer() + object.offset,
												object.count, resources, *(DataType*)object.resource );
				}
				
				// Clear the reference lists for next time.
				referenceObjects.clear();
				references.clear();
				newObject = true;
				newObjectStart = 0;
			}
			
			
		//********************************************************************************
		//******	Reference Object Accessor Methods
			
			
			/// Start collecting reference for a new object.
			OM_INLINE void startObject()
			{
				newObject = true;
				newObjectStart = references.getSize();
			}
			
			
			/// Stop collecting reference for an object.
			OM_INLINE void endObject( void* resource )
			{
				// Update the object data if there was an object allocated.
				if ( !newObject )
				{
					if ( resource )
					{
						ReferenceObject& object = referenceObjects.getLast();
						object.resource = resource;
						object.count = references.getSize() - object.offset;
					}
					else
					{
						// Problem loading object, remove the added references.
						references.removeLast( references.getSize() - newObjectStart );
					}
				}
			}
			
			
		//********************************************************************************
		//******	Reference Accessor Methods
			
			
			/// Return the total number of references in this reference patcher for all objects.
			OM_INLINE Size getReferenceCount() const
			{
				return references.getSize();
			}
			
			
			/// Return a reference to the reference in this reference patcher for the specified reference index.
			OM_INLINE ResourceReference& getReference( Index referenceIndex )
			{
				return references[referenceIndex];
			}
			
			
			/// Return a reference to the reference in this reference patcher for the specified reference index.
			OM_INLINE const ResourceReference& getReference( Index referenceIndex ) const
			{
				return references[referenceIndex];
			}
			
			
			/// Add a new reference for the current object that is being parsed.
			OM_INLINE void addReference( const ResourceReference& newReference )
			{
				if ( newObject )
				{
					referenceObjects.add( ReferenceObject( references.getSize() ) );
					newObject = false;
				}
				
				references.add( newReference );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores information about the references that need to be patched for a single resource object.
			class ReferenceObject
			{
				public:
					
					/// Create a new resource reference data object for the specified resource.
					OM_INLINE ReferenceObject( Index newOffset )
						:	resource( NULL ),
							offset( newOffset ),
							count( 0 )
					{
					}
					
					/// The resource data object that needs references patched.
					void* resource;
					
					/// The offset within the resource reference array for this data type where this object's references start.
					Index offset;
					
					/// The number of references there are for this resource object.
					Size count;
					
			};
		
			
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of resource objects that need references patched for this data type.
			util::ArrayList<ReferenceObject> referenceObjects;
			
			
			/// A packed list of the resource references that need to be patched for this data type.
			util::ArrayList<ResourceReference> references;
			
			
			/// The offset in the reference array where the new object started.
			Index newObjectStart;
			
			
			/// A boolean value indicating whether or not a new object should be created on the next new reference.
			Bool newObject;
			
			
};




//##########################################################################################
//*****************************  End Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_REFERENCE_PATCHER_H
