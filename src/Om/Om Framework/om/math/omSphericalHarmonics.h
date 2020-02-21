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

#ifndef INCLUDE_OM_SPHERICAL_HARMONICS_H
#define INCLUDE_OM_SPHERICAL_HARMONICS_H


#include "omMathConfig.h"


#include "omVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a spherical harmonic expansion of a template (possibly vector) type.
template < typename T >
class SHExpansion
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a spherical harmonic expansion with no cofficients allocated.
			OM_INLINE SHExpansion()
				:	coefficients( NULL ),
					order( 0 ),
					coefficientCount( 0 ),
					coefficientStorage( 0 )
			{
			}
			
			
			/// Create a spherical harmonic expansion of the specified order.
			/**
			  * The coefficients are uninitialized.
			  */
			OM_INLINE SHExpansion( Size newOrder )
				:	order( newOrder ),
					coefficientCount( (newOrder+1)*(newOrder+1) )
			{
				coefficientStorage = coefficientCount;
				coefficients = util::allocateAligned<T>( coefficientCount, 16 );
			}
			
			
			/// Create a spherical harmonic expansion that is a copy of another expansion.
			OM_INLINE SHExpansion( const SHExpansion& other )
				:	order( other.order ),
					coefficientCount( other.coefficientCount )
			{
				if ( other.coefficients )
				{
					coefficientStorage = coefficientCount;
					coefficients = util::allocateAligned<T>( coefficientCount, 16 );
					util::copy( coefficients, other.coefficients, coefficientCount );
				}
				else
				{
					coefficients = NULL;
					coefficientStorage = 0;
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another spherical harmonic expansion to this one.
			OM_INLINE SHExpansion& operator = ( const SHExpansion& other )
			{
				if ( this != &other )
				{
					if ( order < other.order )
						this->setOrderUninitialized( other.order );
					
					const Size coefficientCount = (order+1)*(order+1);
					
					if ( other.coefficients )
						util::copy( coefficients, other.coefficients, coefficientCount );
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Order Accessor Methods
			
			
			/// Return the order of this spherical harmonic expansion.
			/**
			  * An expansion of order N has (N+1)*(N+1) coefficients in its
			  * expansion.
			  */
			OM_FORCE_INLINE Size getOrder() const
			{
				return order;
			}
			
			
			/// Set the order of this spherical harmonic expansion.
			/**
			  * The entries from the previous expansion that are valid for the specified
			  * order are copied into the new expansion array. Any new values in the expansion
			  * are uninitialized.
			  */
			OM_INLINE void setOrder( Size newOrder )
			{
				const Size newCoefficientCount = (newOrder + 1)*(newOrder + 1);
				
				if ( newCoefficientCount > coefficientStorage )
				{
					T* newCoefficients = util::allocateAligned<T>( newCoefficientCount, 16 );
					
					if ( coefficients )
					{
						util::copy( newCoefficients, coefficients, coefficientCount );
						util::deallocateAligned( coefficients );
					}
					
					coefficients = newCoefficients;
					coefficientCount = newCoefficientCount;
					coefficientStorage = newCoefficientCount;
				}
				
				order = newOrder;
				coefficientCount = newCoefficientCount;
			}
			
			
			/// Set the order of this spherical harmonic expansion.
			/**
			  * The entries in the new expansion array are not initialized.
			  */
			OM_INLINE void setOrderUninitialized( Size newOrder )
			{
				const Size newCoefficientCount = (newOrder + 1)*(newOrder + 1);
				
				if ( newCoefficientCount > coefficientStorage )
				{
					if ( coefficients )
						util::deallocateAligned( coefficients );
					
					coefficients = util::allocateAligned<T>( newCoefficientCount, 16 );
					coefficientCount = newCoefficientCount;
					coefficientStorage = newCoefficientCount;
				}
				
				order = newOrder;
				coefficientCount = newCoefficientCount;
			}
			
			
		//********************************************************************************
		//******	Coefficient Accessor Methods
			
			
			/// Return a pointer to the spherical harmonic coefficients for this expansion.
			OM_FORCE_INLINE T* getCoefficients()
			{
				return coefficients;
			}
			
			
			/// Return a const pointer to the spherical harmonic coefficients for this expansion.
			OM_FORCE_INLINE const T* getCoefficients() const
			{
				return coefficients;
			}
			
			
			/// Return the number of coefficients that are in this spherical harmonic expansion.
			OM_FORCE_INLINE Size getCoefficientCount() const
			{
				return coefficientCount;
			}
			
			
		//********************************************************************************
		//******	Zero Method
			
			
			/// Zero-out the storage of this spherical harmonic expansion.
			OM_INLINE void zero()
			{
				if ( coefficients )
					util::zero( coefficients, coefficientCount );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an array of spherical harmonic coefficients.
			T* coefficients;
			
			
			/// The current order of this spherical harmonic expansion.
			Size order;
			
			
			/// The number of valid coefficients in the coefficient array.
			Size coefficientCount;
			
			
			/// The size of the coefficient storage array.
			Size coefficientStorage;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Spherical Math Operations
//############		
//##########################################################################################
//##########################################################################################





/// Compute the dot product of the specified spherical harmonic expansions.
template < typename T >
OM_FORCE_INLINE T dot( const SHExpansion<T>& sh1, const SHExpansion<T>& sh2 )
{
	return math::dot( sh1.getCoefficients(), sh2.getCoefficients(),
					math::min( sh1.getCoefficientCount(), sh2.getCoefficientCount() ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		SH Fit Class Declaration
//############		
//##########################################################################################
//##########################################################################################




/// A class that stores parameters for projecting a function into the spherical harmonic basis.
class SHProjection
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new SH projection with the default parameters.
			OM_INLINE SHProjection()
				:	maxOrder( 9 ),
					maxError( 0.05f ),
					convergence( 0.001f ),
					sampleCount( 2000 )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The maximum spherical harmonic order to use for the projection.
			/**
			  * A higher-order projection produces a SH fit that is closer to the original data,
			  * but the number of coefficients and the filter interpolation time also increases quadratically
			  * with the maximum order.
			  *
			  * A lower-order representation may be used if it satisfies the maximum error constraint.
			  */
			Size maxOrder;
			
			
			/// The maximum allowed error in the projected function, expressed as a fraction (e.g. 0.05 = 5% error).
			Float maxError;
			
			
			/// If the error improves by less than this amount for an iteration, the fitting terminates.
			/**
			  * The fitting terminates when (lastError/error - 1) < convergence.
			  */
			Float convergence;
			
			
			/// The number of integration samples to use for the projection.
			/**
			  * If Monte Carlo integration is used, these samples will be random and uniformly distributed.
			  * The higher the number of samples, the better quality the SH projection will have.
			  * However, the time to compute the projection increases linearly with the number
			  * of samples.
			  */
			Size sampleCount;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Fixed-order Spherical Harmonic Methods
//############		
//##########################################################################################
//##########################################################################################




/// Template prototype for functions that compute a spherical harmonic expansion for a fixed order.
template < typename U, typename T, Index order >
class SHOrder
{
	public:
		
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		static void cartesian( U x, U y, U z, T* expansion );
		
		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		static void accumulate( U x, U y, U z, T weight, T* expansion );
		
};




/// A class containing functions that evaluate spherical harmonics of order 0.
template < typename U, typename T >
class SHOrder<U,T,0>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_FORCE_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			expansion[0] = T(U(0.2820947917738781));
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			expansion[0] += weight*T(U(0.2820947917738781));
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static T dot( U x, U y, U z, const T* expansion )
		{

			T result = expansion[0]*T(U(0.2820947917738781));
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 1.
template < typename U, typename T >
class SHOrder<U,T,1>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_FORCE_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;

			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 2.
template < typename U, typename T >
class SHOrder<U,T,2>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_FORCE_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.5462742152960395);
			expansion[8] = T(fTmpC*fC1);
			expansion[4] = T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpC*fC1);
			expansion[4] += weight*T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.5462742152960395);
			result += expansion[8]*T(fTmpC*fC1);
			result += expansion[4]*T(fTmpC*fS1);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 3.
template < typename U, typename T >
class SHOrder<U,T,3>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_FORCE_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.5900435899266435);
			expansion[15] = T(fTmpC*fC0);
			expansion[9] = T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpC*fC0);
			expansion[9] += weight*T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_FORCE_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpC*fC0);
			result += expansion[9]*T(fTmpC*fS0);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 4.
template < typename U, typename T >
class SHOrder<U,T,4>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6258357354491763);
			expansion[24] = T(fTmpC*fC1);
			expansion[16] = T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpC*fC1);
			expansion[16] += weight*T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6258357354491763);
			result += expansion[24]*T(fTmpC*fC1);
			result += expansion[16]*T(fTmpC*fS1);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 5.
