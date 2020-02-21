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

#ifndef INCLUDE_OM_MATRIX_3D_H
#define INCLUDE_OM_MATRIX_3D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVector3D.h"
#include "omMatrixND.h"
#include "omMatrix2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a 3x3 matrix.
/**
  * Elements in the matrix are stored in column-major order.
  */
template < typename T >
class MatrixND<T,3,3>
{
	public:
	
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a 3x3 matrix with all elements equal to zero.
			OM_FORCE_INLINE MatrixND()
				:	x( 0, 0, 0 ),
					y( 0, 0, 0 ),
					z( 0, 0, 0 )
			{
			}
			
			
			/// Create a 3x3 matrix from three column vectors.
			OM_FORCE_INLINE MatrixND( const VectorND<T,3>& column1, 
										const VectorND<T,3>& column2, 
										const VectorND<T,3>& column3 )
				:	x( column1 ),
					y( column2 ),
					z( column3 )
			{
			}
			
			
			/// Create a 3x3 matrix with elements specified in row-major order.
			OM_FORCE_INLINE MatrixND( T a, T b, T c, 
										T d, T e, T f, 
										T g, T h, T i )
				:	x( a, d, g ),
					y( b, e, h ),
					z( c, f, i )
			{
			}
			
			
			/// Create a 3x3 matrix from a pointer to an array of elements in column-major order.
			OM_FORCE_INLINE explicit MatrixND( const T array[9] )
				:	x( array[0], array[1], array[2] ),
					y( array[3], array[4], array[5] ),
					z( array[6], array[7], array[8] )
			{
			}
			
			
			/// Create an identity matrx with the specified 2x2 matrix in the upper-left corner.
			template < typename U >
			OM_FORCE_INLINE explicit MatrixND( const MatrixND<U,2,2>& other )
				:	x( other.x, T(0) ),
					y( other.y, T(0) ),
					z( T(0), T(0), T(1) )
			{
			}
			
			
			/// Create a copy of the specified 3x3 matrix with different template parameter type.
			template < typename U >
			OM_FORCE_INLINE MatrixND( const MatrixND<U,3,3>& other )
				:	x( other.x ),
					y( other.y ),
					z( other.z )
			{
			}
			
			
		//********************************************************************************
		//******	Static Construction Methods
			
			
			/// Return a skew-symmetric matrix using the elements of the specified vector.
			OM_FORCE_INLINE static MatrixND skewSymmetric( const VectorND<T,3>& vector )
			{
				return MatrixND( 0, -vector.z, vector.y,
									vector.z, 0, -vector.x,
									-vector.y, vector.x, 0 );
			}
			
			
			/// Return an orthogonal matrix defining a basis for the coordinate frame of a plane with the specified normal.
			/**
			  * The normal becomes the z-column of the resulting matrix, and X and Y columns
			  * are generated robustly from the input normal vector. The normal vector must have
			  * non-zero length.
			  */
			OM_FORCE_INLINE static MatrixND planeBasis( const VectorND<T,3>& normal )
			{
				VectorND<T,3> binormal;
				VectorND<T,3> n = math::abs(normal);
				
				if ( n.x <= n.y )
				{
					if ( n.x <= n.z )
					{
						// X smallest.
						binormal.y = -normal.z;		binormal.z = normal.y;
					}
					else
					{
						// Z smallest.
						binormal.x = -normal.y;		binormal.z = normal.x;
					}
				}
				else if ( n.y <= n.z )
				{
					// Y smallest.
					binormal.x = -normal.z;		binormal.z = normal.x;
				}
				else
				{
					// Z smallest.
					binormal.x = -normal.y;		binormal.z = normal.x;
				}
				
				binormal = binormal.normalize();
				
				return MatrixND( math::cross( binormal, normal ), binormal, normal );
			}
			
			
		//********************************************************************************
		//******	Rotation Matrix Construction Methods
			
			
			/// Create a 3x3 rotation matrix about the X-axis with the angle in radians.
			OM_FORCE_INLINE static MatrixND rotateX( T xAngle )
			{
				T cosXRot = math::cos( xAngle );
				T sinXRot = math::sin( xAngle );
				return MatrixND(	1, 0, 0,
									0, cosXRot, -sinXRot,
									0, sinXRot, cosXRot );
			}
			
			
			/// Create a 3x3 rotation matrix about the Y-axis with the angle in radians.
			OM_FORCE_INLINE static MatrixND rotateY( T yAngle )
			{
				T cosYRot = math::cos( yAngle );
				T sinYRot = math::sin( yAngle );
				return MatrixND(	cosYRot, 0, sinYRot,
									0, 1, 0,
									-sinYRot, 0, cosYRot );
			}
			
			
			/// Create a 3x3 rotation matrix about the Z-axis with the angle in radians.
			OM_FORCE_INLINE static MatrixND rotateZ( T zAngle )
			{
				T cosZRot = math::cos( zAngle );
				T sinZRot = math::sin( zAngle );
				return MatrixND(	cosZRot, -sinZRot, 0,
									sinZRot, cosZRot, 0, 
									0, 0, 1 );
			}
			
			
			/// Create a 3x3 rotation matrix about the X-axis with the angle in degrees.
			OM_FORCE_INLINE static MatrixND rotateXDegrees( T xAngle )
			{
				return rotateX( math::degreesToRadians(xAngle) );
			}
			
			
			/// Create a 3x3 rotation matrix about the Y-axis with the angle in degrees.
			OM_FORCE_INLINE static MatrixND rotateYDegrees( T yAngle )
			{
				return rotateY( math::degreesToRadians(yAngle) );
			}
			
			
			/// Create a 3x3 rotation matrix about the Z-axis with the angle in degrees.
			OM_FORCE_INLINE static MatrixND rotateZDegrees( T zAngle )
			{
				return rotateZ( math::degreesToRadians(zAngle) );
			}
			
			
		//********************************************************************************
		//******	Euler Rotation Matrix Construction Methods
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the XYZ order.
			OM_FORCE_INLINE static MatrixND rotateXYZ( T xAngle, T yAngle, T zAngle )
			{
				return rotateX( xAngle )*rotateY( yAngle )*rotateZ( zAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the XYZ order.
			OM_FORCE_INLINE static MatrixND rotateXYZ( const VectorND<T,3>& angle )
			{
				return rotateXYZ( angle.x, angle.y, angle.z );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the XZY order.
			OM_FORCE_INLINE static MatrixND rotateXZY( T xAngle, T yAngle, T zAngle )
			{
				return rotateX( xAngle )*rotateZ( zAngle )*rotateY( yAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the XZY order.
			OM_FORCE_INLINE static MatrixND rotateXZY( const VectorND<T,3>& angle )
			{
				return rotateXZY( angle.x, angle.y, angle.z );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the YXZ order.
			OM_FORCE_INLINE static MatrixND rotateYXZ( T xAngle, T yAngle, T zAngle )
			{
				return rotateY( yAngle )*rotateX( xAngle )*rotateZ( zAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the YXZ order.
			OM_FORCE_INLINE static MatrixND rotateYXZ( const VectorND<T,3>& angle )
			{
				return rotateYXZ( angle.x, angle.y, angle.z );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the YZX order.
			OM_FORCE_INLINE static MatrixND rotateYZX( T xAngle, T yAngle, T zAngle )
			{
				return rotateY( yAngle )*rotateZ( zAngle )*rotateX( xAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the YZX order.
			OM_FORCE_INLINE static MatrixND rotateYZX( const VectorND<T,3>& angle )
			{
				return rotateYZX( angle.x, angle.y, angle.z );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the ZXY order.
			OM_FORCE_INLINE static MatrixND rotateZXY( T xAngle, T yAngle, T zAngle )
			{
				return rotateZ( zAngle )*rotateX( xAngle )*rotateY( yAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the ZXY order.
			OM_FORCE_INLINE static MatrixND rotateZXY( const VectorND<T,3>& angle )
			{
				return rotateZXY( angle.x, angle.y, angle.z );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the ZYX order.
			OM_FORCE_INLINE static MatrixND rotateZYX( T xAngle, T yAngle, T zAngle )
			{
				return rotateZ( zAngle )*rotateY( yAngle )*rotateX( xAngle );
			}
			
			
			/// Create a 3x3 rotation matrix from Euler angles in the ZYX order.
			OM_FORCE_INLINE static MatrixND rotateZYX( const VectorND<T,3>& angle )
			{
				return rotateZYX( angle.x, angle.y, angle.z );
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
			  * The output array must be at least 9 elements long.
			  */
			OM_FORCE_INLINE void toArrayRowMajor( T* outputArray ) const
			{
				outputArray[0] = x.x;	outputArray[1] = y.x;	outputArray[2] = z.x;
				outputArray[3] = x.y;	outputArray[4] = y.y;	outputArray[5] = z.y;
				outputArray[6] = x.z;	outputArray[7] = y.z;	outputArray[8] = z.z;
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,3>& getColumn( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,3>& getColumn( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,3>& operator () ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,3>& operator () ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,3>& operator [] ( Index columnIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the column at the specified index in the matrix.
			OM_FORCE_INLINE const VectorND<T,3>& operator [] ( Index columnIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				return (&x)[columnIndex];
			}
			
			
			/// Get the row at the specified index in the matrix.
			OM_FORCE_INLINE VectorND<T,3> getRow( Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( rowIndex < 3 );
				
				switch ( rowIndex )
				{
					case 0: return VectorND<T,3>( x.x, y.x, z.x );
					case 1: return VectorND<T,3>( x.y, y.y, z.y );
					case 2: return VectorND<T,3>( x.z, y.z, z.z );
					default: return VectorND<T,3>::ZERO;
				}
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE T& get( Index columnIndex, Index rowIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 && rowIndex < 3 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE const T& get( Index columnIndex, Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 3 && rowIndex < 3 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE T& operator () ( Index columnIndex, Index rowIndex )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 && rowIndex < 3 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Get the element at the specified (column, row) index in the matrix.
			OM_FORCE_INLINE const T& operator () ( Index columnIndex, Index rowIndex ) const
			{
				OM_DEBUG_ASSERT( columnIndex < 3 && rowIndex < 3 );
				
				return (&x)[columnIndex][rowIndex];
			}
			
			
			/// Set the element in the matrix at the specified (row, column) index.
			OM_FORCE_INLINE void set( Index rowIndex, Index columnIndex, T value )
			{
				OM_DEBUG_ASSERT( rowIndex < 3 && columnIndex < 3 );
				
				return (&x)[columnIndex][rowIndex] = value;
			}
			
			
			/// Set the column in the matrix at the specified index.
			OM_FORCE_INLINE void setColumn( Index columnIndex, const VectorND<T,3>& newColumn )
			{
				OM_DEBUG_ASSERT( columnIndex < 3 );
				
				(&x)[columnIndex] = newColumn;
			}
			
			
			/// Set the row in the matrix at the specified index.
			OM_FORCE_INLINE void setRow( Index rowIndex, const VectorND<T,3>& newRow )
			{
				OM_DEBUG_ASSERT( rowIndex < 3 );
				
				switch ( rowIndex )
				{
					case 0: x.x = newRow.x;	y.x = newRow.y;	z.x = newRow.z; return;
					case 1: x.y = newRow.x;	y.y = newRow.y;	z.y = newRow.z; return;
					case 2: x.z = newRow.x;	y.z = newRow.y;	z.z = newRow.z; return;
				}
			}
			
			
			/// Return the diagonal vector of this matrix.
			OM_FORCE_INLINE VectorND<T,3> getDiagonal() const
			{
				return VectorND<T,3>( x.x, y.y, z.z );
			}
			
			
			/// Return the upper-left 2x2 submatrix of this matrix.
			OM_FORCE_INLINE MatrixND<T,2,2> getXY() const
			{
				return MatrixND<T,2,2>( x.getXY(), y.getXY() );
			}
			
			
		//********************************************************************************
		//******	Matrix Operation Methods
			
			
			/// Return the determinant of this matrix.
			OM_FORCE_INLINE T getDeterminant() const
			{
				return x.x*( y.y*z.z - z.y*y.z ) - 
					   y.x*( x.y*z.z - z.y*x.z ) + 
					   z.x*( x.y*y.z - y.y*x.z );
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
				
				return MatrixND(	(y.y*z.z - z.y*y.z)*detInv, 
									(z.x*y.z - y.x*z.z)*detInv,
									(y.x*z.y - z.x*y.y)*detInv,
									(z.y*x.z - x.y*z.z)*detInv,
									(x.x*z.z - z.x*x.z)*detInv,
									(z.x*x.y - x.x*z.y)*detInv,
									(x.y*y.z - y.y*x.z)*detInv,
									(y.x*x.z - x.x*y.z)*detInv,
									(x.x*y.y - y.x*x.y)*detInv );
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
				
				inverse = MatrixND(	(y.y*z.z - z.y*y.z)*detInv, 
									(z.x*y.z - y.x*z.z)*detInv,
									(y.x*z.y - z.x*y.y)*detInv,
									(z.y*x.z - x.y*z.z)*detInv,
									(x.x*z.z - z.x*x.z)*detInv,
									(z.x*x.y - x.x*z.y)*detInv,
									(x.y*y.z - y.y*x.z)*detInv,
									(y.x*x.z - x.x*y.z)*detInv,
									(x.x*y.y - y.x*x.y)*detInv );
				
				return true;
			}
			
			
			/// Return the orthonormalization of this matrix.
			/**
			  * This matrix that is returned has all column vectors of unit
			  * length and perpendicular to each other.
			  */
			OM_FORCE_INLINE MatrixND orthonormalize() const
			{
				VectorND<T,3> newX = x.normalize();
				VectorND<T,3> newZ = cross( newX, y ).normalize();
				
				return MatrixND( newX, cross( newZ, newX ).normalize(), newZ );
			}
			
			
			/// Return the transposition of this matrix.
			OM_FORCE_INLINE MatrixND transpose() const
			{
				return MatrixND( x.x, x.y, x.z,
								y.x, y.y, y.z,
								z.x, z.y, z.z );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two matrices component-wise for equality.
			OM_FORCE_INLINE Bool operator == ( const MatrixND& m ) const
			{
				return x == m.x && y == m.y && z == m.z;
			}
			
			
			/// Compare two matrices component-wise for inequality.
			OM_FORCE_INLINE Bool operator != ( const MatrixND& m ) const
			{
				return x != m.x || y != m.y || z != m.z;
			}
			
			
		//********************************************************************************
		//******	Matrix Negation/Positivation Operators
			
			
			/// Negate every element of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - () const
			{
				return MatrixND( -x, -y, -z );
			}
			
			
			/// 'Positivate' every element of this matrix, returning a copy of the original matrix.
			OM_FORCE_INLINE MatrixND operator + () const
			{
				return MatrixND( x, y, z );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this matrix to another and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const MatrixND& matrix ) const
			{
				return MatrixND( x + matrix.x, y + matrix.y, z + matrix.z );
			}
			
			
			/// Add a scalar to the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator + ( const T& value ) const
			{
				return MatrixND( x + value, y + value, z + value );
			}
			
			
			/// Subtract a matrix from this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const MatrixND& matrix ) const
			{
				return MatrixND( x - matrix.x, y - matrix.y, z - matrix.z );
			}
			
			
			/// Subtract a scalar from the elements of this matrix and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator - ( const T& value ) const
			{
				return MatrixND( x - value, y - value, z - value );
			}
			
			
			/// Multiply a matrix by this matrix and return the result.
			OM_FORCE_INLINE MatrixND operator * ( const MatrixND& matrix ) const
			{
				return MatrixND(	x.x*matrix.x.x + y.x*matrix.x.y + z.x*matrix.x.z,
									x.x*matrix.y.x + y.x*matrix.y.y + z.x*matrix.y.z,
									x.x*matrix.z.x + y.x*matrix.z.y + z.x*matrix.z.z,
									x.y*matrix.x.x + y.y*matrix.x.y + z.y*matrix.x.z,
									x.y*matrix.y.x + y.y*matrix.y.y + z.y*matrix.y.z,
									x.y*matrix.z.x + y.y*matrix.z.y + z.y*matrix.z.z,
									x.z*matrix.x.x + y.z*matrix.x.y + z.z*matrix.x.z,
									x.z*matrix.y.x + y.z*matrix.y.y + z.z*matrix.y.z,
									x.z*matrix.z.x + y.z*matrix.z.y + z.z*matrix.z.z );
			}

			
			/// Multiply a vector/point by this matrix and return the result.
			OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& vector ) const
			{
				return VectorND<T,3>(	x.x*vector.x + y.x*vector.y + z.x*vector.z,
									x.y*vector.x + y.y*vector.y + z.y*vector.z,
									x.z*vector.x + y.z*vector.y + z.z*vector.z );
			}
			
			
			/// Multiply the elements of this matrix by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator * ( const T& value ) const
			{
				return MatrixND( x*value, y*value, z*value );
			}
			
			
			/// Divide the elements of this matrix by a scalar and return the resulting matrix.
			OM_FORCE_INLINE MatrixND operator / ( const T& value ) const
			{
				return MatrixND( x/value, y/value, z/value );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add the elements of another matrix to this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const MatrixND& matrix )
			{
				x += matrix.x;
				y += matrix.y;
				z += matrix.z;
				
				return *this;
			}
			
			
			/// Add a scalar value to the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator += ( const T& value )
			{
				x += value;
				y += value;
				z += value;
				
				return *this;
			}
			
			
			/// Subtract the elements of another matrix from this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const MatrixND& matrix )
			{
				x -= matrix.x;
				y -= matrix.y;
				z -= matrix.z;
				
				return *this;
			}
			
			
			/// Subtract a scalar value from the elements of this matrix.
			OM_FORCE_INLINE MatrixND& operator -= ( const T& value )
			{
				x -= value;
				y -= value;
				z -= value;
				
				return *this;
			}
			
			
			/// Multiply the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator *= ( const T& value )
			{
				x *= value;
				y *= value;
				z *= value;
				
				return *this;
			}
			
			
			/// Divide the elements of this matrix by a scalar value.
			OM_FORCE_INLINE MatrixND& operator /= ( const T& value )
			{
				x /= value;
				y /= value;
				z /= value;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 3x3 matrix into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "[ " << x.x << ", " << y.x << ", " << z.x << " ]\n";
				buffer << "[ " << x.y << ", " << y.y << ", " << z.y << " ]\n";
				buffer << "[ " << x.z << ", " << y.z << ", " << z.z << " ]";
				
				return buffer.toString();
			}
			
			
			/// Convert this 3x3 matrix into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The first column vector of the matrix.
			VectorND<T,3> x;
			
			
			/// The second column vector of the matrix.
			VectorND<T,3> y;
			
			
			/// The third column vector of the matrix.
			VectorND<T,3> z;
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// Constant matrix with all elements equal to zero.
			static const MatrixND ZERO;
			
			
			/// Constant matrix with diagonal elements equal to one and all others equal to zero.
			static const MatrixND IDENTITY;
			
			
};




template < typename T >
const MatrixND<T,3,3> MatrixND<T,3,3>:: ZERO( T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) );




template < typename T >
const MatrixND<T,3,3> MatrixND<T,3,3>:: IDENTITY( T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) );




//##########################################################################################
//##########################################################################################
//############		
//############		Reverse Matrix Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a sclar value to a matrix's elements and return the resulting matrix
template < typename T >
OM_FORCE_INLINE MatrixND<T,3,3> operator + ( const T& value, const MatrixND<T,3,3>& matrix )
{
	return MatrixND<T,3,3>( matrix.x + value, matrix.y + value, matrix.z + value );
}





/// 'Reverse' multiply a vector/point by matrix: multiply it by the matrix's transpose.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& vector, const MatrixND<T,3,3>& matrix )
{	
	return VectorND<T,3>(  matrix.x.x * vector.x + matrix.x.y * vector.y + matrix.x.z * vector.z,
						matrix.y.x * vector.x + matrix.y.y * vector.y + matrix.y.z * vector.z,
						matrix.z.x * vector.x + matrix.z.y * vector.y + matrix.z.z * vector.z );
}




/// Multiply a matrix's elements by a scalar and return the resulting matrix
template < typename T >
OM_FORCE_INLINE MatrixND<T,3,3> operator * ( const T& value, const MatrixND<T,3,3>& matrix )
{
	return MatrixND<T,3,3>( matrix.x*value, matrix.y*value, matrix.z*value );
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
OM_FORCE_INLINE MatrixND<T,3,3> abs( const MatrixND<T,3,3>& matrix )
{
	return MatrixND<T,3,3>(	math::abs(matrix.x.x), math::abs(matrix.y.x), math::abs(matrix.z.x),
						math::abs(matrix.x.y), math::abs(matrix.y.y), math::abs(matrix.z.y),
						math::abs(matrix.x.z), math::abs(matrix.y.z), math::abs(matrix.z.z) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Euler Angle Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in XYZ order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerXYZ( const MatrixND<T,3,3>& m )
{
	T rY = math::asin( m.z.x );
	
	if ( rY < T(0.5)*math::pi<T>() )
	{
		if ( rY > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( math::atan2( -m.z.y, m.z.z ), rY, math::atan2( -m.y.x, m.x.x ) );
		else
			return VectorND<T,3>( -math::atan2( m.x.y, m.y.y ), rY, T(0) );
	}
	else
		return VectorND<T,3>( math::atan2( m.x.y, m.y.y ), rY, T(0) );
}




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in XZY order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerXZY( const MatrixND<T,3,3>& m )
{
	T rZ = math::asin( -m.y.x );
	
	if ( rZ < T(0.5)*math::pi<T>() )
	{
		if ( rZ > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( math::atan2( m.y.z, m.y.y ), math::atan2( m.z.x, m.x.x ), rZ );
		else
			return VectorND<T,3>( -math::atan2( -m.x.z, m.z.z ), T(0), rZ );
	}
	else
		return VectorND<T,3>( math::atan2( -m.x.z, m.z.z ), T(0), rZ );
}




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in YXZ order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerYXZ( const MatrixND<T,3,3>& m )
{
	T rX = math::asin( -m.z.y );
	
	if ( rX < T(0.5)*math::pi<T>() )
	{
		if ( rX > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( rX, math::atan2( m.z.x, m.z.z ), math::atan2( m.x.y, m.y.y ) );
		else
			return VectorND<T,3>( rX, -math::atan2( -m.y.x, m.x.x ), T(0) );
	}
	else
		return VectorND<T,3>( rX, math::atan2( -m.y.x, m.x.x ), T(0) );
}




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in YZX order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerYZX( const MatrixND<T,3,3>& m )
{
	T rZ = math::asin( m.x.y );
	
	if ( rZ < T(0.5)*math::pi<T>() )
	{
		if ( rZ > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( math::atan2( -m.z.y, m.y.y ), math::atan2( -m.x.z, m.x.x ), rZ );
		else
			return VectorND<T,3>( T(0), -math::atan2( m.y.z, m.z.z ), rZ );
	}
	else
		return VectorND<T,3>( T(0), math::atan2( m.y.z, m.z.z ), rZ );
}




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in ZXY order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerZXY( const MatrixND<T,3,3>& m )
{
	T rX = math::asin( m.y.z );
	
	if ( rX < T(0.5)*math::pi<T>() )
	{
		if ( rX > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( rX, math::atan2( -m.x.z, m.z.z ), math::atan2( -m.y.x, m.y.y ) );
		else
			return VectorND<T,3>( rX, T(0), -math::atan2( m.z.x, m.x.x ) );
	}
	else
		return VectorND<T,3>( rX, T(0), math::atan2( m.z.x, m.x.x ) );
}




/// Return the euler angles for an orthonormal rotation matrix when the euler angles are composed in ZYX order.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> eulerZYX( const MatrixND<T,3,3>& m )
{
	T rY = math::asin( -m.x.z );
	
	if ( rY < T(0.5)*math::pi<T>() )
	{
		if ( rY > -T(0.5)*math::pi<T>() )
			return VectorND<T,3>( math::atan2( m.y.z, m.z.z ), rY, math::atan2( m.x.y, m.x.x ) );
		else
			return VectorND<T,3>( T(0), rY, -math::atan2( -m.y.x, m.z.x ) );
	}
	else
		return VectorND<T,3>( T(0), rY, math::atan2( -m.y.x, m.z.x ) );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MATRIX_3D_H
