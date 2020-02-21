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

#ifndef INCLUDE_OM_TENSOR_MATH_H
#define INCLUDE_OM_TENSOR_MATH_H


#include "omMathConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class Tensor;


//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Arithmetic Methods
//############		
//##########################################################################################
//##########################################################################################




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are mismatched.
  */
Bool add( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are mismatched.
  */
Bool add( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are mismatched.
  */
Bool subtract( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are mismatched.
  */
Bool subtract( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool multiply( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool multiply( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool multiplyScalars( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool multiplyScalars( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool divide( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool divide( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




/// Add a tensor to another tensor, modifying the first tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool divideScalars( Tensor& tensor1, const Tensor& tensor2 );


/// Add two tensors, storing the result in a third tensor.
/**
  * The method returns whether or not the operation was successful.
  * The method can fail if the operand tensor dimensions are invalid.
  */
Bool divideScalars( Tensor& result, const Tensor& tensor1, const Tensor& tensor2 );




//##########################################################################################
//##########################################################################################
//############		
//############		Tensor Linear Algebra Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute the LU factorization of an input matrix A returning the L and U matrices in the output parameters.
Bool lu( const Tensor& A, Tensor& L, Tensor& U );




/// Do a QR factorization of the specified matrix, returning the Q and R matrices in the output parameters.
/**
  * R is an upper-triangular matrix, Q is an orthonormal (unitary) matrix.
  * The matrix A can be formed by the matrix product Q*R.
  */
Bool qr( const Tensor& A, Tensor& Q, Tensor& R );




/// Use a least-squares algorithm to find the best fitting solution for a linear system A*x = b.
Bool leastSquares( const Tensor& A, const Tensor& b, Tensor& x );




/// Solve a linear system A*x = b, returning whether or not it was able to be solved.
Bool solve( const Tensor& A, const Tensor& b, Tensor& x );




/// Compute the inverse of the matrix A, returning whether or not it was able to be inverted.
Bool invert( const Tensor& A, Tensor& inverse );




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TENSOR_MATH_H