template < typename U, typename T >
class SHOrder<U,T,5>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			expansion[30] = T(U(1.98997487421324)*fZ*expansion[20] + U(-1.002853072844814)*expansion[12]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] = T(fTmpB*fC0);
			expansion[29] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] = T(fTmpA*fC1);
			expansion[28] = T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] = T(fTmpC*fC0);
			expansion[27] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] = T(fTmpA*fC1);
			expansion[16] = T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] = T(fTmpB*fC1);
			expansion[26] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.6563820568401703);
			expansion[35] = T(fTmpC*fC0);
			expansion[25] = T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			expansion[30] += weight*T(temp30);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] += weight*T(fTmpB*fC0);
			expansion[29] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] += weight*T(fTmpA*fC1);
			expansion[28] += weight*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] += weight*T(fTmpC*fC0);
			expansion[27] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpA*fC1);
			expansion[16] += weight*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] += weight*T(fTmpB*fC1);
			expansion[26] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.6563820568401703);
			expansion[35] += weight*T(fTmpC*fC0);
			expansion[25] += weight*T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			result += expansion[30]*T(temp30);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			result += expansion[31]*T(fTmpB*fC0);
			result += expansion[29]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			result += expansion[32]*T(fTmpA*fC1);
			result += expansion[28]*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			result += expansion[33]*T(fTmpC*fC0);
			result += expansion[27]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			result += expansion[24]*T(fTmpA*fC1);
			result += expansion[16]*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			result += expansion[34]*T(fTmpB*fC1);
			result += expansion[26]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.6563820568401703);
			result += expansion[35]*T(fTmpC*fC0);
			result += expansion[25]*T(fTmpC*fS0);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 6.
template < typename U, typename T >
class SHOrder<U,T,6>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			expansion[30] = T(U(1.98997487421324)*fZ*expansion[20] + U(-1.002853072844814)*expansion[12]);
			expansion[42] = T(U(1.993043457183566)*fZ*expansion[30] + U(-1.001542020962219)*expansion[20]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] = T(fTmpB*fC0);
			expansion[29] = T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] = T(fTmpC*fC0);
			expansion[41] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] = T(fTmpA*fC1);
			expansion[28] = T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] = T(fTmpB*fC1);
			expansion[40] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] = T(fTmpC*fC0);
			expansion[27] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] = T(fTmpA*fC0);
			expansion[39] = T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] = T(fTmpA*fC1);
			expansion[16] = T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] = T(fTmpB*fC1);
			expansion[26] = T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] = T(fTmpC*fC1);
			expansion[38] = T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] = T(fTmpA*fC0);
			expansion[25] = T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] = T(fTmpB*fC0);
			expansion[37] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6831841051919144);
			expansion[48] = T(fTmpC*fC1);
			expansion[36] = T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			expansion[30] += weight*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			expansion[42] += weight*T(temp42);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] += weight*T(fTmpB*fC0);
			expansion[29] += weight*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] += weight*T(fTmpC*fC0);
			expansion[41] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] += weight*T(fTmpA*fC1);
			expansion[28] += weight*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] += weight*T(fTmpB*fC1);
			expansion[40] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] += weight*T(fTmpC*fC0);
			expansion[27] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] += weight*T(fTmpA*fC0);
			expansion[39] += weight*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpA*fC1);
			expansion[16] += weight*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] += weight*T(fTmpB*fC1);
			expansion[26] += weight*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] += weight*T(fTmpC*fC1);
			expansion[38] += weight*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] += weight*T(fTmpA*fC0);
			expansion[25] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] += weight*T(fTmpB*fC0);
			expansion[37] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6831841051919144);
			expansion[48] += weight*T(fTmpC*fC1);
			expansion[36] += weight*T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			result += expansion[30]*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			result += expansion[42]*T(temp42);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			result += expansion[31]*T(fTmpB*fC0);
			result += expansion[29]*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			result += expansion[43]*T(fTmpC*fC0);
			result += expansion[41]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			result += expansion[32]*T(fTmpA*fC1);
			result += expansion[28]*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			result += expansion[44]*T(fTmpB*fC1);
			result += expansion[40]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			result += expansion[33]*T(fTmpC*fC0);
			result += expansion[27]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			result += expansion[45]*T(fTmpA*fC0);
			result += expansion[39]*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			result += expansion[24]*T(fTmpA*fC1);
			result += expansion[16]*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			result += expansion[34]*T(fTmpB*fC1);
			result += expansion[26]*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			result += expansion[46]*T(fTmpC*fC1);
			result += expansion[38]*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			result += expansion[35]*T(fTmpA*fC0);
			result += expansion[25]*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			result += expansion[47]*T(fTmpB*fC0);
			result += expansion[37]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.6831841051919144);
			result += expansion[48]*T(fTmpC*fC1);
			result += expansion[36]*T(fTmpC*fS1);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 7.
