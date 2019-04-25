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

#ifndef INCLUDE_OM_RESOURCE_REFERENCE_H
#define INCLUDE_OM_RESOURCE_REFERENCE_H


#include "omResourcesConfig.h"


#include "omResourceID.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a file-local reference to a resource.
/**
  * An resource reference is used in reference patching after a resource file
  * has been loaded. An resource reference contains a user ID that can be used
  * by an ResourceTypeTranscoder subclass to recall the type or usage of a reference
  * so that it can be patched.
  */
class ResourceReference
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default resource reference that doesn't refer to any resource.
			OM_INLINE ResourceReference()
				:	localTypeID( ResourceID::INVALID_LOCAL_ID ),
					localID( ResourceID::INVALID_LOCAL_ID ),
					userType( 0 ),
					userID( 0 ),
					userData( NULL )
			{
			}
			
			
			/// Create a new resource reference for the specified name, local ID, and user ID.
			OM_INLINE ResourceReference( ResourceLocalID newLocalTypeID, ResourceLocalID newLocalID,
										Index newUserType = 0, Index newUserID = 0, void* newUserData = NULL )
				:	localTypeID( newLocalTypeID ),
					localID( newLocalID ),
					userType( newUserType ),
					userID( newUserID ),
					userData( newUserData )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The local ID of the resource's corresponding to this reference, e.g. the index of its type within the resource set.
			ResourceLocalID localTypeID;
			
			
			/// The local ID of the resource corresponding to this reference, e.g. its index within the resource type.
			ResourceLocalID localID;
			
			
			/// A user-defined type ID that is used by an ResourceTypeTranscoder subclass to recall the type of a reference.
			/**
			  * This enables the transcoder to know what reference should be patched with the resource
			  * corresponding to this ID.
			  */
			Index userType;
			
			
			/// A user-defined ID that is used by an ResourceTypeTranscoder subclass to recall the ID or usage of a reference.
			/**
			  * This enables the transcoder to know what reference should be patched with the resource
			  * corresponding to this ID.
			  */
			Index userID;
			
			
			/// An opaque pointer to user information for this resource reference.
			/**
			  * This enables the transcoder to know what reference should be patched with the resource
			  * corresponding to this ID.
			  */
			void* userData;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_REFERENCE_H
