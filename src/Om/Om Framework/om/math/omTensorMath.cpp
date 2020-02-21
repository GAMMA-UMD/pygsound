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

#include "omTensorMath.h"


#include "omTensor.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Scalar Type Switch Methods
//############		
//##########################################################################################
//##########################################################################################




OM_FORCE_INLINE static Bool dummyOperation( const Tensor& tensor, Tensor& result )
{
	result.setType( TensorType() );
	return false;
}




OM_FORCE_INLINE static Bool dummyOperation( Tensor& tensor1, const Tensor& tensor2 )
{
	tensor1.setType( TensorType() );
	return false;
}




OM_FORCE_INLINE static Bool dummyOperation( Tensor& result, const Tensor& tensor1, const Tensor& tensor2  )
{
	result.setType( TensorType() );
	return false;
}




template < template < typename T > class Operator >
OM_FORCE_INLINE static Bool scalarTypeSwitch( const ScalarType& scalarType,
												const Tensor& tensor, Tensor& result )
{
	typedef Bool (*Operation)( const Tensor&, Tensor& );
	
	// Force generation of a jump table.
	const Operation functions[ScalarType::COUNT] =
	{
		dummyOperation,
		Operator<Bool>::operate,
		Operator<Int8>::operate,
		Operator<UInt8>::operate,
		Operator<Int16>::operate,
		Operator<UInt16>::operate,
		Operator<Int32>::operate,
		Operator<UInt32>::operate,
		Operator<Int64>::operate,
		Operator<UInt64>::operate,
		Operator<lang::Float16>::operate,
		Operator<Float32>::operate,
		Operator<Float64>::operate,
		dummyOperation,
		Operator< Complex<Float32> >::operate,
		Operator< Complex<Float64> >::operate
	};
	
	// Call the jump table based on the scalar type.
	return functions[scalarType]( tensor, result );
}




template < template < typename T > class Operator >
OM_FORCE_INLINE static Bool scalarTypeSwitch( const ScalarType& scalarType,
												Tensor& tensor1, const Tensor& tensor2 )
{
	typedef Bool (*Operation)( Tensor&, const Tensor& );
	
	// Force generation of a jump table.
	const Operation functions[ScalarType::COUNT] =
	{
		dummyOperation,
		Operator<Bool>::operate,
		Operator<Int8>::operate,
		Operator<UInt8>::operate,
		Operator<Int16>::operate,
		Operator<UInt16>::operate,
		Operator<Int32>::operate,
		Operator<UInt32>::operate,
		Operator<Int64>::operate,
		Operator<UInt64>::operate,
		Operator<lang::Float16>::operate,
		Operator<Float32>::operate,
		Operator<Float64>::operate,
		dummyOperation,
		Operator< Complex<Float32> >::operate,
		Operator< Complex<Float64> >::operate
	};
	
	// Call the jump table based on the scalar type.
	return functions[scalarType]( tensor1, tensor2 );
}