template < typename U, typename T >
class SHOrder<U,T,7>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			expansion[30] = T(U(1.98997487421324)*fZ*expansion[20] + U(-1.002853072844814)*expansion[12]);
			expansion[42] = T(U(1.993043457183566)*fZ*expansion[30] + U(-1.001542020962219)*expansion[20]);
			expansion[56] = T(U(1.994891434824135)*fZ*expansion[42] + U(-1.000927213921958)*expansion[30]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] = T(fTmpB*fC0);
			expansion[29] = T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] = T(fTmpC*fC0);
			expansion[41] = T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] = T(fTmpA*fC0);
			expansion[55] = T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] = T(fTmpA*fC1);
			expansion[28] = T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] = T(fTmpB*fC1);
			expansion[40] = T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] = T(fTmpC*fC1);
			expansion[54] = T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] = T(fTmpC*fC0);
			expansion[27] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] = T(fTmpA*fC0);
			expansion[39] = T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] = T(fTmpB*fC0);
			expansion[53] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] = T(fTmpA*fC1);
			expansion[16] = T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] = T(fTmpB*fC1);
			expansion[26] = T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] = T(fTmpC*fC1);
			expansion[38] = T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] = T(fTmpA*fC1);
			expansion[52] = T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] = T(fTmpA*fC0);
			expansion[25] = T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] = T(fTmpB*fC0);
			expansion[37] = T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] = T(fTmpC*fC0);
			expansion[51] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] = T(fTmpA*fC1);
			expansion[36] = T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] = T(fTmpB*fC1);
			expansion[50] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7071627325245963);
			expansion[63] = T(fTmpC*fC0);
			expansion[49] = T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			expansion[30] += weight*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			expansion[42] += weight*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			expansion[56] += weight*T(temp56);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] += weight*T(fTmpB*fC0);
			expansion[29] += weight*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] += weight*T(fTmpC*fC0);
			expansion[41] += weight*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] += weight*T(fTmpA*fC0);
			expansion[55] += weight*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] += weight*T(fTmpA*fC1);
			expansion[28] += weight*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] += weight*T(fTmpB*fC1);
			expansion[40] += weight*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] += weight*T(fTmpC*fC1);
			expansion[54] += weight*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] += weight*T(fTmpC*fC0);
			expansion[27] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] += weight*T(fTmpA*fC0);
			expansion[39] += weight*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] += weight*T(fTmpB*fC0);
			expansion[53] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpA*fC1);
			expansion[16] += weight*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] += weight*T(fTmpB*fC1);
			expansion[26] += weight*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] += weight*T(fTmpC*fC1);
			expansion[38] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] += weight*T(fTmpA*fC1);
			expansion[52] += weight*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] += weight*T(fTmpA*fC0);
			expansion[25] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] += weight*T(fTmpB*fC0);
			expansion[37] += weight*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] += weight*T(fTmpC*fC0);
			expansion[51] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] += weight*T(fTmpA*fC1);
			expansion[36] += weight*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] += weight*T(fTmpB*fC1);
			expansion[50] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7071627325245963);
			expansion[63] += weight*T(fTmpC*fC0);
			expansion[49] += weight*T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			result += expansion[30]*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			result += expansion[42]*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			result += expansion[56]*T(temp56);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			result += expansion[31]*T(fTmpB*fC0);
			result += expansion[29]*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			result += expansion[43]*T(fTmpC*fC0);
			result += expansion[41]*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			result += expansion[57]*T(fTmpA*fC0);
			result += expansion[55]*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			result += expansion[32]*T(fTmpA*fC1);
			result += expansion[28]*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			result += expansion[44]*T(fTmpB*fC1);
			result += expansion[40]*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			result += expansion[58]*T(fTmpC*fC1);
			result += expansion[54]*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			result += expansion[33]*T(fTmpC*fC0);
			result += expansion[27]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			result += expansion[45]*T(fTmpA*fC0);
			result += expansion[39]*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			result += expansion[59]*T(fTmpB*fC0);
			result += expansion[53]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			result += expansion[24]*T(fTmpA*fC1);
			result += expansion[16]*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			result += expansion[34]*T(fTmpB*fC1);
			result += expansion[26]*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			result += expansion[46]*T(fTmpC*fC1);
			result += expansion[38]*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			result += expansion[60]*T(fTmpA*fC1);
			result += expansion[52]*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			result += expansion[35]*T(fTmpA*fC0);
			result += expansion[25]*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			result += expansion[47]*T(fTmpB*fC0);
			result += expansion[37]*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			result += expansion[61]*T(fTmpC*fC0);
			result += expansion[51]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			result += expansion[48]*T(fTmpA*fC1);
			result += expansion[36]*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			result += expansion[62]*T(fTmpB*fC1);
			result += expansion[50]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7071627325245963);
			result += expansion[63]*T(fTmpC*fC0);
			result += expansion[49]*T(fTmpC*fS0);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 8.
