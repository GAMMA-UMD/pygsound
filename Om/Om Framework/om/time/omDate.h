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

#ifndef INCLUDE_OM_DATE_H
#define INCLUDE_OM_DATE_H


#include "omTimeConfig.h"


#include "omMonth.h"
#include "omDay.h"
#include "omTimeOfDay.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class Time;




//********************************************************************************
/// A class that represents an instant in time within the modern calendar.
class Date
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a date object that represents the current date.
			Date();
			
			
			/// Create a date corresponding to the specified Time offset since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			Date( const Time& time );
			
			
			/// Create a date corresponding to the specified year, month, and day, at the first instant of the day.
			OM_INLINE Date( Int64 newYear, const Month& newMonth, const Day& newDay )
				:	year( newYear ),
					month( newMonth ),
					day( newDay )
			{
			}
			
			
			/// Create a date corresponding to the specified year, month, day, and time of day.
			OM_INLINE Date( Int64 newYear, const Month& newMonth, const Day& newDay, const TimeOfDay& newTimeOfDay )
				:	year( newYear ),
					month( newMonth ),
					day( newDay ),
					timeOfDay( newTimeOfDay )
			{
			}
			
			
		//********************************************************************************
		//******	Month Accessor Methods
			
			
			/// Return the year of this date. Values are interpreted as relative to the year 0 AD.
			OM_INLINE Int64 getYear() const
			{
				return year;
			}
			
			
			/// Set the year of this date. Values are interpreted as relative to the year 0 AD.
			OM_INLINE void setYear( Int64 newYear )
			{
				year = newYear;
			}
			
			
		//********************************************************************************
		//******	Month Accessor Methods
			
			
			/// Return a reference to an object that represents the month of this date.
			OM_INLINE const Month& getMonth() const
			{
				return month;
			}
			
			
			/// Set the month of this date.
			OM_INLINE void setMonth( const Month& newMonth )
			{
				month = newMonth;
			}
			
			
		//********************************************************************************
		//******	Day Accessor Methods
			
			
			/// Return a reference to an object that represents the day of this date.
			OM_INLINE const Day& getDay() const
			{
				return day;
			}
			
			
			/// Set the day of this date.
			OM_INLINE void setDay( const Day& newDay )
			{
				day = newDay;
			}
			
			
		//********************************************************************************
		//******	Time Of Day Accessor Methods
			
			
			/// Return a reference to an object that represents the time of day of this date.
			OM_INLINE const TimeOfDay& getTimeOfDay() const
			{
				return timeOfDay;
			}
			
			
			/// Set the time of day of this date.
			OM_INLINE void setTimeOfDay( const TimeOfDay& newTimeOfDay )
			{
				timeOfDay = newTimeOfDay;
			}
			
			
		//********************************************************************************
		//******	Time Accessor Methods
			
			
			/// Reset the date to the specified Time offset since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			void setTime( const Time& newTime );
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert the Date object to a String representation.
			data::String toString() const;
			
			
			/// Convert the Date object to a String representation.
			OM_FORCE_INLINE operator data::String() const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Static Time Zone Accessor Methods
			
			
			/// Get the current time zone that dates are being processed as being in.
			/**
			  * The time zone is specified as the signed offset in hours from GMT.
			  *
			  * The default initial value for the time zone is the local time zone.
			  */
			static Int getTimeZone();
			
			
			/// Get the local time zone.
			/**
			  * The time zone is specified as the signed offset in hours from GMT.
			  */
			static Int getLocalTimeZone();
			
			
			/// Set the time zone to use when computing the hour of the day.
			/**
			  * The time zone is specified as the signed offset in hours from GMT.
			  *
			  * The default initial value for the time zone is the local time zone.
			  */
			OM_INLINE static void setTimeZone( Int newTimeZone )
			{
				timeZone = newTimeZone;
				timeZoneIsInitialized = true;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Helper Methods
			
			
			/// Get the date for the time specified by the Time object.
			static void getDateForTimeSinceEpoch( const Time& time, Int64& year, Month& month, Day& day, TimeOfDay& timeOfDay );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An integer representing the year of this Date.
			Int64 year;
			
			
			/// The month of this Date object.
			Month month;
			
			
			/// The day of this Date object.
			Day day;
			
			
			/// The time of day of this date.
			TimeOfDay timeOfDay;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			static Int timeZone;
			
			
			static Bool timeZoneIsInitialized;
			
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DATE_H
