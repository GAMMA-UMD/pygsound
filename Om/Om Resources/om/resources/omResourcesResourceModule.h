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

#ifndef INCLUDE_OM_RESOURCES_RESOURCE_MODULE_H
#define INCLUDE_OM_RESOURCES_RESOURCE_MODULE_H


#include "omResourcesConfig.h"


#include "omResourceModule.h"
#include "omResourcePrototypeTranscoder.h"
#include "omResourceTagTranscoder.h"
#include "omResourceGroupTranscoder.h"
#include "omResourceSetTranscoder.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class that encapsulates a collection of resource set transcoders for resources types.
class ResourcesResourceModule : public ResourceModule
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new resources resource module with a new set of image resource transcoders.
			ResourcesResourceModule();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an resources resource module and all of its resource type transcoders.
			/**
			  * Any ResourceSetTranscoder objects that are using the transcoders provided
			  * by this module should cease using those trancoders before the module is detroyed.
			  */
			~ResourcesResourceModule();
			
			
		//********************************************************************************
		//******	Resource Type Accessor Method
			
			
			/// Add transcoders to the specified resource manager for the resource formats this module supports.
			virtual void getFormats( ResourceManager& manager );
			
			
			/// Add transcoders to the specified resource set transcoder for the resource types this module supports.
			virtual void getTypes( ResourceSetTranscoder& transcoder );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A resource transcoder for the Prototype resource type.
			PrototypeResourceTranscoder prototype;
			
			
			/// A resource transcoder for the ResourceTag resource type.
			ResourceTagTranscoder tag;
			
			
			/// A resource transcoder for the ResourceGroup resource type.
			ResourceGroupTranscoder group;
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCES_RESOURCE_MODULE_H
