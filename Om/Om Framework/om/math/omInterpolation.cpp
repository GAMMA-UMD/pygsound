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

#include "omInterpolation.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		String Conversion Method
//############		
//##########################################################################################
//##########################################################################################




data::String Interpolation:: toString() const
{
	const Char* typeString = "Undefined";
	
	switch ( type )
	{
		case NONE:		typeString = "None";		break;
		case LINEAR:	typeString = "Linear";		break;
		case BEZIER:	typeString = "Bezier";		break;
		case HERMITE:	typeString = "Hermite";		break;
		case B_SPLINE:	typeString = "B-Spline";	break;
		case SINC:		typeString = "Sinc";		break;
		case SLERP:		typeString = "Slerp";		break;
	}
	
	return data::String(typeString);
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
