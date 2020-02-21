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

#ifndef INCLUDE_OM_DIRECTORY_H
#define INCLUDE_OM_DIRECTORY_H


#include "omFileSystemConfig.h"


#include "omFileSystemNode.h"


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a directory within the global file system.
/**
  * A directory is a collection of file system nodes (files or directories) that is
  * also a file system node. The directory class allows the user to query a directory's
  * size, name, path, and children, as well as create and destory directories.
  */
class Directory : public FileSystemNode
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a directory object representing the root directory.
			Directory();
			
			
			/// Create a directory object for the specified path.
			Directory( const Path& newPath );
			
			
		//********************************************************************************
		//******	File Attribute Accessor Methods
			
			
			/// Return whether or not the file system node is a file.
			virtual Bool isFile() const;
			
			
			/// Return whether or not the file system node is a directory.
			virtual Bool isDirectory() const;
			
			
			/// Return whether or not this directory exists.
			virtual Bool exists() const;
			
			
			/// Get the total size of the directory.
			/**
			  * This is the total size of all child file system nodes.
			  */
			virtual LargeSize getSize() const;
			
			
		//********************************************************************************
		//******	Directory Modification Methods
			
			
			/// Set the name of the directory, the last component of its path.
			virtual Bool setName( const UTF8String& newName );
			
			
			/// Create this directory if it doesn't exist.
			/**
			  * If the directory already exists, no operation is performed
			  * and TRUE is returned. If the creation operation was not successful,
			  * FALSE is returned. Otherwise, TRUE is returned and the node is created.
			  */
			virtual Bool create();
			
			
			/// Remove this directory and all children.
			virtual Bool remove();
			
			
		//********************************************************************************
		//******	Child Accessor Methods
			
			
			/// Return the number of child file system nodes this Directory has.
			OM_INLINE Size getChildCount() const
			{
				if ( !hasCachedChildren )
					const_cast<Directory*>(this)->cacheChildren();
				
				return children.getSize();
			}
			
			
			/// Return the name of the directory's child at the specified index.
			const UTF8String& getChildName( Index index ) const;
			
			
			/// Return the path to the directory child at the specified index.
			Path getChildPath( Index index ) const;
			
			
			/// Return whether or not the directory's child at the specified index is a file.
			Bool childIsFile( Index index ) const;
			
			
			/// Return whether or not the directory's child at the specified index is a directory.
			Bool childIsDirectory( Index index ) const;
			
			
			/// Return whether or not the directory has a child with the specified name.
			Bool hasChild( const UTF8String& name ) const;
			
			
			/// Refresh the directory's cache of child file system nodes.
			void refreshChildren();
			
			
		//********************************************************************************
		//******	Current Working Directory Accessor Methods
			
			
			/// Get the absolute path of the current working directory.
			static Path getCurrent();
			
			
			/// Set the path of the current working directory.
			/**
			  * If the attempt to set the current working directory to the specified
			  * path fails, FALSE is returned and the current working directory is not
			  * changed. Otherwise, if the attempt succeeds, TRUE is returned and the
			  * current working directory is set to the specified path.
			  */
			static Bool setCurrent( const Path& path );
			
			
		//********************************************************************************
		//******	Important Directory Accessor Methods
			
			
			/// Return the path to the directory which contains this application's executable.
			static Path getExecutable();
			
			
			/// Return the path to the system's applications directory.
			/**
			  * On Mac OS X, this is the "/Applications/" directory. On Windows
			  * this is the "C:/Program Files/" directory.
			  */
			static Path getApplications();
			
			
			/// Return the path to the current user's home folder.
			static Path getUser();
			
			
			/// Return the path to the current user's documents directory.
			static Path getDocuments();
			
			
			/// Return the path to the current user's desktop directory.
			static Path getDesktop();
			
			
			/// Return the path to the shared application data directory for all users.
			static Path getApplicationData();
			
			
			/// Return the path to the current user's application data directory.
			static Path getUserApplicationData();
			
			
	private:
		
		//********************************************************************************
		//******	Child Info
			
			
			/// A class that contains information about a child file system node of this directory.
			class ChildInfo
			{
				public:
					
					OM_INLINE ChildInfo( const UTF8String& newName, Bool newIsFile )
						:	name( newName ),
							isFile( newIsFile )
					{
					}
					
					
					UTF8String name;
					Bool isFile;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Cache the children of this directory if they haven't been cached yet.
			void cacheChildren();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the names of the child file system nodes of this directory.
			ArrayList<ChildInfo> children;
			
			
			/// Whether or not the children of this directory have been 
			Bool hasCachedChildren;
			
			
			
};




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DIRECTORY_H
