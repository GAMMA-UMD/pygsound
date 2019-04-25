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

#ifndef INCLUDE_OM_TIME_OF_DAY_H
#define INCLUDE_OM_TIME_OF_DAY_H


#include "omTimeConfig.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a particular moment within a 24-hour day to nanosecond resolution.
class TimeOfDay
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			/// Create a TimeOfDay object that represents the first moment after midnight of a day.
			TimeOfDay()
				:	hours( 0 ),
					minutes( 0 ),
					seconds( 0 ),
					nanoseconds( 0 )
			{
			}
			
			
			/// Create a TimeOfDay object that represents the specified moment after midnight of a day.
			/**
			  * This time is specified by the hour after midnight [0,23], minute after the start
			  * of the hour [0,59], second after the start of the minute [0,59], and the
			  * nanoseconds after the start of the second [0,999999999]. If any of these values
			  * are outside of the valid range, they are wrapped around and carried into the
			  * next time measurement unit.
			  *
			  * For instance, specifying a time of 0 hours, 59 minutes,
			  * 59 seconds, and 1.5e9 nanoseconds will result in an actual time of day of
			  * 1 hour, 0 minutes, 0 seconds, and 0.5e9 nanoseconds.
			  */
			TimeOfDay( UInt newHours, UInt newMinutes, UInt newSeconds, UInt32 newNanoseconds = 0 );
			
			
		//********************************************************************************
		//******	Hour Accessor Methods
			
			
			/// Get the hour after midnight of this TimeOfDay.
			OM_INLINE UInt getHour() const
			{
				return (UInt)hours;
			}
			
			
			/// Set the hour after midnight of this TimeOfDay.
			/**
			  * If the specified hour value is outside of the range
			  * [0,23], the value is wrapped around to the next day
			  * so that it lies within the valid range.
			  */
			OM_INLINE void setHour( UInt newHours )
			{
				hours = UByte(newHours % 24);
			}
			
			
		//********************************************************************************
		//******	Minute Accessor Methods
			
			
			/// Get the minute after the start of the hour of this TimeOfDay.
			OM_INLINE UInt getMinute() const
			{
				return (UInt)minutes;
			}
			
			
			/// Set the minute after the start of the hour of this TimeOfDay.
			/**
			  * If the specified minute value is outside of the range
			  * [0,59], the value is wrapped around to the next hour (or further)
			  * and the hour of this TimeOfDay is increased by the required
			  * number of hours.
			  */
			void setMinute( UInt newMinutes );
			
			
		//********************************************************************************
		//******	Seconds Accessor Methods
			
			
			/// Get the seconds after the start of the minute of this TimeOfDay.
			OM_INLINE UInt getSecond() const
			{
				return (UInt)seconds;
			}
			
			
			/// Set the second after the start of the minute of this TimeOfDay.
			/**
			  * If the specified second value is outside of the range
			  * [0,59], the value is wrapped around to the next minute (or further)
			  * and the minute of this TimeOfDay is increased by the required
			  * number of minutes. If necessary, the hour of this TimeOfDay may
			  * also be altered if the wrap-around is sever enough.
			  */
			void setSecond( UInt newSeconds );
			
			
		//********************************************************************************
		//******	Fractional Seconds Accessor Methods
			
			
			/// Get the seconds after the start of the minute of this TimeOfDay.
			OM_INLINE UInt32 getNanoseconds() const
			{
				return nanoseconds;
			}
			
			
			/// Set the second after the start of the minute of this TimeOfDay.
			/**
			  * If the specified second value is outside of the range
			  * [0,59], the value is wrapped around to the next minute (or further)
			  * and the minute of this TimeOfDay is increased by the required
			  * number of minutes. If necessary, the hour of this TimeOfDay may
			  * also be altered if the wrap-around is sever enough.
			  */
			void setNanoseconds( UInt32 newNanoseconds );
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert the TimeOfDay object to a String representation.
			data::String toString() const;
			
			
			/// Convert the TimeOfDay object to a String representation.
			OM_FORCE_INLINE operator data::String() const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of hours since midnight.
			UInt8 hours;
			
			
			/// The number of minutes since the start of the hour.
			UInt8 minutes;
			
			
			/// The number of seconds since the start of the minute.
			UInt8 seconds;
			
			
			/// Padding to 4-byte boundary.
			UInt8 padding;
			
			
			/// The number of nanoseconds that have passed since the start of the second.
			UInt32 nanoseconds;
			
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TIME_OF_DAY_H
