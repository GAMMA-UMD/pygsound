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

#include "omTimeOfDay.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




TimeOfDay:: TimeOfDay( UInt newHours, UInt newMinutes, UInt newSeconds, UInt32 newNanoseconds )
{
	UInt32 extraSeconds = newNanoseconds / UInt32(1000000000);
	nanoseconds = newNanoseconds % UInt32(1000000000);
	
	UInt adjustedSeconds = newSeconds + UInt(extraSeconds);
	seconds = UInt8(adjustedSeconds % 60);
	
	UInt adjustedMinutes = newMinutes + (adjustedSeconds - UInt(seconds))/60;
	minutes = UInt8(adjustedMinutes % 60);
	
	hours = UInt8((newHours + (adjustedMinutes - UInt(minutes))/60) % 24);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Minutes Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void TimeOfDay:: setMinute( UInt newMinutes )
{
	minutes = UInt8(newMinutes % 60);
	hours = UInt8((UInt(hours) + (newMinutes - UInt(minutes))/60) % 24);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seconds Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void TimeOfDay:: setSecond( UInt newSeconds )
{
	seconds = UInt8(newSeconds % 60);
	
	UInt adjustedMinutes = UInt(minutes) + (newSeconds - UInt(seconds))/60;
	minutes = UInt8(adjustedMinutes % 60);
	
	hours = UInt8((UInt(hours) + (adjustedMinutes - UInt(minutes))/60) % 24);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Fractional Seconds Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void TimeOfDay:: setNanoseconds( UInt32 newNanoseconds )
{
	UInt32 extraSeconds = newNanoseconds / UInt32(1000000000);
	nanoseconds = newNanoseconds / UInt32(1000000000);
	
	UInt adjustedSeconds = UInt(seconds) + UInt(extraSeconds);
	seconds = adjustedSeconds % 60;
	
	UInt adjustedMinutes = UInt(minutes) + (adjustedSeconds - UInt(seconds))/60;
	minutes = UInt8(adjustedMinutes % 60);
	
	hours = UInt8((UInt(hours) + (adjustedMinutes - UInt(minutes))/60) % 24);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Time Of Day To String Conversion Method
//############		
//##########################################################################################
//##########################################################################################




data::String TimeOfDay:: toString() const
{
	data::StringBuffer buffer;
	
	buffer << hours << ":";
	
	if ( minutes < 10 )
		buffer << '0';
	
	buffer << minutes << ":";
	
	if ( seconds < 10 )
		buffer << '0';
	
	buffer << data::String( (Double(seconds) + Double(nanoseconds)/Double(1.0e9)), 3 );
	
	return buffer.toString();
}




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
