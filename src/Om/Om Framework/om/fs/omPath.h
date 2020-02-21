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

#ifndef INCLUDE_OM_PATH_H
#define INCLUDE_OM_PATH_H


#include "omFileSystemConfig.h"


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a path to a file or directory in the local file system (not networked, etc).
class Path
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a path corresponding to the root directory
			Path();
			
			
			/// Create a path from the specified NULL-terminated path string.
			/**
			  * This path string is parsed into a sequence of path elements for easier manipulation.
			  */
			Path( const Char* newPathString );
			
			
			/// Create a path from the specified path string.
			/**
			  * This path string is parsed into a sequence of path elements for easier manipulation.
			  */
			Path( const UTF8String& newPathString );
			
			
			/// Create a path from an existing path plus the child path specified in the second parameter.
			/**
			  * This creates a new path which consists of all path components from the first path
			  * plus any additional path components specified by the second path parameter.
			  */
			Path( const Path& path, const Path& children );
			
			
			/// Create a path from an existing path plus the child path string specified in the second parameter.
			/**
			  * This creates a new path which consists of all path components from the first path
			  * plus any additional path components specified by the second path parameter.
			  */
			Path( const Path& path, const UTF8String& children );
			
			
		//********************************************************************************
		//******	Path Equality Comparison Operators
			
			
			/// Return whether or not this path is equal to another.
			OM_INLINE Bool operator == ( const Path& other ) const
			{
				return pathString == other.pathString;
			}
			
			
			/// Return whether or not this path is not equal to another.
			OM_INLINE Bool operator != ( const Path& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Path String Accessor Methods
			
			
			/// Return the full string representing this path.
			OM_INLINE const UTF8String& toString() const
			{
				return pathString;
			}
			
			
			/// Return the full string representing this path.
			OM_INLINE const UTF8String& getString() const
			{
				return pathString;
			}
			
			
			/// Convert this path to a string object.
			OM_INLINE operator const UTF8String& () const
			{
				return this->toString();
			}
			
			
			/// Convert this path to an ASCII string object.
			/**
			  * This conversion may result in corruption of the path string if
			  * any unicode characters are present in the string
			  */
			OM_INLINE operator String () const
			{
				return String(this->toString());
			}
			
			
		//********************************************************************************
		//******	Path Name Accessor Methods
			
			
			/// Return the name of the file or directory specified by this path.
			/**
			  * This name is last component of the path string.
			  */
			UTF8String getName() const;
			
			
			/// Return the base name of the file or directory specified by this path before any file extension.
			/**
			  * The returned string is the same as if the extension (if it exists) had been removed from
			  * the output of getName().
			  */
			UTF8String getBaseName() const;
			
			
			/// Return a string representing the extension of this paths's file name.
			/**
			  * The extension is defined as the characters after the last period '.'
			  * in the last component of the path string, if present. If there is no
			  * extension, the empty string is returned.
			  */
			UTF8String getExtension() const;
			
			
		//********************************************************************************
		//******	Path Component Accessor Methods
			
			
			/// Return the number of components that make up this path.
			OM_INLINE Size getComponentCount() const
			{
				return components.getSize();
			}
			
			
			/// Return the name of path component at the specified index (relative to the first parent) in the path heirarchy.
			UTF8String getComponent( Index index ) const;
			
			
			/// Return the name of path component at the specified depth in the path heirarchy.
			/**
			  * If a depth of 0 is specified, the name of the path's last component is returned.
			  * A depth of 1 returns the name of the parent component of the last component and so on.
			  */
			UTF8String getComponentAtDepth( Index depth ) const;
			
			
			/// Return a path object that is the parent of this path object in the file system hierarchy.
			/**
			  * If the path represents a file system node at root level or is the root directory, the
			  * root path is returned.
			  */
			Path getParent() const;
			
			
			/// Return a path object that is the ancestor of this path object in the file system at the specified age.
			/**
			  * A depth of 0 returns a copy of this path object. A depth of 1 returns the parent directory of this
			  * path and so on. If the requested depth is greater than the depth of the path's hierarchy,
			  * the root path is returned.
			  */
			Path getParentAtDepth( Index depth ) const;
			
			
		//********************************************************************************
		//******	Path Component Add Methods
			
			
			/// Add all of the path components from the specified null-terminated path string as children of this path.
			void append( const Char* pathString );
			
			
			/// Add all of the path components from the specified path string as children of this path.
			void append( const UTF8String& pathString );
			
			
			/// Add all of the path components from the specified path as children of this path.
			void append( const Path& path );
			
			
			/// Add all of the path components from the specified path string as children of this path.
			OM_INLINE Path& operator += ( const Char* pathString )
			{
				this->append( pathString );
				return *this;
			}
			
			
			/// Add all of the path components from the specified path string as children of this path.
			OM_INLINE Path& operator += ( const UTF8String& pathString )
			{
				this->append( pathString );
				return *this;
			}
			
			
			/// Add all of the path components from the specified path as children of this path.
			OM_INLINE Path& operator += ( const Path& path )
			{
				this->append( path );
				return *this;
			}
			
			
			/// Return a new path as the concatenation of the path components from this and another path string.
			OM_INLINE Path operator + ( const Char* pathString ) const
			{
				return Path( *this, UTF8String( pathString ) );
			}
			
			
			/// Return a new path as the concatenation of the path components from this and another path string.
			OM_INLINE Path operator + ( const UTF8String& pathString ) const
			{
				return Path( *this, pathString );
			}
			
			
			/// Return a new path as the concatenation of the path components from this and another path.
			OM_INLINE Path operator + ( const Path& path ) const
			{
				return Path( *this, path );
			}
			
			
		//********************************************************************************
		//******	Path Component Remove Methods
			
			
			/// Remove the last path component from this path, resulting in the path representing its parent.
			void removeLast();
			
			
			/// Remove the specified number of path components from the path.
			/**
			  * Starting with the last path component, this method removes as many path components
			  * as possible from the path until either the specified number is removed or the
			  * root directory is reached.
			  */
			void removeLast( Size number );
			
			
		//********************************************************************************
		//******	Path Attribute Accessor Methods
			
			
			/// Return whether or not this path is relative to the current working directory.
			OM_INLINE Bool isRelative() const
			{
				return relative;
			}
			
			
			/// Return whether or not this path specifies a file or directory at root level.
			OM_INLINE Bool isAtRoot() const
			{
				return components.getSize() == 1 && !relative;
			}
			
			
			/// Return whether or not this path specifies the root system directory.
			OM_INLINE Bool isRoot() const
			{
				return components.getSize() == 0 && !relative;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Path Component Class Definition
			
			
			/// A class representing a section of a path string that is a component of the path.
			class PathComponent
			{
				public:
					
					OM_INLINE PathComponent( Index newStartIndex, Size newNumCharacters )
						:	startIndex( newStartIndex ),
							numCharacters( newNumCharacters )
					{
					}
					
					
					/// The index of the first character within the path string where this component starts.
					Index startIndex;
					
					
					
					/// The number of characters in this path component's name.
					Size numCharacters;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Methods
			
			
			static void sanitizePathString( const UTF8String& pathString, ArrayList<PathComponent>& components,
											Bool pathIsRelative, UTF8String& outputString );
			
			
			static void parsePathAndAddComponents( const UTF8String& pathString, ArrayList<PathComponent>& components,
													Bool& pathIsRelative );
			
			
			OM_FORCE_INLINE static Bool isParent( const PathComponent& component, const UTF8Char* pathString );
			
			
			/// Return whether or not the specified character is a path separator.
			OM_FORCE_INLINE static Bool isAPathSeparator( UTF8Char character );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string representing the entire path.
			UTF8String pathString;
			
			
			/// A list of the components of this Path.
			ArrayList<PathComponent> components;
			
			
			/// Whether or not the Path is relative to the current working directory.
			Bool relative;
			
			
};




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_PATH_H
