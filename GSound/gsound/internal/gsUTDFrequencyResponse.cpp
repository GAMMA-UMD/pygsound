/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsUTDFrequencyResponse.cpp
 * Contents:    A function to compute UTD (Uniform Theory of Diffraction) attenuation coefficients.
 * 
 * Author(s):   Carl Schissler, Micah Taylor, Anish Chandak, Christian Lauterbach
 * Website:     http://gamma.cs.unc.edu/GSOUND/
 * 
 * License:
 * 
 *     Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 */


#include "gsUTDFrequencyResponse.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Helper Method Declarations
//############		
//##########################################################################################
//##########################################################################################




GSOUND_FORCE_INLINE static Real UTD_coefficient( Real n, Real k, Real p, Real r, Real thetaI, Real alphaI, Real alphaD );
GSOUND_FORCE_INLINE static Real UTD_alpha( Real beta, Real n, int nSign );
GSOUND_FORCE_INLINE static Real UTD_L( Real p, Real r, Real thetaI );
GSOUND_FORCE_INLINE static int UTD_N( Real beta, Real n, int nSign );
GSOUND_FORCE_INLINE static Real UTD_cotan( Real numer, Real denom );
GSOUND_FORCE_INLINE static math::Complex<Real> UTD_euler( Real x );
GSOUND_FORCE_INLINE static math::Complex<Real> UTD_estimateF( Real X );
GSOUND_FORCE_INLINE static math::Complex<Real> UTD_freqTerm( Real n, Real k, Real thetaI );
GSOUND_FORCE_INLINE static Real UTD_sphereDisKouyoumjian( Real r, Real p );
GSOUND_FORCE_INLINE static Real UTD_sphereDis( Real r, Real p );
GSOUND_FORCE_INLINE static Real cotangent( Real x );
GSOUND_FORCE_INLINE static Real angleBetween( const Vector3f &v1, const Vector3f &v2 );
GSOUND_FORCE_INLINE static Vector3f projectToPlane( const Vector3f& v, const Vector3f& n );




//##########################################################################################
//##########################################################################################
//############		
//############		UTD Attenuation Method
//############		
//##########################################################################################
//##########################################################################################




FrequencyBandResponse computeUTDAttenuation(
										const Vector3f& sourcePosition,
										const Vector3f& diffractionPoint,
										const Vector3f& listenerPosition,
										const Vector3f& sourceFaceNormal,
										const Vector3f& listenerFaceNormal,
										const Vector3f& edgeAxis,
										Real speedOfSound,
										const FrequencyBands& frequencies )
{
	const Vector3f sourceFaceVector = math::cross( edgeAxis, sourceFaceNormal );
	
	const Real n = (Real(2)*math::pi<Real>() - angleBetween(-sourceFaceNormal, listenerFaceNormal)) / math::pi<Real>();
	Vector3f sourceDirection = sourcePosition - diffractionPoint;  //from apex point to source
	Vector3f listenerDirection = listenerPosition - diffractionPoint; // from apex to receiver
	const Real p = sourceDirection.getMagnitude();
	const Real r = listenerDirection.getMagnitude();
	
	if ( p < math::epsilon<Real>() || r < math::epsilon<Real>() )
		return FrequencyBandResponse();
	
	sourceDirection /= p;
	listenerDirection /= r;
	
	Real thetaI = angleBetween( sourceDirection, edgeAxis );
	
	if ( thetaI > math::pi<Real>()*Real(0.5) )
		thetaI = math::pi<Real>() - thetaI;
	
	// Project source and receiver position to plane perpindicular to edge vector.
	const Vector3f sDir = projectToPlane(sourceDirection, edgeAxis);
	const Vector3f rDir = projectToPlane(listenerDirection, edgeAxis);

	// Get the angles around the wedge for source and receiver, measured from source face.
	const Real alphaI = angleBetween(-sDir, sourceFaceVector);
	const Real alphaD = angleBetween(rDir, sourceFaceVector) + math::pi<Real>();
	const Real alphaSB = alphaI + math::pi<Real>() + Real(0.001);
	const Real lerp = (n*math::pi<Real>() - alphaD) / (n*math::pi<Real>() - alphaSB);
	
	FrequencyBandResponse result;
	
	for ( Index i = 0; i < frequencies.getBandCount(); i++ )
	{
		Real lambda = speedOfSound / frequencies[i];
		Real k = Real(2)*math::pi<Real>() / lambda;
		
		// Shadow boundary normalization proposed by Tsingos 2001.
		Real utdCoeff = UTD_coefficient( n, k, p, r, thetaI, alphaI, alphaD );
		Real sbCoeff = UTD_coefficient( n, k, p, r, thetaI, alphaI, alphaSB );
		Real normCoeff = utdCoeff / sbCoeff;
		Real finalCoeff = (1.0f - lerp)*utdCoeff + (lerp)*normCoeff;
		
		// Square to convert to intensity from pressure.
		result[i] = math::clamp( finalCoeff*finalCoeff, Real(0), Real(1) );
		//result[i] = math::clamp( normCoeff*normCoeff, Real(0), Real(1) );
	}
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		UTD Helper Methods
//############		
//##########################################################################################
//##########################################################################################




GSOUND_FORCE_INLINE static Real UTD_coefficient( Real n, Real k, Real p, Real r, Real thetaI, Real alphaI, Real alphaD )
{
	math::Complex<Real> c1 = UTD_freqTerm( n, k, thetaI );
	math::Complex<Real> F1 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD-alphaI, n, 1) );
	math::Complex<Real> F2 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD-alphaI, n, -1) );
	math::Complex<Real> F3 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD+alphaI, n, 1) );
	math::Complex<Real> F4 = UTD_estimateF( k * UTD_L(p, r, thetaI) * UTD_alpha(alphaD+alphaI, n, -1) );
	
	Real cot1 = UTD_cotan( math::pi<Real>() + (alphaD - alphaI), Real(2)*n );
	Real cot2 = UTD_cotan( math::pi<Real>() - (alphaD - alphaI), Real(2)*n );
	Real cot3 = UTD_cotan( math::pi<Real>() + (alphaD + alphaI), Real(2)*n );
	Real cot4 = UTD_cotan( math::pi<Real>() - (alphaD + alphaI), Real(2)*n );

	math::Complex<Real> coeff = F1*cot1 + F2*cot2 + F3*cot3 + F4*cot4;
	coeff = coeff * c1;
	coeff = UTD_euler(-k*r) * coeff;
	
	// Kouyoumjian calls for a different distance term here
	// I have used the one Tsingos used
	coeff *= math::sqrt( UTD_sphereDisKouyoumjian( r, p ) );
	
	return coeff.getMagnitude();
}




