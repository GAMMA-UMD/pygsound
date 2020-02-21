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

#ifndef INCLUDE_OM_QUATERNION_4D_H
#define INCLUDE_OM_QUATERNION_4D_H


#include "omMathConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A templatized math class representing a 4-component quaternion.
template < typename T >
class Quaternion4D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new identity quaternion corresponding to no rotation.
			OM_FORCE_INLINE Quaternion4D()
				:	w( T(1) ),
					x( T(0) ),
					y( T(0) ),
					z( T(0) )
			{
			}
			
			
			/// Create a new quaternion by specifying it's 4 component values.
			OM_FORCE_INLINE Quaternion4D( T newW, T newX, T newY, T newZ )
				:	w( newW ),
					x( newX ),
					y( newY ),
					z( newZ )
			{
			}
			
			
			/// Create a new quaternion from a pointer to a 4 element array specifying its components.
			OM_FORCE_INLINE Quaternion4D( const T array[4] )
				:	w( array[0] ),
					x( array[1] ),
					y( array[2] ),
					z( array[3] )
			{
			}
			
			
			/// Create a new unit quaternion from a normalized axis and angle representation.
			OM_FORCE_INLINE Quaternion4D( T angle, const VectorND<T,3>& axis )
			{
				const T halfAngle = T(0.5)*angle;
				const T s = math::sin( halfAngle );
				w = math::cos( halfAngle );
				x = s*axis.x;
				y = s*axis.y;
				z = s*axis.z;
			}
			
			
			/// Create a new quaternion from a 3x3 orthonormal rotation matrix.
			OM_FORCE_INLINE Quaternion4D( const MatrixND<T,3,3>& m )
			{
				// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
				// article "Quaternion4D Calculus and Fast Animation".
				const T fTrace = m.x.x + m.y.y + m.z.z;
				
				if ( fTrace > T(0) )
				{
					// |w| > 1/2, may as well choose w > 1/2
					T fRoot = math::sqrt(fTrace + 1.0f);  // 2w
					w = T(0.5)*fRoot;
					fRoot = T(0.5)/fRoot;  // 1/(4w)
					
					x = (m.y.z - m.z.y)*fRoot;
					y = (m.z.x - m.x.z)*fRoot;
					z = (m.x.y - m.y.x)*fRoot;
				}
				else
				{
					// |w| <= 1/2
					Index nextIndex[3] = { 1, 2, 0 };
					Index i = 0;
					
					if ( m.y.y > m.x.x )
						i = 1;
					
					if ( m.z.z > m[i][i] )
						i = 2;
					
					Index j = nextIndex[i];
					Index k = nextIndex[j];
					
					T fRoot = math::sqrt( m[i][i] - m[j][j] - m[k][k] + T(1) );
					
					T* apkQuat[3] = { &x, &y, &z };
					*apkQuat[i] = T(0.5)*fRoot;
					
					fRoot = T(0.5)/fRoot;
					
					w = (m[j][k] - m[k][j])*fRoot;
					*apkQuat[j] = (m[i][j] + m[j][i])*fRoot;
					*apkQuat[k] = (m[i][k] + m[k][i])*fRoot;
				}
			}
			
			
			/// Create a new quaternion from an existing quaternion with different template type.
			/** 
			  * This constructor takes the x, y, z, and w values of the
			  * quaternion parameter and sets the coordinates of this quaternion
			  * to be the same. This is a templatized version of the copy constructor.
			  * 
			  * @param quaternion - The quaternion to be copied.
			  */
			template < typename U >
			OM_FORCE_INLINE Quaternion4D<T>( const Quaternion4D<U>& quaternion )
				:	w( (T)quaternion.w ),
					x( (T)quaternion.x ),
					y( (T)quaternion.y ),
					z( (T)quaternion.z )
			{
			}
			
			
		//********************************************************************************
		//******	Magnitude Methods
			
			
			/// Return the magnitude (norm) of this quaternion.
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( w*w + x*x + y*y + z*z );
			}
			
			
			/// Return the magnitude (norm) of this quaternion.
			OM_FORCE_INLINE T getNorm() const
			{
				return this->getMagnitude();
			}
			
			
			/// Return the squared magnitude (norm) of this quaternion.
			OM_FORCE_INLINE T getMagnitudeSquared() const
			{
				return w*w + x*x + y*y + z*z;
			}
			
			
			/// Return a normalized version of this quaternion.
			/**
			  * This method normalizes this quaternion by dividing
			  * each component by the quaternion's magnitude and
			  * returning the result. This method does not modify
			  * the original quaternion.
			  * 
			  * @return a normalized version of this quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T> normalize() const
			{
				T inverseMagnitude = T(1)/this->getMagnitude();
				
				return Quaternion4D<T>( w*inverseMagnitude,
									x*inverseMagnitude,
									y*inverseMagnitude,
									z*inverseMagnitude );
			}
			
			
			/// Return a normalized version of this quaternion, placing the quaternion's magnitude in the output parameter.
			/**
			  * This method normalizes this quaternion by dividing
			  * each component by the quaternion's magnitude and
			  * returning the result. This method does not modify
			  * the original quaternion. The magnitude of the original quaternion is
			  * returned in the output parameter.
			  * 
			  * @return a normalized version of this quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T> normalize( T& magnitude ) const
			{
				magnitude = this->getMagnitude();
				T inverseMagnitude = T(1)/magnitude;
				
				return Quaternion4D<T>( w*inverseMagnitude,
									x*inverseMagnitude,
									y*inverseMagnitude,
									z*inverseMagnitude );
			}
			
			
		//********************************************************************************
		//******	Inversion Methods
			
			
			/// Return the inverse of this quaternion.
			/**
			  * This method makes no assumptions about the quaternion's magnitude.
			  * If inverting a unit-length quaternion, use invertNormalized() instead
			  * because it is significantly fast.
			  */
			OM_FORCE_INLINE Quaternion4D<T> invert() const
			{
				T magnitudeSquared = this->getMagnitudeSquared();
				T inverseMagnitudeSquared = T(1)/magnitudeSquared;
				
				return Quaternion4D( w*inverseMagnitudeSquared,
								-x*inverseMagnitudeSquared,
								-y*inverseMagnitudeSquared,
								-z*inverseMagnitudeSquared );
			}
			
			
			/// Return the inverse of this normalized quaternion.
			/**
			  * This method assumes that the quaternion is of unit length. This
			  * greatly simplifies the calculations needed to invert the quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T> invertNormalized() const
			{
				return Quaternion4D( w, -x, -y, -z );
			}
			
			
		//********************************************************************************
		//******	Matrix Conversion Methods
			
			
			/// Return the inverse of this quaternion.
			/**
			  * This method makes no assumptions about the quaternion's magnitude.
			  * If converting a unit-length quaternion, use toMatrixNormalized() instead
			  * because it is significantly faster.
			  */
			OM_FORCE_INLINE MatrixND<T,3,3> toMatrix() const
			{
				T ww = w*w;		T xx = x*x;		T yy = y*y;		T zz = z*z;
				T wx = w*x;		T wy = w*y;		T wz = w*z;		T xy = x*y;
				T xz = x*z;		T yz = y*z;
				
				return MatrixND<T,3,3>(
					ww + xx - yy - zz,
					T(2)*(xy - wz),
					T(2)*(xz + wy),
					T(2)*(xy + wz),
					ww - xx + yy - zz,
					T(2)*(yz - wx),
					T(2)*(xz - wy),
					T(2)*(yz + wx),
					ww - xx - yy + zz );
			}
			
			
			/// Return the inverse of this normalized quaternion.
			/**
			  * This method assumes that the quaternion is of unit length. This
			  * greatly simplifies the calculations needed to convert the quaternion
			  * to a matrix.
			  */
			OM_FORCE_INLINE MatrixND<T,3,3> toMatrixNormalized() const
			{
				T temp_b  = x + x;			T temp_c  = y + y;			T temp_d  = z + z;
				T temp_wx = temp_b*w;		T temp_wy = temp_c*w;		T temp_wz = temp_d*w;
				T temp_xx = temp_b*x;		T temp_xy = temp_c*x;		T temp_xz = temp_d*x;
				T temp_yy = temp_c*y;		T temp_yz = temp_d*y;		T temp_zz = temp_d*z;
				
				return MatrixND<T,3,3>(
					T(1) - (temp_yy + temp_zz),
					temp_xy - temp_wz,
					temp_xz + temp_wy,
					temp_xy + temp_wz,
					T(1) - (temp_xx + temp_zz),
					temp_yz - temp_wx,
					temp_xz - temp_wy,
					temp_yz + temp_wx,
					T(1) - (temp_xx + temp_yy) );
			}
			
			
		//********************************************************************************
		//******	Element Ayyessor Methods
			
			
			/// Return a shallow array representation of this quaternion.
			/** 
			  * This method returns a pointer to the address of the X coordinate
			  * of the quaternion and does not do any copying of the elements.
			  * Therefore, this method should only be used where one needs
			  * an array representation of a quaternion without having to 
			  * allocate more memory and copy the quaternion.
			  * 
			  * @return A pointer to a shallow array copy of this quaternion.
			  */
			OM_FORCE_INLINE const T* toArray() const
			{
				return &w;
			}
			
			
			/// Return the W coordinate (real part) of this quaternion.
			OM_FORCE_INLINE T getW() const
			{
				return w;
			}
			
			
			/// Return the X coordinate of this quaternion.
			OM_FORCE_INLINE T getX() const
			{
				return x;
			}
			
			
			/// Return the Y coordinate of this quaternion.
			OM_FORCE_INLINE T getY() const
			{
				return y;
			}
			
			
			/// Return the Z coordinate of this quaternion.
			OM_FORCE_INLINE T getZ() const
			{
				return z;
			}
			
			
			/// Return the XYZ vector of this quaternion.
			OM_FORCE_INLINE T getXYZ() const
			{
				return VectorND<T,3>( x, y, z );
			}
			
			
		//********************************************************************************
		//******	Axis Angle Ayyessor Methods
			
			
			/// Return the rotation angle in radians that this quaternion describes.
			OM_INLINE T getAngle() const
			{
				return T(2)*math::acos(w);
			}
			
			
			/// Return the normalized axis of rotation for this quaternion.
			OM_INLINE VectorND<T,3> getAxis() const
			{
				return VectorND<T,3>( x, y, z ).normalize();
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two quaternions component-wise for equality
			OM_FORCE_INLINE Bool operator == ( const Quaternion4D<T>& q ) const
			{
				return w == q.w && x == q.x && y == q.y && z == q.z;
			}
			
			
			/// Compare two quaternions component-wise for inequality
			OM_FORCE_INLINE Bool operator != ( const Quaternion4D<T>& q ) const
			{
				return w != q.w || x != q.x || y != q.y || z != q.z;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this quaternion to another and return the result.
			/** 
			  * This method adds another quaternion to this one, component-wise,
			  * and returns this addition. It does not modify either of the original
			  * quaternions.
			  * 
			  * @param q - The quaternion to add to this one.
			  * @return The addition of this quaternion and the parameter.
			  */
			OM_FORCE_INLINE Quaternion4D<T> operator + ( const Quaternion4D<T>& q ) const
			{
				return Quaternion4D<T>( w + q.w, x + q.x, y + q.y, z + q.z );
			}
			
			
			/// Subtract a quaternion from this quaternion component-wise and return the result.
			/** 
			  * This method subtracts another quaternion from this one, component-wise,
			  * and returns this subtraction. It does not modify either of the original
			  * quaternions.
			  * 
			  * @param q - The quaternion to subtract from this one.
			  * @return The subtraction of the the parameter from this quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T> operator - ( const Quaternion4D<T>& q ) const
			{
				return Quaternion4D<T>( w - q.w, x - q.x, y - q.y, z - q.z );
			}
			
			
			/// Multiply this quaternion and another quaternion.
			/** 
			  * This operation, like matrix multiplication, is not commutative.
			  */
			OM_FORCE_INLINE Quaternion4D<T> operator * ( const Quaternion4D<T>& q ) const
			{
				return Quaternion4D<T>(  w*q.w - x*q.x - y*q.y - z*q.z,
										w*q.x + x*q.w + y*q.z - z*q.y,
										w*q.y - x*q.z + y*q.w + z*q.x,
										w*q.z + x*q.y - y*q.x + z*q.w );
			}
			
			
			/// Multiply every component of this quaternion by a value and return the result.
			/** 
			  * This method multiplies the value parameter with every component
			  * of the quaternion, and returns a quaternion representing this result.
			  * It does not modifiy the original quaternion.
			  * 
			  * @param value - The value to multiplly with all components of this quaternion.
			  * @return The resulting quaternion of this multiplication.
			  */
			OM_FORCE_INLINE Quaternion4D<T> operator * ( const T& value ) const
			{
				return Quaternion4D<T>( w*value, x*value, y*value, z*value );
			}
			
			
			/// Divide every component of this quaternion by a value and return the result.
			/** 
			  * This method divides every component of the quaternion by the value parameter, 
			  * and returns a quaternion representing this result.
			  * It does not modifiy the original quaternion.
			  * 
			  * @param value - The value to divide all components of this quaternion by.
			  * @return The resulting quaternion of this division.
			  */
			OM_FORCE_INLINE Quaternion4D<T> operator / ( const T& value ) const
			{
				T inverseValue = T(1) / value;
				
				return Quaternion4D<T>( w*inverseValue, x*inverseValue, y*inverseValue, z*inverseValue );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators with Vectors
			
			
			/// Add a quaternion to this quaternion, modifying this original quaternion.
			/**
			  * This method adds another quaternion to this quaternion, component-wise,
			  * and sets this quaternion to have the result of this addition.
			  * 
			  * @param q - The quaternion to add to this quaternion.
			  * @return A reference to this modified quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T>& operator += ( const Quaternion4D<T>& q )
			{
				w += q.w;
				x += q.x;
				y += q.y;
				z += q.z;
				
				return *this;
			}
			
			/// Subtract a quaternion from this quaternion, modifying this original quaternion.
			/**
			  * This method subtracts another quaternion from this quaternion, component-wise,
			  * and sets this quaternion to have the result of this subtraction.
			  * 
			  * @param q - The quaternion to subtract from this quaternion.
			  * @return A reference to this modified quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T>& operator -= ( const Quaternion4D<T>& q )
			{
				w -= q.w;
				x -= q.x;
				y -= q.y;
				z -= q.z;
				
				return *this;
			}
			
			
			/// Multiply component-wise this quaternion and another quaternion and modify this quaternion.
			/** 
			  * This operator multiplies each component of this quaternion
			  * by the corresponding component of the other quaternion and
			  * modifies this quaternion to contain the result.
			  * 
			  * @param q - The quaternion to multiply this quaternion by.
			  * @return A reference to this modified quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T>& operator *= ( const Quaternion4D<T>& q )
			{
				T tempW = w*q.w - x*q.x - y*q.y - z*q.z;
				T tempX = w*q.x + x*q.w + y*q.z - z*q.y;
				T tempY = w*q.y - x*q.z + y*q.w + z*q.x;
				T tempZ = w*q.z + x*q.y - y*q.x + z*q.w;
				w = tempW;
				x = tempX;
				y = tempY;
				z = tempZ;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators with Values
			
			
			/// Multiply a value with each component of this quaternion, modifying it.
			/**
			  * This operator multiplies a value with each component of this quaternion
			  * and modifies this quaternion to store the result.
			  * 
			  * @param value - The value to multiply with every component of this quaternion.
			  * @return A reference to this modified quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T>& operator *= ( const T& value )
			{
				w *= value;
				x *= value;
				y *= value;
				z *= value;
				
				return *this;
			}
			
			
			/// Divide each component of this quaternion by a value, modifying it.
			/**
			  * This operator Divides each component of this quaternion by value
			  * and modifies this quaternion to store the result.
			  * 
			  * @param value - The value to multiply with every component of this quaternion.
			  * @return A reference to this modified quaternion.
			  */
			OM_FORCE_INLINE Quaternion4D<T>& operator /= ( const T& value )
			{
				T inverseValue = T(1) / value;
				
				w *= inverseValue;
				x *= inverseValue;
				y *= inverseValue;
				z *= inverseValue;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this quaternion into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "( " << w << ", " << x << ", " << y << ", " << z << " )";
				
				return buffer.toString();
			}
			
			
			/// Convert this quaternion into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The W coordinate of a quaternion, the real coordinate.
			T w;
			
			/// The X coordinate of a quaternion, the first imaginary coordinate.
			T x;
			
			/// The Y coordinate of a quaternion, the second imaginary coordinate.
			T y;
			
			/// The Z coordinate of a quaternion, the third imaginary coordinate.
			T z;
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant quaternion with all elements equal to zero
			static const Quaternion4D<T> ZERO;
			
			
			/// A constant quaternion representing no rotation.
			static const Quaternion4D<T> IDENTITY;
			
			
};




template <typename T>
const Quaternion4D<T> Quaternion4D<T>:: ZERO( 0, 0, 0, 0 );




template <typename T>
const Quaternion4D<T> Quaternion4D<T>:: IDENTITY( 1, 0, 0, 0 );




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Multiply every component of the quaternion with the value, returning a quaternion result.
/** 
  * This operator multiplies every component of the 2nd paramter, a quaternion,
  * from the 1st paramter, a value, and then returns a quaternion containing the
  * resulting quaternional components. This operator does not modify the orignal quaternion.
  * 
  * @param value - The value to multiply with all components of the quaternion.
  * @param quaternion - The quaternion to be multiplied with.
  * @return The resulting quaternion of this multiplication.
  */
template < typename T >
OM_FORCE_INLINE Quaternion4D<T> operator * ( const T& value, const Quaternion4D<T>& quaternion ) 
{
	return Quaternion4D<T>( quaternion.w*value, quaternion.x*value, quaternion.y*value, quaternion.z*value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Other Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the dot product of two quaternions.
/** 
  * This method adds all components of the component-wise multiplication
  * of the two quaternions together and returns the scalar result. It does
  * not modify either of the original quaternions. If the dot product is
  * zero, then the two quaternions are perpendicular.
  * 
  * @param q1 - The first quaternion of the dot product.
  * @param q2 - The second quaternion of the dot product.
  * @return The dot product of the two quaternion parameters.
  */
template < typename T >
OM_FORCE_INLINE T dot( const Quaternion4D<T>& q1, const Quaternion4D<T>& q2 )
{
	return q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
}




/// Use spherical linear interpolation to interpolate two quaternions.
template < typename T >
OM_FORCE_INLINE Quaternion4D<T> slerp( const Quaternion4D<T>& q1, const Quaternion4D<T>& q2, T a )
{
	// algorithm adapted from Shoemake's paper
	a = a / T(2);
	T theta = math::acos( math::dot( q1, q2 ) );
	
	if ( theta < T(0) )
		theta = -theta;
	
	const T st = math::sin( theta );
	const T sut = math::sin( a*theta );
	const T sout = math::sin( (T(1)-a)*theta );
	const T coeff1 = sout/st;
	const T coeff2 = sut/st;
	
	// Compute and return the normalized result.
	return Quaternion4D<T>( coeff1*q1.w + coeff2*q2.w,
							coeff1*q1.x + coeff2*q2.x,
							coeff1*q1.y + coeff2*q2.y,
							coeff1*q1.z + coeff2*q2.z ).normalize();
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_QUATERNION_4D_H
