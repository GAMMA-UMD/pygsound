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

#ifndef INCLUDE_OM_FILE_H
#define INCLUDE_OM_FILE_H


#include "omFileSystemConfig.h"


#include "omFileSystemNode.h"


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a file in the global file system.
/**
  * A File object can represent a local file, network file, or any other type
  * of file resource. This class also allows the user to create and delete files
  * with the given file path and determine other basic information about the file.
  */
class File : public FileSystemNode
{
	public:
		
		//********************************************************************************
		//******	Access Type Enum
			
			
			/// An enum that describes the access type for a file.
			enum AccessType
			{
				/// File access where reading is enabled.
				READ = (1 << 0),
				
				/// File access where writing is enabled.
				WRITE = (1 << 1),
				
				/// File access where reading and writing is enabled.
				READ_WRITE = READ | WRITE,
				
				/// An undefined file access type.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a file object that corresponds to the specified path.
			File( const Path& newPath );
			
			
			/// Create a new file object that refers to the same file as another file.
			File( const File& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destory this file, releasing all file resources and unmapping the file if mapped.
			~File();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the reference to another to this file.
			File& operator = ( const File& other );
			
			
		//********************************************************************************
		//******	File Attribute Accessor Methods
			
			
			/// Return whether or not the file system node is a file.
			virtual Bool isFile() const;
			
			
			/// Return whether or not the file system node is a directory.
			virtual Bool isDirectory() const;
			
			
			/// Return whether or not this file system node exists.
			virtual Bool exists() const;
			
			
			/// Return the total size of the file system node.
			/**
			  * For files, this is the total size of the file. For directories,
			  * this is the total size of all child file system nodes. If the file
			  * does not exist, the size 0 is returned.
			  */
			virtual LargeSize getSize() const;
			
			
		//********************************************************************************
		//******	File Modification Methods
			
			
			/// Set the name of the file, the last component of its path.
			virtual Bool setName( const UTF8String& newName );
			
			
			/// Create this file if it doesn't exist.
			/**
			  * If the file system node already exists, no operation is performed
			  * and FALSE is returned. If the creation operation was not successful,
			  * FALSE is returned. Otherwise, TRUE is returned and the node is created.
			  */
			virtual Bool create();
			
			
			/// Delete this file system node and all children (if it is a directory).
			virtual Bool remove();
			
			
			/// Erase this file or create it if it doesn't exist.
			/**
			  * If there was an error during creation, FALSE is returned.
			  * Otherwise, TRUE is returned and the file is erased.
			  */
			Bool erase();
			
			
		//********************************************************************************
		//******	Memory Mapped File Methods
			
			
			/// Return whether or not this file is currently memory mapped.
			OM_INLINE Bool isMapped() const
			{
				return mappedFile != INVALID_FILE;
			}
			
			
			/// Memory-map the entire contents of this file and return a pointer to the memory.
			/**
			  * If the function succeeds, the file is mapped to a memory address via virtual memory,
			  * and a pointer to the file's contents is returned. The file has the given
			  * access type. Writes to the memory will asynchronously update the file contents.
			  *
			  * If the function fails, NULL is returned. The function can fail if the
			  * file is too big to fit into the program's virtual address space.
			  */
			void* map( AccessType accessType );
			
			
			/// Memory-map a portion of this file and return a pointer to the memory.
			/**
			  * If the function succeeds, the file contents with the specified offset
			  * and length are mapped to a memory address via virtual memory,
			  * and a pointer to the file's contents at that offset is returned. The file has the given
			  * access type. Writes to the memory will asynchronously update the file contents.
			  *
			  * The offset must be a multiple of the virtual memory page size.
			  *
			  * If the function fails, NULL is returned.
			  */
			void* map( AccessType accessType, LargeIndex offset, Size length );
			
			
			/// Unmap a previously-mapped memory location for this file.
			/**
			  * This method does not completely unmap the file, only a region
			  * of the file. To finish unmapping, call unmap().
			  *
			  * The pointer must be the same as a previously returned mapping pointer.
			  */
			Bool unmap( void* memory );
			
			
			/// Unmap this file entirely, closing it for access.
			void unmap();
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores information about a memory-mapped region of this file.
			class MappedRegion
			{
				public:
					
					/// Create a new memory mapped region with the given starting address and length.
					OM_INLINE MappedRegion( void* newStart, Size newLength )
						:	start( newStart ),
							length( newLength )
					{
					}
					
					
					/// The starting pointer of the mapped memory region.
					void* start;
					
					/// The length of the mapped region in bytes.
					Size length;
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Open this file using the specified access type.
			Bool open( AccessType accessType );
			
			
			/// Find the index of a previously mapped region.
			Bool findRegion( void* start, Index& regionIndex ) const;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
			static const int INVALID_FILE = -1;
#elif defined(OM_PLATFORM_WINDOWS)
			/// A win32 HANDLE for the file, if it is currently memory mapped.
			static void* INVALID_FILE;
#endif
			
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The access type of this file if it is currently memory mapped.
			AccessType mappedAccessType;
			
			
			/// A list of the memory-mapped regions of this file.
			ArrayList<MappedRegion> mappedRegions;
			
			
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
			/// A file descriptor handle, if it is currently memory mapped.
			int mappedFile;
#elif defined(OM_PLATFORM_WINDOWS)
			/// A win32 HANDLE for the file, if it is currently memory mapped.
			void* mappedFile;
			void* fileMapping;
#endif
			
			
			
};




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FILE_H
