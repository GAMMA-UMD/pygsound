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

#ifndef INCLUDE_OM_MATRIX_2D_H
#define INCLUDE_OM_MATRIX_2D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVector2D.h"
#include "omMatrixND.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################





//********************************************************************************
/// A class that represents a 2x2 matrix.
/**
  * Elements in the matrix are stored in column-major order.
  */
template < typename T>
class MatrixND<T,2,2>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a 2x2 matrix with all elements equal to zero.
			OM_FORCE_INLINE MatrixND()
				:	x( 0, 0 ),
					y( 0, 0 )
			{
			}
			
			/// Create a 2x2 matrix from two column vectors.
			OM_FORCE_INLINE MatrixND( const VectorND<T,2>& column1, const VectorND<T,2>& column2 )
				:	x( column1 ),
					y( column2 )
			{
			}
			
			
			/// Create a 2x2 matrix with elements specified in row-major order.
			OM_FORCE_INLINE MatrixND( T a, T b, T c, T d )
				:	x( a, c ),
					y( b, d )
			{
			}
			
			
			/// Create a 2x2 matrix from a pointer to an array of elements in column-major order.
			OM_FORCE_INLINE explicit MatrixND( const T array[4] )
				:	x( array[0], array[1] ),
					y( array[2], array[3] )
			{
			}
			
			
			/// Create a copy of the specified 2x2 matrix with different template parameter type.
			template < typename U >
			OM_FORCE_INLINE MatrixND( const MatrixND<U,2,2>& other )
				:	x( other.x ),
					y( other.y )
			{
			}
			
			
		//********************************************************************************
		//******	Static Matrix Constructors
			
			
			/// Create a 2x2 rotation matrix with the specified rotation in radians.
			OM_FORCE_INLINE static MatrixND rotate( T radians )
			{
				T cosTheta = math::cos(radians);
				T sinTheta = math::sin(radians);
				return MatrixND( cosTheta, -sinTheta,
								sinTheta, cosTheta );
			}
			
			
			/// Create a 2x2 rotation matrix with the specified rotation in degrees.
			OM_FORCE_INLINE static MatrixND rotateDegrees( T degrees )
			{
				return MatrixND::rotate( math::degreesToRadians( degrees ) );
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Return a pointer to the matrix's elements in colunn-major order.
			/**
			  * Since matrix elements are stored in column-major order,
			  * no allocation is performed and the elements are accessed directly.
			  */
			OM_FORCE_INLINE T* toArrayColumnMajor()
			{
				return (T*)&x;
			}
			
			
			/// Return a pointer to the matrix's elements in colunn-major order.
			/**
			  * Since matrix elements are stored in column-major order,
			  * no allocation is performed and the elements are accessed directly.
			  */
			OM_FORCE_INLINE const T* toArrayColumnMajor() const
			{
				return (T*)&x;
			}
			
			
			/// Place the elements of the matrix at the location specified in row-major order.
			/**
			  * The output array must be at least 4 elements long.
			  */
			OM_FORCE_INLINE void toArrayRowMajor( T* outputArray ) const
			{
				outputArray[0] = x.x;	outputArray[1] = y.x;
				outputArray[2] = x.y;	outputArray[3] = y.y;
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,2>& getColumn( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,2>& getColumn( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,2>& operator () ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,2>& operator () ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,2>& operator [] ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,2>& operator [] ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the row at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,2> getRow( Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( rowIndex < 2 );
				
				switch ( rowIndex )
				{
					case 0: return VectorND<T,2>( x.x, y.x );
					case 1: return VectorND<T,2>( x.y, y.y );
					default: return VectorND<T,2>::ZERO;
				}
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE T& get( Index columnIndex, Index rowIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 && rowIndex < 2 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE const T& get( Index columnIndex, Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 2 && rowIndex < 2 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE T& operator () ( Index columnIndex, Index rowIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 && rowIndex < 2 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE const T& operator () ( Index columnIndex, Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 2 && rowIndex < 2 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Return the diagonal vector of this matrix.
			OM_FORCE_INLINE VectorND<T,2> getDiagonal() const
			{
				return VectorND<T,2>( x.x, y.y );
			}
			
			
			/// Set the element in the matrix at the specified (row, column) index.
			OM_FORCE_INLINE void set( Index columnIndex, Index rowIndex, T value )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 && rowIndex < 2 );
				
				return (&x)[columnIndex][rowIndex] = value;
			}
			
			
			/// Set the column in the matrix at the specified index.
			OM_FORCE_INLINE void setColumn( Index columnIndex, const VectorND<T,2>& newColumn )
			{
				OM_DEBUG_ASSERT( columnIndex < 2 );
				
				(&x)[columnIndex] = newColumn;
			}
			
			
			/// Set the row in the matrix at the specified index.
			OM_FORCE_INLINE void setRow( Index rowIndex, const VectorND<T,2>& newRow )
			{
				OM_DEBUG_ASSERT( rowIndex < 2 );
				
				switch ( rowIndex )
				{
					case 0: x.x = newRow.x;	y.x = newRow.y; return;
					case 1: x.y = newRow.x;	y.y = newRow.y; return;
				}
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Return the determinant of this matrix.
			OM_FORCE_INLINE T getDeterminant() const
			{
				return x.x*y.y - y.x*x.y;
			}
			
			
			/// Return the inverse of this matrix, or the zero matrix if the matrix has no inverse.
			/**
			  * Whether or not the matrix is invertable is determined by comparing the determinant
			  * to a threshold - if the absolute value of the determinant is less than the threshold,
			  * the matrix is not invertable.
			  */
			OM_FORCE_INLINE MatrixND invert( T threshold = 0 ) const
			{
				T det = getDeterminant();
				
				if ( math::abs(det) <= threshold )
					return MatrixND::ZERO;
				
				T detInv = T(1)/det;
				
				return MatrixND(	y.y*detInv, -y.x*detInv, -x.y*detInv, x.x*detInv );
			}
			
			
			/// Compute the inverse of this matrix, returning the result in the output parameter.
			/**
			  * The method returns whether or not the matrix was able to be inverted. This propery
			  * is determined by comparing the determinant to a threshold - if the absolute value of 
			  * the determinant is less than the threshold, the matrix is not invertable.
			  */
			OM_FORCE_INLINE Bool invert( MatrixND& inverse, T threshold = 0 ) const
			{
				T det = getDeterminant();
				
				if ( math::abs(det) <= threshold )
					return false;
				
				T detInv = T(1)/det;
				
				inverse = MatrixND(	y.y*detInv, -y.x*detInv, -x.y*detInv, x.x*detInv );
				
				return true;
			}
			
			
			/// Return the orthonormalization of this matrix.
			/**
			  * This matrix that is returned has both column vectors of unit
			  * length and perpendicular to each other.
			  */
			OM_FORCE_INLINE MatrixND orthonormalize() const
			{
				VectorND<T,2> X = x.normalize();
				
				return MatrixND( X, VectorND<T,2>( -X.y, X.x ) );
			}
			
			
			/// Return the transposition of this matrix.
			OM_FORCE_INLINE MatrixND transpose() const
			{
				return MatrixND( x.x, x.y, y.x, y.y );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two matrices component-wise for equality.
			OM_FORCE_INLINE Bool operator == ( const MatrixND& m ) const
			{
				return x == m.x && y == m.y;
			}
			
			
			/// Compare two matrices component-wise for inequality.
			OM_FORCE_INLINE Bool operator != ( const MatrixND& m ) const
			{
				return x != m.x || y != m.y;
			}
			
			
		//********************************************************************************
		//******	Matrix Negation/Positivation Operators
			
			
			/// Negate every element of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - () const
			{
				return MatrixND( -x, -y );
			}
			
			
			/// 'Positivate' every element of this matrix, returning a copy of the original matrix.
			OM_FORCE_INLINE MatrixND operator + () const
			{
				return MatrixND( x, y );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const MatrixND& matrix ) const
			{
				return MatrixND( x + matrix.x, y + matrix.y );
			}
			
			
			/// Add a scalar to the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const T& value ) const
			{
				return MatrixND( x + value, y + value );
			}
			
			
			/// Subtract a matrix from this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const MatrixND& matrix ) const
			{
				return MatrixND( x - matrix.x, y - matrix.y );
			}
			
			
			/// Subtract a scalar from the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const T& value ) const
			{
				return MatrixND( x - value, y - value );
			}
			
			
			/// Multiply a matrix by this matrix and return the result.
			OM_FORCE_INLINE MatrixND operator * ( const MatrixND& matrix ) const
			{
				return MatrixND(	x.x*matrix.x.x + y.x*matrix.x.y,
									x.x*matrix.y.x + y.x*matrix.y.y,
									x.y*matrix.x.x + y.y*matrix.x.y,
									x.y*matrix.y.x + y.y*matrix.y.y );
			}
			
			
			/// Multiply a vector/point by this matrix and return the result.
			OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& vector ) const
			{
				return VectorND<T,2>( x.x*vector.x + y.x*vector.y,
									x.y*vector.x + y.y*vector.y );
			}
			
			
			/// Multiply this matrix's elements by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator * ( const T& value ) const
			{
				return MatrixND( x*value, y*value );
			}
			
			
			/// Divide the elements of this matrix by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator / ( const T& value ) const
			{
				return MatrixND( x/value, y/value );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add the elements of another matrix to this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const MatrixND& matrix2 )
			{
				x += matrix2.x;
				y += matrix2.y;
				
				return *this;
			}
			
			
			/// Subtract the elements of another matrix from this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const MatrixND& matrix2 )
			{
				x -= matrix2.x;
				y -= matrix2.y;
				
				return *this;
			}
			
			
			/// Add a scalar value to the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const T& value )
			{
				x += value;
				y += value;
				
				return *this;
			}
			
			
			/// Subtract a scalar value from the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const T& value )
			{
				x -= value;
				y -= value;
				
				return *this;
			}
			
			
			/// Multiply the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator *= ( const T& value )
			{
				x *= value;
				y *= value;
				
				return *this;
			}
			
			
			/// Divide the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator /= ( const T& value )
			{
				x /= value;
				y /= value;
				
				return *this;
			}
			
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 2x2 matrix into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "[ " << x.x << ", " << y.x << " ]\n";
				buffer << "[ " << x.y << ", " << y.y << " ]";
				
				return buffer.toString();
			}
			
			
			/// Convert this 2x2 matrix into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The first column vector of the matrix.
			VectorND<T,2> x;
			
			
			/// The second column vector of the matrix.
			VectorND<T,2> y;
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// Constant matrix with all elements equal to zero.
			static const MatrixND ZERO;
			
			
			/// Constant matrix with diagonal elements equal to one and all others equal to zero.
			static const MatrixND IDENTITY;
			
			
};




template <typename T>
const MatrixND<T,2,2> MatrixND<T,2,2>:: ZERO( T(0), T(0), T(0), T(0) );




template <typename T>
const MatrixND<T,2,2> MatrixND<T,2,2>:: IDENTITY( T(1), T(0), T(0), T(1) );




//##########################################################################################
//##########################################################################################
//############		
//############		Reverse Matrix Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// 'Reverse' multiply a vector/point by matrix: multiply it by the matrix's transpose.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& vector, const MatrixND<T,2,2>& matrix )
{	
	return VectorND<T,2>( matrix.x.x * vector.x + matrix.x.y * vector.y,
							matrix.y.x * vector.x + matrix.y.y * vector.y );
}




/// Multiply a matrix's elements by a scalar and return the resulting matrix
template < typename T>
OM_FORCE_INLINE MatrixND<T,2,2> operator * ( const T& value, const MatrixND<T,2,2>& matrix )
{
	return MatrixND<T,2,2>( matrix.x*value, matrix.y*value, matrix.z*value );
}




/// Add a scalar to the elements of a matrix and return the resulting matrix.
template < typename T>
OM_FORCE_INLINE MatrixND<T,2,2> operator + ( const T& value, const MatrixND<T,2,2>& matrix )
{
	return MatrixND<T,2,2>( matrix.x + value, matrix.y + value, matrix.z + value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Other Matrix Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the absolute value of the specified matrix, such that the every component is positive.
template < typename T >
OM_FORCE_INLINE MatrixND<T,2,2> abs( const MatrixND<T,2,2>& matrix )
{
	return MatrixND<T,2,2>(	math::abs(matrix.x.x), math::abs(matrix.y.x),
						math::abs(matrix.x.y), math::abs(matrix.y.y) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		2D Matrix Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef MatrixND<int,2,2>		Matrix2i;
typedef MatrixND<float,2,2>		Matrix2f;
typedef MatrixND<double,2,2>	Matrix2d;




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MATRIX_2D_H
