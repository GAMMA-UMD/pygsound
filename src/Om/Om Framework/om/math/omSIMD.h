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

#ifndef INCLUDE_OM_SIMD_H
#define INCLUDE_OM_SIMD_H


#include "omMathConfig.h"


#include "omSIMDConfig.h"
#include "omSIMDTypes.h"
#include "omSIMDFlags.h"

// SIMD Scalar/Vector Classes.
#include "omSIMDScalar.h"
#include "omSIMDScalarInt16_8.h"
#include "omSIMDScalarInt32_4.h"
#include "omSIMDScalarInt64_2.h"
#include "omSIMDScalarFloat32_4.h"
#include "omSIMDScalarFloat64_2.h"

// SIMD Array Classes.
#include "omSIMDArray.h"
#include "omSIMDArrayInt32.h"
#include "omSIMDArrayFloat32.h"

// SIMD Math Primitive Types
#include "omSIMDVector3D.h"
#include "omSIMDRay3D.h"
#include "omSIMDAABB3D.h"
#include "omSIMDPlane3D.h"
#include "omSIMDTriangle3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


// SIMD Scalar Type Definitions for the given primitive type.
typedef SIMDScalar<Float32,4>	SIMDFloat4;
typedef SIMDScalar<Float64,2>	SIMDDouble2;
typedef SIMDScalar<Int32,4>		SIMDInt4;


//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_H
