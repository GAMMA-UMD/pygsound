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

#ifndef INCLUDE_OM_RESOURCE_FLAGS_H
#define INCLUDE_OM_RESOURCE_FLAGS_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates the different boolean flags that Resource can have.
/**
  * These flags provide boolean parameters for resource resources. Flags
  * are indicated by setting a single bit of a 32-bit unsigned integer to 1.
  *
  * Enum values for the different flags are defined as members of the class. Typically,
  * the user would bitwise-OR the flag enum values together to produce a final set
  * of set flags.
  */
class ResourceFlags
{
	public:
		
		//********************************************************************************
		//******	Flag Enum Declaration
			
			
			/// An enum that specifies the different resource flags.
			enum Flag
			{
				/// The flag value when all flags are not set.
				UNDEFINED = 0,
				
				/// A flag indicating that the resource should be stored in an external file.
				/**
				  * If set, the resource will be saved to an external file at its file path location.
				  * If the flag is not set, the resource is stored locally in a resource set.
				  */
				EXTERNAL = (1 << 0),
				
				/// The default flags to use for a resource.
				DEFAULT = UNDEFINED
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new resource flags object with no flags set.
			OM_INLINE ResourceFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new resource flags object with the specified flag value initially set.
			OM_INLINE ResourceFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new resource flags object with the specified initial combined flags value.
			OM_INLINE ResourceFlags( UInt32 newFlags )
				:	flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Integer Cast Operator
			
			
			/// Convert this resource flags object to an integer value.
			/**
			  * This operator is provided so that the CollisionFlags shape
			  * can be used as an integer value for bitwise logical operations.
			  */
			OM_INLINE operator UInt32 () const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Flag Status Accessor Methods
			
			
			/// Return whether or not the specified flag value is set for this flags object.
			OM_INLINE Bool isSet( Flag flag ) const
			{
				return (flags & flag) != UNDEFINED;
			}
			
			
			/// Set whether or not the specified flag value is set for this flags object.
			OM_INLINE void set( Flag flag, Bool newIsSet )
			{
				if ( newIsSet )
					flags |= flag;
				else
					flags &= ~flag;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value indicating the flags that are set for the resource.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_FLAGS_H
