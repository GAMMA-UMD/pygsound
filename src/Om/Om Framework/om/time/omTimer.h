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

#ifndef INCLUDE_OM_TIMER_H
#define INCLUDE_OM_TIMER_H


#include "omTimeConfig.h"


#include "omTime.h"


//##########################################################################################
//*******************************   Start Time Namespace   *********************************
OM_TIME_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A timer class that behaves like a stopwatch.
/**
  * This class serves to provide a way for the user to mark
  * times and to measure the intervals between them. This can
  * be used to do application profiling/timing, to provide a 
  * frame time interval counter for games, etc. It uses the highest
  * performance timers availible on the system it is compiled under,
  * and therefore should have sub-millisecond accuracy. The timer
  * is also able to be paused, such that it then behaves as if it
  * was stuck in that instance in which it was paused. It can later
  * be resumed and carry on as if it had never been paused, reflecting
  * this in it's attributes accordingly.
  */
class Timer
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new timer and start it's first interval (by calling update()).
			/** 
			  * This constructor creates a new timer, and then
			  * initializes the timer by starting it's first time interval,
			  * done by calling the timer's update() function internally.
			  */
			OM_INLINE Timer()
				:	oldTime( 0 ),
					currentTime( Time::getCurrent() ),
					lastInterval( 0 ),
					paused( false )
			{
			}
			
			
		//********************************************************************************
		//******	Stopwatch Timing Methods
			
			
			/// Update the timer to the current time and store the interval between updates.
			/**
			  * This method updates the timer, and then returns and stores
			  * the time between the last update and this update.
			  * If the timer is paused when this function is called,
			  * the the method does nothing.
			  */
			Time update();
			
			
			/// Return the time interval of the last update
			/**
			  * This method retrieves the time interval (in seconds)
			  * of the elapsed time between the last call to the update()
			  * method and the second to last call to the update() method.
			  * 
			  * @return the time interval between the last two timer updates
			  */
			OM_INLINE const Time& getLastInterval() const
			{
				return lastInterval;
			}
			
			
			/// Return the time interval since the last update without updating the timer.
			/**
			  * This method gets the time passed since the last call to the update()
			  * method in seconds. The method does not reset the timer, and therefore
			  * can be used to get a running total of the time since some arbitrary
			  * moment (set by calling the update method). If the timer is paused, 
			  * this method returns the time between the last call to update() and
			  * the time when the timer was paused.
			  * 
			  * @return the time since the last call to update()
			  */
			OM_INLINE Time getElapsedTime() const
			{
				if ( paused )
					return currentTime - oldTime;
				else
					return Time::getCurrent() - currentTime;
			}
			
			
		//********************************************************************************
		//******	Timer State Methods
			
			
			/// Get whether or not the timer is currently paused.
			/**
			  * If the timer is paused, then this means that the timer
			  * is no longer keeping track of time, and will behave as if
			  * it is stuck in the instant that it was paused in until it
			  * is unpaused. This can be useful in many situations where
			  * one needs to stop timing and later resume timing as if
			  * the timer has never been stopped.
			  * 
			  * @return whether or not the timer is currently paused.
			  */
			OM_INLINE Bool getIsPaused() const
			{
				return paused;
			}
			
			
			/// Set whether or not the timer is paused with a boolean flag.
			/**
			  * If the timer is paused, then this means that the timer
			  * is no longer keeping track of time, and will behave as if
			  * it is stuck in the instant that it was paused in until it
			  * is unpaused. This can be useful in many situations where
			  * one needs to stop timing and later resume timing as if
			  * the timer has never been stopped. If this method is called
			  * with a parameter that is equal to the return value of
			  * getIsPaused(), then it does nothing (the timer's state doesn't
			  * need to change).
			  * 
			  * @param newIsPaused - whether or not the timer should be paused.
			  */
			void setIsPaused( Bool newIsPaused );
			
			
			/// Pause the timer.
			/**
			  * If the timer is already paused, then this method does
			  * nothing (the timer's state doesn't need to change to
			  * reflect the function call).
			  */
			OM_INLINE void pause()
			{
				this->setIsPaused( true );
			}
			
			
			/// Resume the timer if it is currently paused.
			/**
			  * If the timer is not paused, then this method does
			  * nothing (the timer's state doesn't need to change to
			  * reflect the function call).
			  */
			OM_INLINE void resume()
			{
				this->setIsPaused( false );
			}
			
			
			
	private:
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The second to last time the timer was updated.
			Time oldTime;
			
			
			/// The last time the timer was updated.
			Time currentTime;
			
			
			/// The positive time interval between the last and second to last times.
			Time lastInterval;
			
			
			/// Whether or not the timer is currently paused.
			Bool paused;
			
			
			
};




//##########################################################################################
//*******************************   End Time Namespace   ***********************************
OM_TIME_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TIMER_H
