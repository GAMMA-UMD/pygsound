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

#ifndef INCLUDE_OM_RESOURCE_SET_TRANSCODER_H
#define INCLUDE_OM_RESOURCE_SET_TRANSCODER_H


#include "omResourcesConfig.h"


#include "omResourceSet.h"
#include "omOpaqueResource.h"
#include "omResourcePrototype.h"
#include "omResourceTypeTranscoder.h"
#include "omResourceSetTypeTranscoder.h"
#include "omResourceReferencePatcher.h"
#include "omResourceEncoder.h"
#include "omResourceDecoder.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class ResourceModule;




//********************************************************************************
/// A class that reads and writes resource sets.
class ResourceSetTranscoder : public ResourceTypeTranscoder<ResourceSet>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new resource transcoder that has no types it can handle.
			ResourceSetTranscoder();
			
			
			/// Create a new resource transcoder that is a copy of another resource transcoder.
			ResourceSetTranscoder( const ResourceSetTranscoder& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this resource transcoder and release all associated resources.
			virtual ~ResourceSetTranscoder();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the state of another resource transcoder to this one.
			ResourceSetTranscoder& operator = ( const ResourceSetTranscoder& other );
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return the number of resource types that this transcoder can handle.
			OM_INLINE Size getTypeCount() const
			{
				return resourceTypes.getSize();
			}
			
			
			/// Return a pointer to the transcoder used for the template resource data type.
			template < typename DataType >
			OM_INLINE ResourceSetTypeTranscoder<DataType>* getType() const
			{
				const ResourceType& type = ResourceType::of<DataType>();
				TypeTranscoderBase* const * transcoder = resourceTypes.get( type.getHashCode(), type );
				
				if ( transcoder == NULL )
					return NULL;
				else
					return ((TypeTranscoder<DataType>*)*transcoder)->transcoder;
			}
			
			
			/// Add all of the resource type transcoders from the specified module to this resource set transcoder.
			void addModule( ResourceModule& module );
			
			
			/// Add a new type to this resource transcoder that is can decode and encode.
			/**
			  * The method returns whether or not the new type was able to be added.
			  * If the resource type already exists in this transcoder, the method
			  * fails and returns FALSE. Otherwise it is added to the transcoder.
			  */
			template < typename DataType >
			OM_INLINE Bool addType( ResourceSetTypeTranscoder<DataType>* transcoder )
			{
				if ( transcoder == NULL )
					return false;
				
				const ResourceType& type = ResourceType::of<DataType>();
				
				/// Check to see if this type is already handled by the transcoder.
				if ( resourceTypes.contains( type.getHashCode(), type ) )
					return false;
				
				/// Add the new type delegate.
				resourceTypes.add( type.getHashCode(), type,
								util::construct< TypeTranscoder<DataType> >( transcoder ) );
				
				return true;
			}
			
			
			/// Remove the specified resource type from this resource transcoder.
			/**
			  * The method returns whether or not the type was able to be removed.
			  */
			Bool removeType( const ResourceType& type );
			
			
			/// Remove all resource types from this resource transcoder.
			void clearTypes();
			
			
		//********************************************************************************
		//******	Resource Format Accessor Methods
			
			
			/// Return an object that represents the resource format that this transcoder can read and write.
			virtual const ResourceFormat& getResourceFormat() const;
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Return whether or not this resource transcoder is able to encode the specified resource.
			virtual Bool canEncode( const ResourceSet& resources ) const;
			
			
			/// Encode the specified generic scene to the file at the specified path.
			/**
			  * If the method fails, FALSE is returned.
			  */
			virtual Bool encode( const ResourceID& identifier, const ResourceSet& resources,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Return whether or not the specified identifier refers to a valid resource file for this transcoder.
			virtual Bool canDecode( const ResourceID& identifier ) const;
			
			
			/// Decode the resource file at the specified path and return a pointer to the decoded scene.
			/**
			  * If the method fails, a NULL pointer is returned.
			  */
			virtual ResourceSet* decode( ResourceID& identifier, ResourceSet* resourceSet = NULL,
										ResourceManager* manager = NULL );
			
			
		//********************************************************************************
		//******	Instancing Methods
			
			
			/// Create a copy of an opaque resource and store the copy in a resource set.
			/**
			  * A pointer to the new resource copy is returned, or NULL if the method fails.
			  */
			OpaqueResource instance( const OpaqueResource& resource, ResourceSet& storage ) const;
			
			
			/// Create a copy of an opaque resource and store the copy in a resource set.
			/**
			  * A pointer to the new resource copy is returned, or NULL if the method fails.
			  */
			OpaqueResource instance( const void* data, const ResourceType& type, ResourceSet& storage ) const;
			
			
			/// Replace child object instances with the new prototype data.
			void replaceInstances( void* data, const ResourceType& type, const Prototype& prototype,
									const Prototype& instance, ResourceSet& storage ) const;
			
			
			/// Return whether or not there are any child object instances with new prototype data.
			Bool findInstances( void* data, const ResourceType& type, const Prototype& prototype,
								const Prototype& instance ) const;
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The resource format that this resource set transcoder is encoding.
			static const ResourceFormat FORMAT;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			class TypeTranscoderBase;
			
			
			/// A class that stores temporary information about a single object that is in a resource set.
			class ObjectInfo
			{
				public:
					
					/// Create a new object info object with no name.
					OM_INLINE ObjectInfo()
						:	fileOffset( 0 ),
							sizeInBytes( 0 ),
							name()
					{
					}
					
					/// Create a new object info object with the given name.
					OM_INLINE ObjectInfo( const data::UTF8String& newName, const data::UTF8String& newPath, const ResourceFlags& newFlags,
										UInt64 newFileOffset = 0, UInt64 newSizeInBytes = 0 )
						:	fileOffset( newFileOffset ),
							sizeInBytes( newSizeInBytes ),
							name( newName ),
							path( newPath ),
							flags( newFlags )
					{
					}
					
					/// The offset in bytes of this object's data from the start of the file.
					UInt64 fileOffset;
					
					/// The size in bytes of this object's data.
					UInt64 sizeInBytes;
					
					/// The object's name.
					data::UTF8String name;
					
					/// The object's file path.
					data::UTF8String path;
					
					/// Boolean configuration flags for the resource.
					ResourceFlags flags;
					
			};
			
			
			/// An object that stores temporary information about objects of a particular resource type.
			class ObjectTable
			{
				public:
					
					/// Create a new default type object table.
					OM_INLINE ObjectTable( const ResourceType& newResourceType, TypeTranscoderBase* newTypeTranscoder,
											UInt64 newFileOffset, UInt64 newNumObjects )
						:	resourceType( newResourceType ),
							resourceTypeID( ResourceID::INVALID_LOCAL_ID ),
							typeTranscoder( newTypeTranscoder ),
							fileOffset( newFileOffset ),
							numObjects( newNumObjects )
					{
					}
					
					/// Reset the object table to its default empty state.
					OM_INLINE void reset()
					{
						objects.clear();
						resourceType = ResourceType::UNDEFINED;
						resourceTypeID = ResourceID::INVALID_LOCAL_ID;
						typeTranscoder = NULL;
						fileOffset = 0;
						numObjects = 0;
					}
					
					/// A list of information for the objects of this type.
					util::ArrayList<ObjectInfo> objects;
					
					/// A pointer to the resource type for the object table type.
					ResourceType resourceType;
					
					/// The local index of the resource type within the resource set.
					ResourceLocalID resourceTypeID;
					
					/// A pointer to the resource type transcoder for the object table type.
					TypeTranscoderBase* typeTranscoder;
					
					/// The offset in bytes from the start of the file where the table is located.
					UInt64 fileOffset;
					
					/// The offset in bytes from the start of the file where the table is located.
					UInt64 numObjects;
					
			};
			
			
			/// An object that stores temporary information about the types and objects in a resource set.
			class TypeTable
			{
				public:
					
					/// Create a new default type table.
					OM_INLINE TypeTable()
						:	maxTypeNameLength( 0 )
					{
					}
					
					/// Reset the type table to its default empty state.
					OM_INLINE void reset()
					{
						types.clear();
						maxTypeNameLength = 0;
					}
					
					/// A list of information for each type in the type table.
					util::ArrayList<ObjectTable> types;
					
					/// The maximum length in UTF-8 code points of any type's name of this type table.
					UInt64 maxTypeNameLength;
					
			};
			
			
			/// The base class for objects that contain encoding/decoding information for an resource type.
			class TypeTranscoderBase
			{
				public:
					
					
					/// Destroy this resource type transcoder object.
					virtual ~TypeTranscoderBase()
					{
					}
					
					
					/// Create and return a copy of this resource type info object.
					virtual TypeTranscoderBase* copy() const = 0;
					
					/// Return a reference to the resource type this type corresponds to.
					virtual const ResourceType& getType() const = 0;
					
					
					virtual Bool encode( const ResourceID& resourceSetID, const ResourceSet& resourceSet, ObjectTable& objectTable,
										ResourceEncoder& encoder, ResourceManager* manager ) = 0;
					
					virtual Bool decode( ResourceID& objectID, ResourceSet& resourceSet,
										ResourceDecoder& decoder, ResourceManager* manager ) = 0;
					
					/// Construct a NULL resource object and add it to the resource set as a placeholder.
					virtual void getNullResource( const ResourceID& objectID, ResourceSet& resourceSet ) = 0;
					
					/// Patch missing references for this transcoder using the specified resources.
					virtual void patchReferences( const TypeTable& types, const ResourceSet& resources ) = 0;
					
					/// Finalize loaded objects for this transcoder using the specified resources.
					virtual void finalize( const ResourceSet& resources ) = 0;
					
					/// Create a copy of the specified opaque resource pointer of this resource type.
					virtual OpaqueResource instance( const void* data, const ResourceID* dataID,
													const ResourceSetTranscoder& rst, ResourceSet& storage ) const = 0;
					
					/// Replace child object instances with the new prototype data.
					virtual void replaceInstances( void* data, const Prototype& prototype, const Prototype& instance,
													const ResourceSetTranscoder& rst, ResourceSet& storage ) const = 0;
					
					/// Return whether or not there are any child object instances with new prototype data.
					virtual Bool findInstances( const void* data, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& rst ) const = 0;
					
			};
			
			
			template < typename DataType >
			class TypeTranscoder;
			
			
		//********************************************************************************
		//******	Encoding / Decoding Helper Methods
			
			
			void encodeVersion( Index version, const ResourceID& identifier, const ResourceSet& resources,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
			void decodeVersion( Index version, ResourceID& identifier, ResourceSet& resources,
								ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
			template < Index version >
			void encodeVersion( const ResourceID& identifier, const ResourceSet& resources,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
			template < Index version >
			void decodeVersion( ResourceID& identifier, ResourceSet& resources,
								ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A map from resource types to objects that decode and encode those types.
			util::HashMap<ResourceType,TypeTranscoderBase*> resourceTypes;
			
			
			/// An object that handles binary encoding of resource set objects.
			ResourceEncoder encoder;
			
			
			/// An object that handles binary dencoding of resource set objects.
			ResourceDecoder decoder;
			
			
			/// A temporary type/object table that is used to keep track of information about encoded/decoded objects/types.
			TypeTable typeTable;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Type Info Class Declaration
//############		
//##########################################################################################
//##########################################################################################




template < typename DataType >
class ResourceSetTranscoder:: TypeTranscoder : public TypeTranscoderBase
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new type transcoder that uses the specified transcoder object.
			OM_INLINE TypeTranscoder( ResourceSetTypeTranscoder<DataType>* newTranscoder )
				:	transcoder( newTranscoder )
			{
			}
			
			
			/// Construct and return a copy of this type transcoder.
			virtual TypeTranscoder* copy() const
			{
				return util::construct< TypeTranscoder<DataType> >( *this );
			}
			
			
		//********************************************************************************
		//******	Resource Type Accessor Method
			
			
			/// Return a reference to the resource type that this type transcoder handles.
			virtual const ResourceType& getType() const
			{
				return ResourceType::of<DataType>();
			}
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			virtual Bool encode( const ResourceID& resourceSetID, const ResourceSet& resourceSet, ObjectTable& objectTable,
								ResourceEncoder& encoder, ResourceManager* manager )
			{
				if ( transcoder == NULL )
					return false;
				
				// Get the resources of the templated type that are in the resource set.
				const ResourceTypeSet<DataType>* resourceList = resourceSet.getResources<DataType>();
				
				if ( resourceList == NULL )
					return false;
				
				// Encode each resource object.
				const Size numResources = resourceList->getResourceCount();
				const Resource<DataType>* resources = resourceList->getResources();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					const ResourceID* resourceID = resource.getID();
					ObjectInfo& object = objectTable.objects[i];
					
					// Skip resources that fail to load.
					if ( !resource.load() )
						continue;
					
					// Remember the start offset of the object.
					object.fileOffset = encoder.getPosition();
					
					ResourceID objectID( ResourceType::of<DataType>(),
										resourceID == NULL ? FORMAT : resourceID->getFormat(),
										resourceID->getURL(), object.name );
					
					// Encode the resource data.
					transcoder->encode( objectID, *resource, resourceSet, encoder, manager );
					
					// Remember the size of the object.
					object.sizeInBytes = encoder.getPosition() - object.fileOffset;
				}
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			virtual Bool decode( ResourceID& objectID, ResourceSet& resourceSet,
								ResourceDecoder& decoder, ResourceManager* manager )
			{
				if ( transcoder == NULL )
					return false;
				
				patcher.startObject();
				
				// Attempt to decode the resource object.
				DataType* resourceData = transcoder->decode( objectID, decoder, patcher, manager );
				
				patcher.endObject( resourceData );
				
				if ( resourceData == NULL )
					return false;
				
				// Create a new resource for the resource object and add the new resource to the resource set.
				resourceSet.addResource( Resource<DataType>( lang::Shared<DataType>( resourceData ), objectID ) );
				
				return true;
			}
			
			
			/// Construct a NULL resource object and add it to the resource set as a placeholder.
			virtual void getNullResource( const ResourceID& objectID, ResourceSet& resourceSet )
			{
				// Create a NULL resource with the object ID for the resource.
				resourceSet.addResource( Resource<DataType>( lang::Shared<DataType>(), objectID ) );
			}
			
			
			/// Patch missing references for this transcoder using the specified resources.
			virtual void patchReferences( const TypeTable& types, const ResourceSet& resources )
			{
				if ( transcoder == NULL )
					return;
				
				// Update the IDs of the types in the reference patcher.
				const Size numReferences = patcher.getReferenceCount();
				const Size numTableTypes = types.types.getSize();
				
				for ( Index i = 0; i < numReferences; i++ )
				{
					ResourceReference& reference = patcher.getReference(i);
					
					if ( reference.localTypeID < numTableTypes )
						reference.localTypeID = types.types[reference.localTypeID].resourceTypeID;
				}
				
				// Patch the references.
				patcher.patchReferences( *transcoder, resources );
			}
			
			
			/// Finalize resources that have been loaded after all references have been patched.
			virtual void finalize( const ResourceSet& resourceSet )
			{
				if ( transcoder == NULL )
					return;
				
				// Get the resources of the templated type that are in the resource set.
				const ResourceTypeSet<DataType>* resourceList = resourceSet.getResources<DataType>();
				
				if ( resourceList == NULL )
					return;
				
				const Size numResources = resourceList->getResourceCount();
				const Resource<DataType>* resources = resourceList->getResources();
				
				// Finalize the individual resources.
				for ( Index i = 0; i < numResources; i++ )
					transcoder->finalize( *resources[i] );
			}
			
			
		//********************************************************************************
		//******	Instancing Methods
			
			
			/// Create a copy of the specified opaque resource pointer of this resource type.
			virtual OpaqueResource instance( const void* data, const ResourceID* dataID,
												const ResourceSetTranscoder& rst, ResourceSet& storage ) const
			{
				if ( data == NULL || transcoder == NULL )
					return OpaqueResource();
				
				// Create a copy of the resource data.
				DataType* data2 = transcoder->instance( *(const DataType*)data, rst, storage );
				
				Resource<DataType> resource;
				
				if ( dataID != NULL )
					resource = Resource<DataType>( lang::Shared<DataType>( data2 ), *dataID );
				else
					resource = Resource<DataType>( lang::Shared<DataType>( data2 ) );
				
				// Add the copy to the resource set.
				storage.addResource( resource );
				
				return OpaqueResource( resource );
			}
			
			
			/// Replace child object instances with the new prototype data.
			virtual void replaceInstances( void* data, const Prototype& prototype, const Prototype& instance,
											const ResourceSetTranscoder& rst, ResourceSet& storage ) const
			{
				if ( data == NULL || transcoder == NULL )
					return;
				
				// Replace child instances for the resource data.
				transcoder->replaceInstances( *(DataType*)data, prototype, instance, rst, storage );
			}
			
			
			/// Return whether or not there are any child object instances with new prototype data.
			virtual Bool findInstances( const void* data, const Prototype& prototype, const Prototype& instance,
										const ResourceSetTranscoder& rst ) const
			{
				if ( data == NULL || transcoder == NULL )
					return false;
				
				// Find child instances for the resource data.
				return transcoder->findInstances( *(const DataType*)data, prototype, instance, rst );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Friend Class Declaration
			
			
			// Declare the resource set transcoder as a friend so that it can access internal data.
			friend class ResourceSetTranscoder;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an object that handles encoding and decoding this resource type.
			ResourceSetTypeTranscoder<DataType>* transcoder;
			
			
			/// An object that stores reference patching data for this data type.
			ResourceReferencePatcher patcher;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename DataType > template < typename InstanceType >
InstanceType* ResourceSetTypeTranscoder<DataType>:: replaceInstance( InstanceType* data, const Prototype& prototype,
																	const Prototype& instance, const ResourceSetTranscoder& transcoder,
																	ResourceSet& resources )
{
	Index componentIndex;
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return (InstanceType*)instanceData;
	}
	else
	{
		// Check to see if the component needs to be copied because it has a child
		// object that is copied, even though it is not part of the prototype.
		if ( transcoder.findInstances( data, ResourceType::of<InstanceType>(), prototype, instance ) )
		{
			OpaqueResource instance = transcoder.instance( data, ResourceType::of<InstanceType>(), resources );
			
			if ( instance.getData() != NULL )
				return (InstanceType*)instance.getData();
		}
	}
	
	return data;
}




template < typename DataType >
OpaqueResource ResourceSetTypeTranscoder<DataType>:: replaceInstance( const OpaqueResource& resource, const Prototype& prototype,
																	const Prototype& instance, const ResourceSetTranscoder& transcoder,
																	ResourceSet& resources )
{
	Index componentIndex;
	void* data = resource.getData();
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return component.resource;
	}
	else
	{
		// Check to see if the component needs to be copied because it has a child
		// object that is copied, even though it is not part of the prototype.
		const ResourceID* dataID = resource.getID();
		
		if ( dataID && transcoder.findInstances( data, dataID->getType(), prototype, instance ) )
		{
			OpaqueResource instance = transcoder.instance( data, dataID->getType(), resources );
			
			if ( instance.getData() != NULL )
				return instance;
		}
	}
	
	return resource;
}




template < typename DataType >
void* ResourceSetTypeTranscoder<DataType>:: replaceInstance( void* data, const ResourceType& dataType, const Prototype& prototype,
																	const Prototype& instance, const ResourceSetTranscoder& transcoder,
																	ResourceSet& resources )
{
	Index componentIndex;
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return instanceData;
	}
	else
	{
		// Check to see if the component needs to be copied because it has a child
		// object that is copied, even though it is not part of the prototype.
		if ( transcoder.findInstances( data, dataType, prototype, instance ) )
		{
			OpaqueResource instance = transcoder.instance( data, dataType, resources );
			void* instanceData = instance.getData();
			
			if ( instanceData != NULL )
				return instanceData;
		}
	}
	
	return data;
}




template < typename DataType > template < typename InstanceType >
Bool ResourceSetTypeTranscoder<DataType>:: findInstance( InstanceType* data, const Prototype& prototype, const Prototype& instance,
														const ResourceSetTranscoder& transcoder )
{
	Index componentIndex;
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return true;
	}
	
	// Check to see if the object has any prototype component instances.
	if ( transcoder.findInstances( data, ResourceType::of<InstanceType>(), prototype, instance ) )
		return true;
	
	return false;
}




template < typename DataType >
Bool ResourceSetTypeTranscoder<DataType>:: findInstance( const OpaqueResource& resource, const Prototype& prototype, const Prototype& instance,
														const ResourceSetTranscoder& transcoder )
{
	Index componentIndex;
	void* data = resource.getData();
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return true;
	}
	
	// Check to see if the object has any prototype component instances.
	const ResourceID* dataID = resource.getID();
	
	if ( dataID && transcoder.findInstances( data, dataID->getType(), prototype, instance ) )
		return true;
	
	return false;
}




template < typename DataType >
Bool ResourceSetTypeTranscoder<DataType>:: findInstance( void* data, const ResourceType& dataType, const Prototype& prototype, const Prototype& instance,
														const ResourceSetTranscoder& transcoder )
{
	Index componentIndex;
	
	// Check to see if the data is part of the prototype.
	if ( prototype.getComponentIndex( data, componentIndex ) )
	{
		const Prototype::Component& component = instance.getComponent( componentIndex );
		void* instanceData = component.resource.getData();
		
		// If the component instance is different, then return the copied object pointer.
		if ( data != instanceData )
			return true;
	}
	
	// Check to see if the object has any prototype component instances.
	if ( transcoder.findInstances( data, dataType, prototype, instance ) )
		return true;
	
	return false;
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_SET_TRANSCODER_H
