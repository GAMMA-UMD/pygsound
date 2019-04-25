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

#ifndef INCLUDE_OM_SOUND_MIDI_TIME_H
#define INCLUDE_OM_SOUND_MIDI_TIME_H


#include "omSoundBaseConfig.h"


#include "omSoundTimeSignature.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a musical position within a MIDI sequence.
/**
  * This position is used for MIDI playback and sequencing, as well as synchronizing
  * audio effects with MIDI data.
  *
  * The MIDI time is represented by a tempo, measure index, time signature, and
  * fractional number of beats since the beginning of the measure.
  */
class MIDITime
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default MIDI time object with the default tempo of 120 quarter notes per minute.
			OM_INLINE MIDITime()
				:	tempo( 120.0f ),
					beat( 0.0f ),
					measure( 0 ),
					timeSignature()
			{
			}
			
			
			/// Create a new MIDI Time object with the specified tempo, measure index, beat number, and time signature.
			OM_INLINE MIDITime( Float newTempo, Index newMeasure, Float newBeat, TimeSignature newTimeSignature )
				:	tempo( newTempo ),
					beat( newBeat ),
					measure( (UByte)newMeasure ),
					timeSignature( newTimeSignature )
			{
			}
			
			
		//********************************************************************************
		//******	Tempo Accessor Methods
			
			
			/// Return the number of quarter notes per minutes for the current position in the MIDI sequence.
			/**
			  * This value can be used to convert the beat value into a time in seconds.
			  */
			OM_INLINE Float getTempo() const
			{
				return tempo;
			}
			
			
			/// Return the number of quarter notes per minutes for the current position in the MIDI sequence.
			/**
			  * This value can be used to convert the beat value into a time in seconds.
			  *
			  * The new tempo is clamped to be greater than or equal to 0.
			  */
			OM_INLINE void setTempo( Float newTempo )
			{
				tempo = math::max( newTempo, Float(0) );
			}
			
			
		//********************************************************************************
		//******	Measure Accessor Methods
			
			
			/// The index of the current measure within the MIDI sequence.
			/**
			  * This value should not be used to compute the current time position
			  * within the MIDI sequence (along with the time signature and tempo), because
			  * the time signature and tempo may change throughout a given sequence, making
			  * a simple computation of the current time impossible given only the measure
			  * index.
			  */
			OM_INLINE Index getMeasure() const
			{
				return (Index)measure;
			}
			
			
			/// Set the index of the current measure within the MIDI sequence.
			/**
			  * This value should not be used to compute the current time position
			  * within the MIDI sequence (along with the time signature and tempo), because
			  * the time signature and tempo may change throughout a given sequence, making
			  * a simple computation of the current time impossible given only the measure
			  * index.
			  */
			OM_INLINE void setMeasure( Index newMeasure )
			{
				measure = (UByte)newMeasure;
			}
			
			
		//********************************************************************************
		//******	Beat Accessor Methods
			
			
			/// Return the fractional number of time signature beats since the beginning of the measure.
			/**
			  * This value is represented in units of the time signature,
			  * so if the time signature is 6/8, the beat value can be in the
			  * range from 0 to 5.99999 and indicates the number of eigth notes
			  * since the start of the measure.
			  */
			OM_INLINE Float getBeat() const
			{
				return beat;
			}
			
			
			/// Set the fractional number of time signature beats since the beginning of the measure.
			/**
			  * This value is represented in units of the time signature,
			  * so if the time signature is 6/8, the beat value can be in the
			  * range from 0 to 5.99999 and indicates the number of eigth notes
			  * since the start of the measure.
			  *
			  * The new beat value is clamped to be greater than 0.
			  */
			OM_INLINE void setBeat( Float newBeat )
			{
				beat = math::max( newBeat, Float(0) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of quarter notes per minute for the current position in the MIDI sequence.
			Float tempo;
			
			
			/// The fractional number of time signature beats since the beginning of the measure.
			/**
			  * This value is represented in units of the time signature,
			  * so if the time signature is 6/8, the beat value can be in the
			  * range from 0 to 5.99999 and indicates the number of eigth notes
			  * since the start of the measure.
			  */
			Float beat;
			
			
			/// The index of the current measure within the MIDI sequence.
			UInt32 measure;
			
			
			/// The time signature for the current measure.
			TimeSignature timeSignature;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_TIME_H
