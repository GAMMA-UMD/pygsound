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

#include "omScalarType.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




Bool ScalarType:: isSigned() const
{
	switch ( type )
	{
		case ScalarType::BOOLEAN:
		case ScalarType::UINT8:
		case ScalarType::UINT16:
		case ScalarType::UINT32:
		case ScalarType::UINT64:
			return false;
		case ScalarType::INT8:
		case ScalarType::INT16:
		case ScalarType::INT32:
		case ScalarType::INT64:
		case ScalarType::FLOAT16:
		case ScalarType::FLOAT32:
		case ScalarType::FLOAT64:
		case ScalarType::COMPLEX_FLOAT32:
		case ScalarType::COMPLEX_FLOAT64:
			return true;
		default:
			return false;
	}
}



Bool ScalarType:: isInteger() const
{
	switch ( type )
	{
		case ScalarType::UINT8:
		case ScalarType::UINT16:
		case ScalarType::UINT32:
		case ScalarType::UINT64:
		case ScalarType::INT8:
		case ScalarType::INT16:
		case ScalarType::INT32:
		case ScalarType::INT64:
			return true;
		default:
			return false;
	}
}




Bool ScalarType:: isFloatingPoint() const
{
	switch ( type )
	{
		case ScalarType::FLOAT16:
		case ScalarType::FLOAT32:
		case ScalarType::FLOAT64:
		case ScalarType::COMPLEX_FLOAT32:
		case ScalarType::COMPLEX_FLOAT64:
			return true;
		default:
			return false;
	}
}




Size ScalarType:: getSizeInBytes() const
{
	switch ( type )
	{
		case ScalarType::BOOLEAN:			return sizeof(Bool);
		case ScalarType::INT8:				return sizeof(Int8);
		case ScalarType::UINT8:				return sizeof(UInt8);
		case ScalarType::INT16:				return sizeof(Int16);
		case ScalarType::UINT16:			return sizeof(UInt16);
		case ScalarType::INT32:				return sizeof(Int32);
		case ScalarType::UINT32:			return sizeof(UInt32);
		case ScalarType::INT64:				return sizeof(Int64);
		case ScalarType::UINT64:			return sizeof(UInt64);
		case ScalarType::FLOAT16:			return sizeof(lang::Float16);
		case ScalarType::FLOAT32:			return sizeof(Float32);
		case ScalarType::FLOAT64:			return sizeof(Float64);
		case ScalarType::COMPLEX_FLOAT32:	return sizeof(Complex<Float32>);
		case ScalarType::COMPLEX_FLOAT64:	return sizeof(Complex<Float64>);
		default:							return 0;
	}
}




data::String ScalarType:: toString() const
{
	const Char* typeString = "Undefined";
	
	switch ( type )
	{
		case BOOLEAN:			typeString = "Boolean";		break;
		case INT8:				typeString = "Int8";		break;
		case UINT8:				typeString = "UInt8";		break;
		case INT16:				typeString = "Int16";		break;
		case UINT16:			typeString = "UInt16";		break;
		case INT32:				typeString = "Int32";		break;
		case UINT32:			typeString = "UInt32";		break;
		case INT64:				typeString = "Int64";		break;
		case UINT64:			typeString = "UInt64";		break;
		case FLOAT16:			typeString = "Float16";		break;
		case FLOAT32:			typeString = "Float32";		break;
		case FLOAT64:			typeString = "Float64";		break;
		case COMPLEX_FLOAT32:	typeString = "Complex32f";	break;
		case COMPLEX_FLOAT64:	typeString = "Complex64f";	break;
		case UNDEFINED:			typeString = "Undefined";	break;
	}
	
	return data::String(typeString);
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
