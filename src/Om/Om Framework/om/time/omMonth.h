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

#ifndef INCLUDE_OM_MONTH_H
#define INCLUDE_OM_MONTH_H


#include "omTimeConfig.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a particular month in the modern calendar.
class Month
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a Month object representing the first month of the year.
			OM_INLINE Month()
				:	number( 1 )
			{
			}
			
			
			/// Create a Month object with the specified index within the year, starting at 1.
			OM_INLINE Month( Index newNumber )
				:	number( newNumber )
			{
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Get the standard name for the month with this index.
			/**
			  * If the month does not have a standard name, the empty string is returned.
			  */
			OM_INLINE data::String getName() const
			{
				if ( !hasInitializedStandardNames )
					initializeStandardNames();
				
				const data::String* name;
				
				if ( names.find( Hash(number), number, name ) )
					return *name;
				else
					return data::String();
			}
			
			
		//********************************************************************************
		//******	Number Accessor Methods
			
			
			/// Get the index of the month within the year, starting at 1.
			OM_INLINE Index getNumber() const
			{
				return number;
			}
			
			
			/// Set the index of the month within the year, starting at 1.
			OM_INLINE void setNumber( Index newNumber )
			{
				number = newNumber;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Methods
			
			
			/// Initialize the standard month names.
			static void initializeStandardNames();
			
			
		//********************************************************************************
		//******	Private Data Member
			
			
			/// The number of the month within the year, starting at 1.
			Index number;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Standard names for months with standard indices.
			static util::HashMap<Index,data::String> names;
			
			
			/// Whether or not the standard month names have been initialized.
			static Bool hasInitializedStandardNames;
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MONTH_H
