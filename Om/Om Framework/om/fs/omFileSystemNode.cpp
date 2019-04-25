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

#include "omFileSystemNode.h"


#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#include <sys/stat.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
	
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
//############		Date Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




#if defined(OM_PLATFORM_WINDOWS)
static time::Time convertFileTime( const FILETIME& fileTime )
{
	// Initialize a system time structure representing the epoch date.
	SYSTEMTIME epochSystemTime;
	epochSystemTime.wYear = 1970;
	epochSystemTime.wMonth = 1;
	epochSystemTime.wDayOfWeek = 4; // Thursday
	epochSystemTime.wDay = 1;
	epochSystemTime.wHour = 0;
	epochSystemTime.wMinute = 0;
	epochSystemTime.wSecond = 0;
	epochSystemTime.wMilliseconds = 0;
	
	// Convert the epoch system time into a file time.
	FILETIME epochFileTime;
	
	if ( !SystemTimeToFileTime( &epochSystemTime, &epochFileTime ) )
		return time::Time( 0 );
	
	// Copy the epoch time to a large integer.
	ULARGE_INTEGER epochTime;
	epochTime.LowPart = epochFileTime.dwLowDateTime;
	epochTime.HighPart = epochFileTime.dwHighDateTime;
	
	// Copy the current time to a large integer.
	ULARGE_INTEGER modifiedTime;
	modifiedTime.LowPart = fileTime.dwLowDateTime;
	modifiedTime.HighPart = fileTime.dwHighDateTime;
	
	// Compute the total offset since the epoch in nanoseconds.
	Int64 nanoseconds = Int64(modifiedTime.QuadPart - epochTime.QuadPart)*Int64(100);
	
	return time::Time( nanoseconds );
}
#endif




time::Date FileSystemNode:: getDateCreated() const
{
#if defined(OM_PLATFORM_APPLE)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	// If the file does not exist, return the current date.
	if ( success != 0 )
		return time::Date();
	
	Int64 nanoseconds = Int64(fileData.st_birthtimespec.tv_sec)*Int64(1000000000) + Int64(fileData.st_birthtimespec.tv_nsec);
	
	return time::Date( time::Time( nanoseconds ) );
	
#elif defined(OM_PLATFORM_LINUX)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	// If the file does not exist, return the current date.
	if ( success != 0 )
		return time::Date();
	
	return time::Date( time::Time(Int64(Int64(fileData.st_ctime)*Int64(1000000000))) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	if ( !exists() )
		return time::Date();
	
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	BOOL success;
	
	TString pathString = this->getPathString();
	success = GetFileAttributesExW( (LPCWSTR)pathString.getCString(), GetFileExInfoStandard, (void*)&fileData );
	
	if ( !success )
		return time::Date();
	
	return time::Date( convertFileTime( fileData.ftCreationTime ) );
#endif
}




time::Date FileSystemNode:: getDateModified() const
{
#if defined(OM_PLATFORM_APPLE)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	// If the file does not exist, return the current date.
	if ( success != 0 )
		return time::Date();
	
	Int64 nanoseconds = Int64(fileData.st_mtimespec.tv_sec)*Int64(1000000000) + Int64(fileData.st_mtimespec.tv_nsec);
	
	return time::Date( time::Time( nanoseconds ) );
	
#elif defined(OM_PLATFORM_LINUX)
	
	struct stat fileData;
	
	int success = stat( (const char*)getPathString().getCString(), &fileData );
	
	// If the file does not exist, return the current date.
	if ( success != 0 )
		return time::Date();
	
	return time::Date( time::Time(Int64(Int64(fileData.st_mtime)*Int64(1000000000))) );
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	if ( !exists() )
		return time::Date();
	
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	BOOL success;
	
	TString pathString = this->getPathString();
	success = GetFileAttributesExW( (LPCWSTR)pathString.getCString(), GetFileExInfoStandard, (void*)&fileData );
	
	if ( !success )
		return time::Date();
	
	return time::Date( convertFileTime( fileData.ftLastWriteTime ) );
#endif
}




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