template < typename U, typename T >
class SHOrder<U,T,8>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			expansion[30] = T(U(1.98997487421324)*fZ*expansion[20] + U(-1.002853072844814)*expansion[12]);
			expansion[42] = T(U(1.993043457183566)*fZ*expansion[30] + U(-1.001542020962219)*expansion[20]);
			expansion[56] = T(U(1.994891434824135)*fZ*expansion[42] + U(-1.000927213921958)*expansion[30]);
			expansion[72] = T(U(1.996089927833914)*fZ*expansion[56] + U(-1.000600781069515)*expansion[42]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] = T(fTmpB*fC0);
			expansion[29] = T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] = T(fTmpC*fC0);
			expansion[41] = T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] = T(fTmpA*fC0);
			expansion[55] = T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			expansion[73] = T(fTmpB*fC0);
			expansion[71] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] = T(fTmpA*fC1);
			expansion[28] = T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] = T(fTmpB*fC1);
			expansion[40] = T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] = T(fTmpC*fC1);
			expansion[54] = T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			expansion[74] = T(fTmpA*fC1);
			expansion[70] = T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] = T(fTmpC*fC0);
			expansion[27] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] = T(fTmpA*fC0);
			expansion[39] = T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] = T(fTmpB*fC0);
			expansion[53] = T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			expansion[75] = T(fTmpC*fC0);
			expansion[69] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] = T(fTmpA*fC1);
			expansion[16] = T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] = T(fTmpB*fC1);
			expansion[26] = T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] = T(fTmpC*fC1);
			expansion[38] = T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] = T(fTmpA*fC1);
			expansion[52] = T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			expansion[76] = T(fTmpB*fC1);
			expansion[68] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] = T(fTmpA*fC0);
			expansion[25] = T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] = T(fTmpB*fC0);
			expansion[37] = T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] = T(fTmpC*fC0);
			expansion[51] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			expansion[77] = T(fTmpA*fC0);
			expansion[67] = T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] = T(fTmpA*fC1);
			expansion[36] = T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] = T(fTmpB*fC1);
			expansion[50] = T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			expansion[78] = T(fTmpC*fC1);
			expansion[66] = T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			expansion[63] = T(fTmpA*fC0);
			expansion[49] = T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			expansion[79] = T(fTmpB*fC0);
			expansion[65] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.72892666017483);
			expansion[80] = T(fTmpC*fC1);
			expansion[64] = T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			expansion[30] += weight*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			expansion[42] += weight*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			expansion[56] += weight*T(temp56);
			U temp72 = U(1.996089927833914)*fZ*temp56 + U(-1.000600781069515)*temp42;
			expansion[72] += weight*T(temp72);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] += weight*T(fTmpB*fC0);
			expansion[29] += weight*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] += weight*T(fTmpC*fC0);
			expansion[41] += weight*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] += weight*T(fTmpA*fC0);
			expansion[55] += weight*T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			expansion[73] += weight*T(fTmpB*fC0);
			expansion[71] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] += weight*T(fTmpA*fC1);
			expansion[28] += weight*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] += weight*T(fTmpB*fC1);
			expansion[40] += weight*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] += weight*T(fTmpC*fC1);
			expansion[54] += weight*T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			expansion[74] += weight*T(fTmpA*fC1);
			expansion[70] += weight*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] += weight*T(fTmpC*fC0);
			expansion[27] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] += weight*T(fTmpA*fC0);
			expansion[39] += weight*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] += weight*T(fTmpB*fC0);
			expansion[53] += weight*T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			expansion[75] += weight*T(fTmpC*fC0);
			expansion[69] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpA*fC1);
			expansion[16] += weight*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] += weight*T(fTmpB*fC1);
			expansion[26] += weight*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] += weight*T(fTmpC*fC1);
			expansion[38] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] += weight*T(fTmpA*fC1);
			expansion[52] += weight*T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			expansion[76] += weight*T(fTmpB*fC1);
			expansion[68] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] += weight*T(fTmpA*fC0);
			expansion[25] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] += weight*T(fTmpB*fC0);
			expansion[37] += weight*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] += weight*T(fTmpC*fC0);
			expansion[51] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			expansion[77] += weight*T(fTmpA*fC0);
			expansion[67] += weight*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] += weight*T(fTmpA*fC1);
			expansion[36] += weight*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] += weight*T(fTmpB*fC1);
			expansion[50] += weight*T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			expansion[78] += weight*T(fTmpC*fC1);
			expansion[66] += weight*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			expansion[63] += weight*T(fTmpA*fC0);
			expansion[49] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			expansion[79] += weight*T(fTmpB*fC0);
			expansion[65] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.72892666017483);
			expansion[80] += weight*T(fTmpC*fC1);
			expansion[64] += weight*T(fTmpC*fS1);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			result += expansion[30]*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			result += expansion[42]*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			result += expansion[56]*T(temp56);
			U temp72 = U(1.996089927833914)*fZ*temp56 + U(-1.000600781069515)*temp42;
			result += expansion[72]*T(temp72);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			result += expansion[31]*T(fTmpB*fC0);
			result += expansion[29]*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			result += expansion[43]*T(fTmpC*fC0);
			result += expansion[41]*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			result += expansion[57]*T(fTmpA*fC0);
			result += expansion[55]*T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			result += expansion[73]*T(fTmpB*fC0);
			result += expansion[71]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			result += expansion[32]*T(fTmpA*fC1);
			result += expansion[28]*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			result += expansion[44]*T(fTmpB*fC1);
			result += expansion[40]*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			result += expansion[58]*T(fTmpC*fC1);
			result += expansion[54]*T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			result += expansion[74]*T(fTmpA*fC1);
			result += expansion[70]*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			result += expansion[33]*T(fTmpC*fC0);
			result += expansion[27]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			result += expansion[45]*T(fTmpA*fC0);
			result += expansion[39]*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			result += expansion[59]*T(fTmpB*fC0);
			result += expansion[53]*T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			result += expansion[75]*T(fTmpC*fC0);
			result += expansion[69]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			result += expansion[24]*T(fTmpA*fC1);
			result += expansion[16]*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			result += expansion[34]*T(fTmpB*fC1);
			result += expansion[26]*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			result += expansion[46]*T(fTmpC*fC1);
			result += expansion[38]*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			result += expansion[60]*T(fTmpA*fC1);
			result += expansion[52]*T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			result += expansion[76]*T(fTmpB*fC1);
			result += expansion[68]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			result += expansion[35]*T(fTmpA*fC0);
			result += expansion[25]*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			result += expansion[47]*T(fTmpB*fC0);
			result += expansion[37]*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			result += expansion[61]*T(fTmpC*fC0);
			result += expansion[51]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			result += expansion[77]*T(fTmpA*fC0);
			result += expansion[67]*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			result += expansion[48]*T(fTmpA*fC1);
			result += expansion[36]*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			result += expansion[62]*T(fTmpB*fC1);
			result += expansion[50]*T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			result += expansion[78]*T(fTmpC*fC1);
			result += expansion[66]*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			result += expansion[63]*T(fTmpA*fC0);
			result += expansion[49]*T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			result += expansion[79]*T(fTmpB*fC0);
			result += expansion[65]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpC = U(0.72892666017483);
			result += expansion[80]*T(fTmpC*fC1);
			result += expansion[64]*T(fTmpC*fS1);
			
			return result;
		}
};




