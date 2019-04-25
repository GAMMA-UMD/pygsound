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

#ifndef INCLUDE_OM_RESOURCE_PROTOTYPE_FLAGS_H
#define INCLUDE_OM_RESOURCE_PROTOTYPE_FLAGS_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates the different boolean flags that Prototype can have.
/**
  * These flags provide boolean parameters for resource prototypes. Flags
  * are indicated by setting a single bit of a 32-bit unsigned integer to 1.
  *
  * Enum values for the different flags are defined as members of the class. Typically,
  * the user would bitwise-OR the flag enum values together to produce a final set
  * of set flags.
  */
class PrototypeFlags
{
	public:
		
		//********************************************************************************
		//******	Flag Enum Declaration
			
			
			/// An enum that specifies the different prototype flags.
			enum Flag
			{
				/// The flag value when all flags are not set.
				UNDEFINED = 0,
				
				/// A flag indicating the prototype component is enabled.
				ENABLED = (1 << 0),
				
				/// A flag indicating that the prototype component should be shared globally.
				/**
				  * If the flag is not set, the prototype component is copied when the prototype is instanced.
				  */
				SHARED = (1 << 1),
				
				/// The default flags to use for a prototype.
				DEFAULT = UNDEFINED
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new prototype flags object with no flags set.
			OM_INLINE PrototypeFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new prototype flags object with the specified flag value initially set.
			OM_INLINE PrototypeFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new prototype flags object with the specified initial combined flags value.
			OM_INLINE PrototypeFlags( UInt32 newFlags )
				:	flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Integer Cast Operator
			
			
			/// Convert this prototype flags object to an integer value.
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
			
			
			/// A value indicating the flags that are set for the prototype.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_PROTOTYPE_FLAGS_H
