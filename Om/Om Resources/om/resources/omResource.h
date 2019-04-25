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

#ifndef INCLUDE_OM_RESOURCE_H
#define INCLUDE_OM_RESOURCE_H


#include "omResourcesConfig.h"


#include "omResourceID.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates a handle to resource data.
/**
  * The class allows the user to either load the resource data manually, or
  * to provide a unique identifier and ResourceLoader object to defer loading the
  * resource until it is needed.
  */
template < typename DataType >
class Resource
{
	public:
		
		//********************************************************************************
		//******	Public Type Declaration
			
			
			/// Define the type for a resource loading callback function.
			typedef lang::Function< lang::Shared<DataType> ( const ResourceID& ) > LoadCallback;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default resource with no data and no resource identifier or loader.
			/**
			  * The created resource is essentially unuseable and can be used to indicate an
			  * invalid resource or a resource that has not yet been set.
			  */
			OM_INLINE Resource()
				:	info( NULL )
			{
			}
			
			
			/// Create a new resource for the specified data pointer.
			/**
			  * If the specified data pointer is NULL, the resource is unusable because it has
			  * no way to load any data for the resource.
			  */
			OM_INLINE Resource( const lang::Shared<DataType>& newData )
				:	info( util::construct<ResourceInfo>( newData ) )
			{
			}
			
			
			/// Create a new resource for the specified data that is associated with the specified unique identifier.
			/**
			  * If the specified data pointer is NULL, the resource is unusable because it has
			  * no way to load the resource associated with the given ID.
			  */
			OM_INLINE Resource( const lang::Shared<DataType>& newData, const ResourceID& newIdentifier )
				:	info( util::construct<ResourceInfo>( newData, newIdentifier ) )
			{
			}
			
			
			/// Create a new resource for the specified unique resource identifier.
			OM_INLINE Resource( const ResourceID& newIdentifier )
				:	info( util::construct<ResourceInfo>( newIdentifier ) )
			{
			}
			
			
			/// Create a new resource for the specified unique resource identifier using the specified loader object.
			/**
			  * A resource created using this constructor will lazily load the resource when it is first
			  * dereferenced. The specified ID is used to load the resource using the given ResourceLoader
			  * object. If the loader object pointer is NULL, the new resource will be useless because
			  * it has no way to load any data for the resource.
			  */
			OM_INLINE Resource( const ResourceID& newIdentifier, 
								const LoadCallback& newLoadCallback )
				:	info( util::construct<ResourceInfo>( newIdentifier, newLoadCallback ) )
			{
			}
			
			
			OM_INLINE Resource( const Resource& other )
				:	info( other.info )
			{
				if ( info )
					info->referenceCount++;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~Resource()
			{
				if ( info && (--info->referenceCount) == 0 )
					util::destruct( info );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			OM_INLINE Resource& operator = ( const Resource& other )
			{
				if ( info != other.info )
				{
					if ( info && (--info->referenceCount) == 0 )
						util::destruct( info );
					
					info = other.info;
					
					if ( info )
						info->referenceCount++;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource refers to the same resource as another.
			/**
			  * This operator returns TRUE if the data pointer they refer to is equal, or
			  * if they both have the same resource identifier/loader. Otherwise FALSE is
			  * returned.
			  */
			OM_INLINE Bool operator == ( const Resource& other ) const
			{
				if ( info == other.info )
					return true;
				else if ( info && other.info && (info->data == other.info->data || info->identifier == other.info->indentifier) )
					return true;
				else
					return false;
			}
			
			
			/// Return whether or not this resource refers to a different resource than another.
			/**
			  * This operator returns FALSE if the data pointer they refer to is equal, or
			  * if they both have the same resource identifier/loader. Otherwise TRUE is
			  * returned.
			  */
			OM_INLINE Bool operator != ( const Resource& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Return a pointer to the data associated with this resource.
			/**
			  * The method can return NULL if the resource's data has not yet
			  * been loaded.
			  */
			OM_INLINE lang::Shared<DataType> getData() const
			{
				if ( info )
					return info->data;
				else
					return lang::Shared<DataType>();
			}
			
			
			/// Set a pointer to the data associated with this resource.
			OM_INLINE void setData( const lang::Shared<DataType>& newData )
			{
				if ( info )
					info->data = newData;
				else
					info = util::construct<ResourceInfo>( newData );
			}
			
			
		//********************************************************************************
		//******	Pointer Accesor Method
			
			
			/// Return a pointer to the data associated with this resource.
			/**
			  * The method can return NULL if the resource's data has not yet
			  * been loaded.
			  */
			OM_INLINE DataType* getPointer() const
			{
				if ( info )
					return info->data;
				else
					return NULL;
			}
			
			
		//********************************************************************************
		//******	Cast Operators
			
			
			/// Cast this Resource object to a raw pointer.
			OM_INLINE operator DataType* () const
			{
				if ( info )
					return info->data.getPointer();
				else
					return NULL;
			}
			
			
			/// Cast this Resource object to a raw pointer of a different type to allow polymorphism.
			template < typename U >
			OM_INLINE operator U* () const
			{
				if ( info )
					return info->data.getPointer();
				else
					return NULL;
			}
			
			
			/// Cast this Resource object to a reference-counted pointer.
			OM_INLINE operator lang::Shared<DataType> () const
			{
				if ( info )
					return info->data;
				else
					return lang::Shared<DataType>();
			}
			
			
		//********************************************************************************
		//******	Dereferencing Operators
			
			
			/// Return a reference to the data associated with this resource.
			/**
			  * This operator dereferences the data for the resource. If the data
			  * pointer is currently NULL, the resource attempts to use the associated resource
			  * loader to load the resource data. An assertion is raised if there is no identifier
			  * or loader for the resource if the data is NULL.
			  */
			OM_INLINE DataType& operator * () const
			{
				OM_DEBUG_ASSERT_MESSAGE( info != NULL && info->data.isSet(), "Tried to access resource with NULL info or data." );
				
				return *info->data;
			}
			
			
			/// Return a pointer to the data associated with this resource.
			/**
			  * This operator dereferences the data for the resource. If the data
			  * pointer is currently NULL, the resource attempts to use the associated resource
			  * loader to load the resource data. An assertion is raised if there is no identifier
			  * or loader for the resource if the data is NULL.
			  */
			OM_INLINE DataType* operator -> () const
			{
				if ( info == NULL )
					return NULL;
				
				if ( info->data.isNull() && info->loadCallback.isSet() )
					info->data = info->loadCallback( info->identifier );
				
				return info->data;
			}
			
			
		//********************************************************************************
		//******	Resource Identifier Accessor Methods
			
			
			/// Return whether or not this resource has a unique identifier associated with it.
			OM_INLINE Bool hasID() const
			{
				return info != NULL;
			}
			
			
			/// Return a pointer to the unique identifier associated with this resource.
			/**
			  * If the resource has no identifier associated with it, NULL is returned.
			  */
			OM_INLINE ResourceID* getID() const
			{
				if ( info )
					return &info->identifier;
				else
					return NULL;
			}
			
			
			/// Set the unique identifier associated with this resource.
			OM_INLINE void setID( const ResourceID& newID )
			{
				if ( info == NULL )
					info = util::construct<ResourceInfo>( newID );
				else
					info->identifier = newID;
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a pointer to the name associated with this resource.
			OM_INLINE const data::UTF8String* getName() const
			{
				if ( info == NULL )
					return NULL;
				
				return &info->identifier.getName();
			}
			
			
			/// Set the name associated with this resource.
			OM_INLINE void setName( const data::UTF8String& newName )
			{
				// Create a new info object if it is NULL.
				if ( info == NULL )
					info = util::construct<ResourceInfo>();
				
				info->identifier.setName( newName );
			}
			
			
		//********************************************************************************
		//******	Resource Loader Accessor Methods
			
			
			/// Return whether or not this resource has a ResourceLoader object associated with it.
			OM_INLINE Bool hadLoadCallback() const
			{
				return info != NULL && info->loadCallback.isSet();
			}
			
			
			/// Return a pointer to the resource loading callback function associated with this resource.
			/**
			  * If the resource has no callback function associated with it, NULL is returned.
			  */
			OM_INLINE const LoadCallback* getLoadCallback() const
			{
				if ( info )
					return &info->loadCallback;
				else
					return NULL;
			}
			
			
			/// Set the resource loading callback function associated with this resource.
			/**
			  * If the resource has no callback function associated with it, an assertion is raised.
			  */
			OM_INLINE void setLoadCallback( const LoadCallback& newLoadCallback )
			{
				if ( info == NULL )
					info = util::construct<ResourceInfo>( newLoadCallback );
				else
					info->loadCallback = newLoadCallback;
			}
			
			
		//********************************************************************************
		//******	Resource Loading/Releasing Methods
			
			
			/// Attempt to load the data for this resource if necessary.
			/**
			  * This method tries to ensure that the resource data is loaded and available
			  * for use. If the data is already loaded, the method returns TRUE. Otherwise,
			  * the method loads the data using the resource's ID and loader object and
			  * returns TRUE. If there is no loader or ID for this resource, FALSE is returned,
			  * indicating that the resource was unable to be loaded.
			  */
			OM_INLINE Bool load() const
			{
				if ( info == NULL )
					return false;
				else if ( info->data.isSet() )
					return true;
				else
				{
					if ( info->loadCallback.isSet() )
						info->data = info->loadCallback( info->identifier );
					
					return info->data.isSet();
				}
			}
			
			
			/// Release the data assoicated with this resource, but keep the same resource ID and loader.
			/**
			  * This method just releases the reference to the data. If other resource objects reference
			  * the same data, the data is not deallocated until they all release their data.
			  */
			OM_INLINE void release()
			{
				if ( info )
					info->data.release();
			}
			
			
			/// Return whether or not the data associated with this resource is currently NULL.
			/**
			  * The data can be NULL if the resource has not yet been loaded, or if the
			  * resource is itself invalid.
			  */
			OM_INLINE Bool isNull() const
			{
				return info == NULL || info->data.isNull();
			}
			
			
			/// Return whether or not the data associated with this resource is currently set.
			/**
			  * The data can be NULL if the resource has not yet been loaded, or if the
			  * resource is itself invalid.
			  */
			OM_INLINE Bool isSet() const
			{
				return info != NULL && info->data.isSet();
			}
			
			
			/// Return the number of shared references that there are to this resource's data.
			/**
			  * If the returned value is 1, this means that this resource is the only object
			  * which owns the data associated with it.
			  */
			OM_INLINE Size getReferenceCount() const
			{
				if ( info )
					return info->referenceCount;
				else
					return 0;
			}
			
			
		//********************************************************************************
		//******	Construction Helper Methods
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			OM_INLINE static Resource construct()
			{
				return Resource( lang::Shared<DataType>::construct() );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1 >
			OM_INLINE static Resource construct( P1 p1 )
			{
				return Resource( lang::Shared<DataType>::construct( p1 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2 >
			OM_INLINE static Resource construct( P1 p1, P2 p2 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE static Resource construct( P1 p1, P2 p2, P3 p3 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2, p3 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE static Resource construct( P1 p1, P2 p2, P3 p3, P4 p4 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2, p3, p4 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE static Resource construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2, p3, p4, p5 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			OM_INLINE static Resource construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2, p3, p4, p5, p6 ) );
			}
			
			
			/// Construct a resource object of the templated type with the specified arguments for its constructor.
			template < typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			OM_INLINE static Resource construct( P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7 )
			{
				return Resource( lang::Shared<DataType>::construct( p1, p2, p3, p4, p5, p6, p7 ) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that holds shared information about a particular resource.
			class ResourceInfo
			{
				public:
					
					OM_INLINE ResourceInfo()
						:	referenceCount( 1 ),
							identifier()
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					OM_INLINE ResourceInfo( const lang::Shared<DataType>& newData )
						:	data( newData ),
							referenceCount( 1 )
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					
					OM_INLINE ResourceInfo( const lang::Shared<DataType>& newData, const ResourceID& newIdentifier )
						:	data( newData ),
							identifier( newIdentifier ),
							referenceCount( 1 )
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					
					OM_INLINE ResourceInfo( const ResourceID& newIdentifier )
						:	referenceCount( 1 ),
							identifier( newIdentifier )
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					
					OM_INLINE ResourceInfo( const LoadCallback& newLoadCallback )
						:	referenceCount( 1 ),
							loadCallback( newLoadCallback )
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					
					OM_INLINE ResourceInfo( const ResourceID& newIdentifier,
											const LoadCallback& newLoadCallback )
						:	referenceCount( 1 ),
							identifier( newIdentifier ),
							loadCallback( newLoadCallback )
					{
						identifier.setType( ResourceType::of<DataType>() );
					}
					
					
					
					/// A smart pointer to the data associated with this resource.
					lang::Shared<DataType> data;
					
					
					/// The reference count of this resource info object.
					Size referenceCount;
					
					
					/// An identifier associated with this resource which uniquely locates it.
					ResourceID identifier;
					
					
					/// A function that loads the data associated with this resource.
					LoadCallback loadCallback;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to information about this resource (possibly NULL).
			ResourceInfo* info;
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_H
