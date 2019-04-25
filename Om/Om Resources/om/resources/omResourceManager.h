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

#ifndef INCLUDE_OM_RESOURCE_MANAGER_H
#define INCLUDE_OM_RESOURCE_MANAGER_H


#include "omResourcesConfig.h"


#include "omResource.h"
#include "omResourceID.h"
#include "omResourceTypeTranscoder.h"
#include "omResourceSet.h"
#include "omResourceMode.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class ResourceModule;




//********************************************************************************
/// A class that manages resources for a set of formats that it can handle.
/**
  * A resource manager contains ResourceTranscoder objects that enable it
  * to load and save resources of the corresponding types. A resource
  * manager is able to cache resources that have been previously loaded
  * so that duplicates are not loaded.
  */
class ResourceManager
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty resource manager with no formats it can manage.
			ResourceManager();
			
			
			/// Create a new resource manager that is a copy of the other resource manager.
			ResourceManager( const ResourceManager& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this resource manager and release all associated resources.
			virtual ~ResourceManager();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another resource manager to this one.
			ResourceManager& operator = ( const ResourceManager& other );
			
			
		//********************************************************************************
		//******	Resource Loading Methods
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const Char* filePath )
			{
				return this->load<DataType>( ResourceID(UTF8String(filePath)) );
			}
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const UTF8String& filePath )
			{
				return this->load<DataType>( ResourceID(filePath) );
			}
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const Path& filePath )
			{
				return this->load<DataType>( ResourceID(filePath.toString()) );
			}
			
			
			/// Load a resource object for the specified resource identifier.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType> load( const ResourceID& identifier )
			{
				// Get the manager for the identifier's type and format.
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return Resource<DataType>( identifier );
				
				// Load the resource.
				return typeManager->load( identifier, &resources, this );
			}
			
			
			/// Load a resource object for the specified resource identifier.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool load( Resource<DataType>& resource )
			{
				ResourceID* identifier = resource.getID();
				
				if ( identifier == NULL )
					return false;
				
				// Get the manager for the identifier's type and format.
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return Resource<DataType>( identifier );
				
				// Load the resource.
				return typeManager->load( resource, &resources, this );
			}
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const Char* filePath, ResourceSet* resourceSet )
			{
				return this->load<DataType>( ResourceID(UTF8String(filePath)), resourceSet );
			}
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const data::UTF8String& filePath, ResourceSet* resourceSet )
			{
				return this->load<DataType>( ResourceID(filePath), resourceSet );
			}
			
			
			/// Load a resource object for the specified file path.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_INLINE Resource<DataType> load( const Path& filePath, ResourceSet* resourceSet )
			{
				return this->load<DataType>( ResourceID(filePath.toString()), resourceSet );
			}
			
			
			/// Load a resource object for the specified resource identifier.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType> load( const ResourceID& identifier, ResourceSet* resourceSet )
			{
				// Get the manager for the identifier's type and format.
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return Resource<DataType>( identifier );
				
				// Load the resource.
				return typeManager->load( identifier, resourceSet, this );
			}
			
			
			/// Load a resource object for the specified resource identifier.
			/**
			  * If loaded successfully, the resource is inserted into the resource set parameter if it is not NULL.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool load( Resource<DataType>& resource, ResourceSet* resourceSet )
			{
				ResourceID* identifier = resource.getID();
				
				if ( identifier == NULL )
					return false;
				
				// Get the manager for the identifier's type and format.
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return Resource<DataType>( identifier );
				
				// Load the resource.
				return typeManager->load( resource, resourceSet, this );
			}
			
			
			/// Load an opaque resource object for the specified resource identifier into a resource set.
			/**
			  * This function loads a resource without requiring the user to have knowledge of its
			  * concrete data type. The resource format and type is inferred from the identifier and then
			  * the resource is loaded into the resource set for the inferred data type.
			  *
			  * If loaded successfully, the resource is inserted into the resource set parameter and a
			  * void pointer to the resource is returned.
			  * If loading fails, a NULL pointer is returned.
			  */
			void* load( const ResourceID& identifier, ResourceSet& resourceSet );
			
			
		//********************************************************************************
		//******	Resource Saving Methods
			
			
			/// Save resource data to the location specified by the given file path.
			/**
			  * The method returns whether or not the resource data was able to be
			  * successfully saved.
			  */
			template < typename DataType >
			OM_INLINE Bool save( const Char* filePath, const DataType& data,
									const ResourceSet* resourceSet = NULL )
			{
				return this->save( ResourceID(UTF8String(filePath)), data, resourceSet );
			}
			
			
			/// Save resource data to the location specified by the given file path.
			/**
			  * The method returns whether or not the resource data was able to be
			  * successfully saved.
			  */
			template < typename DataType >
			OM_INLINE Bool save( const UTF8String& filePath, const DataType& data,
									const ResourceSet* resourceSet = NULL )
			{
				return this->save( ResourceID(filePath), data, resourceSet );
			}
			
			
			/// Save resource data to the location specified by the given file path.
			/**
			  * The method returns whether or not the resource data was able to be
			  * successfully saved.
			  */
			template < typename DataType >
			OM_INLINE Bool save( const Path& filePath, const DataType& data,
									const ResourceSet* resourceSet = NULL )
			{
				return this->save( ResourceID(filePath.toString()), data, resourceSet );
			}
			
			
			/// Save resource data to the location specified by the given resource ID.
			/**
			  * The method returns whether or not the resource data was able to be
			  * successfully saved.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool save( const ResourceID& identifier, const DataType& data,
									const ResourceSet* resourceSet = NULL )
			{
				// Get the resource manager for the resource type.
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return false;
				
				// Save the resource.
				return typeManager->save( identifier, data, resourceSet == NULL ? &resources : resourceSet, this );
			}
			
			
			/// Save resource data to the location specified by the given resource ID.
			/**
			  * This function saves a resource without requiring the user to have knowledge of its
			  * concrete data type. The resource format and type is inferred from the identifier and then
			  * the resource is queried from the resource set for the inferred data type.
			  * The resource is then written at the specified location.
			  *
			  * The method returns whether or not the resource data was able to be
			  * successfully saved.
			  */
			Bool save( const ResourceID& identifier, const void* data, const ResourceSet& resourceSet );
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return the number of resource types that this manager can load.
			OM_INLINE Size getTypeCount() const
			{
				return types.getSize();
			}
			
			
			/// Return the number of resource formats that this manager can load.
			OM_INLINE Size getFormatCount() const
			{
				return formats.getSize();
			}
			
			
			/// Return the resource format that the manager has at the specified index.
			OM_INLINE const ResourceFormat& getFormat( Index formatIndex ) const
			{
				return formats[formatIndex]->getResourceFormat();
			}
			
			
			/// Return the resource type for the format that the manager has at the specified index.
			OM_INLINE const ResourceType& getFormatType( Index formatIndex ) const
			{
				return formats[formatIndex]->getResourceType();
			}
			
			
			/// Add all of the resource formats from the specified module to this resource manager.
			void addModule( ResourceModule& module );
			
			
			/// Add a new resource format to this manager.
			/**
			  * The method fails if the format transcoder is NULL or if its format is not valid.
			  * The new transcoder replaces any previously existing transcoder for its format.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool addFormat( ResourceTypeTranscoder<DataType>* transcoder )
			{
				// Make sure the new transcoder is not NULL.
				if ( transcoder == NULL )
					return false;
				
				TypeManagerT<DataType>* typeManager = this->initializeTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return false;
				
				// Add the transcoder to the manager for the type.
				typeManager->addFormat( transcoder );
				addFormatInternal( transcoder );
				
				return true;
			}
			
			
			/// Remove the transcoder for the templated type and format from this resource manager.
			/**
			  * The method returns whether or not the specified format was found and removed.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool removeFormat( const ResourceFormat& format )
			{
				TypeManagerT<DataType>* typeManager = this->getTypeManager<DataType>();
				
				if ( typeManager == NULL )
					return false;
				
				// Add the transcoder to the manager for the type.
				typeManager->removeFormat( format );
				removeFormatInternal( format );
				
				// Remove the type manager if the type manager is now empty.
				if ( typeManager->getFormatCount() == Size(0) )
				{
					const ResourceType& type = ResourceType::of<DataType>();
					types.remove( type.getHashCode(), type );
					util::destruct( typeManager );
				}
				
				return true;
			}
			
			
			/// Remove all formats from this resource manager.
			void clearFormats();
			
			
		//********************************************************************************
		//******	Resource Set Accessor Methods
			
			
			/// Return a reference to the internal resource set for this manager.
			/**
			  * This resource set is used to store resources whenever no other resource set
			  * is provided when loading resources. The resources are stored here to avoid
			  * being leaked or prematurely released.
			  */
			OM_INLINE ResourceSet& getResources()
			{
				return resources;
			}
			
			
			/// Return a const reference to the internal resource set for this manager.
			/**
			  * This resource set is used to store resources whenever no other resource set
			  * is provided when loading resources. The resources are stored here to avoid
			  * being leaked or prematurely released.
			  */
			OM_INLINE const ResourceSet& getResources() const
			{
				return resources;
			}
			
			
			/// Clear this resource manager's cached resources.
			OM_INLINE void clearResources()
			{
				resources.clearResources();
			}
			
			
		//********************************************************************************
		//******	Resource Mode Accessor Methods
			
			
			/// Return the current mode that the resource manager is loading resources in.
			OM_INLINE const ResourceMode& getMode() const
			{
				return mode;
			}
			
			
			/// Set the current mode that the resource manager is loading resources in.
			OM_INLINE void setMode( const ResourceMode& newMode )
			{
				mode = newMode;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// The base class for objects that manage resource formats for a particular ResourceType.
			class TypeManager
			{
				public:
					
					
					/// Destroy a format mangager, releasing all internal and cached resources.
					virtual ~TypeManager()
					{
					}
					
					
					/// Construct and return a copy of the concrete type of this format manager.
					virtual TypeManager* copy() const = 0;
					
					
					/// Load a resource with the given ID into the resource set without knowing its concrete type.
					virtual void* load( const ResourceID& resourceID, ResourceSet& resourceSet, ResourceManager* manager ) const = 0;
					
					
					/// Save a resource with the given ID and pointer from a resource set without knowing its concrete type.
					virtual Bool save( const ResourceID& resourceID, const void* data,
										const ResourceSet& resourceSet, ResourceManager* manager ) const = 0;
					
					
			};
			
			
			/// A class that stores a collection resource transcoders of formats for the templated ResourceType.
			template < typename DataType >
			class TypeManagerT;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Initialize the resource type manager for the templated data type and return a pointer to it.
			template < typename DataType >
			OM_NO_INLINE TypeManagerT<DataType>* initializeTypeManager()
			{
				const ResourceType& type = ResourceType::of<DataType>();
				TypeManager** typeManagerBase;
				
				if ( types.find( type.getHashCode(), type, typeManagerBase ) )
					return dynamic_cast<TypeManagerT<DataType>*>( *typeManagerBase );
				else
					return (TypeManagerT<DataType>*)*types.add( type.getHashCode(), type,
																util::construct< TypeManagerT<DataType> >() );
			}
			
			
			/// Return a pointer to the resource type manager for the templated data type if it exists.
			template < typename DataType >
			OM_NO_INLINE TypeManagerT<DataType>* getTypeManager()
			{
				const ResourceType& type = ResourceType::of<DataType>();
				TypeManager** typeManagerBase;
				
				if ( types.find( type.getHashCode(), type, typeManagerBase ) )
					return dynamic_cast<TypeManagerT<DataType>*>( *typeManagerBase );
				
				return NULL;
			}
			
			
			void addFormatInternal( ResourceTranscoder* transcoder );
			
			
			void removeFormatInternal( const ResourceFormat& format );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A map from resource types to managers for those types.
			util::HashMap< ResourceType, TypeManager* > types;
			
			
			/// A list of the resource transcoders that are part of this manager.
			ArrayList<ResourceTranscoder*> formats;
			
			
			/// An internal data structure of cached orphan resources that would otherwise go unloaded.
			ResourceSet resources;
			
			
			/// The current mode that the resource manager is loading resources in.
			ResourceMode mode;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Format Manager Class
//############		
//##########################################################################################
//##########################################################################################




template < typename DataType >
class ResourceManager:: TypeManagerT : public TypeManager
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new manager for the template resource type.
			OM_INLINE TypeManagerT()
			{
			}
			
			
		//********************************************************************************
		//******	Copy Method
			
			
			/// Construct and return a copy of the concrete type of this type manager.
			virtual TypeManagerT<DataType>* copy() const
			{
				return util::construct<TypeManagerT<DataType> >( *this );
			}
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return the number of resource formats there are for this type manager.
			OM_INLINE Size getFormatCount() const
			{
				return formats.getSize();
			}
			
			
			/// Return a pointer to the best transcoder for the given ID or NULL if none exists.
			OM_INLINE ResourceTypeTranscoder<DataType>* getFormatForID( const ResourceID& identifier ) const
			{
				const ResourceFormat& format = identifier.getFormat();
				const Size numFormats = formats.getSize();
				
				if ( format == ResourceFormat::UNDEFINED )
				{
					// Check the file extension to see if we can guess the format.
					data::UTF8String extension = fs::Path( identifier.getURL() ).getExtension();
					
					for ( Index i = 0; i < numFormats; i++ )
					{
						if ( formats[i]->getResourceFormat().hasExtension( extension ) )
							return formats[i];
					}
				}
				else
				{
					// Find a compatible format.
					for ( Index i = 0; i < numFormats; i++ )
					{
						if ( formats[i]->getResourceFormat() == format )
							return formats[i];
					}
				}
				
				return NULL;
			}
			
			
			/// Add a new resource transcoder format to this type manager.
			OM_INLINE Bool addFormat( ResourceTypeTranscoder<DataType>* transcoder )
			{
				if ( transcoder == NULL )
					return false;
				
				formats.add( transcoder );
				return true;
			}
			
			
		//********************************************************************************
		//******	Resource Loading Methods
			
			
			/// Load a resource with the given ID into the resource set without knowing its concrete type.
			virtual void* load( const ResourceID& resourceID, ResourceSet& resourceSet, ResourceManager* manager ) const
			{
				return this->load( resourceID, &resourceSet, manager ).getPointer();
			}
			
			
			/// Load resource data into a resource using the given resource's ID.
			OM_NO_INLINE Bool load( Resource<DataType>& resource, ResourceSet* resourceSet,
									ResourceManager* manager ) const
			{
				const ResourceID* identifier = resource.getID();
				
				if ( identifier == NULL )
					return false;
				
				if ( resourceSet != NULL )
				{
					// Check the cache to see if there is anything already loaded in the cache.
					Resource<DataType>* cachedResource = resourceSet->getResource<DataType>( *identifier );
					
					if ( cachedResource != NULL )
					{
						resource = *cachedResource;
						return true;
					}
				}
				
				ResourceTypeTranscoder<DataType>* transcoder = this->getFormatForID( *identifier );
				
				if ( transcoder == NULL )
					return false;
				
				// Try loading the resource using the transcoder.
				DataType* resourceData = transcoder->decode( *identifier, manager, resourceSet );
				
				// If the loading failed, return a NULL resource.
				if ( !resourceData )
					return false;
				
				resource.setData( lang::Shared<DataType>( resourceData ) );
				
				// Insert the new resource in the resource set if it is not NULL.
				if ( resourceSet != NULL )
					resourceSet->addResource( resource );
				
				return true;
			}
			
			
			/// Load and return a resource for the given resource's ID.
			OM_NO_INLINE Resource<DataType> load( const ResourceID& id, ResourceSet* resourceSet,
													ResourceManager* manager ) const
			{
				// Check the cache to see if there is anything already loaded in the cache.
				if ( resourceSet != NULL )
				{
					Resource<DataType>* cachedResource = resourceSet->getResource<DataType>( id );
					
					if ( cachedResource != NULL )
						return *cachedResource;
				}
				
				ResourceTypeTranscoder<DataType>* transcoder = this->getFormatForID( id );
				
				if ( transcoder == NULL )
					return Resource<DataType>( id );
				
				// Create a new ID for the resource that has all of its information filled in.
				ResourceID resourceID( ResourceType::of<DataType>(), transcoder->getResourceFormat(),
										id.getURL(), id.getName() );
				
				// Try loading the resource using the transcoder.
				DataType* resourceData = transcoder->decode( resourceID, resourceSet, manager );
				
				// If the loading failed, return a NULL resource.
				if ( resourceData == NULL )
					return Resource<DataType>( resourceID );
				
				Resource<DataType> resource( lang::Shared<DataType>( resourceData ), resourceID );
				
				// Insert the new resource in the resource set if it is not NULL.
				if ( resourceSet != NULL )
					resourceSet->addResource( resource );
				
				return resource;
			}
			
			
		//********************************************************************************
		//******	Resource Saving Methods
			
			
			/// Save a resource with the given ID from the resource set without knowing its concrete type.
			virtual Bool save( const ResourceID& resourceID, const void* data, const ResourceSet& resourceSet, ResourceManager* manager ) const
			{
				// Get the data from the resource set.
				const ResourceTypeSet<DataType>* typeSet = resourceSet.getResources<DataType>();
				ResourceLocalID localID;
				
				if ( typeSet == NULL || typeSet->getResourceLocalID( data, localID ) )
					return false;
				
				const Resource<DataType>* resource = typeSet->getResource( localID );
				
				if ( resource == NULL )
					return false;
				
				return this->save( resourceID, *(*resource), &resourceSet, manager );
			}
			
			
			/// Save the specified resource data to the location specified by the given identifier.
			OM_NO_INLINE Bool save( const ResourceID& identifier, const DataType& data,
									const ResourceSet* resourceSet, ResourceManager* manager ) const
			{
				ResourceTypeTranscoder<DataType>* transcoder = this->getFormatForID( identifier );
				
				if ( transcoder == NULL )
					return false;
				
				return transcoder->encode( identifier, data, resourceSet, manager );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of resource transcoders of this resource type for various formats.
			util::ArrayList<ResourceTypeTranscoder<DataType>*> formats;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_MANAGER_H
