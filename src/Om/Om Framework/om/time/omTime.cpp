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

#include "omTime.h"


#include <ctime>


#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#include <sys/time.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
#else
	#error unsupported platform
#endif


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Platform Specific Static Data
//############		
//##########################################################################################
//##########################################################################################




#if defined(OM_PLATFORM_WINDOWS)
	
	/// Whether or not the timer period has been initialized.
	static Bool hasTimerPeriod = false;
	
	/// The period of the window's timer, expressed as the number of nanoseconds per counter period.
	static Double nanosecondsPerCount = Double(0);
	
	/// Whether or not the Epoch offset has been calibrated.
	static Bool hasCalibratedTimer = false;
	
	/// The value of the counter in nanoseconds when the time was last calibrated.
	static Int64 lastCalibrationCount = 0;
	
	/// The offset in nanoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
	static Int64 epochOffset = 0;
	
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################





Bool Time:: getCurrentTime( Int64& nanoseconds )
{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	
	// A struct representing the current state of the system timer on Mac OS X.
	timeval timeData;
	
	int error = gettimeofday( &timeData, NULL );
	
	// Make sure there wasn't an error.
	if ( error != 0 )
		return false;
	
	// Compute the current time in nanoseconds since the epoch.
	nanoseconds = UInt64(timeData.tv_sec)*UInt64(1000000000) + UInt64(timeData.tv_usec)*UInt64(1000);
	
	return true;
	
#elif defined(OM_PLATFORM_WINDOWS)
	
	
	// Get the performance counter frequency if it has not already been queried.
	if ( !hasTimerPeriod )
	{
		LARGE_INTEGER performanceCounterFrequency;
		
		BOOL success = QueryPerformanceFrequency( &performanceCounterFrequency );
		
		// If there was an error in querying the counter frequency, return that there was an error.
		if ( !success )
			return false;
		
		nanosecondsPerCount = Double(1000000000) / Double(performanceCounterFrequency.QuadPart);
		hasTimerPeriod = true;
	}
	
	// Get the number of ticks of the performance counter.
	UInt64 numCounts;
	
	if ( !QueryPerformanceCounter( (LARGE_INTEGER*)&numCounts ) )
		return false;
	
	Int64 counterNanoseconds = Int64(Double(numCounts)*nanosecondsPerCount);
	
	// Calibrate the timer so that it is relative to the current system time.
	if ( !hasCalibratedTimer )
	{
		// Get the current system time.
		FILETIME t1;
		GetSystemTimeAsFileTime( &t1 );
		
		// Wait for the system time to change so that the time we get is as accurrate as possible.
		// This is because the system time updates less frequently than we would like.
		FILETIME t2;
		
		do
		{
			// Yield this thread.
			Sleep(0);
			
			// Get the new current system time.
			GetSystemTimeAsFileTime( &t2 );
		}
		while ( t1.dwLowDateTime == t2.dwLowDateTime && t1.dwHighDateTime == t2.dwHighDateTime );
		
		// Copy the current time to a large integer.
		ULARGE_INTEGER currentTime;
		currentTime.LowPart = t2.dwLowDateTime;
		currentTime.HighPart = t2.dwHighDateTime;
		
		// Initialize a system time structure representing the epoch date.
		SYSTEMTIME epochSystemTime;
		epochSystemTime.wYear = 1970;
		epochSystemTime.wMonth = 1;
		epochSystemTime.wDayOfWeek = 4; // Thursday
		epochSystemTime.wDay = 1;
		epochSystemTime.wHour = 0;
		epochSystemTime.wMinute = 0;
		epochSystemTime.wSecond = 0;
		epochSystemTime.wMilliseconds = 0;
		
		// Convert the epoch system time into a file time.
		FILETIME epochFileTime;
		
		if ( !SystemTimeToFileTime( &epochSystemTime, &epochFileTime ) )
			return false;
		
		// Copy the epoch time to a large integer.
		ULARGE_INTEGER epochTime;
		epochTime.LowPart = epochFileTime.dwLowDateTime;
		epochTime.HighPart = epochFileTime.dwHighDateTime;
		
		// Compute the total offset since the epoch in nanoseconds.
		epochOffset = (currentTime.QuadPart - epochTime.QuadPart)*100;
		lastCalibrationCount = counterNanoseconds;
		hasCalibratedTimer = true;
	}
	
	// Compute the final time offset since the epoch in nanoseconds.
	nanoseconds = epochOffset + (counterNanoseconds - lastCalibrationCount);
	
	return true;
#endif
}




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
