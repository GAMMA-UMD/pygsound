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

#include "omDirectory.h"


#include "omFile.h"


#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#if defined(OM_PLATFORM_ANDROID)
		#include <dirent.h>
	#else
		#include <sys/dir.h>
	#endif

	#include <sys/stat.h>
	#include <unistd.h>
	#include <errno.h>
	
	#if defined(OM_PLATFORM_APPLE)
		#include <mach-o/dyld.h>
		#include <CoreServices/CoreServices.h>
	#else
		#include <pwd.h>
	#endif
	
#if defined(OM_PLATFORM_APPLE)
	#include <mach-o/dyld.h>
	#include <CoreServices/CoreServices.h>
#endif
	
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
	#include "shlobj.h"
	
	/// Define the type to use for windows unicode strings.
	typedef om::data::UTF16String TString;
#else
	#error unsupported platform
#endif


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Directory:: Directory()
	:	FileSystemNode( Path() ),
		hasCachedChildren( false )
{
}




Directory:: Directory( const Path& newPath )
	:	FileSystemNode( newPath.isRelative() ? Path( Directory::getCurrent(), newPath ) : newPath ),
		hasCachedChildren( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Child Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const UTF8String& Directory:: getChildName( Index index ) const
{
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
	OM_DEBUG_ASSERT_MESSAGE( index < children.getSize(), "Cannot access directory child at invalid index." );
	
	return children[index].name;
}




Path Directory:: getChildPath( Index index ) const
{
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
	OM_DEBUG_ASSERT_MESSAGE( index < children.getSize(), "Cannot access directory child at invalid index." );
	
	return Path( this->getPath(), children[index].name );
}




Bool Directory:: childIsFile( Index index ) const
{
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
	if ( index < children.getSize() )
		return !children[index].isFile;
	else
		return false;
}




Bool Directory:: childIsDirectory( Index index ) const
{
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
	if ( index < children.getSize() )
		return !children[index].isFile;
	else
		return false;
}




Bool Directory:: hasChild( const UTF8String& childName ) const
{
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
	for ( Index i = 0; i < children.getSize(); i++ )
	{
		if ( children[i].name.equalsIgnoreCase(childName) )
			return true;
	}
	
	return false;
}




void Directory:: refreshChildren()
{
	hasCachedChildren = false;
	children.clear();
	cacheChildren();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directory Attribute Accesor Method
//############		
//##########################################################################################
//##########################################################################################




Bool Directory:: isFile() const
{
	return false;
}




Bool Directory:: isDirectory() const
{
	return true;
}




LargeSize Directory:: getSize() const
{
	if ( !exists() )
		return LargeSize(0);
	
	// Cache the directory's children.
	if ( !hasCachedChildren )
		const_cast<Directory*>(this)->cacheChildren();
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	// Get the size of the directory information.
	struct stat fileData;
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	if ( success != 0 )
		return LargeSize(0);
	
	LargeSize size = 0;
	
	// Sum the size of all children.
	for ( Index i = 0; i < children.getSize(); i++ )
	{
		if ( children[i].isFile )
			size += File( getChildPath(i) ).getSize();
		else
			size += Directory( getChildPath(i) ).getSize();
	}
	
	return size;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	LargeSize size = 0;
	
	// Sum the size of all children.
	for ( Index i = 0; i < children.getSize(); i++ )
	{
		if ( children[i].isFile )
			size += File( getChildPath(i) ).getSize();
		else
			size += Directory( getChildPath(i) ).getSize();
	}
	
	return size;
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directory Existance Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool Directory:: exists() const
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	DIR* directory = opendir( (const char*)getPathString().getCString() );
	
	if ( directory != NULL )
	{
		closedir(directory);
		
		return true;
	}
	else
		return false;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	// Convert to windows wide string.
	TString tString( getPathString() );
	
	return GetFileAttributesW( (LPCWSTR)tString.getCString() ) != -1;
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directory Remove Method.
//############		
//##########################################################################################
//##########################################################################################




Bool Directory:: remove()
{
	if ( !exists() )
		return true;
	
	// Make sure the directory contents are up to date.
	refreshChildren();
	Bool success = true;
	
	// remove all children recursively.
	for ( Index i = 0; i < children.getSize(); i++ )
	{
		if ( children[i].isFile )
			success &= File( getChildPath(i) ).remove();
		else
			success &= Directory( getChildPath(i) ).remove();
	}
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	children.clear();
	hasCachedChildren = false;
	
	// remove this directory
	success &= rmdir( (const char*)getPathString().getCString() ) == 0;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	// Convert to windows wide string.
	TString tString( getPathString() );
	
	success &= RemoveDirectoryW( (LPCWSTR)tString.getCString() ) != 0;
	
#endif
	
	return success;
}





//##########################################################################################
//##########################################################################################
//############		
//############		Directory Renaming Method
//############		
//##########################################################################################
//##########################################################################################




Bool Directory:: setName( const UTF8String& newName )
{
	if ( std::rename( (const char*)getPathString().getCString(),
					(const char*)newName.getCString() ) == 0 )
	{
		path = path.getParent() + newName;
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directory Create Method.
//############		
//##########################################################################################
//##########################################################################################




Bool Directory:: create()
{
	if ( exists() )
		return true;
	
	const Path& path = this->getPath();
	
	Bool success = true;
	
	if ( path.getComponentCount() > 1 )
	{
		// create all parent directories if they don't exist
		for ( Index i = path.getComponentCount() - 1; i >= 1; i-- )
		{
			Directory parent( path.getParentAtDepth(i) );
			
			if ( !parent.exists() )
				success &= parent.create();
		}
	}
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	// create this directory.
	success &= mkdir( (const char*)getPathString().getCString(), S_IRWXU ) == 0;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TString pathString = path.toString();
	success &= CreateDirectoryW( (LPCWSTR)pathString.getCString(), NULL ) != 0;
	
#endif
	
	return success;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Working Directory Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Path Directory:: getCurrent()
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	errno = 0;
	Size bufferSize = 1024;
	Char* buffer = NULL;
	Char* path = NULL;
	
	while ( path == NULL )
	{
		buffer = util::allocate<Char>( bufferSize );
		
		path = getcwd( buffer, bufferSize );
		
		if ( path == NULL )
		{
			if ( errno == ENOMEM || errno == ERANGE )
			{
				util::deallocate( buffer );
				bufferSize *= 2;
			}
			else
			{
				util::deallocate( buffer );
				return Path();
			}
		}
	}
	
	UTF8String pathString( buffer );
	
	// Clean up the temporary buffer.
	util::deallocate( buffer );
	
	return Path( pathString );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	// Get the required buffer size.
	DWORD bufferSize = GetCurrentDirectory( 0, NULL );
	
	// Allocate a buffer of wide characters to hold the directory name.
	TCHAR* buffer = util::allocate<TCHAR>( bufferSize );
	
	// Get the current directory name.
	DWORD numCharsInBuffer = GetCurrentDirectory( bufferSize, buffer );
	
	if ( numCharsInBuffer == 0 || numCharsInBuffer > bufferSize )
		return Path();
	
	data::UTF8String pathString( (const UTF16Char*)buffer );
	
	// Clean up the temporary buffer.
	util::deallocate( buffer );
	
	return Path( pathString );
	
#endif
}




Bool Directory:: setCurrent( const Path& path )
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	data::UTF8String pathString = path.toString();
	
	int result = chdir( (const char*)pathString.getCString() );
	
	return result != -1;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TString pathString = path.toString();
	
	// Set the current directory name.
	return SetCurrentDirectoryW( (LPCWSTR)pathString.getCString() ) != FALSE;
	
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Important Directory Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Path Directory:: getExecutable()
{
#if defined(OM_PLATFORM_APPLE)
	
	UInt32 bufferSize = PATH_MAX;
	char pathToExecutable[PATH_MAX];
	_NSGetExecutablePath( pathToExecutable, &bufferSize );
	
	char realPath[bufferSize];
	char* result = realpath( pathToExecutable, realPath );
	
	if ( result )
		return Path( UTF8String( realPath ) ).getParent();
	else
		return Path( UTF8String( (const UTF8Char*)pathToExecutable, bufferSize ) ).getParent();
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	const Size bufferSize = 1024;
	TCHAR pathToExecutable[1024];
	const Size resultLength = GetModuleFileName( NULL, pathToExecutable, (DWORD)bufferSize );
	
	Path path = UTF8String( (const UTF16Char*)pathToExecutable, resultLength );
	
	return path.getParent();
	
#endif
}




Path Directory:: getApplications()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kLocalDomain, kApplicationsFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




Path Directory:: getUser()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kUserDomain, kCurrentUserFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




Path Directory:: getDocuments()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kUserDomain, kDocumentsFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




Path Directory:: getDesktop()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kUserDomain, kDesktopFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




Path Directory:: getUserApplicationData()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kUserDomain, kApplicationSupportFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




Path Directory:: getApplicationData()
{
#if defined(OM_PLATFORM_APPLE)
	
	FSRef folder;
	
	if ( FSFindFolder( kLocalDomain, kApplicationSupportFolderType, kDontCreateFolder, &folder ) != noErr )
		return Path();
	
	const Size bufferSize = PATH_MAX;
	UTF8Char path[bufferSize];
	
	if ( FSRefMakePath( &folder, path, bufferSize ) != noErr )
		return Path();
	
	return Path( UTF8String( path ) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	TCHAR path[MAX_PATH];
	
	if ( !SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)) )
		return Path();
	
	return Path( UTF8String( (const UTF16Char*)path ) );
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directory Child Cache Method.
//############		
//##########################################################################################
//##########################################################################################




void Directory:: cacheChildren()
{
	if ( hasCachedChildren )
		return;
	
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	DIR* directory = opendir((const char*)getPathString().getCString());
	
	if ( directory == NULL )
		return;
	
	struct dirent* child = readdir(directory);
	
	while ( child != NULL )
	{
		UTF8String childName(child->d_name);
		UTF8String childFullPath = getPathString() + '/' + childName;
		
		struct stat info;
		
		// Get information for this file system node.
		if ( stat( (const char*)childFullPath.getCString(), &info ) != 0 )
		{
			// Error getting file info. Ignore this file.
			break;
		}
		
		// is the child a directory?
		if ( S_ISDIR(info.st_mode) && childName != "." && childName != ".." )
		{
			// the child is a directory
			children.add( ChildInfo( childName, false ) );
		}
		else if ( S_ISREG(info.st_mode) )
		{
			// the child is a file
			children.add( ChildInfo( childName, true ) );
		}
		else
		{
			// something else, ignore for now
		}
		
		child = readdir(directory);
	}
	
	closedir(directory);
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	// A structure which holds information about the files found in this directory.
	WIN32_FIND_DATAW findData;
	
	// Find the first file in the directory and get a handle to the query.
	HANDLE queryHandle = FindFirstFileW( (LPCWSTR)TString(getPathString() + "\\*").getCString(), &findData );
	
	// If there was an error or no files were found, abort.
	if ( queryHandle == INVALID_HANDLE_VALUE )
		return;

	do
	{
		UTF8String childName((UTF16Char*)findData.cFileName);
		
		if ( childName == "." || childName == ".." )
			continue;
		
		// Is the child a directory?
		if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// The child is a directory.
			children.add( ChildInfo( childName, false ) );
		}
		else
		{
			// The child is some type of file.
			children.add( ChildInfo( childName, true ) );
		}
	}
	while ( FindNextFileW( queryHandle, &findData ) != 0 );
	
	// Clean up the query.
	FindClose( queryHandle );
	
#endif
	
	hasCachedChildren = true;
}




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

