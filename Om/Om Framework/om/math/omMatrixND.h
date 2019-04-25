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

#ifndef INCLUDE_OM_MATRIX_ND_H
#define INCLUDE_OM_MATRIX_ND_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVectorND.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a matrix of a fixed arbitrary number of rows and columns.
template < typename T, Size numRows, Size numColumns >
class MatrixND
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a matrix with its elements all equal to zero.
			OM_FORCE_INLINE MatrixND()
				:	column()
			{
			}
			
			
			/// Create a matrix from a pointer to an array with elements specified in column-major order.
			OM_FORCE_INLINE MatrixND( const T array[numRows*numColumns] )
			{
				for ( Index i = 0; i < numColumns; i++ )
					for ( Index j = 0; j < numRows; j++ )
						column[i][j] = array[i*numRows + j];
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
				return (T*)&column[0];
			}
			
			
			/// Return a pointer to the matrix's elements in colunn-major order.
			/**
			  * Since matrix elements are stored in column-major order,
			  * no allocation is performed and the elements are accessed directly.
			  */
			OM_FORCE_INLINE const T* toArrayColumnMajor() const
			{
				return (T*)&column[0];
			}
			
			
			/// Return a reference to the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,numRows>& getColumn( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return a const reference to the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,numRows>& getColumn( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return a reference to the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,numRows>& operator () ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return a const reference to the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,numRows>& operator () ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return a reference the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,numRows>& operator [] ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return a const reference to the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,numRows>& operator [] ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex];
			}
			
			
			/// Return the row at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,numColumns> getRow( Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				
				VectorND<T,numColumns> result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.set( i, column[i][rowIndex] );
				
				return result;
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE T& get( Index rowIndex, Index columnIndex )
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex][rowIndex];
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE const T& get( Index rowIndex, Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex][rowIndex];
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE T& operator () ( Index rowIndex, Index columnIndex )
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex][rowIndex];
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE const T& operator () ( Index rowIndex, Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				return column[columnIndex][rowIndex];
			}
			
			
			/// Set the column vector at the specified index in the matrix.
			OM_FORCE_INLINE void setColumn( Index columnIndex, const VectorND<T,numRows>& newColumn )
			{
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				column[columnIndex] = newColumn;
			}
			
			
			/// Set the row vector at the specified index in the matrix.
			OM_FORCE_INLINE void setRow( Index rowIndex, const VectorND<T,numColumns>& newRow )
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				
				for ( Index i = 0; i < numColumns; i++ )
					column[i].set( rowIndex, newRow[i] );
			}
			
			
			/// Get the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE void set( Index rowIndex, Index columnIndex, T value )
			{
				OM_DEBUG_ASSERT( rowIndex < numRows );
				OM_DEBUG_ASSERT( columnIndex < numColumns );
				
				column[columnIndex].set( rowIndex, value );
			}
			
			
		//********************************************************************************
		//******	MatrixND Operations
			
			
			/// Get the determinant of the matrix.
			OM_FORCE_INLINE T getDeterminant() const
			{
			}
			
			
			/// Get the inverse of the matrix if it has one.
			OM_FORCE_INLINE MatrixND invert() const
			{
			}
			
			
			/// Return the orthonormalization of this matrix.
			OM_FORCE_INLINE MatrixND orthonormalize() const
			{
				MatrixND<T,numRows,numColumns> result;
				
				for ( Index i = 0; i < numColumns; i++ )
				{
					VectorND<T,numRows> newColumn = getColumn(i);
					
					for ( Index j = 0; j < i; j++ )
						newColumn -= getColumn(i).projectOn( result.getColumn(j) );
					
					result.setColumn( i, newColumn.normalize() );
				}
				
				return result;
			}
			
			
			/// Return the transpose of this matrix.
			OM_FORCE_INLINE MatrixND<T,numColumns,numRows> transpose() const
			{
				MatrixND<T,numColumns,numRows> result;
				
				for ( Index i = 0; i < numRows; i++ )
					result.setColumn( i, getRow(i) );
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not every component in this matrix is equal to that in another matrix.
			OM_FORCE_INLINE Bool operator == ( const MatrixND<T,numRows,numColumns> &matrix ) const
			{
				Bool result = true;
				
				for ( Index i = 0; i < numColumns; i++ )
					result &= (column[i] == matrix.getColumn(i));
				
				return result;
			}
			
			
			/// Return whether or not some component in this matrix is not equal to that in another matrix.
			OM_FORCE_INLINE Bool operator != ( const MatrixND<T,numRows,numColumns>& matrix ) const
			{
				return !(operator==( matrix ));
			}
			
			
		//********************************************************************************
		//******	MatrixND Negation/Positivation Operators
			
			
			/// Negate every element of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - () const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = -column[i];
				
				return result;
			}
			
			
			/// 'Positivate' every element of this matrix, returning a copy of the original matrix.
			OM_FORCE_INLINE MatrixND operator + () const
			{
				return *this;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const MatrixND<T,numRows,numColumns>& matrix ) const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] + matrix.column[i];
				
				return result;
			}
			
			
			/// Add a scalar to the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const T& value ) const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] + value;
				
				return result;
			}
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const MatrixND<T,numRows,numColumns>& matrix ) const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] - matrix.column[i];
				
				return result;
			}
			
			
			/// Subtract a scalar from the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const T& value ) const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] - value;
				
				return result;
			}
			
			
			/// Multiply a matrix by this matrix and return the result.
			template < Size otherColumnDimension >
			OM_FORCE_INLINE MatrixND<T,numRows,otherColumnDimension> operator * ( const MatrixND<T,numColumns,otherColumnDimension>& matrix )
			{
				MatrixND<T,numRows,otherColumnDimension> result;
				
				for ( Index i = 0; i < numRows; i++ )
				{
					for ( Index j = 0; j < otherColumnDimension; j++ )
					{
						T dot = T(0);
						
						for ( Index k = 0; k < numColumns; k++ )
							dot += column[k][i] * matrix.column[j][k];
						
						result.column[j][i] = dot;
					}
				}
				
				return result;
			}
			
			
			/// Multiply a vector/point by this matrix and return the result.
			OM_FORCE_INLINE VectorND<T,numRows> operator * ( const VectorND<T,numColumns>& vector )
			{
				VectorND<T,numRows> result;
				
				for ( Index i = 0; i < numRows; i++ )
				{
					T dot = T(0);
					
					for ( Index j = 0; j < numColumns; j++ )
						dot += column[j][i] * vector[j];
					
					result[i] = dot;
				}
				
				return result;
			}
			
			
			/// Multiply the elements of this matrix by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator * ( const T& value ) const
			{
				MatrixND result;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] * value;
				
				return result;
			}
			
			
			/// Divide the elements of this matrix by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator / ( const T& value ) const
			{
				MatrixND result;
				T inverseValue = T(1)/value;
				
				for ( Index i = 0; i < numColumns; i++ )
					result.column[i] = column[i] * inverseValue;
				
				return result;
			}
			
			
		//********************************************************************************
		//******	MatrixND Operators
			
			
			/// Add the elements of another matrix to this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const MatrixND<T,numRows,numColumns>& matrix )
			{
				for ( Index i = 0; i < numColumns; i++ )
					column[i] += matrix.column[i];
				
				return *this;
			}
			
			
			/// Add a scalar value to the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const T& value )
			{
				for ( Index i = 0; i < numColumns; i++ )
					column[i] += value;
				
				return *this;
			}
			
			
			/// Subtract the elements of another matrix from this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const MatrixND<T,numRows,numColumns>& matrix )
			{
				for ( Index i = 0; i < numColumns; i++ )
					column[i] -= matrix.column[i];
				
				return *this;
			}
			
			
			/// Subtract a scalar value from the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const T& value )
			{
				for ( Index i = 0; i < numColumns; i++ )
					column[i] -= value;
				
				return *this;
			}
			
			
			/// Multiply the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator *= ( const T& value )
			{
				for ( Index i = 0; i < numColumns; i++ )
					column[i] *= value;
				
				return *this;
			}
			
			
			/// Divide the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator /= ( const T& value )
			{
				T inverseValue = T(1)/value;
				
				for ( Index i = 0; i < numColumns; i++ )
					column[i] *= inverseValue;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this matrix into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				for ( Index i = 0; i < numRows; i++ )
				{
					buffer << "[ ";
					
					for ( Index j = 0; j < numColumns; j++ )
					{
						if ( j != numColumns - 1 )
							buffer << column[i][j] << ", ";
						else
							buffer << column[i][j] << ' ';
					}
					
					if ( i != numRows - 1 )
						buffer << "]\n";
					else
						buffer << ']';
				}
				
				return buffer.toString();
			}
			
			
			/// Convert this matrix into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// Constant matrix with all elements equal to zero.
			static const MatrixND ZERO;
			
			
			/// Constant matrix that is the identity matrix.
			static const MatrixND IDENTITY;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Return the identity matrix for this matrix size.
			static MatrixND getIdentity()
			{
				MatrixND result;
				Size minDimension = math::min( numRows, numColumns );
				
				for ( Index i = 0; i < minDimension; i++ )
					result[i][i] = T(1);
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of column vectors for this matrix.
			VectorND<T,numRows> column[numColumns];
			
			
		//********************************************************************************
		//******	Private Friend Class
			
			
			template < typename U, Size r, Size c >
			friend class MatrixND;
			
			
			
};




