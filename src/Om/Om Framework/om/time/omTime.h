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

#ifndef INCLUDE_OM_TIME_H
#define INCLUDE_OM_TIME_H


#include "omTimeConfig.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a high-resolution time interval.
/**
  * The time is stored internally as a 64-bit signed integer that indicates the number
  * of nanoseconds in the time interval.
  */
class Time
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a Time object that represents a time interval of 0 seconds.
			OM_INLINE Time()
				:	nanoseconds( 0 )
			{
			}
			
			
			/// Create a Time object that represents the specified interval of time in nanoseconds.
			/**
			  * This overload is provided to allow initialization with 0, which might cause ambiguities.
			  */
			OM_INLINE Time( int newNanoseconds )
				:	nanoseconds( newNanoseconds )
			{
			}
			
			
			/// Create a Time object that represents the specified interval of time in nanoseconds.
			OM_INLINE Time( Int64 newNanoseconds )
				:	nanoseconds( newNanoseconds )
			{
			}
			
			
			/// Create a Time object that represents the specified interval of time in seconds.
			/**
			  * The time is specified by a single floating-point value that gives
			  * the number of seconds in the time interval.
			  */
			OM_INLINE Time( Float newSeconds )
			{
				Float seconds = math::floor( newSeconds );
				Float secondsFraction = newSeconds - seconds;
				
				nanoseconds = Int64(seconds)*Int64(1000000000) + Int64(secondsFraction*Float(1.0e9));
			}
			
			
			/// Create a Time object that represents the specified interval of time in seconds.
			/**
			  * The time is specified by a single floating-point value that gives
			  * the number of seconds in the time interval.
			  */
			OM_INLINE Time( Double newSeconds )
			{
				Double seconds = math::floor( newSeconds );
				Double secondsFraction = newSeconds - seconds;
				
				nanoseconds = Int64(seconds)*Int64(1000000000) + Int64(secondsFraction*Double(1.0e9));
			}
			
			
		//********************************************************************************
		//******	Seconds Accessor Methods
			
			
			/// Return the number of seconds that this Time object represents.
			OM_INLINE Double getSeconds() const
			{
				return Double(nanoseconds) / Double(1.0e9);
			}
			
			
			/// Return the number of nanoseconds that this Time object represents.
			OM_INLINE Int64 getNanoseconds() const
			{
				return nanoseconds;
			}
			
			
		//********************************************************************************
		//******	Time Cast Operators
			
			
			/// Convert this Time object into a floating point representation.
			/**
			  * Doing this may reduce the accuracy of the time interval due to the
			  * inaccuracies of floating-point formats.
			  */
			/*
			OM_INLINE operator Float () const
			{
				return Float(nanoseconds) / Float(1.0e9);
			}
			*/
			
			/// Convert this Time object into a double floating point representation.
			/**
			  * Doing this may reduce the accuracy of the time interval due to the
			  * inaccuracies of floating-point formats.
			  */
			OM_INLINE operator Double () const
			{
				return this->getSeconds();
			}
			
			
		//********************************************************************************
		//******	Time Comparison Operators
			
			
			/// Return whether or not this Time object represents the same time as another.
			OM_INLINE Bool operator == ( const Time& other ) const
			{
				return nanoseconds == other.nanoseconds;
			}
			
			
			/// Return whether or not this Time object represents a different time than another.
			OM_INLINE Bool operator != ( const Time& other ) const
			{
				return nanoseconds != other.nanoseconds;
			}
			
			
			/// Return whether or not this Time object represents an earlier time than another.
			OM_INLINE Bool operator < ( const Time& other ) const
			{
				return nanoseconds < other.nanoseconds;
			}
			
			
			/// Return whether or not this Time object represents a later time than another.
			OM_INLINE Bool operator > ( const Time& other ) const
			{
				return nanoseconds > other.nanoseconds;
			}
			
			
			/// Return whether or not this Time object represents an earlier or equal time than another.
			OM_INLINE Bool operator <= ( const Time& other ) const
			{
				return nanoseconds < other.nanoseconds;
			}
			
			
			/// Return whether or not this Time object represents a later or equal time than another.
			OM_INLINE Bool operator >= ( const Time& other ) const
			{
				return nanoseconds > other.nanoseconds;
			}
			
			
		//********************************************************************************
		//******	Time Arithmetic Operators
			
			
			/// Return the time interval represented by the sum of this time interval and another.
			OM_INLINE Time operator + ( const Time& other ) const
			{
				return Time( nanoseconds + other.nanoseconds );
			}
			
			
			/// Add the specified time interval to this time interval, modifying it.
			OM_INLINE Time& operator += ( const Time& other )
			{
				nanoseconds += other.nanoseconds;
				
				return *this;
			}
			
			
			/// Return the time interval represented by the difference between this time interval and another.
			OM_INLINE Time operator - ( const Time& other ) const
			{
				return Time( nanoseconds - other.nanoseconds );
			}
			
			
			/// Subtract the specified time interval from this time interval, modifying it.
			OM_INLINE Time& operator -= ( const Time& other )
			{
				nanoseconds -= other.nanoseconds;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Return a string representation of this time object.
			OM_INLINE data::String toString() const
			{
				return data::String( this->getSeconds() );
			}
			
			
			/// Convert this time object to a string representation.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Current Time Accessor Method
			
			
			/// Return a time object that represents the current system time.
			/**
			  * The time is specified as the time since the Epoch,
			  * 1970-01-01 00:00:00 +0000 (UTC).
			  */
			OM_INLINE static Time getCurrent()
			{
				Int64 nanoseconds = 0;
				
				getCurrentTime( nanoseconds );
				
				return Time( nanoseconds );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Method
			
			
			/// Get the current system time as the number of nanoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			/**
			  * The method returns whether or not the current time was successfully queried.
			  */
			static Bool getCurrentTime( Int64& nanoseconds );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of nanoseconds that this time interval represents.
			Int64 nanoseconds;
			
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TIME_H
