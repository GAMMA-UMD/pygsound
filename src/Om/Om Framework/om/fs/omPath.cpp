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

#include "omPath.h"


#if defined(OM_PLATFORM_WINDOWS)
	#define DEFAULT_PATH_SEPARATOR Char('\\')
	#define DEFAULT_ROOT_PATH UTF8String("\\")
#else
	#define DEFAULT_PATH_SEPARATOR Char('/')
	#define DEFAULT_ROOT_PATH UTF8String("/")
#endif


//##########################################################################################
//**************************  Start Om File System Namespace  ******************************
OM_FILE_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




using om::data::UTF8StringBuffer;
using om::data::UTF8StringIterator;




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Path:: Path()
	:	pathString( DEFAULT_ROOT_PATH ),
		relative( false )
{
}




Path:: Path( const Char* newPathString )
{
	if ( newPathString == NULL || *newPathString == '\0' )
	{
		// The path string is of zero length, make the path represent the root directory.
		pathString = DEFAULT_ROOT_PATH;
		relative = false;
	}
	else
	{
		UTF8String newPathStringUTF8( newPathString );
		parsePathAndAddComponents( newPathStringUTF8, components, relative );
		sanitizePathString( newPathStringUTF8, components, relative, pathString );
	}
}




Path:: Path( const UTF8String& newPathString )
{
	if ( newPathString.getLength() == Size(0) )
	{
		// The path string is of zero length, make the path represent the root directory.
		pathString = DEFAULT_ROOT_PATH;
		relative = false;
	}
	else
	{
		parsePathAndAddComponents( newPathString, components, relative );
		sanitizePathString( newPathString, components, relative, pathString );
	}
}




Path:: Path( const Path& path, const UTF8String& childString )
	:	pathString( path.pathString ),
		relative( path.relative )
{
	// Add the nodes from the first path object.
	components.addAll( path.components );
	
	// Add the nodes from the second path object.
	this->append( childString );
}




