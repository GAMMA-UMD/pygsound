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

#include "omResourceType.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



const ResourceType ResourceType:: UNDEFINED("?");


const ResourceType ResourceType:: INT8("int8");
const ResourceType ResourceType:: UINT8("uint8");
const ResourceType ResourceType:: INT16("int16");
const ResourceType ResourceType:: UINT16("uint16");
const ResourceType ResourceType:: INT32("int32");
const ResourceType ResourceType:: UINT32("uint32");
const ResourceType ResourceType:: INT64("int64");
const ResourceType ResourceType:: UINT64("uint64");
const ResourceType ResourceType:: FLOAT16("float16");
const ResourceType ResourceType:: FLOAT32("float32");
const ResourceType ResourceType:: FLOAT64("float64");


const ResourceType ResourceType:: VECTOR2_F32("Vector2f32");
const ResourceType ResourceType:: VECTOR2_F64("Vector2f64");
const ResourceType ResourceType:: VECTOR3_F32("Vector3f32");
const ResourceType ResourceType:: VECTOR3_F64("Vector3f64");
const ResourceType ResourceType:: VECTOR4_F32("Vector4f32");
const ResourceType ResourceType:: VECTOR4_F64("Vector4f64");


const ResourceType ResourceType:: MATRIX2_F32("Matrix2f32");
const ResourceType ResourceType:: MATRIX2_F64("Matrix2f64");
const ResourceType ResourceType:: MATRIX3_F32("Matrix3f32");
const ResourceType ResourceType:: MATRIX3_F64("Matrix3f64");
const ResourceType ResourceType:: MATRIX4_F32("Matrix4f32");
const ResourceType ResourceType:: MATRIX4_F64("Matrix4f64");


const ResourceType ResourceType:: STRING_ASCII("StringASCII");
const ResourceType ResourceType:: STRING_UTF8("StringUTF8");
const ResourceType ResourceType:: STRING_UTF16("StringUTF16");
const ResourceType ResourceType:: STRING_UTF32("StringUTF32");




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
