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

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_FLAGS_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_FLAGS_H


#include "omSoundFiltersConfig.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates the different flags that a sound filter parameter can have.
/**
  * These flags provide boolean information about a certain filter parameter.
  * For example, flags can indicate the read/write status of a parameter. Flags
  * are indicated by setting a single bit of a 32-bit unsigned integer to 1.
  *
  * Enum values for the different flags are defined as members of the class. Typically,
  * the user would bitwise-OR the flag enum values together to produce a final set
  * of set flags.
  */
class FilterParameterFlags
{
	public:
		
		//********************************************************************************
		//******	Parameter Type Enum Declaration
			
			
			/// An enum that specifies the different filter parameter flags.
			enum Enum
			{
				/// A flag set when a parameter's value can be read.
				READ_ACCESS = 1 << 0,
				
				/// A flag set when a parameter's value can be changed.
				WRITE_ACCESS = 1 << 1,
				
				/// A flag set when some of a parameter's values may have special names associated with them.
				NAMED_VALUES = 1 << 2,
				
				/// The flag value when all flags are not set.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new filter parameter flags object with no flags set.
			OM_INLINE FilterParameterFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new filter parameter flags object with the specified flags value.
			OM_INLINE FilterParameterFlags( UInt32 newFlags )
				:	flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Integer Cast Operator
			
			
			/// Convert this filter parameter flags object to an integer value.
			/**
			  * This operator is provided so that the FilterParameterFlags object
			  * can be used as an integer value for bitwise logical operations.
			  */
			OM_INLINE operator UInt32 () const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Read Status Accessor Methods
			
			
			/// Return whether or not these parameter flags indicate that read access is enabled.
			OM_INLINE Bool getIsReadable() const
			{
				return (flags & READ_ACCESS) != UNDEFINED;
			}
			
			
			/// Set whether or not these parameter flags indicate that read access is enabled.
			OM_INLINE void setIsReadable( Bool newIsReadable )
			{
				if ( newIsReadable )
					flags |= READ_ACCESS;
				else
					flags &= ~READ_ACCESS;
			}
			
			
		//********************************************************************************
		//******	Write Status Accessor Methods
			
			
			/// Return whether or not these parameter flags indicate that write access is enabled.
			OM_INLINE Bool getIsWriteable() const
			{
				return (flags & WRITE_ACCESS) != UNDEFINED;
			}
			
			
			/// Set whether or not these parameter flags indicate that write access is enabled.
			OM_INLINE void setIsWriteable( Bool newIsWritable )
			{
				if ( newIsWritable )
					flags |= WRITE_ACCESS;
				else
					flags &= ~WRITE_ACCESS;
			}
			
			
		//********************************************************************************
		//******	Name Value Status Accessor Methods
			
			
			/// Return whether or not these parameter flags indicate the parameter has any specially named values.
			OM_INLINE Bool getHasNamedValues() const
			{
				return (flags & NAMED_VALUES) != UNDEFINED;
			}
			
			
			/// Set whether or not these parameter flags indicate the parameter has any specially named values.
			OM_INLINE void setHasNamedValues( Bool newHasNamedValues )
			{
				if ( newHasNamedValues )
					flags |= NAMED_VALUES;
				else
					flags &= ~NAMED_VALUES;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value indicating the flags that are set for this filter parameter.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_FLAGS_H