Path:: Path( const Path& path, const Path& childPath )
	:	pathString( path.pathString ),
		relative( path.relative )
{
	// Add the nodes from the first path object.
	components.addAll( path.components );
	
	// Add the nodes from the second path object.
	this->append( childPath );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Path:: getName() const
{
	if ( this->isRoot() )
		return UTF8String("/");
	else
	{
		const PathComponent& component = components.getLast();
		return UTF8String( pathString.getPointer() + component.startIndex, component.numCharacters );
	}
}




UTF8String Path:: getBaseName() const
{
	if ( !this->isRoot() )
	{
		// Get the string for the last path componet.
		const PathComponent& component = components.getLast();
		const UTF8Char* nameStart = pathString.getPointer() + component.startIndex;
		
		// Find the index of the last period in the path string.
		UTF8StringIterator i( nameStart );
		Index lastPeriodIndex = 0;
		
		while ( i )
		{
			UTF32Char c = *i;
			
			if ( c == '.' )
				lastPeriodIndex = i.getIndex();
			
			i++;
		}
		
		if ( lastPeriodIndex != 0 )
			return UTF8String( nameStart, lastPeriodIndex );
	}
	
	return this->getName();
}




UTF8String Path:: getExtension() const
{
	if ( !this->isRoot() )
	{
		// Get the string for the last path componet.
		const PathComponent& component = components.getLast();
		const UTF8Char* nameStart = pathString.getPointer() + component.startIndex;
		
		// Find the index of the last period in the path string.
		UTF8StringIterator i( nameStart );
		Index lastPeriodIndex = 0;
		
		while ( i )
		{
			UTF32Char c = *i;
			
			if ( c == '.' )
				lastPeriodIndex = i.getIndex() + 1;
			
			i++;
		}
		
		if ( lastPeriodIndex != 0 )
			return UTF8String( nameStart + lastPeriodIndex );
	}
	
	return UTF8String();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Component Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Path:: getComponent( Index index ) const
{
	OM_DEBUG_ASSERT_MESSAGE( index < components.getSize(), "Cannot access path component at invalid depth." );
	
	const PathComponent& component = components[index];
	return UTF8String( pathString.getPointer() + component.startIndex, component.numCharacters );
}




UTF8String Path:: getComponentAtDepth( Index depth ) const
{
	OM_DEBUG_ASSERT_MESSAGE( depth < components.getSize(), "Cannot access path component at invalid depth." );
	
	const PathComponent& component = components[components.getSize() - depth - 1];
	return UTF8String( pathString.getPointer() + component.startIndex, component.numCharacters );
}




Path Path:: getParent() const
{
	if ( this->isRoot() || this->isAtRoot() )
		return Path();
	else
	{
		Path result(*this);
		result.removeLast();
		
		return result;
	}
}




Path Path:: getParentAtDepth( Index depth ) const
{
	if ( this->isRoot() )
		return Path();
	else
	{
		Path result(*this);
		result.removeLast( depth );
		
		return result;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Component Add Methods
//############		
//##########################################################################################
//##########################################################################################




void Path:: append( const Char* childString )
{
	this->append( Path( childString ) );
}




void Path:: append( const UTF8String& childString )
{
	this->append( Path( childString ) );
}




void Path:: append( const Path& childPath )
{
	const Size numChildren = childPath.components.getSize();
	const Size relativeOffset = (Size)childPath.relative + pathString.getLength();
	const UTF8Char* const childPathString = childPath.pathString.getCString();
	
	// Update the path string.
	pathString = childPath.relative ? pathString + DEFAULT_PATH_SEPARATOR + childPath.pathString :
										pathString + childPath.pathString;
	
	Bool removedComponent = false;
	
	// Add the path components.
	for ( Index i = 0; i < numChildren; i++ )
	{
		const PathComponent& child = childPath.components[i];
		
		// If there is a reference to the parent directory, go up one level and remove the component.
		if ( isParent( child, childPathString ) && components.getSize() > 0 )
		{
			// Remove the last component.
			components.removeLast();
			removedComponent = true;
			continue;
		}
		
		components.add( PathComponent( child.startIndex + relativeOffset, child.numCharacters ) );
	}
	
	/// Sanitize the path string if any path components were collapsed.
	if ( removedComponent )
		sanitizePathString( pathString, components, relative, pathString );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Component Remove Methods
//############		
//##########################################################################################
//##########################################################################################




void Path:: removeLast()
{
	if ( components.getSize() > 1 )
	{
		const PathComponent& component = components.getLast();
		pathString = UTF8String( pathString.getPointer(), component.startIndex - 1 );
		components.removeLast();
	}
	else if ( components.getSize() == 1 )
	{
		pathString = UTF8String();
		components.clear();
	}
}




void Path:: removeLast( Size number )
{
	if ( number == 0 )
		return;
	
	if ( components.getSize() > number )
	{
		const PathComponent& component = components[components.getSize() - number];
		pathString = UTF8String( pathString.getPointer(), component.startIndex - 1 );
		components.removeLast( number );
	}
	else if ( components.getSize() <= number )
	{
		pathString = UTF8String();
		components.clear();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path String Sanitize Method
//############		
//##########################################################################################
//##########################################################################################




void Path:: sanitizePathString( const UTF8String& pathString, ArrayList<PathComponent>& components,
								Bool pathIsRelative, UTF8String& outputString )
{
	if ( components.getSize() == 0 )
	{
		if ( pathIsRelative )
			outputString = UTF8String();
		else
			outputString = UTF8String(DEFAULT_PATH_SEPARATOR);
	}
	else
	{
		// Create a buffer large enough to hold the original string.
		UTF8StringBuffer buffer( pathString.getLength() );
		
		// Append a path separator if this path is absolute.
#if !defined(OM_PLATFORM_WINDOWS)
		if ( !pathIsRelative )
			buffer.append(DEFAULT_PATH_SEPARATOR);
#endif
		const UTF8Char* const pathStringStart = pathString.getPointer();
		
		// Append each component string to the buffer.
		for ( Index i = 0; i < components.getSize(); )
		{
			PathComponent& component = components[i];
			const UTF8Char* const componentStart = pathStringStart + component.startIndex;
			
			if ( component.numCharacters == 1 )
			{
				if ( componentStart[0] == '.' )
				{
					// Ignore this component.
					components.removeAtIndex(i);
					continue;
				}
			}
			else if ( isParent( component, pathStringStart ) && i > 0 &&
					!isParent( components[i-1], buffer.getCString() ) )
			{
				// Remove the last path component and its path separator.
				components.removeAtIndex(i);
				i--;
				buffer.remove( components[i].numCharacters + 1 );
				components.removeAtIndex(i);
				continue;
			}
			
			// Save the new starting index of the path component.
			Index start = buffer.getSize();
			
			// Append the path component string to the buffer.
			buffer.append( componentStart, component.numCharacters );
			
			// Store the final start index of the path component.
			component.startIndex = start;
			
			// Move to the next path component.
			i++;
			
			// If this is not the last component, add a path separator.
			if ( i != components.getSize() )
				buffer.append(DEFAULT_PATH_SEPARATOR);
		}
		
		outputString = buffer.toString();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Parsing Method
//############		
//##########################################################################################
//##########################################################################################




void Path:: parsePathAndAddComponents( const UTF8String& pathString, ArrayList<PathComponent>& components,
										Bool& pathIsRelative )
{
	const UTF8Char* const pathStringStart = pathString.getCString();
	const UTF8Char* currentChar = pathStringStart;
	const UTF8Char* end = pathStringStart + pathString.getLength();
	
	// Assume a relative path.
	pathIsRelative = true;
	
#if defined(OM_PLATFORM_WINDOWS)
	// Check to see if the path starts with a drive letter or is a network drive.
	if ( pathString.getLength() >= 2 )
	{
		if ( UTF8String::isLetter(currentChar[0]) && currentChar[1] == ':' )
		{
			// This path starts with a drive letter. Add the drive letter as the first component.
			components.add( PathComponent( 0, 2 ) );
			currentChar += 2;
			pathIsRelative = false;
		}
		else if ( currentChar[0] == '\\' && currentChar[1] == '\\' )
		{
			// This path represents a network drive. Add the network drive name as the first component.
			currentChar += 2;
			
			while ( currentChar != end && !isAPathSeparator(*currentChar) )
				currentChar++;
			
			components.add( PathComponent( 0, currentChar - pathStringStart ) );
			pathIsRelative = false;
		}
	}
#elif defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	// Check to see if the path is relative to the root.
	if ( isAPathSeparator(*currentChar) )
	{
		currentChar++;
		pathIsRelative = false;
	}
#endif
	
	while ( currentChar < end )
	{
		const UTF8Char* componentStart = currentChar;
		
		// Look for the next path separator in the path string.
		while (	!isAPathSeparator(*currentChar) && currentChar != end )
			currentChar++;
		
		Size componentLength = currentChar - componentStart;
		
		if ( componentLength > 0 )
			components.add( PathComponent( componentStart - pathStringStart, componentLength ) );
		
		currentChar++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Parsing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Path:: isParent( const PathComponent& component, const UTF8Char* pathString )
{
	if ( component.numCharacters == 2 )
	{
		const UTF8Char* const componentStart = pathString + component.startIndex;
		
		if ( componentStart[0] == '.' && componentStart[1] == '.' )
			return true;
	}
	
	return false;
}





Bool Path:: isAPathSeparator( UTF8Char character )
{
	return character == '/' || character == '\\';
}




//##########################################################################################
//**************************  End Om File System Namespace  ********************************
OM_FILE_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