/// A class containing functions that evaluate spherical harmonics of order 9.
template < typename U, typename T >
class SHOrder<U,T,9>
{
	public:
		/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
		OM_INLINE static void cartesian( U x, U y, U z, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] = T(U(0.2820947917738781));
			expansion[2] = T(U(0.4886025119029199)*fZ);
			expansion[6] = T(U(0.9461746957575601)*fZ2 + U(-0.31539156525252));
			expansion[12] = T(fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346)));
			expansion[20] = T(U(1.984313483298443)*fZ*expansion[12] + U(-1.006230589874905)*expansion[6]);
			expansion[30] = T(U(1.98997487421324)*fZ*expansion[20] + U(-1.002853072844814)*expansion[12]);
			expansion[42] = T(U(1.993043457183566)*fZ*expansion[30] + U(-1.001542020962219)*expansion[20]);
			expansion[56] = T(U(1.994891434824135)*fZ*expansion[42] + U(-1.000927213921958)*expansion[30]);
			expansion[72] = T(U(1.996089927833914)*fZ*expansion[56] + U(-1.000600781069515)*expansion[42]);
			expansion[90] = T(U(1.996911195067937)*fZ*expansion[72] + U(-1.000411437993134)*expansion[56]);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] = T(fTmpA*fC0);
			expansion[1] = T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] = T(fTmpB*fC0);
			expansion[5] = T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] = T(fTmpC*fC0);
			expansion[11] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] = T(fTmpA*fC0);
			expansion[19] = T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] = T(fTmpB*fC0);
			expansion[29] = T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] = T(fTmpC*fC0);
			expansion[41] = T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] = T(fTmpA*fC0);
			expansion[55] = T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			expansion[73] = T(fTmpB*fC0);
			expansion[71] = T(fTmpB*fS0);
			fTmpC = U(2.009353129741012)*fZ*fTmpB + U(-0.9987492177719088)*fTmpA;
			expansion[91] = T(fTmpC*fC0);
			expansion[89] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] = T(fTmpA*fC1);
			expansion[4] = T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] = T(fTmpB*fC1);
			expansion[10] = T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] = T(fTmpC*fC1);
			expansion[18] = T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] = T(fTmpA*fC1);
			expansion[28] = T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] = T(fTmpB*fC1);
			expansion[40] = T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] = T(fTmpC*fC1);
			expansion[54] = T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			expansion[74] = T(fTmpA*fC1);
			expansion[70] = T(fTmpA*fS1);
			fTmpB = U(2.048122358357819)*fZ*fTmpA + U(-0.9934852726704042)*fTmpC;
			expansion[92] = T(fTmpB*fC1);
			expansion[88] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] = T(fTmpA*fC0);
			expansion[9] = T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] = T(fTmpB*fC0);
			expansion[17] = T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] = T(fTmpC*fC0);
			expansion[27] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] = T(fTmpA*fC0);
			expansion[39] = T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] = T(fTmpB*fC0);
			expansion[53] = T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			expansion[75] = T(fTmpC*fC0);
			expansion[69] = T(fTmpC*fS0);
			fTmpA = U(2.118044171189805)*fZ*fTmpC + U(-0.9836628449792094)*fTmpB;
			expansion[93] = T(fTmpA*fC0);
			expansion[87] = T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] = T(fTmpA*fC1);
			expansion[16] = T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] = T(fTmpB*fC1);
			expansion[26] = T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] = T(fTmpC*fC1);
			expansion[38] = T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] = T(fTmpA*fC1);
			expansion[52] = T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			expansion[76] = T(fTmpB*fC1);
			expansion[68] = T(fTmpB*fS1);
			fTmpC = U(2.229177150706235)*fZ*fTmpB + U(-0.9671528397231821)*fTmpA;
			expansion[94] = T(fTmpC*fC1);
			expansion[86] = T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] = T(fTmpA*fC0);
			expansion[25] = T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] = T(fTmpB*fC0);
			expansion[37] = T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] = T(fTmpC*fC0);
			expansion[51] = T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			expansion[77] = T(fTmpA*fC0);
			expansion[67] = T(fTmpA*fS0);
			fTmpB = U(2.401636346922062)*fZ*fTmpA + U(-0.9392246042043708)*fTmpC;
			expansion[95] = T(fTmpB*fC0);
			expansion[85] = T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] = T(fTmpA*fC1);
			expansion[36] = T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] = T(fTmpB*fC1);
			expansion[50] = T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			expansion[78] = T(fTmpC*fC1);
			expansion[66] = T(fTmpC*fS1);
			fTmpA = fZ*(U(21.39289019090864)*fZ2 + U(-3.775215916042701));
			expansion[96] = T(fTmpA*fC1);
			expansion[84] = T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			expansion[63] = T(fTmpA*fC0);
			expansion[49] = T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			expansion[79] = T(fTmpB*fC0);
			expansion[65] = T(fTmpB*fS0);
			fTmpC = U(-9.263393182848905)*fZ2 + U(0.5449054813440533);
			expansion[97] = T(fTmpC*fC0);
			expansion[83] = T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.72892666017483);
			expansion[80] = T(fTmpA*fC1);
			expansion[64] = T(fTmpA*fS1);
			fTmpB = U(3.177317648954698)*fZ;
			expansion[98] = T(fTmpB*fC1);
			expansion[82] = T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7489009518531884);
			expansion[99] = T(fTmpC*fC0);
			expansion[81] = T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static void accumulate( U x, U y, U z, T weight, T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			expansion[0] += weight*T(U(0.2820947917738781));
			expansion[2] += weight*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			expansion[6] += weight*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			expansion[12] += weight*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			expansion[20] += weight*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			expansion[30] += weight*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			expansion[42] += weight*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			expansion[56] += weight*T(temp56);
			U temp72 = U(1.996089927833914)*fZ*temp56 + U(-1.000600781069515)*temp42;
			expansion[72] += weight*T(temp72);
			U temp90 = U(1.996911195067937)*fZ*temp72 + U(-1.000411437993134)*temp56;
			expansion[90] += weight*T(temp90);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			expansion[3] += weight*T(fTmpA*fC0);
			expansion[1] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			expansion[7] += weight*T(fTmpB*fC0);
			expansion[5] += weight*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			expansion[13] += weight*T(fTmpC*fC0);
			expansion[11] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			expansion[21] += weight*T(fTmpA*fC0);
			expansion[19] += weight*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			expansion[31] += weight*T(fTmpB*fC0);
			expansion[29] += weight*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			expansion[43] += weight*T(fTmpC*fC0);
			expansion[41] += weight*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			expansion[57] += weight*T(fTmpA*fC0);
			expansion[55] += weight*T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			expansion[73] += weight*T(fTmpB*fC0);
			expansion[71] += weight*T(fTmpB*fS0);
			fTmpC = U(2.009353129741012)*fZ*fTmpB + U(-0.9987492177719088)*fTmpA;
			expansion[91] += weight*T(fTmpC*fC0);
			expansion[89] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			expansion[8] += weight*T(fTmpA*fC1);
			expansion[4] += weight*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			expansion[14] += weight*T(fTmpB*fC1);
			expansion[10] += weight*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			expansion[22] += weight*T(fTmpC*fC1);
			expansion[18] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			expansion[32] += weight*T(fTmpA*fC1);
			expansion[28] += weight*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			expansion[44] += weight*T(fTmpB*fC1);
			expansion[40] += weight*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			expansion[58] += weight*T(fTmpC*fC1);
			expansion[54] += weight*T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			expansion[74] += weight*T(fTmpA*fC1);
			expansion[70] += weight*T(fTmpA*fS1);
			fTmpB = U(2.048122358357819)*fZ*fTmpA + U(-0.9934852726704042)*fTmpC;
			expansion[92] += weight*T(fTmpB*fC1);
			expansion[88] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			expansion[15] += weight*T(fTmpA*fC0);
			expansion[9] += weight*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			expansion[23] += weight*T(fTmpB*fC0);
			expansion[17] += weight*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			expansion[33] += weight*T(fTmpC*fC0);
			expansion[27] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			expansion[45] += weight*T(fTmpA*fC0);
			expansion[39] += weight*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			expansion[59] += weight*T(fTmpB*fC0);
			expansion[53] += weight*T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			expansion[75] += weight*T(fTmpC*fC0);
			expansion[69] += weight*T(fTmpC*fS0);
			fTmpA = U(2.118044171189805)*fZ*fTmpC + U(-0.9836628449792094)*fTmpB;
			expansion[93] += weight*T(fTmpA*fC0);
			expansion[87] += weight*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			expansion[24] += weight*T(fTmpA*fC1);
			expansion[16] += weight*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			expansion[34] += weight*T(fTmpB*fC1);
			expansion[26] += weight*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			expansion[46] += weight*T(fTmpC*fC1);
			expansion[38] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			expansion[60] += weight*T(fTmpA*fC1);
			expansion[52] += weight*T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			expansion[76] += weight*T(fTmpB*fC1);
			expansion[68] += weight*T(fTmpB*fS1);
			fTmpC = U(2.229177150706235)*fZ*fTmpB + U(-0.9671528397231821)*fTmpA;
			expansion[94] += weight*T(fTmpC*fC1);
			expansion[86] += weight*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			expansion[35] += weight*T(fTmpA*fC0);
			expansion[25] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			expansion[47] += weight*T(fTmpB*fC0);
			expansion[37] += weight*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			expansion[61] += weight*T(fTmpC*fC0);
			expansion[51] += weight*T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			expansion[77] += weight*T(fTmpA*fC0);
			expansion[67] += weight*T(fTmpA*fS0);
			fTmpB = U(2.401636346922062)*fZ*fTmpA + U(-0.9392246042043708)*fTmpC;
			expansion[95] += weight*T(fTmpB*fC0);
			expansion[85] += weight*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			expansion[48] += weight*T(fTmpA*fC1);
			expansion[36] += weight*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			expansion[62] += weight*T(fTmpB*fC1);
			expansion[50] += weight*T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			expansion[78] += weight*T(fTmpC*fC1);
			expansion[66] += weight*T(fTmpC*fS1);
			fTmpA = fZ*(U(21.39289019090864)*fZ2 + U(-3.775215916042701));
			expansion[96] += weight*T(fTmpA*fC1);
			expansion[84] += weight*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			expansion[63] += weight*T(fTmpA*fC0);
			expansion[49] += weight*T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			expansion[79] += weight*T(fTmpB*fC0);
			expansion[65] += weight*T(fTmpB*fS0);
			fTmpC = U(-9.263393182848905)*fZ2 + U(0.5449054813440533);
			expansion[97] += weight*T(fTmpC*fC0);
			expansion[83] += weight*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.72892666017483);
			expansion[80] += weight*T(fTmpA*fC1);
			expansion[64] += weight*T(fTmpA*fS1);
			fTmpB = U(3.177317648954698)*fZ;
			expansion[98] += weight*T(fTmpB*fC1);
			expansion[82] += weight*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7489009518531884);
			expansion[99] += weight*T(fTmpC*fC0);
			expansion[81] += weight*T(fTmpC*fS0);
		}


		/// Multiply-accumulate the spherical harmonic expansion for the given 3D normalized cartesian vector and weight.
		OM_INLINE static T dot( U x, U y, U z, const T* expansion )
		{
			U fX = x, fY = y, fZ = z;
			U fC0, fS0, fTmpA;
			U fC1, fS1, fTmpB, fTmpC;
			U fZ2 = fZ*fZ;
			
			T result = expansion[0]*T(U(0.2820947917738781));
			result += expansion[2]*T(U(0.4886025119029199)*fZ);
			U temp6 = U(0.9461746957575601)*fZ2 + U(-0.31539156525252);
			result += expansion[6]*T(temp6);
			U temp12 = fZ*(U(1.865881662950577)*fZ2 + U(-1.119528997770346));
			result += expansion[12]*T(temp12);
			U temp20 = U(1.984313483298443)*fZ*temp12 + U(-1.006230589874905)*temp6;
			result += expansion[20]*T(temp20);
			U temp30 = U(1.98997487421324)*fZ*temp20 + U(-1.002853072844814)*temp12;
			result += expansion[30]*T(temp30);
			U temp42 = U(1.993043457183566)*fZ*temp30 + U(-1.001542020962219)*temp20;
			result += expansion[42]*T(temp42);
			U temp56 = U(1.994891434824135)*fZ*temp42 + U(-1.000927213921958)*temp30;
			result += expansion[56]*T(temp56);
			U temp72 = U(1.996089927833914)*fZ*temp56 + U(-1.000600781069515)*temp42;
			result += expansion[72]*T(temp72);
			U temp90 = U(1.996911195067937)*fZ*temp72 + U(-1.000411437993134)*temp56;
			result += expansion[90]*T(temp90);
			fC0 = fX;
			fS0 = fY;
			
			fTmpA = U(-0.48860251190292);
			result += expansion[3]*T(fTmpA*fC0);
			result += expansion[1]*T(fTmpA*fS0);
			fTmpB = U(-1.092548430592079)*fZ;
			result += expansion[7]*T(fTmpB*fC0);
			result += expansion[5]*T(fTmpB*fS0);
			fTmpC = U(-2.285228997322329)*fZ2 + U(0.4570457994644658);
			result += expansion[13]*T(fTmpC*fC0);
			result += expansion[11]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-4.683325804901025)*fZ2 + U(2.007139630671868));
			result += expansion[21]*T(fTmpA*fC0);
			result += expansion[19]*T(fTmpA*fS0);
			fTmpB = U(2.03100960115899)*fZ*fTmpA + U(-0.991031208965115)*fTmpC;
			result += expansion[31]*T(fTmpB*fC0);
			result += expansion[29]*T(fTmpB*fS0);
			fTmpC = U(2.021314989237028)*fZ*fTmpB + U(-0.9952267030562385)*fTmpA;
			result += expansion[43]*T(fTmpC*fC0);
			result += expansion[41]*T(fTmpC*fS0);
			fTmpA = U(2.015564437074638)*fZ*fTmpC + U(-0.9971550440218319)*fTmpB;
			result += expansion[57]*T(fTmpA*fC0);
			result += expansion[55]*T(fTmpA*fS0);
			fTmpB = U(2.011869540407391)*fZ*fTmpA + U(-0.9981668178901745)*fTmpC;
			result += expansion[73]*T(fTmpB*fC0);
			result += expansion[71]*T(fTmpB*fS0);
			fTmpC = U(2.009353129741012)*fZ*fTmpB + U(-0.9987492177719088)*fTmpA;
			result += expansion[91]*T(fTmpC*fC0);
			result += expansion[89]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.5462742152960395);
			result += expansion[8]*T(fTmpA*fC1);
			result += expansion[4]*T(fTmpA*fS1);
			fTmpB = U(1.445305721320277)*fZ;
			result += expansion[14]*T(fTmpB*fC1);
			result += expansion[10]*T(fTmpB*fS1);
			fTmpC = U(3.31161143515146)*fZ2 + U(-0.47308734787878);
			result += expansion[22]*T(fTmpC*fC1);
			result += expansion[18]*T(fTmpC*fS1);
			fTmpA = fZ*(U(7.190305177459987)*fZ2 + U(-2.396768392486662));
			result += expansion[32]*T(fTmpA*fC1);
			result += expansion[28]*T(fTmpA*fS1);
			fTmpB = U(2.11394181566097)*fZ*fTmpA + U(-0.9736101204623268)*fTmpC;
			result += expansion[44]*T(fTmpB*fC1);
			result += expansion[40]*T(fTmpB*fS1);
			fTmpC = U(2.081665999466133)*fZ*fTmpB + U(-0.9847319278346618)*fTmpA;
			result += expansion[58]*T(fTmpC*fC1);
			result += expansion[54]*T(fTmpC*fS1);
			fTmpA = U(2.06155281280883)*fZ*fTmpC + U(-0.9903379376602873)*fTmpB;
			result += expansion[74]*T(fTmpA*fC1);
			result += expansion[70]*T(fTmpA*fS1);
			fTmpB = U(2.048122358357819)*fZ*fTmpA + U(-0.9934852726704042)*fTmpC;
			result += expansion[92]*T(fTmpB*fC1);
			result += expansion[88]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.5900435899266435);
			result += expansion[15]*T(fTmpA*fC0);
			result += expansion[9]*T(fTmpA*fS0);
			fTmpB = U(-1.770130769779931)*fZ;
			result += expansion[23]*T(fTmpB*fC0);
			result += expansion[17]*T(fTmpB*fS0);
			fTmpC = U(-4.403144694917254)*fZ2 + U(0.4892382994352505);
			result += expansion[33]*T(fTmpC*fC0);
			result += expansion[27]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-10.13325785466416)*fZ2 + U(2.763615778544771));
			result += expansion[45]*T(fTmpA*fC0);
			result += expansion[39]*T(fTmpA*fS0);
			fTmpB = U(2.207940216581961)*fZ*fTmpA + U(-0.959403223600247)*fTmpC;
			result += expansion[59]*T(fTmpB*fC0);
			result += expansion[53]*T(fTmpB*fS0);
			fTmpC = U(2.15322168769582)*fZ*fTmpB + U(-0.9752173865600178)*fTmpA;
			result += expansion[75]*T(fTmpC*fC0);
			result += expansion[69]*T(fTmpC*fS0);
			fTmpA = U(2.118044171189805)*fZ*fTmpC + U(-0.9836628449792094)*fTmpB;
			result += expansion[93]*T(fTmpA*fC0);
			result += expansion[87]*T(fTmpA*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6258357354491763);
			result += expansion[24]*T(fTmpA*fC1);
			result += expansion[16]*T(fTmpA*fS1);
			fTmpB = U(2.075662314881041)*fZ;
			result += expansion[34]*T(fTmpB*fC1);
			result += expansion[26]*T(fTmpB*fS1);
			fTmpC = U(5.550213908015966)*fZ2 + U(-0.5045649007287241);
			result += expansion[46]*T(fTmpC*fC1);
			result += expansion[38]*T(fTmpC*fS1);
			fTmpA = fZ*(U(13.49180504672677)*fZ2 + U(-3.113493472321562));
			result += expansion[60]*T(fTmpA*fC1);
			result += expansion[52]*T(fTmpA*fS1);
			fTmpB = U(2.304886114323221)*fZ*fTmpA + U(-0.9481763873554654)*fTmpC;
			result += expansion[76]*T(fTmpB*fC1);
			result += expansion[68]*T(fTmpB*fS1);
			fTmpC = U(2.229177150706235)*fZ*fTmpB + U(-0.9671528397231821)*fTmpA;
			result += expansion[94]*T(fTmpC*fC1);
			result += expansion[86]*T(fTmpC*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.6563820568401703);
			result += expansion[35]*T(fTmpA*fC0);
			result += expansion[25]*T(fTmpA*fS0);
			fTmpB = U(-2.366619162231753)*fZ;
			result += expansion[47]*T(fTmpB*fC0);
			result += expansion[37]*T(fTmpB*fS0);
			fTmpC = U(-6.745902523363385)*fZ2 + U(0.5189155787202604);
			result += expansion[61]*T(fTmpC*fC0);
			result += expansion[51]*T(fTmpC*fS0);
			fTmpA = fZ*(U(-17.24955311049054)*fZ2 + U(3.449910622098108));
			result += expansion[77]*T(fTmpA*fC0);
			result += expansion[67]*T(fTmpA*fS0);
			fTmpB = U(2.401636346922062)*fZ*fTmpA + U(-0.9392246042043708)*fTmpC;
			result += expansion[95]*T(fTmpB*fC0);
			result += expansion[85]*T(fTmpB*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.6831841051919144);
			result += expansion[48]*T(fTmpA*fC1);
			result += expansion[36]*T(fTmpA*fS1);
			fTmpB = U(2.6459606618019)*fZ;
			result += expansion[62]*T(fTmpB*fC1);
			result += expansion[50]*T(fTmpB*fS1);
			fTmpC = U(7.984991490893139)*fZ2 + U(-0.5323327660595426);
			result += expansion[78]*T(fTmpC*fC1);
			result += expansion[66]*T(fTmpC*fS1);
			fTmpA = fZ*(U(21.39289019090864)*fZ2 + U(-3.775215916042701));
			result += expansion[96]*T(fTmpA*fC1);
			result += expansion[84]*T(fTmpA*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpA = U(-0.7071627325245963);
			result += expansion[63]*T(fTmpA*fC0);
			result += expansion[49]*T(fTmpA*fS0);
			fTmpB = U(-2.91570664069932)*fZ;
			result += expansion[79]*T(fTmpB*fC0);
			result += expansion[65]*T(fTmpB*fS0);
			fTmpC = U(-9.263393182848905)*fZ2 + U(0.5449054813440533);
			result += expansion[97]*T(fTmpC*fC0);
			result += expansion[83]*T(fTmpC*fS0);
			fC1 = fX*fC0 - fY*fS0;
			fS1 = fX*fS0 + fY*fC0;
			
			fTmpA = U(0.72892666017483);
			result += expansion[80]*T(fTmpA*fC1);
			result += expansion[64]*T(fTmpA*fS1);
			fTmpB = U(3.177317648954698)*fZ;
			result += expansion[98]*T(fTmpB*fC1);
			result += expansion[82]*T(fTmpB*fS1);
			fC0 = fX*fC1 - fY*fS1;
			fS0 = fX*fS1 + fY*fC1;
			
			fTmpC = U(-0.7489009518531884);
			result += expansion[99]*T(fTmpC*fC0);
			result += expansion[81]*T(fTmpC*fS0);
			
			return result;
		}
};




