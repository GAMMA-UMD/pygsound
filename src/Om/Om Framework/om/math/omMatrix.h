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

#ifndef INCLUDE_OM_MATRIX_H
#define INCLUDE_OM_MATRIX_H


#include "omMathConfig.h"


#include "omArrayMath.h"


#include "../io/omLog.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "../util/omCopy.h"
#include "../threads/omAtomics.h"
#include "omMatrix2D.h"
#include "omMatrix3D.h"
#include "omMatrix4D.h"
#include "omMatrixND.h"
#include "omVector2D.h"
#include "omVector3D.h"
#include "omVector4D.h"
#include "omVectorND.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a matrix of an arbitrary number of rows and columns.
template < typename T >
class Matrix
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty matrix with 0 row and 0 columns.
			OM_INLINE Matrix()
				:	numRows( 0 ),
					numColumns( 0 ),
					scalars( NULL )
			{
			}
			
			
			/// Create an uninitialized column vector with the specified number of rows.
			OM_INLINE explicit Matrix( Size newNumRows )
				:	numRows( newNumRows ),
					numColumns( 1 ),
					scalars( allocateScalars( newNumRows, 1 ) )
			{
			}
			
			
			/// Create an uninitialized matrix with the specified number of rows and columns.
			OM_INLINE explicit Matrix( Size newNumRows, Size newNumColumns )
				:	numRows( newNumRows ),
					numColumns( newNumColumns ),
					scalars( allocateScalars( newNumRows, newNumColumns ) )
			{
			}
			
			
			/// Create a matrix with the specified number of rows, columns, and scalar initilizer.
			OM_INLINE explicit Matrix( Size newNumRows, Size newNumColumns, const T& initializer )
				:	numRows( newNumRows ),
					numColumns( newNumColumns ),
					scalars( allocateScalars( newNumRows, newNumColumns ) )
			{
				util::set( scalars, initializer, newNumRows*newNumColumns );
			}
			
			
			/// Create a matrix from a pointer to an array with elements specified in column-major order.
			OM_INLINE explicit Matrix( Size newNumRows, Size newNumColumns, const T* initializer )
				:	numRows( newNumRows ),
					numColumns( newNumColumns ),
					scalars( allocateScalars( newNumRows, newNumColumns ) )
			{
				util::copy( scalars, initializer, newNumRows*newNumColumns );
			}
			
			
			/// Create a matrix copy of the specified 2x2 matrix.
			OM_INLINE Matrix( const MatrixND<T,2,2>& matrix )
				:	numRows( 2 ),
					numColumns( 2 ),
					scalars( allocateScalars( 2, 2 ) )
			{
				util::copy( scalars, matrix.toArrayColumnMajor(), 2*2 );
			}
			
			
			/// Create a matrix copy of the specified 3x3 matrix.
			OM_INLINE Matrix( const MatrixND<T,3,3>& matrix )
				:	numRows( 3 ),
					numColumns( 3 ),
					scalars( allocateScalars( 3, 3 ) )
			{
				util::copy( scalars, matrix.toArrayColumnMajor(), 3*3 );
			}
			
			
			/// Create a matrix copy of the specified MxN matrix.
			template < Size newNumRows, Size newNumColumns >
			OM_INLINE Matrix( const MatrixND<T,newNumRows,newNumColumns>& matrix )
				:	numRows( newNumRows ),
					numColumns( newNumColumns ),
					scalars( allocateScalars( newNumRows, newNumColumns ) )
			{
				util::copy( scalars, matrix.toArrayColumnMajor(), newNumRows*newNumColumns );
			}
			
			
			/// Create a matrix copy of the specified N-component column vector.
			template < Size newNumRows >
			OM_INLINE Matrix( const VectorND<T,newNumRows>& vector )
				:	numRows( newNumRows ),
					numColumns( 1 ),
					scalars( allocateScalars( newNumRows, 1 ) )
			{
				util::copy( scalars, vector.toArray(), newNumRows );
			}
			
			
			/// Create a copy of another matrix.
			OM_INLINE Matrix( const Matrix& other )
				:	numRows( other.numRows ),
					numColumns( other.numColumns )
			{
				if ( other.scalars )
				{
					scalars = allocateScalars( other.numRows, other.numColumns );
					util::copy( scalars, other.scalars, other.numRows*other.numColumns );
				}
				else
					scalars = NULL;
			}
			
			
			/// Create a copy of another matrix of a different scalar type.
			template < typename U >
			OM_INLINE Matrix( const Matrix<U>& other )
				:	numRows( other.numRows ),
					numColumns( other.numColumns )
			{
				if ( other.scalars )
				{
					scalars = allocateScalars( other.numRows, other.numColumns );
					T* scalar = scalars;
					T* scalarsEnd = scalars + other.numRows*other.numColumns;
					const U* otherScalar = other.scalars;
					
					while ( scalar != scalarsEnd )
					{
						*scalar = T( *otherScalar );
						scalar++;
						otherScalar++;
					}
				}
				else
					scalars = NULL;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this matrix object, release its reference to the internal storage.
			OM_INLINE ~Matrix()
			{
				if ( scalars )
					deallocateScalars( scalars );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another matrix to this one.
			OM_INLINE Matrix& operator = ( const Matrix& other )
			{
				if ( this != &other )
				{
					if ( other.scalars )
					{
						if ( scalars )
						{
							// Reallocate the array if the size changes.
							if ( numRows != other.numRows || numColumns != other.numColumns )
							{
								deallocateScalars( scalars );
								scalars = allocateScalars( other.numRows, other.numColumns );
							}
						}
						else
							scalars = allocateScalars( other.numRows, other.numColumns );
						
						// Copy the scalars.
						util::copy( scalars, other.scalars, other.numRows*other.numColumns );
					}
					else
					{
						if ( scalars )
							deallocateScalars( scalars );
					}
					
					numRows = other.numRows;
					numColumns = other.numColumns;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return a 2D vector indicating the (row count, column count) in this matrix.
			OM_FORCE_INLINE VectorND<Size,2> getSize() const
			{
				return VectorND<Size,2>( numRows, numColumns );
			}
			
			
			/// Change the size of this matrix without initializing the new matrix.
			/**
			  * Use this method to change the size of a matrix without having to
			  * initialize its elements. This can be faster if you know that valid
			  * entries will later be written to the matrix.
			  */
			OM_INLINE void setSize( Size newNumRows, Size newNumColumns )
			{
				if ( newNumRows == numRows && newNumColumns == numColumns )
					return;
				else
				{
					if ( scalars )
						deallocateScalars( scalars );
					
					if ( newNumRows != Size(0) && newNumColumns != Size(0) )
						scalars = allocateScalars( newNumRows, newNumColumns );
					else
						scalars = NULL;
					
					numRows = newNumRows;
					numColumns = newNumColumns;
				}
			}
			
			
			/// Change the size of this matrix, filling the new entries with the specified value.
			/**
			  * If the matrix size shrinks along a dimension, the matrix is truncated in that dimension.
			  */
			OM_INLINE void setSize( Size newNumRows, Size newNumColumns, const T& initializer )
			{
				if ( newNumRows == numRows && newNumColumns == numColumns )
					return;
				else
				{
					if ( scalars )
					{
						const Size numRowsToCopy = math::min( numRows, newNumRows );
						const Size numColunnsToCopy = math::min( numColumns, newNumColumns );
						
						T* newScalars = allocateScalars( newNumRows, newNumColumns );
						const T* const newScalarsEnd = newScalars + newNumRows*newNumColumns;
						Index column = 0;
						
						// Copy the old columns.
						if ( newNumRows > numRowsToCopy )
						{
							const Size numInitializedRows = newNumRows - numRowsToCopy;
							
							for ( ; column < numColunnsToCopy; column++ )
							{
								T* columnStart = newScalars + column*newNumRows;
								util::copy( columnStart, scalars + column*numRows, numRowsToCopy );
								util::set( columnStart + numRowsToCopy, initializer, numInitializedRows );
							}
						}
						else
						{
							for ( ; column < numColunnsToCopy; column++ )
							{
								T* columnStart = newScalars + column*newNumRows;
								util::copy( columnStart, scalars + column*numRows, numRowsToCopy );
							}
						}
						
						// Initialize any additional columns.
						T* columnStart = newScalars + column*newNumRows;
						util::set( columnStart, initializer, newScalarsEnd - columnStart );
						
						// Deallocate the old storage.
						deallocateScalars( scalars );
						scalars = newScalars;
					}
					else if ( newNumRows != Size(0) && newNumColumns != Size(0) )
					{
						// Initialize the matrix.
						scalars = allocateScalars( newNumRows, newNumColumns );
						util::set( scalars, initializer, newNumRows*newNumColumns );
					}
					else
						scalars = NULL;
						
					numRows = newNumRows;
					numColumns = newNumColumns;
				}
			}
			
			
			/// Return the approximate total size of this matrix's storage in bytes.
			OM_FORCE_INLINE Size getSizeInBytes() const
			{
				return sizeof(Matrix) + sizeof(T)*numRows*numColumns;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return a pointer to the column-major storage for this matrix's elements.
			OM_FORCE_INLINE T* getScalars()
			{
				return scalars;
			}
			
			
			/// Return a const pointer to the column-major storage for this matrix's elements.
			OM_FORCE_INLINE const T* getScalars() const
			{
				return scalars;
			}
			
			
			/// Return the total number of scalars that are stored in this matri.
			OM_FORCE_INLINE Size getScalarCount() const
			{
				return numRows*numColumns;
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE T& get( Index rowIndex, Index columnIndex )
			{
				OM_DEBUG_ASSERT_MESSAGE( rowIndex < numRows, "Invalid matrix row index" );
				OM_DEBUG_ASSERT_MESSAGE( columnIndex < numColumns, "Invalid matrix column index" );
				
				return scalars[rowIndex + columnIndex*numRows];
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE const T& get( Index rowIndex, Index columnIndex ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( rowIndex < numRows, "Invalid matrix row index" );
				OM_DEBUG_ASSERT_MESSAGE( columnIndex < numColumns, "Invalid matrix column index" );
				
				return scalars[rowIndex + columnIndex*numRows];
			}
			
			
			/// Set the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE void set( Index rowIndex, Index columnIndex, const T& value )
			{
				OM_DEBUG_ASSERT_MESSAGE( rowIndex < numRows, "Invalid matrix row index" );
				OM_DEBUG_ASSERT_MESSAGE( columnIndex < numColumns, "Invalid matrix column index" );
				
				scalars[rowIndex + columnIndex*numRows] = value;
			}
			
			
			/// Set all of the elements in the matrix to the specified scalar value.
			OM_FORCE_INLINE void setAll( const T& scalar )
			{
				util::set( scalars, scalar, numRows*numColumns );
			}
			
			
			/// Set all of the elements in the matrix to zero.
			OM_FORCE_INLINE void zero()
			{
				util::zero( scalars, numRows*numColumns );
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE T& operator () ( Index rowIndex, Index columnIndex )
			{
				return this->get( rowIndex, columnIndex );
			}
			
			
			/// Return the element at the specified (row, column) in the matrix.
			OM_FORCE_INLINE const T& operator () ( Index rowIndex, Index columnIndex ) const
			{
				return this->get( rowIndex, columnIndex );
			}
			
			
			/// Return whether or not this matrix's internal storage is not allocated.
			OM_FORCE_INLINE Bool isNull() const
			{
				return scalars == NULL;
			}
			
			
			/// Return whether or not this matrix's internal storage is allocated.
			OM_FORCE_INLINE Bool isSet() const
			{
				return scalars != NULL;
			}
			
			
		//********************************************************************************
		//******	Column Accessor Methods
			
			
			/// Return the number of columns that this matrix has.
			OM_FORCE_INLINE Size getColumnCount() const
			{
				return numColumns;
			}
			
			
			/// Return a reference to the column at the specified index in the matrix.
			OM_INLINE Matrix getColumn( Index columnIndex )
			{
				if ( columnIndex >= numColumns )
					return Matrix();
				
				// Create storage for the result.
				Matrix result( numRows, 1 );
				
				// Copy the column.
				util::copy( result.scalars, scalars + columnIndex*numRows, numRows );
				
				return Matrix( result );
			}
			
			
			/// Set the column vector at the specified index in the matrix.
			OM_INLINE Bool setColumn( Index columnIndex, const Matrix& newColumn )
			{
				const Size numOtherRows = newColumn.numRows;
				const Size numOtherColumns = newColumn.numColumns;
				
				if ( columnIndex >= numColumns || numRows != numOtherRows || numOtherColumns != Size(1) )
					return false;
				
				// Copy the column.
				util::copy( scalars + columnIndex*numRows, newColumn.scalars, numRows );
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Row Accessor Methods
			
			
			/// Return the number of rows that this matrix has.
			OM_FORCE_INLINE Size getRowCount() const
			{
				return numRows;
			}
			
			
			/// Return the row at the specified index in the matrix.
			OM_INLINE Matrix getRow( Index rowIndex ) const
			{
				if ( rowIndex >= numRows )
					return Matrix();
				
				// Create storage for the result.
				Matrix result( 1, numColumns );
				T* resultScalar = result.scalars;
				const T* const resultScalarsEnd = resultScalar + numColumns;
				const T* scalar = scalars + rowIndex;
				
				while ( resultScalar != resultScalarsEnd )
				{
					*resultScalar = *scalar;
					resultScalar++;
					scalar += numColumns;
				}
				
				return Matrix( result );
			}
			
			
			/// Set the row vector at the specified index in the matrix.
			OM_INLINE Bool setRow( Index rowIndex, const Matrix& newRow )
			{
				const Size numOtherRows = newRow.numRows;
				const Size numOtherColumns = newRow.numColumns;
				
				if ( rowIndex >= numRows || numColumns != numOtherColumns || numOtherRows != Size(1) )
					return false;
				
				T* scalar = scalars + rowIndex;
				const T* const scalarsEnd = scalar + numColumns*numRows;
				const T* rowScalar = newRow.scalars;
				
				while ( scalar != scalarsEnd )
				{
					*scalar = *rowScalar;
					scalar += numRows;
					rowScalar += numOtherRows;
				}
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Matrix Operations
			
			/*
			/// Get the determinant of the matrix.
			OM_FORCE_INLINE T getDeterminant() const
			{
				return T(1);
			}
			*/
			
			/// Return the inverse of the matrix if it has one.
			/**
			  * If there is no inverse, or the matrix is not square, a 0 by 0 matrix
			  * is returned.
			  */
			/*
			OM_FORCE_INLINE Matrix invert() const
			{
				Size numRows = storage->numRows;
				Size numColumns = storage->numColumns;
				
				if ( numRows != numColumns )
					return Matrix();
				
				Storage* result = allocateScalars( numRows, numColumns );
				
				return Matrix( result );
			}
			*/
			/*
			
			
			/// Return the orthonormalization of this matrix.
			OM_FORCE_INLINE Matrix orthonormalize() const
			{
				Matrix<T,numRows,numColumns> result;
				
				for ( Index i = 0; i < numColumns; i++ )
				{
					VectorND<T,numRows> newColumn = getColumn(i);
					
					for ( Index j = 0; j < i; j++ )
						newColumn -= getColumn(i).projectOn( result.getColumn(j) );
					
					result.setColumn( i, newColumn.normalize() );
				}
				
				return result;
			}*/
			
			
			/// Return the transpose of this matrix.
			OM_INLINE Matrix transpose() const
			{
				// Create storage for the result.
				Matrix result;
				this->transpose( result );
				
				return Matrix( result );
			}
			
			
			/// Write the transpose of this matrix to the output parameter.
			OM_INLINE void transpose( Matrix& result ) const
			{
				result.setSize( numColumns, numRows );
				T* resultElement = result.scalars;
				const T* matrixElements = scalars;
				const T* matrixElementsEnd = matrixElements + numRows;
				
				while ( matrixElements != matrixElementsEnd )
				{
					const T* matrixRow = matrixElements;
					const T* const matrixRowEnd = matrixRow + numRows*numColumns;
					
					while ( matrixRow != matrixRowEnd )
					{
						*resultElement = *matrixRow;
						resultElement++;
						matrixRow += numRows;
					}
					
					matrixElements++;
				}
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not every element in this matrix is equal to that in another matrix.
			OM_INLINE Bool operator == ( const Matrix& other ) const
			{
				if ( numRows != other.numRows || numColumns != other.numColumns )
					return false;
				
				const T* element1 = scalars;
				const T* element1End = element1 + numRows*numColumns;
				const T* element2 = other.scalars;
				
				while ( element1 != element1End )
				{
					if ( *element1 != *element2 )
						return false;
					
					element1++;
					element2++;
				}
				
				return true;
			}
			
			
			/// Return whether or not some element in this matrix is not equal to that in another matrix.
			OM_FORCE_INLINE Bool operator != ( const Matrix<T>& other ) const
			{
				return !(operator == ( other ));
			}
			
			
		//********************************************************************************
		//******	Matrix Negation/Positivation Operators
			
			
			/// Negate every element of this matrix and return the resulting matrix.
			OM_INLINE Matrix operator - () const
			{
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				T* resultElement = result.scalars;
				const T* const resultElementsEnd = resultElement + numRows*numColumns;
				const T* matrix1Element = scalars;
				
				while ( resultElement != resultElementsEnd )
				{
					*resultElement = -*matrix1Element;
					resultElement++;
					matrix1Element++;
				}
				
				return result;
			}
			
			
			/// 'Positivate' every element of this matrix, returning a copy of the original matrix.
			OM_FORCE_INLINE const Matrix& operator + () const
			{
				return *this;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_INLINE Matrix operator + ( const Matrix& matrix ) const
			{
				// Make sure the matrix dimensions match.
				if ( numRows != matrix.numRows || numColumns != matrix.numColumns )
					return Matrix();
				
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Add the elements and store the result.
				math::add( result.scalars, scalars, matrix.scalars, numRows*numColumns );
				
				return result;
			}
			
			
			/// Add a scalar to the elements of this matrix and return the resulting matrix.
			OM_INLINE Matrix operator + ( const T& scalar ) const
			{
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Add the elements and store the result.
				math::add( result.scalars, scalars, scalar, numRows*numColumns );
				
				return result;
			}
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_INLINE Matrix operator - ( const Matrix& matrix ) const
			{
				// Make sure the matrix dimensions match.
				if ( numRows != matrix.numRows || numColumns != matrix.numColumns )
					return Matrix();
				
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Subtract the elements and store the result.
				math::subtract( result.scalars, scalars, matrix.scalars, numRows*numColumns );
				
				return result;
			}
			
			
			/// Subtract a scalar from the elements of this matrix and return the resulting matrix.
			OM_INLINE Matrix operator - ( const T& scalar ) const
			{
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Subtract the elements and store the result.
				math::subtract( result.scalars, scalars, scalar, numRows*numColumns );
				
				return result;
			}
			
			
			/// Multiply a matrix by this matrix and return the result.
			OM_INLINE Matrix operator * ( const Matrix& matrix ) const
			{
				const Size numOtherRows = matrix.numRows;
				const Size numOtherColumns = matrix.numColumns;
				
				// Make sure the inner matrix dimensions match.
				if ( numColumns != numOtherRows )
					return Matrix();
				
				// Create storage for the result.
				Matrix result( numRows, numOtherColumns );
				T* resultElements = result.scalars;
				const T* matrix2Elements = matrix.scalars;
				
				// Transpose the matrix if the matrix is large because the computation is faster.
				if ( numOtherRows >= 16 )
				{
					Matrix transposed = (*this).transpose();
					const T* matrix1Elements = transposed.scalars;
					
					for ( Index i = 0; i < numRows; i++ )
					{
						const T* m1Column = matrix1Elements + i*numOtherRows;
						for ( Index j = 0; j < numOtherColumns; j++ )
						{
							const T* m2Column = matrix2Elements + j*numOtherRows;
							resultElements[i + j*numRows] = math::dot( m1Column, m2Column, numOtherRows );
						}
					}
				}
				else
				{
					const T* matrix1Elements = scalars;
					
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
				}
				
				return result;
			}
			
			
			/// Multiply the elements of this matrix by a scalar and return the resulting matrix.
			OM_INLINE Matrix operator * ( const T& scalar ) const
			{
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Multiply the elements and store the result.
				math::multiply( result.scalars, scalars, scalar, numRows*numColumns );
				
				return result;
			}
			
			
			/// Divide the elements of this matrix by a scalar and return the resulting matrix.
			OM_INLINE Matrix operator / ( const T& scalar ) const
			{
				// Create storage for the result.
				Matrix result( numRows, numColumns );
				
				// Multiply the elements and store the result.
				math::multiply( result.scalars, scalars, T(1)/scalar, numRows*numColumns );
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Matrix Operators
			
			
			/// Add the elements of another matrix to this matrix.
			OM_INLINE Matrix& operator += ( const Matrix& matrix )
			{
				// Make sure the matrix dimensions match.
				if ( numRows == matrix.numRows && numColumns == matrix.numColumns )
					math::add( scalars, matrix.scalars, numRows*numColumns );
				
				return *this;
			}
			
			
			/// Add a scalar value to the elements of this matrix.
			OM_INLINE Matrix& operator += ( const T& scalar )
			{
				math::add( scalars, scalar, numRows*numColumns );
				
				return *this;
			}
			
			
			/// Subtract the elements of another matrix from this matrix.
			OM_INLINE Matrix& operator -= ( const Matrix& matrix )
			{
				// Make sure the matrix dimensions match.
				if ( numRows == matrix.numRows && numColumns == matrix.numColumns )
					math::subtract( scalars, matrix.scalars, numRows*numColumns );
				
				return *this;
			}
			
			
			/// Subtract a scalar value from the elements of this matrix.
			OM_INLINE Matrix& operator -= ( const T& scalar )
			{
				math::subtract( scalars, scalar, numRows*numColumns );
				
				return *this;
			}
			
			
			/// Multiply the elements of this matrix by a scalar value.
			OM_INLINE Matrix& operator *= ( const T& scalar )
			{
				math::multiply( scalars, scalar, numRows*numColumns );
				
				return *this;
			}
			
			
			/// Divide the elements of this matrix by a scalar value.
			OM_INLINE Matrix& operator /= ( const T& scalar )
			{
				math::multiply( scalars, T(1)/scalar, numRows*numColumns );
				
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
							buffer << scalars[i + j*numRows] << ", ";
						else
							buffer << scalars[i + j*numRows] << ' ';
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
		//******	Static Matrix Creation Methods
			
			
			/// Get the square identity matrix for the specified matrix size in the output parameter.
			static void getIdentity( Size size, Matrix& matrix )
			{
				matrix.setSize( size, size );
				
				// Zero the matrix.
				matrix.zero();
				
				// Set the diagonal elements to one.
				for ( Index i = 0; i < size; i++ )
					matrix.scalars[i*size + i] = T(1);
			}
			
			
		//********************************************************************************
		//******	Matrix Operation Classes
			
			
			/// A class that uses the least squares algorithm to find a solution overdetermined equations of the form A*x=b.
			class LeastSquares;
			
			
			/// A class that computes the QR factorization of an input matrix.
			class QR;
			
			
	private:
		
		//********************************************************************************
		//******	Private Friend Declarations
			
			
			template < typename U >
			friend class Matrix;
			
			
		//********************************************************************************
		//******	Private Storage Helper Methods
			
			
			/// Allocate a storage object with the specified number of rows and columns.
			static T* allocateScalars( Size numRows, Size numColumns )
			{
				return util::allocateAligned<T>( numRows*numColumns, 16 );
			}
			
			
			/// Deallocate the specified storage object.
			static void deallocateScalars( T* storage )
			{
				util::deallocateAligned( storage );
			}
			
			
		//********************************************************************************
		//******	Private Linear Algebra Methods
			
			
			/// Perform a householder rank reduction on an input matrix.
			static void householderReduction( Index n, Index p, T* A, T* columnNorms, T* workspace, Index* pivots );
			
			
			/// Compute the least squares solution to a system of equations using the output of a householder reduction.
			static Index householderLeastSquares( Index n, Index p, T* A, T* columnNorms, T* b, T* x );
			
			
			/// Compute the Q matrix of a QR factorization using the output of householder reduction.
			static void householderQ( Size n, Size p, T* A, T* columnNorms, Index* pivot, T* Q );
			
			
			/// Compute the R matrix of a QR factorization using the output of householder reduction.
			static void householderR( Size n, Size p, T* R );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of rows that are in this matrix.
			Size numRows;
			
			
			/// The number of columns that are in this matrix.
			Size numColumns;
			
			
			/// A pointer to the storage for this matrix's elements.
			T* scalars;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Reverse Matrix Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar to a matrix's elements and return the resulting matrix
template < typename T >
OM_FORCE_INLINE Matrix<T> operator + ( const T& c, const Matrix<T>& matrix ) 
{
	return matrix + c;
}




/// Subtract a matrix's elements from a scalar and return the resulting matrix
template < typename T >
OM_FORCE_INLINE Matrix<T> operator - ( const T& c, const Matrix<T>& matrix ) 
{
	return -matrix + c;
}




/// Multiply a matrix's elements by a scalar and return the resulting matrix
template < typename T >
OM_FORCE_INLINE Matrix<T> operator * ( const T& c, const Matrix<T>& matrix ) 
{
	return matrix * c;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Least-Squares Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
class Matrix<T>:: LeastSquares
{
	public:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// Create a new default least squares object.
			OM_INLINE LeastSquares()
				:	rank( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Solve Method
			
			
			/// Use a least-squares algorithm to find the best solution for a rectangular linear system A*x = b.
			OM_NO_INLINE Bool solve( const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x )
			{
				const Size numRows = A.getRowCount();
				const Size numColumns = A.getColumnCount();
				
				// Make sure the input is well-formed.
				if ( A.isNull() || b.isNull() || numRows != b.getRowCount() || b.getColumnCount() != Size(1) )
					return false;
				
				// If the system is undetermined, compute A' = Q*R instead of A = Q*R.
				if ( numRows < numColumns )
				{
					A.transpose( matrix );
					rhs.setSize( numRows, 1 );
					pivots.setSize( numRows, 1 );
				}
				else
				{
					matrix = A;
					rhs = b;
					pivots.setSize( numColumns, 1 );
				}
				
				// Initialize the temporary storage.
				columnNorms.setSize( numColumns, 1 );
				workspace.setSize( numColumns, 1 );
				x.setSize( numColumns, 1 );
				
				// Perform a householder rank reduction on the matrix.
				householderReduction( matrix.getRowCount(), matrix.getColumnCount(), matrix.getScalars(),
										columnNorms.getScalars(), workspace.getScalars(), pivots.getScalars() );
				
				// Reorder the right hand side based on the pivots if the system is underdetermined.
				if ( numRows < numColumns )
				{
					// Reorder the solution elements based on the pivots.
					for ( Index i = 0; i < numRows; i++ )
					{
						Index pivot = pivots(i,0);
						rhs(i,0) = b(pivot,0);
					}
				}
				
				// Solve the system.
				householderLeastSquares( numRows, numColumns, matrix.getScalars(),
										columnNorms.getScalars(), rhs.getScalars(), x.getScalars() );
				
				// Reorder the solution elements based on the pivots.
				if ( numRows >= numColumns )
				{
					for ( Index i = 0; i < numColumns; i++ )
					{
						while ( i != pivots(i,0) )
						{
							Index pivot = pivots(i,0);
							util::swap( x(i,0), x(pivot,0) );
							util::swap( pivots(i,0), pivots(pivot,0) );
						}
					}
				}
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Rank Accessor Method
			
			
			/// Return the rank of the last input matrix that was solved.
			OM_INLINE Size getRank() const
			{
				return rank;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A copy of the input matrix that is modified in-place to computed the solution.
			Matrix<T> matrix;
			
			
			/// An array of column norms that are used in the householder reduction.
			Matrix<T> columnNorms;
			
			
			/// A secondary workspace.
			Matrix<T> workspace;
			
			
			/// A secondary workspace.
			Matrix<T> rhs;
			
			
			/// An array of column pivot indices.
			Matrix<Index> pivots;
			
			
			/// The rank of the last input matrix.
			Size rank;
			
};




/// Use a least-squares algorithm to find the best fitting solution for a linear system A*x = b.
template < typename T >
OM_INLINE Bool leastSquares( const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x )
{
	typename Matrix<T>::LeastSquares algorithm;
	return algorithm.solve( A, b, x );
}




/// Solve a linear system of the form A*x = b, returning whether or not it was able to be solved.
/**
  * If there is no direct solution (e.g. the input matrix A is not invertible),
  * then the least squares solution is computed instead and the method returns FALSE.
  * Otherwise the exact solution is computed and TRUE is returned.
  */
template < typename T >
OM_INLINE Bool solve( const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x )
{
	// Solve the system using the least squares algorithm.
	typename Matrix<T>::LeastSquares algorithm;
	algorithm.solve( A, b, x );
	
	// Return whether or not the solution is exact.
	return algorithm.getRank() != A.getRowCount();
}




//##########################################################################################
//##########################################################################################
//############		
//############		QR Factorization Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
class Matrix<T>:: QR
{
	public:
		
		//********************************************************************************
		//******	Solve Methods
			
			
			/// Factorize a matrix A into the product A = Q*R, where Q is orthornormal and R is upper-triangular.
			/**
			  * The method computes the QR factorization without column pivoting.
			  */
			OM_NO_INLINE Bool solve( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R )
			{
				const Size numRows = A.getRowCount();
				const Size numColumns = A.getColumnCount();
				
				// Make sure the input is well-formed.
				if ( A.isNull() )
					return false;
				
				// Initialize the temporary storage.
				R = A;
				Q.setSize( numRows, numRows );
				columnNorms.setSize( numColumns, 1 );
				workspace.setSize( numColumns, 1 );
				
				// Perform a householder rank reduction on the matrix.
				householderReduction( numRows, numColumns, R.getScalars(),
										columnNorms.getScalars(), workspace.getScalars(), NULL );
				
				// Compute the Q and R matrices.
				householderQ( numRows, numColumns, R.getScalars(), columnNorms.getScalars(), NULL, Q.getScalars() );
				householderR( numRows, numColumns, R.getScalars() );
				
				return true;
			}
			
			
			/// Factorize a matrix A into the product A = Q*R, where Q is orthornormal and R is upper-triangular.
			/**
			  * The method uses column pivoting to compute the QR factorization with improved robustness
			  * and numerical stability. The permutation of the columns of matrix A is returned,
			  * such that permutation(A) = Q*R.
			  */
			OM_NO_INLINE Bool solve( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R, Matrix<Index>& permutation )
			{
				const Size numRows = A.getRowCount();
				const Size numColumns = A.getColumnCount();
				
				// Make sure the input is well-formed.
				if ( A.isNull() )
					return false;
				
				// Initialize the temporary storage.
				R = A;
				Q.setSize( numRows, numRows );
				columnNorms.setSize( numColumns, 1 );
				workspace.setSize( numColumns, 1 );
				permutation.setSize( numColumns, 1 );
				
				// Perform a householder rank reduction on the matrix.
				householderReduction( numRows, numColumns, R.getScalars(),
										columnNorms.getScalars(), workspace.getScalars(), permutation.getScalars() );
				
				// Compute the Q and R matrices.
				householderQ( numRows, numColumns, R.getScalars(), columnNorms.getScalars(), permutation.getScalars(), Q.getScalars() );
				householderR( numRows, numColumns, R.getScalars() );
				
				return true;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of column norms that are used in the householder reduction.
			Matrix<T> columnNorms;
			
			
			/// A secondary workspace.
			Matrix<T> workspace;
			
};




/// Factorize a matrix A into the product A = Q*R, where Q is orthornormal and R is upper-triangular.
/**
  * The method computes the QR factorization without column pivoting.
  */
template < typename T >
OM_INLINE void qr( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R )
{
	typename Matrix<T>::QR algorithm;
	algorithm.solve( A, Q, R );
}




/// Factorize a matrix A into the product A = Q*R, where Q is orthornormal and R is upper-triangular.
/**
  * The method uses column pivoting to compute the QR factorization with improved robustness
  * and numerical stability. The permutation of the columns of matrix A is returned,
  * such that permutation(A) = Q*R.
  */
template < typename T >
OM_INLINE void qr( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R, Matrix<Index>& permutation )
{
	typename Matrix<T>::QR algorithm;
	algorithm.solve( A, Q, R, permutation );
}




//##########################################################################################
//##########################################################################################
//############		
//############		LU Factorization Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
OM_NO_INLINE void lu( const Matrix<T>& matrix, Matrix<T>& L, Matrix<T>& U )
{
	
}




//##########################################################################################
//##########################################################################################
//############		
//############		Householder Reduction Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
void Matrix<T>:: householderReduction( Index n, Index p, T* A, T* columnNorms, T* workspace, Index* pivots )
{
	if ( pivots )
	{
		// Initialize the column pivots.
		for ( Index j = 0; j < p; j++ )
			pivots[j] = j;
		
		// Compute the 2-norm of each column.
		for ( Index j = 0; j < p; j++ )
			workspace[j] = columnNorms[j] = math::norm( A + j*n, n );
	}
	
	const Index minSize = math::min( n, p );
	const Index lastN = n - 1;
	
	// Perform the Householder reduction of A.
	for ( Index l = 0; l < minSize; l++ )
	{
		T* const column = A + l*n;
		
		if ( pivots )
		{
			// Bring the column of largest norm into the pivot position.
			T maxnrm = T(0);
			Index maxj = l;
			
			for ( Index j = l; j < p; j++ )
			{
				if ( columnNorms[j] > maxnrm )
				{
					maxnrm = columnNorms[j];
					maxj = j;
				}
			}
			
			// Swap the current column with the one with the largest norm.
			if ( maxj != l )
			{
				util::swap( A + maxj*n, column, n );
				util::swap( pivots[maxj], pivots[l] );
				columnNorms[maxj] = columnNorms[l];
				workspace[maxj] = workspace[l];
			}
		}
		
		columnNorms[l] = T(0);
		
		// Compute the Householder transformation for column L.
		if ( l != lastN )
		{
			T* const subcolumn = column + l;
			const Size subcolumnSize = n - l;
			T nrmxl = math::norm( subcolumn, subcolumnSize );

			if ( nrmxl != T(0) )
			{
				if ( *subcolumn != T(0) )
					nrmxl = nrmxl * math::sign( *subcolumn );
				
				math::multiply( subcolumn, T(1) / nrmxl, subcolumnSize );
				*subcolumn = T(1) + *subcolumn;
				
				// Apply the transformation to the remaining columns, updating the norms.
				for ( Index j = l + 1; j < p; j++ )
				{
					T* const subcolumn2 = A + j*n + l;
					T t = -math::dot( subcolumn, subcolumn2, subcolumnSize ) / *subcolumn;
					math::multiplyAdd( subcolumn2, subcolumn, t, subcolumnSize );
					
					if ( pivots )
					{
						if ( columnNorms[j] != T(0) )
						{
							T tt = T(1) - math::square( math::abs( *subcolumn2 ) / columnNorms[j] );
							tt = math::max( tt, T(0) );
							t = tt;
							tt = T(1) + T(0.05) * tt * math::square( columnNorms[j] / workspace[j] );
							
							if ( tt != T(1) )
								columnNorms[j] = columnNorms[j] * math::sqrt( t );
							else
							{
								columnNorms[j] = math::norm( subcolumn2, subcolumnSize );
								workspace[j] = columnNorms[j];
							}
						}
					}
				}
				
				//  Save the transformation.
				columnNorms[l] = *subcolumn;
				*subcolumn = -nrmxl;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Householder Least Squares Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Index Matrix<T>:: householderLeastSquares( Index n, Index p, T* A, T* columnNorms, T* b, T* x )
{
	Index rank = math::min( n, p );
	Index ju = math::min( n, p );
	const T tolerance = math::epsilon<T>();
	
	if ( n >= p )
	{
		// Compute Q'*b.
		for ( Index j = 0; j < ju; j++ )
		{
			if ( columnNorms[j] != T(0) )
			{
				T temp = A[j*n + j];
				A[j*n + j] = columnNorms[j];
				T t = -math::dot( A + j*n + j, b + j, n-j ) / A[j*n + j];
				math::multiplyAdd( b + j, A + j*n + j, t, n-j );
				A[j+j*n] = temp;
			}
		}
		
		// Compute x.
		for ( Index j = 0; j < p; j++ )
			x[j] = b[j];
		
		for ( Index jj = 0; jj < p; jj++ )
		{
			Index j = p - jj - 1;
			
			// Test to see if the matrix is not full rank.
			if ( math::abs( A[j + j*n] ) <= tolerance )
			{
				rank--;
				continue;
			}
			
			x[j] = x[j] / A[j+j*n];
			
			if ( j != 0 )
				math::multiplyAdd( x, A + j*n, -x[j], j );
		}
	}
	else
	{
		// Compute inverse(R')*b
		for ( Index j = 0; j < n; j++ )
		{
			// Test to see if the matrix is not full rank.
			if ( math::abs( A[j + j*p] ) <= tolerance )
			{
				rank--;
				continue;
			}
			
			for ( Index i = 0; i < j; i++ )
				b[j] -= b[i]*A[i+j*p];
			
			b[j] = b[j] / A[j+j*p];
		}
		
		// Compute x = Q*inverse(R')*b
		for ( Index j = 0; j < n; j++ )
			x[j] = b[j];
		
		for ( Index j = n; j < p; j++ )
			x[j] = T(0);
		
		for ( Index jj = 0; jj < ju; jj++ )
		{
			Index j = ju - jj - 1;
			
			if ( columnNorms[j] != T(0) )
			{
				T temp = A[j+j*p];
				A[j+j*p] = columnNorms[j];
				T t = -math::dot( A+j+j*p, x+j, p-j ) / A[j+j*p];
				math::multiplyAdd( x+j, A+j+j*p, t, p-j );
				A[j+j*p] = temp;
			}
		}
	}
	
	return rank;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Householder QR Methods
//############		
//##########################################################################################
//##########################################################################################





template < typename T >
void Matrix<T>:: householderQ( Size n, Size p, T* A, T* columnNorms, Index* pivots, T* Q )
{
	Index minSize = math::min( n, p );
	
	for ( Index c = 0; c < n; c++ )
	{
		Index pivotC = pivots ? pivots[c] : c;
		T* qy = Q + pivotC*n;
		util::zero( qy, n );
		qy[pivotC] = 1;
		
		for ( Index jj = 0; jj < minSize; jj++ )
		{
			Index j = minSize - jj - 1;
			
			if ( columnNorms[j] != T(0) )
			{
				T temp = A[j+j*n];
				A[j+j*n] = columnNorms[j];
				T t = -math::dot( A+j+j*n, qy+j, n-j ) / A[j+j*n];
				math::multiplyAdd( qy+j, A+j+j*n, t, n-j );
				A[j+j*n] = temp;
			}
		}
	}
}





template < typename T >
void Matrix<T>:: householderR( Size n, Size p, T* R )
{
	// Set the lower triangle of R to zero.
	for ( Index i = 0; i < p; i++ )
		util::zero( R + i*n + i + 1, p - i - 1 );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MATRIX_H