template < typename T, Size numRows, Size numColumns >
const MatrixND<T,numRows,numColumns> MatrixND<T,numRows,numColumns>:: ZERO;




template < typename T, Size numRows, Size numColumns >
const MatrixND<T,numRows,numColumns> MatrixND<T,numRows,numColumns>:: IDENTITY = MatrixND<T,numRows,numColumns>::getIdentity();




//##########################################################################################
//##########################################################################################
//############		
//############		Reverse Matrix Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Multiply a matrix's elements by a scalar and return the resulting matrix
template <typename T, Size numRows, Size numColumns>
OM_FORCE_INLINE MatrixND<T,numRows,numColumns> operator + ( const T& c, const MatrixND<T,numRows,numColumns>& matrix ) 
{
	MatrixND<T,numRows,numColumns> result;
	
	for ( Index i = 0; i < numColumns; i++ )
		result.setColumn( i, matrix.getColumn(i) * c );
	
	return result;
}




/// 'Reverse' multiply a vector/point by matrix: multiply it by the matrix's transpose.
template <typename T, Size numRows, Size numColumns>
OM_FORCE_INLINE VectorND<T,numColumns> operator * ( const VectorND<T,numRows>& vector, const MatrixND<T,numRows,numColumns>& matrix ) 
{
	VectorND<T,numColumns> result;
	
	for ( Index i = 0; i < numColumns; i++ )
	{
		T dot = T(0);
		
		for ( Index j = 0; j < numRows; j++ )
			dot += matrix[i][j] * vector[j];
		
		result[i] = dot;
	}
	
	return result;
}




/// Multiply a matrix's elements by a scalar and return the resulting matrix
template <typename T, Size numRows, Size numColumns>
OM_FORCE_INLINE MatrixND<T,numRows,numColumns> operator * ( const T& c, const MatrixND<T,numRows,numColumns>& matrix ) 
{
	MatrixND<T,numRows,numColumns> result;
	
	for ( int i = 0; i < numColumns; i++ )
		result.setColumn( i, matrix.getColumn(i) * c );
	
	return result;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MATRIX_ND_H
