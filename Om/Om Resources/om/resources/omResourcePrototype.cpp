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

#include "omResourcePrototype.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceType Prototype:: RESOURCE_TYPE("Prototype");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Prototype:: Prototype()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Prototype:: ~Prototype()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Component Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Prototype:: hasComponent( const void* data ) const
{
	if ( data == NULL )
		return false;
	
	const Size numComponents = components.getSize();
	
	for ( Index i = 0; i < numComponents; i++ )
	{
		if ( components[i].resource.getData() == data )
			return true;
	}
	
	return false;
}




Bool Prototype:: getComponentIndex( const void* data, Index& componentIndex ) const
{
	if ( data == NULL )
		return false;
	
	const Size numComponents = components.getSize();
	
	for ( Index i = 0; i < numComponents; i++ )
	{
		if ( components[i].resource.getData() == data )
		{
			componentIndex = i;
			return true;
		}
	}
	
	return false;
}




void Prototype:: addComponent( const Component& newComponent )
{
	components.add( newComponent );
}




Bool Prototype:: insertComponent( Index componentIndex, const Component& newComponent )
{
	if ( componentIndex > components.getSize() )
		return false;
	
	components.insert( componentIndex, newComponent );
	
	return true;
}




void Prototype:: removeComponent( Index componentIndex )
{
	if ( componentIndex >= components.getSize() )
		return;
	
	components.removeAtIndex( componentIndex );
}




void Prototype:: clearComponents()
{
	components.clear();
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
