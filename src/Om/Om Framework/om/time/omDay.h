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

#ifndef INCLUDE_OM_DAY_H
#define INCLUDE_OM_DAY_H


#include "omTimeConfig.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a single day within a week, month, and year of the modern calendar.
class Day
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a Day object with the specified day of the week, month, and year indicies, starting at 1.
			OM_INLINE Day( Index newDayOfTheWeek, Index newDayOfTheMonth, Index newDayOfTheYear )
				:	dayOfTheWeek( newDayOfTheWeek ),
					dayOfTheMonth( newDayOfTheMonth ),
					dayOfTheYear( newDayOfTheYear )
			{
			}
			
			
		//********************************************************************************
		//******	Day of the Week Accessor Methods
			
			
			/// Get the day of the week index of this Day, starting at 1.
			OM_INLINE Index getDayOfTheWeek() const
			{
				return dayOfTheWeek;
			}
			
			
			/// Set the day of the week index of this Day, starting at 1.
			OM_INLINE void setDayOfTheWeek( Index newDayOfTheWeek ) 
			{
				dayOfTheWeek = newDayOfTheWeek;
			}
			
			
		//********************************************************************************
		//******	Day of the Month Accessor Methods
			
			
			/// Get the day of the month index of this Day, starting at 1.
			OM_INLINE Index getDayOfTheMonth() const
			{
				return dayOfTheMonth;
			}
			
			
			/// Set the day of the month index of this Day, starting at 1.
			OM_INLINE void setDayOfTheMonth( Index newDayOfTheMonth )
			{
				dayOfTheMonth = newDayOfTheMonth;
			}
			
			
		//********************************************************************************
		//******	Day of the Year Accessor Methods
			
			
			/// Get the day of the year index of this Day, starting at 1.
			OM_INLINE Index getDayOfTheYear() const
			{
				return dayOfTheYear;
			}
			
			
			/// Set the day of the year index of this Day, starting at 1.
			OM_INLINE void setDayOfTheYear( Index newDayOfTheYear )
			{
				dayOfTheYear = newDayOfTheYear;
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Get the standard name for the day with this day of the week index.
			/**
			  * If the day does not have a standard name, the empty string is returned.
			  */
			data::String getName() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Methods
			
			
			/// Initialize the standard day of the week names.
			static void initializeStandardNames();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The index of the day of the week, starting at 1.
			Index dayOfTheWeek;
			
			
			/// The index of the day of the month, starting at 1.
			Index dayOfTheMonth;
			
			
			/// The index of the day of the year, starting at 1.
			Index dayOfTheYear;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Standard names for days of the week with standard indices.
			static util::HashMap<Index,data::String> names;
			
			
			/// Whether or not the standard day names have been initialized.
			static Bool hasInitializedStandardNames;
			
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DAY_H