GSOUND_FORCE_INLINE static Real UTD_alpha( Real beta, Real n, int nSign )
{
	int N = UTD_N(beta, n, nSign);
	Real numer = Real(2)*math::pi<Real>()*n*Real(N) - beta;
	Real denom = 2;
	Real cosine = math::cos(numer/denom);
	Real alpha = Real(2) * cosine * cosine;
	
	return alpha;
}




GSOUND_FORCE_INLINE static Real UTD_L( Real p, Real r, Real thetaI )
{
	Real sine = math::sin(thetaI);
	Real L = UTD_sphereDis(r,p) * sine * sine;
	
	return L;
}




GSOUND_FORCE_INLINE static int UTD_N( Real beta, Real n, int nSign )
{
	if ( nSign > 0 )
	{
		if ( beta <= math::pi<Real>()*(n-1) )
			return 0;
		else  // beta > Pi*(n-1)
			return 1;
	}
	else
	{
		if ( beta < math::pi<Real>()*(1-n) )
			return -1;
		else if ( beta >= math::pi<Real>()*(1-n) && beta <= math::pi<Real>()*(1+n) ) // pi(1-n) <= beta <= pi(1+n)
			return 0;
		else // beta > pi*(1+n)
			return 1;
	}
}




GSOUND_FORCE_INLINE static Real UTD_cotan( Real numer, Real denom )
{
	return cotangent( numer / denom );
}




GSOUND_FORCE_INLINE static math::Complex<Real> UTD_euler( Real x )
{
	return math::Complex<Real>( math::cos(x), math::sin(x) );
}




GSOUND_FORCE_INLINE static math::Complex<Real> UTD_estimateF( Real X )
{
	Real numerPhase = X;
	Real denomPhase = X + Real(1.4);
	math::Complex<Real> phaseTerm = UTD_euler(math::pi<Real>()*Real(0.25) * math::sqrt(numerPhase/denomPhase));
	math::Complex<Real> F;

	if ( X < Real(0.8) )
	{
		Real numer = math::sqrt(X);
		Real denom = Real(0.7)*math::sqrt(X) + Real(1.2);
		F = phaseTerm * math::sqrt(math::pi<Real>()*X) * (1 - numer/denom);
	}
	else
	{
		Real numer = Real(0.8);
		Real denom = math::square(X + Real(1.25));
		F = (Real(1) - numer/denom) * phaseTerm;
	}
	
	return F;
}




GSOUND_FORCE_INLINE static math::Complex<Real> UTD_freqTerm( Real n, Real k, Real thetaI )
{
	math::Complex<Real> numer = UTD_euler( -math::pi<Real>()*Real(0.25) );
	Real denom = Real(2) * n * math::sqrt(Real(2)*math::pi<Real>()*k) * math::sin( thetaI );
	
	return -numer / denom;
}




GSOUND_FORCE_INLINE static Real UTD_sphereDisKouyoumjian( Real r, Real p )
{
	Real numer = p;
	Real denom = r*(p + r);
	
	return numer / denom;
}




GSOUND_FORCE_INLINE static Real UTD_sphereDis( Real r, Real p )
{
	Real numer = p * r;
	Real denom = p + r;
	
	return numer / denom;
}




GSOUND_FORCE_INLINE static Real cotangent( Real x )
{
	if ( math::abs(x) < math::epsilon<Real>() )
		return math::max<Real>();
	
	return Real(1) / math::tan(x);
}





GSOUND_FORCE_INLINE static Real angleBetween( const Vector3f &v1, const Vector3f &v2 )
{
	Real angle = math::dot( v1.normalize(), v2.normalize() );
	
	if ( angle > Real(1) )
		return Real(0);
	else if ( angle < -Real(1) )
		return math::pi<Real>();
	
	return math::acos(angle);
}




GSOUND_FORCE_INLINE static Vector3f projectToPlane( const Vector3f& v, const Vector3f& n )
{
	Real projAmount = math::dot(v, n);
	
	return v - projAmount * n;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
