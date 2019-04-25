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

#include "omResourceSetTranscoder.h"


#include "omResourceModule.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceFormat ResourceSetTranscoder:: FORMAT("ResourceSet","om");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ResourceSetTranscoder:: ResourceSetTranscoder()
{
}




ResourceSetTranscoder:: ResourceSetTranscoder( const ResourceSetTranscoder& other )
{
	// Copy the type transcoders from the other transcoder.
	util::HashMap<ResourceType,TypeTranscoderBase*>::ConstIterator type = other.resourceTypes.getIterator();
	
	while ( type )
	{
		resourceTypes.add( type.getKeyHash(), type.getKey(), type.getValue()->copy() );
		type++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ResourceSetTranscoder:: ~ResourceSetTranscoder()
{
	this->clearTypes();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ResourceSetTranscoder& ResourceSetTranscoder:: operator = ( const ResourceSetTranscoder& other )
{
	if ( this != &other )
	{
		// Clear the previous set of type transcoders in this transcoder.
		this->clearTypes();
		
		// Copy the type transcoders from the other transcoder.
		HashMap<ResourceType,TypeTranscoderBase*>::ConstIterator type = other.resourceTypes.getIterator();
		
		while ( type )
		{
			resourceTypes.add( type.getKeyHash(), type.getKey(), type.getValue()->copy() );
			type++;
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Type Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceSetTranscoder:: addModule( ResourceModule& module )
{
	module.getTypes( *this );
}




Bool ResourceSetTranscoder:: removeType( const ResourceType& type )
{
	// Get the type info so that we can delete it.
	TypeTranscoderBase** typeTranscoder = resourceTypes.get( type.getHashCode(), type );
	
	if ( typeTranscoder == NULL )
		return false;
	
	util::destruct( *typeTranscoder );
	
	// Remove the type info from the type map.
	return resourceTypes.remove( type.getHashCode(), type );
}




void ResourceSetTranscoder:: clearTypes()
{
	util::HashMap<ResourceType,TypeTranscoderBase*>::Iterator type = resourceTypes.getIterator();
	
	while ( type )
	{
		util::destruct( *type );
		type++;
	}
	
	resourceTypes.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




OpaqueResource ResourceSetTranscoder:: instance( const OpaqueResource& resource, ResourceSet& storage ) const
{
	if ( resource.getData() == NULL || resource.getID() == NULL )
		return OpaqueResource();
	
	// Get the manager for this resource type.
	const ResourceType& type = resource.getID()->getType();
	TypeTranscoderBase* const * typeTranscoder = resourceTypes.get( type.getHashCode(), type );
	
	if ( typeTranscoder == NULL )
		return OpaqueResource();
	
	// Create a copy of the resource.
	OpaqueResource result = (*typeTranscoder)->instance( resource.getData(), resource.getID(), *this, storage );
	
	return result;
}




OpaqueResource ResourceSetTranscoder:: instance( const void* data, const ResourceType& type, ResourceSet& storage ) const
{
	if ( data == NULL )
		return OpaqueResource();
	
	// Get the manager for this resource type.
	TypeTranscoderBase* const * typeTranscoder = resourceTypes.get( type.getHashCode(), type );
	
	if ( typeTranscoder == NULL )
		return OpaqueResource();
	
	// Create a copy of the resource.
	OpaqueResource result = (*typeTranscoder)->instance( data, NULL, *this, storage );
	
	return result;
}




void ResourceSetTranscoder:: replaceInstances( void* data, const ResourceType& type, const Prototype& prototype,
												const Prototype& instance, ResourceSet& storage ) const
{
	if ( data == NULL )
		return;
	
	// Get the manager for this resource type.
	TypeTranscoderBase* const * typeTranscoder = resourceTypes.get( type.getHashCode(), type );
	
	if ( typeTranscoder == NULL )
		return;
	
	// Replace instances for the resource.
	(*typeTranscoder)->replaceInstances( data, prototype, instance, *this, storage );
}




Bool ResourceSetTranscoder:: findInstances( void* data, const ResourceType& type, const Prototype& prototype,
											const Prototype& instance ) const
{
	if ( data == NULL )
		return false;
	
	// Get the manager for this resource type.
	TypeTranscoderBase* const * typeTranscoder = resourceTypes.get( type.getHashCode(), type );
	
	if ( typeTranscoder == NULL )
		return false;
	
	// Replace instances for the resource.
	return (*typeTranscoder)->findInstances( data, prototype, instance, *this );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Format Accessor Method
//############		
//##########################################################################################
//##########################################################################################




const ResourceFormat& ResourceSetTranscoder:: getResourceFormat() const
{
	return FORMAT;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceSetTranscoder:: canEncode( const ResourceSet& resources ) const
{
	return true;
}




Bool ResourceSetTranscoder:: encode( const ResourceID& identifier, const ResourceSet& resources,
									const ResourceSet* resourceSet, ResourceManager* manager )
{
	// Create a file reader object to read the contents of the file.
	fs::Path filePath( identifier.getURL() );
	io::FileWriter writer( filePath );
	
	// Open the file and return NULL if the file couldn't be opened.
	if ( !writer.open() )
		return false;
	
	// Erase the file.
	writer.erase();
	
	// Setup the encoder for the file stream.
	encoder.setStream( &writer );
	encoder.setFileID( identifier );
	
	//***************************************************************************
	
	// The binary format that the file should be encoding in.
	io::BinaryFormat format;
	
	//***************************************************************************
	// Write the base header.
	
	// Write the header ID
	const UByte headerID[8] = { 'R','E','S','O','U','R','C','E' };
	encoder.write( headerID, 8 );
	
	// Write the endianness.
	const UInt8 endianness = format.endianness != data::Endianness::LITTLE ? 1 : 0;
	encoder.write( endianness );
	
	// Write the alignment.
	const UInt8 alignment = (UInt8)math::clamp( format.alignment, Size(1), Size(math::max<UInt8>()) );
	encoder.write( alignment );
	
	// Initialize the encoder with the binary format of this file.
	encoder.setFormat( format );
	
	// Write the version.
	const UInt16 version = 1;
	encoder.write( version );
	
	// Write the checksum.
	const UInt32 checksum = 0;
	encoder.write( checksum );
	
	//***************************************************************************	
	// Write the main part of the file.
	
	// Encode the resource set based on the version number.
	encodeVersion( version, identifier, resources, resourceSet, manager );
	
	//***************************************************************************	
	// Finish encoding.
	
	// Flush the encoder.
	encoder.flush();
	encoder.setStream( NULL );
	
	// Close the file.
	writer.close();
	
	//***************************************************************************
	
	// Reset the type table.
	typeTable.reset();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceSetTranscoder:: canDecode( const ResourceID& identifier ) const
{
	return true;
}




ResourceSet* ResourceSetTranscoder:: decode( ResourceID& identifier, ResourceSet* resourceSet,
											ResourceManager* manager )
{
	fs::Path filePath( identifier.getURL() );
	fs::Path parentPath = filePath.getParent();
	fs::File file( filePath );
	
	// Don't open if the file does not exist.
	if ( !file.exists() )
		return NULL;
	
	// Memory map the file.
	const UByte* const fileData = (const UByte*)file.map( fs::File::READ );
	
	// If the memory mapping failed, return failure.
	if ( fileData == NULL )
		return NULL;
	
	// Setup the decoder for the memory-mapped file.
	decoder.setData( fileData, file.getSize() );
	decoder.setFileID( identifier );
	
	//***************************************************************************
	
	// The binary format that the file should be decoded with.
	io::BinaryFormat format;
	
	//***************************************************************************
	// Read the header.
	
	const Size headerIDSize = 8;
	UByte headerID[headerIDSize];
	
	if ( !decoder.read( headerID, headerIDSize ) )
		return NULL;
	
	// Check the format code
	if ( headerID[0] != 'R' || headerID[1] != 'E' || headerID[2] != 'S' || headerID[3] != 'O' ||
		headerID[4] != 'U' || headerID[5] != 'R' || headerID[6] != 'C' || headerID[7] != 'E' )
		return NULL;
	
	// Read the endianness of the file.
	UInt8 endianness = (format.endianness != data::Endianness::LITTLE);
	if ( !decoder.read( endianness ) )
		return NULL;
	
	format.endianness = endianness != 0 ? data::Endianness::BIG : data::Endianness::LITTLE;
	
	// Read the alignment.
	UInt8 alignment = (UInt8)format.alignment;
	if ( !decoder.read( alignment ) )
		return NULL;
	
	// Initialize the decoder with the binary format of this file.
	decoder.setFormat( format );
	
	// Read the version.
	UInt16 version;
	if ( !decoder.read( version ) )
		return NULL;
	
	// Read the checksum.
	UInt32 checksum;
	if ( !decoder.read( checksum ) )
		return NULL;
	
	//***************************************************************************
	// Read the main part of the file.
	
	// Create a resource set.
	ResourceSet* newResourceSet = util::construct<ResourceSet>();
	
	// Decode the resource set based on the version number.
	decodeVersion( version, identifier, *newResourceSet, resourceSet, manager );
	
	// Unmap the file since we are finished parsing.
	file.unmap();
	
	//***************************************************************************
	// Patch references for all resource types.
	
	// Update the final local IDs in the object table of all types that were able to be loaded.
	const Size numTableTypes = typeTable.types.getSize();
	
	for ( Index i = 0; i < numTableTypes; i++ )
	{
		ObjectTable& objectTable = typeTable.types[i];
		
		// Lookup the ID of the object type.
		newResourceSet->getTypeIndex( objectTable.resourceType, objectTable.resourceTypeID );
	}
	
	// Patch references for all resource types.
	util::HashMap<ResourceType,TypeTranscoderBase*>::Iterator type = resourceTypes.getIterator();
	
	while ( type )
	{
		(*type)->patchReferences( typeTable, *newResourceSet );
		type++;
	}
	
	//***************************************************************************
	// Finalize all resources.
	
	type.reset();
	
	while ( type )
	{
		(*type)->finalize( *newResourceSet );
		type++;
	}
	
	//***************************************************************************
	
	identifier.setName( newResourceSet->getName() );
	
	//***************************************************************************
	
	// Reset the type table.
	typeTable.reset();
	
	return newResourceSet;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version 1 Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
void ResourceSetTranscoder:: encodeVersion<1>( const ResourceID& identifier, const ResourceSet& resources,
												const ResourceSet* resourceSet, ResourceManager* manager )
{
	//***************************************************************************
	// Write the version header information.
	
	// Write the total header size in bytes.
	UInt64 headerSize = 0;
	encoder.write( headerSize );
	
	// Write the name of the resource set.
	encoder.writeUTF8String( resources.getName() );
	
	// Write the description of the resource set.
	encoder.writeUTF8String( resources.getDescription() );
	
	//***************************************************************************
	// Build the type table for the resource set.
	
	const Size numResourceSetTypes = resources.getTypeCount();
	
	for ( Index i = 0; i < numResourceSetTypes; i++ )
	{
		const ResourceTypeSetBase* resourceList = resources.getType(i);
		const Size numResources = resourceList->getResourceCount();
		const ResourceType& resourceType = resourceList->getResourceType();
		
		// Get the type transcoder that should be used for this type.
		TypeTranscoderBase** typeTranscoderPointer = resourceTypes.get( resourceType.getHashCode(), resourceType );
		TypeTranscoderBase* typeTranscoder = typeTranscoderPointer ? *typeTranscoderPointer : NULL;
		
		// Add a new table for this type to the type table.
		typeTable.types.addNew( resourceType, typeTranscoder, 0, numResources );
		ObjectTable& objectTable = typeTable.types.getLast();
		objectTable.resourceTypeID = i;
		
		// Get the resources of the type.
		for ( Index i = 0; i < numResources; i++ )
		{
			const OpaqueResource& resource = resourceList->getOpaqueResource(i);
			const ResourceID* resourceID = resource.getID();
			
			// Add a reference for the object to the table.
			if ( resourceID )
				objectTable.objects.addNew( resourceID->getName(), resourceID->getURL(), resourceID->getFlags() );
			else
				objectTable.objects.addNew();
		}
	}
	
	//***************************************************************************
	// Encode a placeholder for the type table.
	
	// Remember the start offset of the type table.
	const UInt64 typeTableStart = encoder.getPosition();
	
	// Write the number of types.
	const UInt64 numTypes = typeTable.types.getSize();
	encoder.write( numTypes );
	
	// Write the table of types.
	for ( Index i = 0; i < numTypes; i++ )
	{
		const ObjectTable& objectType = typeTable.types[i];
		
		// Write the offset in the file where the object table for this type is located.
		encoder.write( objectType.fileOffset );
		
		// Write the number of objects in the object table.
		const UInt64 numObjects = objectType.objects.getSize();
		encoder.write( numObjects );
		
		// Write the resource type.
		encoder.writeUTF8String( objectType.resourceType.getName() );
	}
	
	//***************************************************************************
	// Encode a placeholder for each type's object table.
	
	// Write the object table for each type.
	for ( Index i = 0; i < numTypes; i++ )
	{
		ObjectTable& objectType = typeTable.types[i];
		const Size numObjects = objectType.objects.getSize();
		
		// Remember the start of the object table for this type.
		objectType.fileOffset = encoder.getPosition();
		
		for ( Index o = 0; o < numObjects; o++ )
		{
			const ObjectInfo& object = objectType.objects[o];
			
			// Write the offset in the file where the data for the object is located.
			encoder.write( object.fileOffset );
			
			// Write the size in bytes of the data for the object.
			encoder.write( object.sizeInBytes );
			
			// Write the flags for the object.
			encoder.write( (UInt64)object.flags );
			
			// Write the name of the object.
			encoder.writeUTF8String( object.name );
			
			// Write the file path of the object.
			encoder.writeUTF8String( object.path );
		}
	}
	
	//***************************************************************************
	// Encode all of the objects of each type in the resource set.
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		ObjectTable& objectType = typeTable.types[i];
		TypeTranscoderBase* typeTranscoder = objectType.typeTranscoder;
		
		// Skip the type if the transcoder is not set.
		if ( typeTranscoder == NULL )
			continue;
		
		// Encode the objects for the type.
		typeTranscoder->encode( identifier, resources, objectType, encoder, manager );
	}
	
	//***************************************************************************
	// Update the object type table with the final information.
	
	// Seek to the start of the type table.
	Int64 seekAmount = (Int64)typeTableStart - (Int64)encoder.getPosition();
	
	if ( seekAmount == 0 || encoder.seek( seekAmount ) == seekAmount )
	{
		// Write the number of types.
		const UInt64 numTypes = typeTable.types.getSize();
		encoder.write( numTypes );
		
		// Write the table of types.
		for ( Index i = 0; i < numTypes; i++ )
		{
			const ObjectTable& objectType = typeTable.types[i];
			
			// Write the offset in the file where the object table for this type is located.
			encoder.write( objectType.fileOffset );
			
			// Write the number of objects in the object table.
			const UInt64 numObjects = objectType.objects.getSize();
			encoder.write( numObjects );
			
			// Write the resource type.
			encoder.writeUTF8String( objectType.resourceType.getName() );
		}
		
		// Write the object table for each type.
		for ( Index i = 0; i < numTypes; i++ )
		{
			const ObjectTable& objectType = typeTable.types[i];
			const Size numObjects = objectType.objects.getSize();
			
			for ( Index o = 0; o < numObjects; o++ )
			{
				const ObjectInfo& object = objectType.objects[o];
				
				// Write the offset in the file where the data for the object is located.
				encoder.write( object.fileOffset );
				
				// Write the size in bytes of the data for the object.
				encoder.write( object.sizeInBytes );
			
				// Write the flags for the object.
				encoder.write( (UInt64)object.flags );
				
				// Write the name of the object.
				encoder.writeUTF8String( object.name );
				
				// Write the file path of the object.
				encoder.writeUTF8String( object.path );
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version 1 Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
void ResourceSetTranscoder:: decodeVersion<1>( ResourceID& identifier, ResourceSet& resources,
												ResourceSet* resourceSet, ResourceManager* manager )
{
	//***************************************************************************
	// Read the version header information.
	
	// Read the total header size in bytes.
	UInt64 headerSize = 0;
	decoder.read( headerSize );
	
	// Read the name of the resource set.
	resources.setName( decoder.readUTF8String() );
	
	// Read the description of the resource set.
	resources.setDescription( decoder.readUTF8String() );
	
	//***************************************************************************
	// Read the type table header.
	
	// Read the number of types in the table.
	UInt64 numTypes = 0;
	decoder.read( numTypes );
	
	//***************************************************************************
	// Read the type table types.
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		// Read the offset in the file where the object table for this type is located.
		UInt64 fileOffset = 0;
		decoder.read( fileOffset );
		
		// Read the number of objects in the object table.
		UInt64 numObjects = 0;
		decoder.read( numObjects );
		
		// Read the resource type.
		ResourceType resourceType( decoder.readUTF8String() );
		
		// Find the type transcoder for this type.
		TypeTranscoderBase* typeTranscoder = NULL;
		TypeTranscoderBase** typeTranscoderPointer;
		
		if ( resourceTypes.find( resourceType.getHashCode(), resourceType, typeTranscoderPointer ) )
			typeTranscoder = *typeTranscoderPointer;
		
		/// Add the type to the list of types.
		typeTable.types.addNew( resourceType, typeTranscoder, fileOffset, numObjects );
	}
	
	//***************************************************************************
	// Read the type table objects.
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		ObjectTable& objectTable = typeTable.types[i];
		
		// Seek to the object table offset.
		const Int64 seekAmount = (Int64)objectTable.fileOffset - (Int64)decoder.getPosition();
		
		// Skip this object type if the seek operation fails.
		if ( decoder.seek( seekAmount ) != seekAmount )
			continue;
		
		// Read each object table entry.
		const Size numObjects = objectTable.numObjects;
		for ( Index o = 0; o < numObjects; o++ )
		{
			// Read the offset in the file where the data for the object is located.
			UInt64 fileOffset = 0;
			decoder.read( fileOffset );
			
			// Read the size in bytes of the data for the object.
			UInt64 sizeInBytes = 0;
			decoder.read( sizeInBytes );
			
			// Read the flags of the object.
			UInt64 flags = 0;
			decoder.read( flags );
			
			// Read the name of the object.
			UTF8String name = decoder.readUTF8String();
			
			// Read the file path of the object.
			UTF8String path = decoder.readUTF8String();
			
			// Add the object to the type table.
			objectTable.objects.addNew( name, path, ResourceFlags((UInt32)flags), fileOffset, sizeInBytes );
		}
	}
	
	//***************************************************************************
	// Read the objects of each type.
	
	for ( Index i = 0; i < numTypes; i++ )
	{
		const ObjectTable& objectTable = typeTable.types[i];
		const Size numObjects = objectTable.objects.getSize();
		TypeTranscoderBase* const typeTranscoder = objectTable.typeTranscoder;
		
		// If there is no transcoder for this resource type, skip it.
		if ( typeTranscoder == NULL )
			continue;
		
		const ResourceType& objectType = typeTranscoder->getType();
		
		// Read each object.
		for ( Index o = 0; o < numObjects; o++ )
		{
			const ObjectInfo& objectInfo = objectTable.objects[o];
			
			// Create an ID for the object.
			ResourceID objectID( objectType, FORMAT, objectInfo.path, objectInfo.name, objectInfo.flags );
			
			// Make sure the object information is valid.
			if ( objectInfo.fileOffset != 0 && objectInfo.sizeInBytes > 0 )
			{
				// Seek to the object offset.
				const Int64 seekAmount = (Int64)objectInfo.fileOffset - (Int64)decoder.getPosition();
				
				// Make sure the seek was successful.
				if ( seekAmount == 0 || decoder.seek( seekAmount ) == seekAmount )
				{
					// Try to decode the object.
					if ( typeTranscoder->decode( objectID, resources, decoder, manager ) )
						continue;
				}
			}
			
			// Object is invalid or was not able to be read, get a NULL placeholder resource.
			typeTranscoder->getNullResource( objectID, resources );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceSetTranscoder:: encodeVersion( Index version, const ResourceID& identifier, const ResourceSet& resources,
											const ResourceSet* resourceSet, ResourceManager* manager )
{
	// Encode the resource set for each supported version number.
	switch ( version )
	{
		case 1:		encodeVersion<1>( identifier, resources, resourceSet, manager );	break;
	}
}




void ResourceSetTranscoder:: decodeVersion( Index version, ResourceID& identifier, ResourceSet& resources,
											ResourceSet* resourceSet, ResourceManager* manager )
{
	// Decode the resource set for each supported version number.
	switch ( version )
	{
		case 1:		decodeVersion<1>( identifier, resources, resourceSet, manager );	break;
	}
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
