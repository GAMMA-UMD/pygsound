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

#ifndef INCLUDE_OM_RESOURCE_SUB_TYPE_H
#define INCLUDE_OM_RESOURCE_SUB_TYPE_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents the sub type of a generic resource.
/**
  * An resource sub type is defined by a unique string.
  */
class ResourceSubType
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resource sub type with an UNDEFINED sub type string.
			OM_INLINE ResourceSubType()
				:	name(UNDEFINED.getName())
			{
			}
			
			
			/// Create a new resource sub type with the specified constant NULL-terminated sub type string.
			OM_INLINE ResourceSubType( const Char* newName )
				:	name( newName )
			{
			}
			
			
			/// Create a new resource sub type with the specified sub type string.
			OM_INLINE ResourceSubType( const data::UTF8String& newName )
				:	name( newName )
			{
			}

			
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a string representing the name of this resource sub type.
			OM_INLINE const data::UTF8String& getName() const
			{
				return name;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this resource sub type is equal to another.
			OM_INLINE Bool operator == ( const ResourceSubType& other ) const
			{
				return name == other.name;
			}
			
			
			/// Return whether or not this resource sub type is equal to another.
			OM_INLINE Bool operator != ( const ResourceSubType& other ) const
			{
				return name != other.name;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this resource sub type.
			OM_INLINE Hash getHashCode() const
			{
				return name.getHashCode();
			}
			
			
		//********************************************************************************
		//******	Public Resource Type Declarations
			
			
			/// An undefined resource sub type for when no sub type is specified.
			static const ResourceSubType UNDEFINED;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A string which represents this resource sub type.
			data::UTF8String name;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_SUB_TYPE_H