template < template < typename T > class Operator >
OM_FORCE_INLINE static Bool scalarTypeSwitch( const ScalarType& scalarType,
												Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	typedef Bool (*Operation)( Tensor&, const Tensor&, const Tensor& );
	
	// Force generation of a jump table.
	const Operation functions[ScalarType::COUNT] =
	{
		dummyOperation,
		Operator<Bool>::operate,
		Operator<Int8>::operate,
		Operator<UInt8>::operate,
		Operator<Int16>::operate,
		Operator<UInt16>::operate,
		Operator<Int32>::operate,
		Operator<UInt32>::operate,
		Operator<Int64>::operate,
		Operator<UInt64>::operate,
		Operator<lang::Float16>::operate,
		Operator<Float32>::operate,
		Operator<Float64>::operate,
		dummyOperation,
		Operator< Complex<Float32> >::operate,
		Operator< Complex<Float64> >::operate
	};
	
	// Call the jump table based on the scalar type.
	return functions[scalarType]( result, tensor1, tensor2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Add Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements tensor add operations for a templated type.
template < typename T >
struct TensorAdd
{
	OM_INLINE static Bool operate( Tensor& tensor1, const Tensor& tensor2 )
	{
		math::add( (T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
	
	OM_INLINE static Bool operate( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
	{
		math::add( (T*)result.getPointer(), (const T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
};




Bool add( Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	return scalarTypeSwitch<TensorAdd>( type1.getScalarType(), tensor1, tensor2 );
}




Bool add( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	// Initialize the result storage.
	result.setType( type1 );
	
	return scalarTypeSwitch<TensorAdd>( type1.getScalarType(), result, tensor1, tensor2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Subtract Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements tensor subtract operations for a templated type.
template < typename T >
struct TensorSubtract
{
	OM_INLINE static Bool operate( Tensor& tensor1, const Tensor& tensor2 )
	{
		math::subtract( (T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
	
	OM_INLINE static Bool operate( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
	{
		math::subtract( (T*)result.getPointer(), (const T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
};




Bool subtract( Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	return scalarTypeSwitch<TensorSubtract>( type1.getScalarType(), tensor1, tensor2 );
}




Bool subtract( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	// Initialize the result storage.
	result.setType( type1 );
	
	return scalarTypeSwitch<TensorSubtract>( type1.getScalarType(), result, tensor1, tensor2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Scalar Multiply Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements per-scalar tensor multiply operations for a templated type.
template < typename T >
struct TensorMultiplyScalar
{
	OM_INLINE static Bool operate( Tensor& tensor1, const Tensor& tensor2 )
	{
		math::multiply( (T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
	
	OM_INLINE static Bool operate( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
	{
		math::multiply( (T*)result.getPointer(), (const T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
};




Bool multiplyScalars( Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	return scalarTypeSwitch<TensorMultiplyScalar>( type1.getScalarType(), tensor1, tensor2 );
}




Bool multiplyScalars( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	// Initialize the result storage.
	result.setType( type1 );
	
	return scalarTypeSwitch<TensorMultiplyScalar>( type1.getScalarType(), result, tensor1, tensor2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Multiply Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements per-scalar tensor multiply operations for a templated type.
template < typename T >
struct TensorMultiply
{
	OM_INLINE static Bool operate( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
	{
		const Size numRows = tensor1.getType().getSize(0);
		const Size numOtherRows = tensor2.getType().getSize(0);
		const Size numOtherColumns = tensor2.getType().getSize(1);
		const T* matrix1Elements = (T*)tensor1.getPointer();
		const T* matrix2Elements = (T*)tensor2.getPointer();
		T* resultElements = (T*)result.getPointer();
		
		for ( Index i = 0; i < numRows; i++ )
		{
			const T* const m1RowStart = matrix1Elements + i;
			
			for ( Index j = 0; j < numOtherColumns; j++ )
			{
				const T* m2Column = matrix2Elements + j*numOtherRows;
				const T* const m2ColumnEnd = m2Column + numOtherRows;
				const T* m1Row = m1RowStart;
				
				T dot = T(0);
				
				while ( m2Column != m2ColumnEnd )
				{
					dot += (*m1Row)*(*m2Column);
					m1Row += numRows;
					m2Column++;
				}
				
				resultElements[i + j*numRows] = dot;
			}
		}
		
		return true;
	}
};




Bool multiply( Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1.getScalarType() != type2.getScalarType() )
		return false;
	
	// Make sure the tensors don't have a dimension higher than 2 (matrix).
	if ( type1.getDimensionCount() > Size(2) || type2.getDimensionCount() > Size(2) )
		return false;
	
	// Make sure the inner matrix dimensions match.
	if ( type1.getSize(1) != type2.getSize(0) )
		return false;
	
	// If the second operand is a scalar, call the faster per-component method.
	if ( type2.isScalar() )
		return scalarTypeSwitch<TensorMultiplyScalar>( type1.getScalarType(), tensor1, tensor2 );
	else
	{
		// Copy the first tensor into a temporary tensor.
		Tensor tensor1Copy = tensor1;
		
		if ( &tensor1 == &tensor2 )
		{
			tensor1.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
			
			return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), tensor1, tensor1Copy, tensor1Copy );
		}
		else
		{
			tensor1.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
			
			return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), tensor1, tensor1Copy, tensor2 );
		}
	}
}




Bool multiply( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1.getScalarType() != type2.getScalarType() )
		return false;
	
	// Make sure the tensors don't have a dimension higher than 2 (matrix).
	if ( type1.getDimensionCount() > Size(2) || type2.getDimensionCount() > Size(2) )
		return false;
	
	// Make sure the inner matrix dimensions match.
	if ( type1.getSize(1) != type2.getSize(0) )
		return false;
	
	// If the result aliases with either input, copy the offending input.
	if ( &result == &tensor1 )
	{
		Tensor tensor1Copy = tensor1;
		
		if ( &result == &tensor2 )
		{
			Tensor tensor2Copy = tensor2;
			result.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
			
			return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), result, tensor1Copy, tensor2Copy );
		}
		else
		{
			result.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
			
			return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), result, tensor1Copy, tensor2 );
		}
	}
	else if ( &result == &tensor2 )
	{
		Tensor tensor2Copy = tensor2;
		result.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
		
		return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), result, tensor1, tensor2Copy );
	}
	else
	{
		result.setType( TensorType( type1.getScalarType(), type1.getSize(0), type2.getSize(1) ) );
		
		return scalarTypeSwitch<TensorMultiply>( type1.getScalarType(), result, tensor1, tensor2 );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Scalar Divide Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements per-scalar tensor divide operations for a templated type.
template < typename T >
struct TensorDivideScalar
{
	OM_INLINE static Bool operate( Tensor& tensor1, const Tensor& tensor2 )
	{
		math::divide( (T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
	
	OM_INLINE static Bool operate( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
	{
		math::divide( (T*)result.getPointer(), (const T*)tensor1.getPointer(), (const T*)tensor2.getPointer(), tensor1.getScalarCount() );
		return true;
	}
};




Bool divideScalars( Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	return scalarTypeSwitch<TensorDivideScalar>( type1.getScalarType(), tensor1, tensor2 );
}




Bool divideScalars( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 )
{
	const TensorType& type1 = tensor1.getType();
	const TensorType& type2 = tensor2.getType();
	
	// Make sure the tensors have the same type and are valid.
	if ( tensor1.isNull() || tensor2.isNull() || type1 != type2 )
		return false;
	
	// Initialize the result storage.
	result.setType( type1 );
	
	return scalarTypeSwitch<TensorDivideScalar>( type1.getScalarType(), result, tensor1, tensor2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Matrix Inversion Methods
//############		
//##########################################################################################
//##########################################################################################




/// A class that implements matrix inversion operations for a templated type.
template < typename T >
struct MatrixInvert
{
	OM_INLINE static Bool operate( const Tensor& tensor, Tensor& result )
	{
		//const Size numRows = tensor.getType().getSize(0);
		//const Size numColumns = tensor.getType().getSize(1);
		//const T* m = (T*)tensor.getPointer();
		
		
		
		
		return true;
	}
};




Bool invert( const Tensor& tensor, Tensor& result )
{
	// Make sure the tensor is valid.
	if ( tensor.isNull() )
		return false;
	
	const TensorType& type = tensor.getType();
	
	// Make sure the tensor is square and not 3D.
	if ( type.getSize(0) != type.getSize(1) || type.getDimensionCount() > Size(2) )
		return false;
	
	// Initialize the result storage.
	result.setType( type );
	
	return scalarTypeSwitch<MatrixInvert>( type.getScalarType(), tensor, result );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
