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

#include "omSoundFilterParameterUnits.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




UTF8String FilterParameterUnits:: getAbbreviation() const
{
	const Char* typeString;
	
	switch ( units )
	{
		case GENERIC:
			typeString = "";
			break;
		case DECIBELS:
			typeString = "dB";
			break;
		case PERCENT:
			typeString = "%";
			break;
		case RATIO:
			typeString = "";
			break;
		case SECONDS:
			typeString = "s";
			break;
		case MILLISECONDS:
			typeString = "ms";
			break;
		case HERTZ:
			typeString = "Hz";
			break;
		case METERS:
			typeString = "m";
			break;
		case DEGREES:
			typeString = "°";
			break;
		case INDEX:
			typeString = "";
			break;
		case SAMPLES:
			typeString = "";
			break;
		case SAMPLE_RATE:
			typeString = "Hz";
			break;
		case BPM:
			typeString = "bpm";
			break;
		case BEATS:
			typeString = "beats";
			break;
		case CENTS:
			typeString = "c";
			break;
		case SEMITONES:
			typeString = "";
			break;
		case OCTAVES:
			typeString = "oct";
			break;
		case MIDI_NOTE_NUMBER:
			typeString = "";
			break;
		case MIDI_CONTROL:
			typeString = "";
			break;
		default:
			typeString = "";
			break;
	}
	
	return UTF8String( typeString );
}




UTF8String FilterParameterUnits:: toString() const
{
	const Char* typeString;
	
	switch ( units )
	{
		case GENERIC:
			typeString = "Generic";
			break;
		case DECIBELS:
			typeString = "Decibels";
			break;
		case PERCENT:
			typeString = "Percent";
			break;
		case RATIO:
			typeString = "Ratio";
			break;
		case SECONDS:
			typeString = "Seconds";
			break;
		case MILLISECONDS:
			typeString = "Milliseconds";
			break;
		case HERTZ:
			typeString = "Hertz";
			break;
		case METERS:
			typeString = "Meters";
			break;
		case DEGREES:
			typeString = "Degrees";
			break;
		case INDEX:
			typeString = "Index";
			break;
		case SAMPLES:
			typeString = "Samples";
			break;
		case SAMPLE_RATE:
			typeString = "Sample Rate";
			break;
		case BPM:
			typeString = "BPM";
			break;
		case BEATS:
			typeString = "Beats";
			break;
		case CENTS:
			typeString = "Cents";
			break;
		case SEMITONES:
			typeString = "Semitones";
			break;
		case OCTAVES:
			typeString = "Octaves";
			break;
		case MIDI_NOTE_NUMBER:
			typeString = "MIDI Note Number";
			break;
		case MIDI_CONTROL:
			typeString = "MIDI Control";
			break;
		default:
			typeString = "Undefined";
			break;
	}
	
	return UTF8String( typeString );
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
