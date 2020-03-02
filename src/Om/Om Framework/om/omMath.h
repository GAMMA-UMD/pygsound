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

#ifndef INCLUDE_OM_MATH_H
#define INCLUDE_OM_MATH_H


#include "math/omMathConfig.h"


#include "math/omScalarMath.h"
#include "math/omArrayMath.h"


#include "math/omAABB1D.h"
#include "math/omAABB2D.h"
#include "math/omAABB3D.h"


#include "math/omVector2D.h"
#include "math/omVector3D.h"
#include "math/omVector4D.h"
#include "math/omVectorND.h"

#include "math/omMatrix2D.h"
#include "math/omMatrix3D.h"
#include "math/omMatrix4D.h"
#include "math/omMatrixND.h"
#include "math/omMatrix.h"

#include "math/omQuaternion.h"

#include "math/omTransform3D.h"


#include "math/omRay3D.h"


#include "math/omPlane3D.h"


#include "math/omSphere3D.h"


#include "math/omComplex.h"


// SIMD Scalar/Vector Classes.
#include "math/omSIMD.h"

#include "math/omMathPrimitives.h"

#include "math/omRandom.h"

#include "math/omScalarType.h"

#include "math/omFFTBase.h"
#include "math/omFFTComplex.h"
#include "math/omFFTReal.h"
#include "math/omFFT.h"
#include "math/omResample.h"
#include "math/omInterpolation.h"


#include "math/omSphericalHarmonics.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




// 1D Bounding Box Type Definitions.
typedef AABB1D<int>			AABB1i;
typedef AABB1D<float>		AABB1f;
typedef AABB1D<double>		AABB1d;

// 2D Bounding Box Type Definitions.
typedef AABB2D<int>			AABB2i;
typedef AABB2D<float>		AABB2f;
typedef AABB2D<double>		AABB2d;

// 3D Bounding Box Type Definitions.
typedef AABB3D<int>			AABB3i;
typedef AABB3D<float>		AABB3f;
typedef AABB3D<double>		AABB3d;




// 2D Matrix Type Definitions.
typedef MatrixND<int,2,2>		Matrix2i;
typedef MatrixND<float,2,2>		Matrix2f;
typedef MatrixND<double,2,2>	Matrix2d;

// 3D Matrix Type Definitions.
typedef MatrixND<int,3,3>		Matrix3i;
typedef MatrixND<float,3,3>		Matrix3f;
typedef MatrixND<double,3,3>	Matrix3d;

// 4D Matrix Type Definitions.
typedef MatrixND<int,4,4>		Matrix4i;
typedef MatrixND<float,4,4>		Matrix4f;
typedef MatrixND<double,4,4>	Matrix4d;




// 2D Vector Type Definitions.
typedef VectorND<int,2>			Vector2i;
typedef VectorND<float,2>		Vector2f;
typedef VectorND<double,2>		Vector2d;
typedef VectorND<Size,2>		Size2D;

// 3D Vector Type Definitions.
typedef VectorND<int,3>			Vector3i;
typedef VectorND<float,3>		Vector3f;
typedef VectorND<double,3>		Vector3d;
typedef VectorND<Size,3>		Size3D;

// 4D Vector Type Definitions.
typedef VectorND<int,4>			Vector4i;
typedef VectorND<float,4>		Vector4f;
typedef VectorND<double,4>		Vector4d;
typedef VectorND<Size,4>		Size4D;




// Quaternion Type Definitions.
typedef Quaternion4D<int>		Quaternion4i;
typedef Quaternion4D<float>		Quaternion4f;
typedef Quaternion4D<double>	Quaternion4d;




// 3D Plane Type Definitions.
typedef Plane3D<int> Plane3i;
typedef Plane3D<float> Plane3f;
typedef Plane3D<double> Plane3d;


// 3D Ray Type Definitions.
typedef Ray3D<int>		Ray3i;
typedef Ray3D<float>	Ray3f;
typedef Ray3D<double>	Ray3d;



// 3D Sphere Type Definitions.
typedef Sphere3D<int>		Sphere3i;
typedef Sphere3D<float>		Sphere3f;
typedef Sphere3D<double>	Sphere3d;




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_MATH_H
