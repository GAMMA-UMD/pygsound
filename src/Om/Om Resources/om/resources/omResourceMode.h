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

#ifndef INCLUDE_OM_RESOURCE_MODE_H
#define INCLUDE_OM_RESOURCE_MODE_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class that specifies how resources should be handled when loading/saving.
class ResourceMode
{
	public:
		
		//********************************************************************************
		//******	Resource Mode Enum Definition
			
			
			/// An enum type which represents the different resource modes.
			enum Enum
			{
				/// A resource mode where the resources are loaded for only reading.
				/**
				  * This mode allows certain actions to be streamlined and reduces the
				  * amount of data that must be loaded into memory in some cases.
				  *
				  * For instance, a lossy format needs to store its original encoding
				  * to avoid generation loss if the file is saved again. The read-only
				  * mode can avoid storing that kind of information that is only needed for
				  * writing files.
				  *
				  * Saving resources that have been loaded with the read-only mode may result
				  * in loss of information.
				  */
				READ_ONLY,
				
				/// A resource mode where the resources are loaded for reading and writing.
				/**
				  * In this mode, all data needed to losslessly save resources is
				  * loaded into memory.
				  */
				READ_WRITE
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new resource mode with the READ_WRITE mode.
			OM_INLINE ResourceMode()
				:	mode( READ_WRITE )
			{
			}
			
			
			/// Create a new resource mode with the specified resource mode enum value.
			OM_INLINE ResourceMode( Enum newMode )
				:	mode( newMode )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this resource mode type to an enum value.
			OM_INLINE operator Enum () const
			{
				return mode;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the resource mode.
			data::String toString() const;
			
			
			/// Convert this resource mode into a string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value that indicates the type of resource mode.
			Enum mode;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_MODE_H
