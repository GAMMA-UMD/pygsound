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

#ifndef INCLUDE_OM_TENSOR_TYPE_H
#define INCLUDE_OM_TENSOR_TYPE_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "omScalarType.h"
#include "omVector2D.h"
#include "omVector3D.h"
#include "omVector4D.h"
#include "omMatrix2D.h"
#include "omMatrix3D.h"
#include "omMatrix4D.h"
#include "omQuaternion.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the type of a general math tensor.
/**
  * A TensorType can represent tensors up to 3 dimensions.
  * It is specified by a ScalarType enumeration indicating the type
  * of the elements of the tensor, plus the size along each of 3 dimensions.
  */
class TensorType
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an undefined tensor type with 0 rows and columns.
			OM_FORCE_INLINE TensorType()
				:	type( ScalarType::UNDEFINED )
			{
				size[0] = 0;
				size[1] = 0;
				size[2] = 0;
			}
			
			
			/// Create a scalar tensor type for the specified scalar type.
			OM_FORCE_INLINE TensorType( const ScalarType& newScalarType )
				:	type( newScalarType )
			{
				size[0] = 0;
				size[1] = 0;
				size[2] = 0;
			}
			
			
			/// Create a scalar tensor type for the specified scalar type.
			OM_FORCE_INLINE TensorType( ScalarType::Enum newScalarType )
				:	type( newScalarType )
			{
				size[0] = 1;
				size[1] = 1;
				size[2] = 1;
			}
			
			
			/// Create a vector tensor type for the specified scalar type and number of components (rows).
			OM_INLINE TensorType( ScalarType::Enum newScalarType, Size newSize0 )
				:	type( newScalarType )
			{
				size[0] = newSize0;
				size[1] = 1;
				size[2] = 1;
			}
			
			
			/// Create a matrix tensor type for the specified scalar type and number of rows/columns.
			OM_INLINE TensorType( ScalarType::Enum newScalarType, Size newSize0, Size newSize1 )
				:	type( newScalarType )
			{
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = 1;
			}
			
			
			/// Create a 3D matrix tensor type for the specified scalar type and dimensions.
			OM_INLINE TensorType( ScalarType::Enum newScalarType, Size newSize0, Size newSize1, Size newSize2 )
				:	type( newScalarType )
			{
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = newSize2;
			}
			
			
		//********************************************************************************
		//******	Static Shader Tensor Type Templated Accessor Method
			
			
			/// Return an TensorType object for the specified templated type.
			/**
			  * This method returns a valid tensor type for all standard
			  * scalar/vector/matrix/color types. 
			  * Calling this empty method will produce a compiler error if the
			  * templated type is not a supported tensor type.
			  */
			template < typename T >
			OM_FORCE_INLINE static TensorType of();
			
			
		//********************************************************************************
		//******	Tensor Type Validity Accessor Method
			
			
			/// Check to see if the templated type is a supported tensor type.
			/**
			  * Calling this empty method will produce a compiler error if the
			  * templated type is not a supported tensor type.
			  */
			template < typename T >
			OM_FORCE_INLINE static void check();
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this tensor type is equal to another.
			OM_FORCE_INLINE Bool operator == ( const TensorType& other ) const
			{
				return type == other.type && size[0] == other.size[0] && size[1] == other.size[1] && size[2] == other.size[2];
			}
			
			
			/// Return whether or not this tensor type is not equal to another.
			OM_FORCE_INLINE Bool operator != ( const TensorType& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Primitive Type Accessor Methods
			
			
			/// Return whether or not this tensor's scalar type is a floating-point format (32 or 64-bit).
			OM_FORCE_INLINE Bool isFloatingPoint() const
			{
				return type.isFloatingPoint();
			}
			
			
			/// Return the scalar type of this tensor type.
			OM_FORCE_INLINE const ScalarType& getScalarType() const
			{
				return type;
			}
			
			
			/// Set the scalar type of this tensor type.
			OM_FORCE_INLINE void setScalarType( ScalarType newScalarType )
			{
				type = newScalarType;
			}
			
			
		//********************************************************************************
		//******	Row and Column Count Accessor Methods
			
			
			/// Return the size of this tensor type along the specified dimension index.
			OM_FORCE_INLINE Size getSize( Index dimension ) const
			{
				return size[dimension];
			}
			
			
			/// Set the size of this tensor type along the specified dimension index.
			OM_FORCE_INLINE void setSize( Index dimension, Size newSize )
			{
				size[dimension] = newSize;
			}
			
			
			/// Return the total number of components that are part of this tensor type.
			OM_FORCE_INLINE Size getScalarCount() const
			{
				return size[0]*size[1]*size[2];
			}
			
			
			/// Return the number of dimensions in this tensor.
			/**
			  * A scalar has dimension 0, a vector has dimension 1, a matrix
			  * has dimension 2, and a 3D matrix has dimension 3.
			  */
			OM_FORCE_INLINE Size getDimensionCount() const
			{
				if ( size[2] > Size(1) )
					return 3;
				else if ( size[1] > Size(1) )
					return 2;
				else if ( size[0] > Size(1) )
					return 1;
				else
					return 0;
			}
			
			
			/// Return the maximum number of dimensions that a tensor can have, 3.
			OM_FORCE_INLINE static Size getMaxDimensionCount()
			{
				return MAX_DIMENSION_COUNT;
			}
			
			
		//********************************************************************************
		//******	Shader Tensor Type Accessor Methods
			
			
			/// Return whether or not this tensor type represents a scalar index type, e.g. unsigned integers.
			OM_FORCE_INLINE Bool isIndex() const
			{
				return size[0] == Size(1) && size[1] == Size(1) && size[2] == Size(1) &&
						type.isInteger() && !type.isSigned();
			}
			
			
			/// Return whether or not this tensor type represents a scalar type.
			OM_FORCE_INLINE Bool isScalar() const
			{
				return size[0] == Size(1) && size[1] == Size(1) && size[2] == Size(1);
			}
			
			
			/// Return whether or not this tensor type represents a vector type.
			OM_FORCE_INLINE Bool isVector() const
			{
				return size[0] > Size(1) && size[1] == Size(1) && size[2] == Size(1);
			}
			
			
			/// Return whether or not this tensor type represents a matrix type.
			OM_FORCE_INLINE Bool isMatrix() const
			{
				return size[0] > Size(1) && size[1] > Size(1) && size[2] == Size(1);
			}
			
			
		//********************************************************************************
		//******	Byte Size Accessor Method
			
			
			/// Return the size of an tensor with this type in bytes.
			OM_FORCE_INLINE Size getSizeInBytes() const
			{
				return type.getSizeInBytes()*this->getScalarCount();
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the tensor type.
			data::String toString() const;
			
			
			/// Convert this tensor type into a string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The maximum number of dimensions that a tensor can have, 3.
			static const Size MAX_DIMENSION_COUNT = 3;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The type of this tensor's scalar elements.
			ScalarType type;
			
			
			/// The size of this tensor type along each dimension.
			Size size[MAX_DIMENSION_COUNT];
			
			
			
};




#define OM_DECLARE_TENSOR_TYPE( T, ST, N0, N1, N2 )\
template <> OM_INLINE void TensorType:: check<T>() {}\
template <> OM_INLINE TensorType TensorType:: of<T>() { return TensorType( ScalarType::ST, N0, N1, N2 ); }



#define OM_MACRO_COMMA ,
#define OM_DECLARE_TENSOR_SCALAR_TYPE( ST1, ST2 )\
	OM_DECLARE_TENSOR_TYPE( ST1, ST2, 1, 1, 1 )\
	OM_DECLARE_TENSOR_TYPE( VectorND< ST1 OM_MACRO_COMMA 2 >, ST2, 2, 1, 1 )\
	OM_DECLARE_TENSOR_TYPE( VectorND< ST1 OM_MACRO_COMMA 3 >, ST2, 3, 1, 1 )\
	OM_DECLARE_TENSOR_TYPE( VectorND< ST1 OM_MACRO_COMMA 4 >, ST2, 4, 1, 1 )\
	OM_DECLARE_TENSOR_TYPE( Quaternion4D< ST1 >, ST2, 4, 1, 1 )\
	OM_DECLARE_TENSOR_TYPE( MatrixND< ST1 OM_MACRO_COMMA 2 OM_MACRO_COMMA 2 >, ST2, 2, 2, 1 )\
	OM_DECLARE_TENSOR_TYPE( MatrixND< ST1 OM_MACRO_COMMA 3 OM_MACRO_COMMA 3 >, ST2, 3, 3, 1 )\
	OM_DECLARE_TENSOR_TYPE( MatrixND< ST1 OM_MACRO_COMMA 4 OM_MACRO_COMMA 4 >, ST2, 4, 4, 1 )\




//##########################################################################################
//##########################################################################################
//############		
//############		Fixed-Size Tensor Type Specializations
//############		
//##########################################################################################
//##########################################################################################




OM_DECLARE_TENSOR_SCALAR_TYPE( Bool, BOOLEAN );
OM_DECLARE_TENSOR_SCALAR_TYPE( Int8, INT8 );
OM_DECLARE_TENSOR_SCALAR_TYPE( UInt8, UINT8 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Int16, INT16 );
OM_DECLARE_TENSOR_SCALAR_TYPE( UInt16, UINT16 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Int32, INT32 );
OM_DECLARE_TENSOR_SCALAR_TYPE( UInt32, UINT32 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Int64, INT64 );
OM_DECLARE_TENSOR_SCALAR_TYPE( UInt64, UINT64 );
OM_DECLARE_TENSOR_SCALAR_TYPE( lang::Float16, FLOAT16 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Float32, FLOAT32 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Float64, FLOAT64 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Complex<Float32>, COMPLEX_FLOAT32 );
OM_DECLARE_TENSOR_SCALAR_TYPE( Complex<Float64>, COMPLEX_FLOAT64 );




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TENSOR_TYPE_H
