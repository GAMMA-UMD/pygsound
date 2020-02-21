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

#include "omDate.h"


#include <ctime>


#include "omTime.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




Int Date:: timeZone = 0;
Bool Date:: timeZoneIsInitialized = false;




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Date:: Date()
	:	day( 0, 0, 0 )
{
	getDateForTimeSinceEpoch( Time::getCurrent(), year, month, day, timeOfDay );
}




Date:: Date( const Time& time )
	:	day( 0, 0, 0 )
{
	getDateForTimeSinceEpoch( time, year, month, day, timeOfDay );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Time Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Date:: setTime( const Time& newTime )
{
	getDateForTimeSinceEpoch( newTime, year, month, day, timeOfDay );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Date To String Conversion Method
//############		
//##########################################################################################
//##########################################################################################




data::String Date:: toString() const
{
	data::StringBuffer buffer;
	
	buffer << timeOfDay;
	
	buffer << " " << day.getName() << ", " << month.getName() << " " << day.getDayOfTheMonth() << ", " << year;
	
	return buffer.toString();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Date For Relative Time Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Date:: getDateForTimeSinceEpoch( const Time& time, Int64& year, Month& month, Day& day, TimeOfDay& timeOfDay )
{
	//*********************************************************
	// Get the GMT time using the second offset since the epoch.
	
	time_t secondOffset = (time_t)time.getSeconds();
	
	// Get the time zone to get the date in.
	Int timeZone = getTimeZone();
	
	struct tm* timeGMT = gmtime(&secondOffset);
	
	//*********************************************************
	
	// Set the year output variable.
	year = timeGMT->tm_year + 1900;
	
	// Set the month output variable.
	month = Month( timeGMT->tm_mon + 1 );
	
	//*********************************************************
	// Set the time of day output variable.
	
	UInt dayOfTheWeek = timeGMT->tm_wday + 1;
	UInt dayOfTheMonth = timeGMT->tm_mday;
	UInt dayOfTheYear = timeGMT->tm_yday + 1;
	
	Int hour = timeGMT->tm_hour + timeZone;
	Bool isTomorrow = hour > 23;
	Bool isYesterday = hour < 0;
	
	if ( isTomorrow )
	{
		dayOfTheWeek = (dayOfTheWeek) % 7 + 1;
		dayOfTheMonth += 1;
		dayOfTheYear += 1;
		hour = hour % 24;
	}
	else if ( isYesterday )
	{
		dayOfTheWeek = (dayOfTheWeek + 7 - 2) % 7 + 1;
		dayOfTheMonth -= 1;
		dayOfTheYear -= 1;
		hour += 24;
	}
	
	// Get the number of nanoseconds after the start of the second.
	UInt32 nanoseconds = UInt32(time.getNanoseconds() - ((Int64)time.getSeconds())*Int64(1000000000));
	
	timeOfDay = TimeOfDay( UInt(hour), timeGMT->tm_min, timeGMT->tm_sec, nanoseconds );
	
	//*********************************************************
	// Set the day output variable.
	
	day = Day( dayOfTheWeek, dayOfTheMonth, dayOfTheYear );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Time Zone Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Int Date:: getTimeZone()
{
	if ( !timeZoneIsInitialized )
	{
		timeZone = getLocalTimeZone();
		timeZoneIsInitialized = true;
	}
	
	return timeZone;
}




Int Date:: getLocalTimeZone()
{
	std::time_t timeSinceEpoch = std::time(NULL);
	
	struct tm* gmTime = std::gmtime(&timeSinceEpoch);
	
	int gmDay = gmTime->tm_yday;
	int gmHour = gmTime->tm_hour;
	
	struct tm* localTime = std::localtime(&timeSinceEpoch);
	
	int localDay = localTime->tm_yday;
	int localHour = localTime->tm_hour;
	
	int hourDifference = localHour - gmHour;
	
	if ( gmDay > localDay )
		hourDifference -= 24;
	else if ( gmDay < localDay )
		hourDifference += 24;
	
	return Int(hourDifference);
}




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
