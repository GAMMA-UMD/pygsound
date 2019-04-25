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

#include "omResourceFormat.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceFormat ResourceFormat:: UNDEFINED( "?", "?" );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ResourceFormat:: ResourceFormat()
	:	name(UNDEFINED.getName())
{
}




ResourceFormat:: ResourceFormat( const data::UTF8String& newName )
	:	name( newName )
{
}




ResourceFormat:: ResourceFormat( const data::UTF8String& newName,
								const data::UTF8String& newExtension )
	:	name( newName )
{
	extensions.add( newExtension );
}




ResourceFormat:: ResourceFormat( const data::UTF8String& newName,
								const data::UTF8String& newExtension1, const data::UTF8String& newExtension2 )
	:	name( newName )
{
	extensions.add( newExtension1 );
	extensions.add( newExtension2 );
}




ResourceFormat:: ResourceFormat( const data::UTF8String& newName,
								const data::UTF8String& newExtension1, const data::UTF8String& newExtension2,
								const data::UTF8String& newExtension3 )
	:	name( newName )
{
	extensions.add( newExtension1 );
	extensions.add( newExtension2 );
	extensions.add( newExtension3 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Extension Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceFormat:: hasExtension( const data::UTF8String& newExtension ) const
{
	const Size numExtensions = extensions.getSize();
	
	for ( Index i = 0; i < numExtensions; i++ )
	{
		if ( extensions[i].equalsIgnoreCase( newExtension ) )
			return true;
	}
	
	return false;
}




void ResourceFormat:: addExtension( const data::UTF8String& newExtension )
{
	return extensions.add( newExtension );
}




void ResourceFormat:: clearExtensions()
{
	extensions.clear();
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
