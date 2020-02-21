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

#include "omFile.h"


#include "omDirectory.h"

#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/mman.h>
	#include <fcntl.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
	
	/// Define the type to use for windows unicode strings.
	typedef om::data::UTF16String TString;
#else
	#error Unsupported platform
#endif


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


#if defined(OM_PLATFORM_WINDOWS)
void* File:: INVALID_FILE = (void*)(-1);
#endif


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




File:: File( const Path& newPath )
	:	FileSystemNode( newPath.isRelative() ? Path( Directory::getCurrent(), newPath ) : newPath ),
		mappedFile( INVALID_FILE )
#if defined(OM_PLATFORM_WINDOWS)
		,fileMapping( NULL )
#endif
{
}




File:: File( const File& other )
	:	FileSystemNode( other ),
		mappedFile( INVALID_FILE )
#if defined(OM_PLATFORM_WINDOWS)
		,fileMapping( NULL )
#endif
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




File:: ~File()
{
	// Ummap the file if it is mapped.
	if ( this->isMapped() )
		this->unmap();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




File& File:: operator = ( const File& other )
{
	if ( this != &other )
	{
		// Ummap the file if it is mapped.
		if ( this->isMapped() )
			this->unmap();
		
		// Copy the file's path.
		path = other.path;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool File:: isFile() const
{
	return true;
}




Bool File:: isDirectory() const
{
	return false;
}




LargeSize File:: getSize() const
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	// If the file does not exist, return a size of 0.
	if ( success != 0 )
		return LargeSize(0);
	
	return LargeSize(fileData.st_size);
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	if ( !exists() )
		return LargeSize(0);
	
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	BOOL success;
	
	TString pathString = this->getPathString();
	success = GetFileAttributesExW( (LPCWSTR)pathString.getCString(), GetFileExInfoStandard, (void*)&fileData );
	
	if ( !success )
		return LargeSize(0);
	
	// Compute and return the 64-bit size.
	return (LargeSize(fileData.nFileSizeHigh) << 32) | LargeSize(fileData.nFileSizeLow);
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Modification Methods
//############		
//##########################################################################################
//##########################################################################################




Bool File:: setName( const UTF8String& newName )
{
	Path newPath = path.getParent() + newName;
	
	if ( std::rename( (const char*)getPathString().getCString(),
					(const char*)newPath.toString().getCString() ) == 0 )
	{
		path = newPath;
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Modification Methods
//############		
//##########################################################################################
//##########################################################################################




Bool File:: create()
{
	// Don't create a file if it already exists or is open.
	if ( exists() )
		return false;
	
	// Create the file's parent directory if it doesn't exist.
	Directory parent( getPath().getParent() );
	
	if ( !parent.exists() )
	{
		if ( !parent.create() )
			return false;
	}
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	int fd = creat( (const char*)getPathString().getCString(), S_IRWXU );
	
	// Make sure creation did not fail. Otherwise, close the new file handle.
	if ( fd < 0 )
		return false;
	else
		close( fd );
	
#elif defined(OM_PLATFORM_WINDOWS)
	TString pathString = this->getPathString();
	HANDLE fd = CreateFileW( (LPCWSTR)pathString.getCString(), GENERIC_WRITE, 0, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	
	// Make sure creation did not fail. Otherwise, close the new file handle.
	if ( fd == INVALID_HANDLE_VALUE )
		return false;
	else
		CloseHandle( fd );
#endif
	
	return true;
}




Bool File:: erase()
{
	// Create the file if it doesn't exist.
	if ( !exists() )
		return create();
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	int fd = ::open( (const char*)getPathString().getCString(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU );
	
	// Make sure truncation did not fail. Otherwise, close the new file handle.
	if ( fd < 0 )
		return false;
	else
		close( fd );
	
#elif defined(OM_PLATFORM_WINDOWS)
	TString pathString = this->getPathString();
	HANDLE fd = CreateFileW( (LPCWSTR)pathString.getCString(), GENERIC_WRITE, 0, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	// Make sure truncation did not fail. Otherwise, close the new file handle.
	if ( fd == INVALID_HANDLE_VALUE )
		return false;
	else
		CloseHandle( fd );
#endif
	
	return true;
}




Bool File:: exists() const
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	return success == 0;

#elif defined(OM_PLATFORM_WINDOWS)
	
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	
	TString pathString = this->getPathString();
	BOOL success = GetFileAttributesExW( (LPCWSTR)pathString.getCString(), GetFileExInfoStandard, (void*)&fileData );
	
	if ( !success )
		return false;
	
	// Make sure the file is not a directory.
	return !(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	
#endif
}





Bool File:: remove()
{
	if ( !exists() )
		return true;
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)

	return unlink( (const char*)getPathString().getCString() ) == 0;
	
#elif defined(OM_PLATFORM_WINDOWS)

	TString pathString = this->getPathString();
	return DeleteFile( (LPCTSTR)pathString.getCString() ) != BOOL(0);
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Memory Mapping Methods
//############		
//##########################################################################################
//##########################################################################################




Bool File:: open( AccessType accessType )
{
	if ( this->isMapped() )
		return false;
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	int flags = 0;
	
	switch ( accessType )
	{
		case READ:			flags |= O_RDONLY;				break;
		case WRITE:			flags |= O_WRONLY | O_CREAT;	break;
		case READ_WRITE:	flags |= O_RDWR | O_CREAT;		break;
		default:
			return false;
	}
	
	int fd = ::open( (const char*)getPathString().getCString(), flags, S_IRWXU );
	
	// Make sure opening did not fail.
	if ( fd < 0 )
	{
		mappedFile = INVALID_FILE;
		mappedAccessType = UNDEFINED;
		return false;
	}
	
	mappedFile = fd;
	mappedAccessType = accessType;
	
#elif defined(OM_PLATFORM_WINDOWS)
	DWORD readWriteStatus = 0;
	DWORD createStatus = 0;
	DWORD protection = 0;
	
	switch ( accessType )
	{
		case READ:
			readWriteStatus = GENERIC_READ;
			createStatus = OPEN_EXISTING;
			protection = PAGE_READONLY;
			break;
		case WRITE:
			readWriteStatus = GENERIC_WRITE;
			createStatus = OPEN_ALWAYS;
			protection = PAGE_READWRITE;
			break;
		case READ_WRITE:
			readWriteStatus = GENERIC_READ | GENERIC_WRITE;
			createStatus = OPEN_ALWAYS;
			protection = PAGE_READWRITE;
			break;
		default:
			return false;
	}
	
	TString pathString = this->getPathString();
	HANDLE fd = CreateFileW( (LPCTSTR)pathString.getCString(), readWriteStatus, 0, NULL,
							createStatus, FILE_ATTRIBUTE_NORMAL, NULL );
	
	// Make sure opening did not fail.
	if ( fd == INVALID_HANDLE_VALUE )
	{
		mappedFile = INVALID_FILE;
		mappedAccessType = UNDEFINED;
		return false;
	}
	
	// Create a file mapping for the file.
	HANDLE mapping = CreateFileMappingW( fd, NULL, protection, 0, 0, NULL );
	
	if ( mapping == NULL )
	{
		// Close the opened file since creating a mapping failed.
		CloseHandle( fd );
		return false;
	}
	
	mappedFile = fd;
	fileMapping = mapping;
	mappedAccessType = accessType;
	
#endif
	
	return true;
}




void* File:: map( AccessType accessType )
{
	// If this file is not already opened, open it for mapping.
	if ( !this->isMapped() && !this->open( accessType ) )
		return NULL;
	
	LargeSize fileSize = this->getSize();
	
	// Make sure the file is not too big for the address space.
	if ( fileSize >= LargeSize(math::max<Size>()) )
		return NULL;
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	// Determine the memory page access type.
	int protection = PROT_NONE;
	
	switch ( accessType )
	{
		case READ:			protection = PROT_READ;					break;
		case WRITE:			protection = PROT_WRITE;				break;
		case READ_WRITE:	protection = PROT_READ | PROT_WRITE;	break;
//		default:
//			return false;
	}
	
	// Map the file.
	void* result = mmap( NULL, size_t(fileSize), protection, MAP_SHARED, mappedFile, off_t(0) );
	
	// If the mapping was successful, add it to the internal list of mappings.
	if ( result != NULL )
		mappedRegions.add( MappedRegion( result, Size(fileSize) ) );
	
	return result;
	
#elif defined(OM_PLATFORM_WINDOWS)
	// Determine the memory page access type.
	DWORD protection = 0;
	
	switch ( accessType )
	{
		case READ:			protection = FILE_MAP_READ;		break;
		case WRITE:			protection = FILE_MAP_WRITE;	break;
		case READ_WRITE:	protection = FILE_MAP_WRITE;	break;
//		default:
//			return false;
	}
	
	// Map the file.
	void* result = MapViewOfFile( fileMapping, protection, 0, 0, Size(fileSize) );
	
	// If the mapping was successful, add it to the internal list of mappings.
	if ( result != NULL )
		mappedRegions.add( MappedRegion( result, Size(fileSize) ) );
	
	return result;
#endif
}




void* File:: map( AccessType accessType, LargeIndex offset, Size length )
{
	// If this file is not already opened, open it for mapping.
	if ( !this->isMapped() && !this->open( accessType ) )
		return NULL;
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	// Determine the memory page access type.
	int protection = PROT_NONE;
	
	switch ( accessType )
	{
		case READ:			protection = PROT_READ;					break;
		case WRITE:			protection = PROT_WRITE;				break;
		case READ_WRITE:	protection = PROT_READ | PROT_WRITE;	break;
//		default:
//			return false;
	}
	
	// Map the file region.
	void* result = mmap( NULL, length, protection, MAP_SHARED, mappedFile, off_t(offset) );
	
	// If the mapping was successful, add it to the internal list of mappings.
	if ( result != NULL )
		mappedRegions.add( MappedRegion( result, length ) );
	
	return result;
	
#elif defined(OM_PLATFORM_WINDOWS)
	// Determine the memory page access type.
	DWORD protection = 0;
	
	switch ( accessType )
	{
		case READ:			protection = FILE_MAP_READ;		break;
		case WRITE:			protection = FILE_MAP_WRITE;	break;
		case READ_WRITE:	protection = FILE_MAP_WRITE;	break;
//		default:
//			return false;
	}
	
	// Compute the low and high part of the 64-bit offset.
	DWORD lowOffset = (LargeIndex(offset) & LargeSize(0xFFFFFFFF));
	DWORD highOffset = (LargeIndex(offset) >> 32);
	
	// Map the file region.
	void* result = MapViewOfFile( fileMapping, protection, highOffset, lowOffset, length );
	
	// If the mapping was successful, add it to the internal list of mappings.
	if ( result != NULL )
		mappedRegions.add( MappedRegion( result, length ) );
	
	return result;
#endif
}




Bool File:: unmap( void* memory )
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	Index regionIndex;
	
	if ( !this->findRegion( memory, regionIndex ) )
		return false;
	
	// Unmap the region.
	int result = munmap( memory, mappedRegions[regionIndex].length );
	
	if ( result != 0 )
		return false;
	
	// Update internal data structures.
	mappedRegions.removeAtIndexUnordered( regionIndex );
	
#elif defined(OM_PLATFORM_WINDOWS)
	Index regionIndex;
	
	if ( !this->findRegion( memory, regionIndex ) )
		return false;
	
	// Unmap the region.
	BOOL result = UnmapViewOfFile( memory );
	
	if ( result == 0 )
		return false;
	
	// Update internal data structures.
	mappedRegions.removeAtIndexUnordered( regionIndex );
	
#endif
	return true;
}




void File:: unmap()
{
	if ( mappedFile != INVALID_FILE )
	{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
		// Unmap all previously mapped regions that have not been unmapped.
		const Size numRegions = mappedRegions.getSize();
		
		for ( Index i = 0; i < numRegions; i++ )
		{
			const MappedRegion& region = mappedRegions[i];
			munmap( region.start, region.length );
		}
		
		// Close the memory mapped file.
		close( mappedFile );
		
#elif defined(OM_PLATFORM_WINDOWS)
		// Unmap all previously mapped regions that have not been unmapped.
		const Size numRegions = mappedRegions.getSize();
		
		for ( Index i = 0; i < numRegions; i++ )
		{
			const MappedRegion& region = mappedRegions[i];
			UnmapViewOfFile( region.start );
		}
		
		CloseHandle( fileMapping );
		CloseHandle( mappedFile );
		fileMapping = NULL;
#endif
		
		// Clear this list of mapped regions.
		mappedRegions.clear();
		mappedFile = INVALID_FILE;
		mappedAccessType = UNDEFINED;
	}
}




Bool File:: findRegion( void* start, Index& regionIndex ) const
{
	for ( Index i = 0; i < mappedRegions.getSize(); i++ )
	{
		const MappedRegion& region = mappedRegions[i];
		
		if ( region.start == start )
		{
			regionIndex = i;
			return true;
		}
	}
	
	return false;
}




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
