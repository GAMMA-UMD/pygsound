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

#ifndef INCLUDE_OM_FILE_SYSTEM_NODE_H
#define INCLUDE_OM_FILE_SYSTEM_NODE_H


#include "omFileSystemConfig.h"


#include "omPath.h"


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the interface for a node within the global file system.
/**
  * A file system node can be either a file or a directory. The file system node allows
  * the user to query basic properties of the node (size, path, name, type) and to
  * create and remove the referenced file system node.
  */
class FileSystemNode
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a file system node object.
			virtual ~FileSystemNode()
			{
			}
			
			
		//********************************************************************************
		//******	Path Accessor Methods
			
			
			/// Return the name of the file system node, the last component of its path.
			OM_INLINE UTF8String getName() const
			{
				return path.getName();
			}
			
			
			/// Set the name of the file system node, the last component of its path.
			virtual Bool setName( const UTF8String& newName ) = 0;
			
			
			/// Return a string representing the extension of this node's file name.
			/**
			  * The extension is defined as the characters after the last period '.'
			  * in the node's file name.
			  */
			OM_INLINE UTF8String getExtension() const
			{
				return path.getExtension();
			}
			
			
			/// Return a path object representing the path to this file system node.
			OM_INLINE const Path& getPath() const
			{
				return path;
			}
			
			
			/// Return a string representing the path to the file system node.
			OM_INLINE const UTF8String& getPathString() const
			{
				return path.toString();
			}
			
			
		//********************************************************************************
		//******	Node Attribute Accessor Methods
			
			
			/// Return whether or not the file system node is a file.
			virtual Bool isFile() const = 0;
			
			
			/// Return whether or not the file system node is a directory.
			virtual Bool isDirectory() const = 0;
			
			
			/// Return whether or not the file system node is at the root level of the file system.
			OM_INLINE Bool isAtRoot() const
			{
				return path.isAtRoot();
			}
			
			
			/// Return whether or not this file system node exists.
			virtual Bool exists() const = 0;
			
			
			/// Return the total size of the file system node.
			/**
			  * For files, this is the total size of the file. For directories,
			  * this is the total size of all child file system nodes.
			  */
			virtual LargeSize getSize() const = 0;
			
			
		//********************************************************************************
		//******	Node Modification Methods
			
			
			/// Create this file system node if it doesn't exist.
			/**
			  * If the file system node already exists, no operation is performed
			  * and FALSE is returned. If the creation operation was not successful,
			  * FALSE is returned. Otherwise, TRUE is returned and the node is created.
			  */
			virtual Bool create() = 0;
			
			
			/// Remove this file system node and all children (if it is a directory).
			virtual Bool remove() = 0;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Constructor
			
			
			/// Create a file system node that is represented by the specified path string.
			OM_INLINE FileSystemNode( const UTF8String& newPathString )
				:	path( newPathString )
			{
			}
			
			
			/// Create a file system node that is represented by the specified path.
			OM_INLINE FileSystemNode( const Path& newPath )
				:	path( newPath )
			{
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The path to this file system node.
			Path path;
			
			
			
};




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FILE_SYSTEM_NODE_H