//##########################################################################################
//##########################################################################################
//############		
//############		Arbitrary-order Spherical Harmonic Method
//############		
//##########################################################################################
//##########################################################################################




/// A class that contains functions that evaluate spherical harmonics at arbitrary order.
class SH
{
	public:
			
		//********************************************************************************
		//******	Coefficient Count Method
			
			
			/// Return the number of coefficients for the spherical harmonic expansion of the given order.
			OM_FORCE_INLINE static Size getCoefficientCount( Index order )
			{
				return math::square(order+1);
			}
			
			
		//********************************************************************************
		//******	Cartesian SH Functions
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void cartesian( Index order, const VectorND<U,3>& v, SHExpansion<T>& expansion )
			{
				if ( expansion.getOrder() < order || expansion.getCoefficientCount() < math::square(order+1) )
					expansion.setOrderUninitialized( order );
				
				SH::cartesian( order, v, expansion.getCoefficients() );
			}
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void cartesian( Index order, const VectorND<U,3>& v, T* expansion )
			{
				SH::cartesian( order, v.x, v.y, v.z, expansion );
			}
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void cartesian( Index order, U x, U y, U z, T* expansion )
			{
				typedef void (*SHFunction)( U x, U y, U z, T* );
				
				//***************************************************************
				// Use the fast generated functions for low-order coefficients.
				
				// Force generation of a jump table.
				const SHFunction functions[MAX_GENERATED_ORDER+1] =
				{
					SHOrder<U,T,0>::cartesian,
					SHOrder<U,T,1>::cartesian,
					SHOrder<U,T,2>::cartesian,
					SHOrder<U,T,3>::cartesian,
					SHOrder<U,T,4>::cartesian,
					SHOrder<U,T,5>::cartesian,
					SHOrder<U,T,6>::cartesian,
					SHOrder<U,T,7>::cartesian,
					SHOrder<U,T,8>::cartesian,
					SHOrder<U,T,9>::cartesian
				};
				
				//***************************************************************
				// For arbitrary order, use the generic but slow implementation.
				
				if ( order <= MAX_GENERATED_ORDER )
					functions[order]( x, y, z, expansion );
				else
				{
					// Use the fast generated functions for low-order coefficients.
					functions[MAX_GENERATED_ORDER]( x, y, z, expansion );
					
					// Convert to spherical coordinates.
					const U theta = math::acos(z);
					const U vxyMag = math::sqrt(x*x + y*y);
					const U t = (vxyMag > U(0)) ? (x / vxyMag) : U(0);
					const U phi = y >= U(0) ? math::acos(t) : (2*math::pi<U>() - math::acos(t));
					const int maxL = (int)order;
					
					for ( int l = (int)MAX_GENERATED_ORDER + 1; l <= maxL; l++ )
					{
						int ll1 = l*(l+1);
						for ( int m = -l; m <= l; m++ )
							expansion[ll1 + m] = T(Y( l, m, theta, phi ));
					}
				}
			}
			
			
		//********************************************************************************
		//******	SH Accumulate Functions
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void accumulate( Index order, const VectorND<U,3>& v, T weight, SHExpansion<T>& expansion )
			{
				if ( expansion.getOrder() < order || expansion.getCoefficientCount() < math::square(order+1) )
					expansion.setOrderUninitialized( order );
				
				SH::cartesian( order, v, weight, expansion.getCoefficients() );
			}
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void accumulate( Index order, const VectorND<U,3>& v, T weight, T* expansion )
			{
				SH::cartesian( order, v.x, v.y, v.z, weight, expansion );
			}
			
			
			/// Compute the spherical harmonic expansion for the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static void accumulate( Index order, U x, U y, U z, T weight, T* expansion )
			{
				typedef void (*SHFunction)( U x, U y, U z, T, T* );
				
				//***************************************************************
				// Use the fast generated functions for low-order coefficients.
				
				// Force generation of a jump table.
				const SHFunction functions[MAX_GENERATED_ORDER+1] =
				{
					SHOrder<U,T,0>::accumulate,
					SHOrder<U,T,1>::accumulate,
					SHOrder<U,T,2>::accumulate,
					SHOrder<U,T,3>::accumulate,
					SHOrder<U,T,4>::accumulate,
					SHOrder<U,T,5>::accumulate,
					SHOrder<U,T,6>::accumulate,
					SHOrder<U,T,7>::accumulate,
					SHOrder<U,T,8>::accumulate,
					SHOrder<U,T,9>::accumulate
				};
				
				//***************************************************************
				// For arbitrary order, use the generic but slow implementation.
				
				if ( order <= MAX_GENERATED_ORDER )
					functions[order]( x, y, z, weight, expansion );
				else
				{
					// Use the fast generated functions for low-order coefficients.
					functions[MAX_GENERATED_ORDER]( x, y, z, weight, expansion );
					
					// Convert to spherical coordinates.
					const U theta = math::acos(z);
					const U vxyMag = math::sqrt(x*x + y*y);
					const U t = (vxyMag > U(0)) ? (x / vxyMag) : U(0);
					const U phi = y >= U(0) ? math::acos(t) : (2*math::pi<U>() - math::acos(t));
					const int maxL = (int)order;
					
					for ( int l = (int)MAX_GENERATED_ORDER + 1; l <= maxL; l++ )
					{
						int ll1 = l*(l+1);
						for ( int m = -l; m <= l; m++ )
							expansion[ll1 + m] += weight*T(Y( l, m, theta, phi ));
					}
				}
			}
			
			
		//********************************************************************************
		//******	SH Dot Functions
			
			
			/// Compute the dot product of a spherical harmonic expansion with the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static T dot( Index order, const VectorND<U,3>& v, const SHExpansion<T>& expansion )
			{
				return SH::dot( math::min( order, expansion.getOrder() ), v, expansion.getCoefficients() );
			}
			
			
			/// Compute the dot product of a spherical harmonic expansion with the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static T dot( Index order, const VectorND<U,3>& v, const T* expansion )
			{
				return SH::dot( order, v.x, v.y, v.z, expansion );
			}
			
			
			/// Compute the dot product of a spherical harmonic expansion with the given 3D normalized cartesian vector.
			template < typename U, typename T >
			OM_FORCE_INLINE static T dot( Index order, U x, U y, U z, const T* expansion )
			{
				typedef T (*SHFunction)( U x, U y, U z, const T* );
				
				//***************************************************************
				// Use the fast generated functions for low-order coefficients.
				
				// Force generation of a jump table.
				const SHFunction functions[MAX_GENERATED_ORDER+1] =
				{
					SHOrder<U,T,0>::dot,
					SHOrder<U,T,1>::dot,
					SHOrder<U,T,2>::dot,
					SHOrder<U,T,3>::dot,
					SHOrder<U,T,4>::dot,
					SHOrder<U,T,5>::dot,
					SHOrder<U,T,6>::dot,
					SHOrder<U,T,7>::dot,
					SHOrder<U,T,8>::dot,
					SHOrder<U,T,9>::dot
				};
				
				//***************************************************************
				// For arbitrary order, use the generic but slow implementation.
				
				if ( order <= MAX_GENERATED_ORDER )
					return functions[order]( x, y, z, expansion );
				else
				{
					// Use the fast generated functions for low-order coefficients.
					T result = functions[MAX_GENERATED_ORDER]( x, y, z, expansion );
					
					// Convert to spherical coordinates.
					const U theta = math::acos(z);
					const U vxyMag = math::sqrt(x*x + y*y);
					const U t = (vxyMag > U(0)) ? (x / vxyMag) : U(0);
					const U phi = y >= U(0) ? math::acos(t) : (2*math::pi<U>() - math::acos(t));
					const int maxL = (int)order;
					
					for ( int l = (int)MAX_GENERATED_ORDER + 1; l <= maxL; l++ )
					{
						int ll1 = l*(l+1);
						for ( int m = -l; m <= l; m++ )
							result += expansion[ll1 + m]*T(Y( l, m, theta, phi ));
					}
					
					return result;
				}
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The maximum order for where there is optimized generated code.
			static const Size MAX_GENERATED_ORDER = 9;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Functions
			
			
			template< typename U >
			OM_FORCE_INLINE static U P( int l, int m, U x )
			{
				// Code taken from 'Robin Green - Spherical Harmonic Lighting'.
				U pmm = 1;
				
				if ( m > 0 )
				{
					U somx2 = math::sqrt( (1-x)*(1+x) );
					U fact = 1;
					
					for ( int i = 1; i <= m; i++ )
					{
						pmm *= -fact * somx2;
						fact += 2;
					}
				}
				
				if ( l == m )
					return pmm;
				
				U pmmp1 = x * (2*m+1) * pmm;
				
				if ( l == m+1 )
					return pmmp1;
				
				U pll = U(0.0);
				
				for ( int ll = m+2; ll <= l; ++ll )
				{
					pll = ( (2*ll-1) * x * pmmp1 - (ll+m-1) * pmm ) / (ll-m);
					pmm = pmmp1;
					pmmp1 = pll;
				}
				
				return pll;
			}
			
			
			template< typename U >
			OM_FORCE_INLINE static U K( int l, int m )
			{
				U lMinusM = U(l-m);
				U lPlusM = U(l+m);
				U result = math::sqrt( U((2*l+1) * factorial(lMinusM)) /
								(4*math::pi<U>() * factorial(lPlusM)) );
				
				return result;
			}
			
			
			template< typename U >
			OM_FORCE_INLINE static U Y( int l, int m, U theta, U phi )
			{
				const U sqrt2 = math::sqrt( U(2) );
				
				if ( m == 0 )
					return K<U>(l,0) * P(l,m,math::cos(theta));
				else if ( m > 0 )
					return sqrt2 * K<U>(l,m) * math::cos(m*phi) * P(l,m,math::cos(theta));
				else
					return sqrt2 * K<U>(l,-m) * math::sin(-m*phi) * P(l,-m,math::cos(theta));
			}
			
			
			template< typename U >
			OM_FORCE_INLINE static U factorial( U n )
			{
				U result = U(1.0);
				
				for( U i = U(2.0); i <= n; i++ )
					result *= i;
				
				return result;
			}
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_SPHERICAL_HARMONICS_H
