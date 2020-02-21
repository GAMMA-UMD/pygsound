/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundDirectivity.cpp
 * Contents:    gsound::SoundDirectivity class implementation
 * 
 * Author(s):   Carl Schissler
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


#include "gsSoundDirectivity.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundDirectivity:: SoundDirectivity()
	:	orientation( Matrix3f::IDENTITY ),
		timeStamp( math::random<Index>() )
{
}




SoundDirectivity:: SoundDirectivity( const UTF8String& newName )
	:	orientation( Matrix3f::IDENTITY ),
		name( newName ),
		timeStamp( math::random<Index>() )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundDirectivity:: setSample( Index sampleIndex, const FrequencyResponse& newResponse )
{
	if ( sampleIndex < samples.getSize() )
	{
		samples[sampleIndex].response = newResponse;
		timeStamp++;
	}
}




void SoundDirectivity:: addSample( const Vector3f& direction, const FrequencyResponse& response )
{
	samples.add( Sample( direction, response ) );
	timeStamp++;
}





void SoundDirectivity:: reset()
{
	samples.clear();
	timeStamp++;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Orientation Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundDirectivity:: setOrientation( const Matrix3f& newOrientation )
{
	orientation = newOrientation.orthonormalize();
	timeStamp++;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Predefined Sound Directivity Initializers
//############		
//##########################################################################################
//##########################################################################################




#define DIRECTIVITY( functionName, responses ) \
GSOUND_INLINE static SoundDirectivity new##functionName##Directivity()\
{\
	SoundDirectivity directivity( #functionName );\
	FrequencyResponse response;\
	responses;\
	return directivity;\
}\

#define NEW_DIRECTIVITY( functionName ) new##functionName##Directivity()


#define F( f, a ) response.setFrequency( f, a );
#define RESPONSE( direction, responses ) { response.reset(); responses; directivity.addSample( direction, response ); }
#define XZ_POLAR( theta ) Vector3f::polarXZ( math::degreesToRadians( Float(theta) ) )
#define YZ_POLAR( theta ) Vector3f::polarYZ( math::degreesToRadians( Float(theta) ) )
#define SPHERICAL( theta, phi ) Matrix3f(0,0,1,0,1,0,1,0,0)*Vector3f::spherical( math::degreesToRadians( Float(theta) ), math::degreesToRadians( Float(phi) ) )




//##########################################################################################
//##########################################################################################
//############		
//############		Human Voice Directivity
//############		
//##########################################################################################
//##########################################################################################



// Data from "Acoustics and the Performance of Music", Jürgen Meyer 1978
// Horizontal, from front to back.
#define HUMAN_VOICE_0_0 F(125.0f,1.15f) F(250.0f,1.29f) F(500.0f,1.33f) F(1000.0f,1.02f) F(2000.0f,1.23f) F(4000.0f,1.66f) F(8000.0f,1.97f)
#define HUMAN_VOICE_0_20 F(125.0f,1.25f) F(250.0f,1.41f) F(500.0f,1.40f) F(1000.0f,1.08f) F(2000.0f,1.38f) F(4000.0f,1.62f) F(8000.0f,2.0f)
#define HUMAN_VOICE_0_40 F(125.0f,1.11f) F(250.0f,1.24f) F(500.0f,1.33f) F(1000.0f,1.15f) F(2000.0f,1.62f) F(4000.0f,1.32f) F(8000.0f,1.97f)
#define HUMAN_VOICE_0_60 F(125.0f,0.91f) F(250.0f,1.06f) F(500.0f,1.14f) F(1000.0f,1.06f) F(2000.0f,1.41f) F(4000.0f,1.17f) F(8000.0f,1.14f)
#define HUMAN_VOICE_0_80 F(125.0f,0.99f) F(250.0f,1.02f) F(500.0f,1.15f) F(1000.0f,1.23f) F(2000.0f,1.08f) F(4000.0f,1.40f) F(8000.0f,1.12f)
#define HUMAN_VOICE_0_90 F(125.0f,0.86f) F(250.0f,0.91f) F(500.0f,1.0f) F(1000.0f,1.20f) F(2000.0f,0.88f) F(4000.0f,1.04f) F(8000.0f,0.67f)
#define HUMAN_VOICE_0_100 F(125.0f,0.94f) F(250.0f,0.96f) F(500.0f,0.94f) F(1000.0f,1.13f) F(2000.0f,0.85f) F(4000.0f,0.97f) F(8000.0f,0.61f)
#define HUMAN_VOICE_0_120 F(125.0f,0.95f) F(250.0f,0.86f) F(500.0f,0.76f) F(1000.0f,0.90f) F(2000.0f,0.71f) F(4000.0f,0.72f) F(8000.0f,0.45f)
#define HUMAN_VOICE_0_140 F(125.0f,0.84f) F(250.0f,0.69f) F(500.0f,0.55f) F(1000.0f,0.62f) F(2000.0f,0.48f) F(4000.0f,0.43f) F(8000.0f,0.25f)
#define HUMAN_VOICE_0_160 F(125.0f,0.71f) F(250.0f,0.65f) F(500.0f,0.48f) F(1000.0f,0.46f) F(2000.0f,0.25f) F(4000.0f,0.28f) F(8000.0f,0.12f)
#define HUMAN_VOICE_0_180 F(125.0f,0.78f) F(250.0f,0.68f) F(500.0f,0.47f) F(1000.0f,0.50f) F(2000.0f,0.27f) F(4000.0f,0.25f) F(8000.0f,0.09f)

// Vertical, from front to back, overhead.
#define HUMAN_VOICE__40_0 F(125.0f,1.07f) F(250.0f,1.18f) F(500.0f,1.31f) F(1000.0f,1.48f) F(2000.0f,0.90f) F(4000.0f,1.10f) F(8000.0f,1.42f)
#define HUMAN_VOICE__20_0 F(125.0f,1.33f) F(250.0f,1.35f) F(500.0f,1.35f) F(1000.0f,1.45f) F(2000.0f,2.0f) F(4000.0f,1.94f) F(8000.0f,2.30f)
#define HUMAN_VOICE_20_0 F(125.0f,1.18f) F(250.0f,1.29f) F(500.0f,1.18f) F(1000.0f,1.08f) F(2000.0f,1.28f) F(4000.0f,1.65f) F(8000.0f,1.80f)
#define HUMAN_VOICE_40_0 F(125.0f,1.48f) F(250.0f,1.42f) F(500.0f,1.10f) F(1000.0f,1.28f) F(2000.0f,1.33f) F(4000.0f,1.35f) F(8000.0f,1.40f)
#define HUMAN_VOICE_60_0 F(125.0f,1.42f) F(250.0f,1.25f) F(500.0f,1.0f) F(1000.0f,1.05f) F(2000.0f,1.23f) F(4000.0f,1.13f) F(8000.0f,1.13f)
#define HUMAN_VOICE_90_0 F(125.0f,1.08f) F(250.0f,1.08f) F(500.0f,0.98f) F(1000.0f,0.70f) F(2000.0f,0.78f) F(4000.0f,0.84f) F(8000.0f,0.54f)
#define HUMAN_VOICE_120_0 F(125.0f,1.05f) F(250.0f,0.98f) F(500.0f,0.90f) F(1000.0f,0.91f) F(2000.0f,0.77f) F(4000.0f,0.60f) F(8000.0f,0.34f)
#define HUMAN_VOICE_140_0 F(125.0f,1.0f) F(250.0f,0.88f) F(500.0f,0.76f) F(1000.0f,0.91f) F(2000.0f,0.43f) F(4000.0f,0.29f) F(8000.0f,0.17f)
#define HUMAN_VOICE_160_0 F(125.0f,0.74f) F(250.0f,0.66f) F(500.0f,0.58f) F(1000.0f,0.58f) F(2000.0f,0.31f) F(4000.0f,0.24f) F(8000.0f,0.11f)
#define HUMAN_VOICE_200_0 F(125.0f,0.84f) F(250.0f,0.67f) F(500.0f,0.52f) F(1000.0f,0.44f) F(2000.0f,0.31f) F(4000.0f,0.21f) F(8000.0f,0.08f)
#define HUMAN_VOICE_220_0 F(125.0f,0.78f) F(250.0f,0.78f) F(500.0f,0.53f) F(1000.0f,0.63f) F(2000.0f,0.21f) F(4000.0f,0.13f) F(8000.0f,0.06f)


DIRECTIVITY( HumanVoice,
	// Horizontal, from front to back.
	RESPONSE( XZ_POLAR(0), HUMAN_VOICE_0_0 )
	RESPONSE( XZ_POLAR(20), HUMAN_VOICE_0_20 ) RESPONSE( XZ_POLAR(-20), HUMAN_VOICE_0_20 )
	RESPONSE( XZ_POLAR(40), HUMAN_VOICE_0_40 ) RESPONSE( XZ_POLAR(-40), HUMAN_VOICE_0_40 )
	RESPONSE( XZ_POLAR(60), HUMAN_VOICE_0_60 ) RESPONSE( XZ_POLAR(-60), HUMAN_VOICE_0_60 )
	RESPONSE( XZ_POLAR(80), HUMAN_VOICE_0_80 ) RESPONSE( XZ_POLAR(-80), HUMAN_VOICE_0_80 )
	RESPONSE( XZ_POLAR(90), HUMAN_VOICE_0_90 ) RESPONSE( XZ_POLAR(-90), HUMAN_VOICE_0_90 )
	RESPONSE( XZ_POLAR(100), HUMAN_VOICE_0_100 ) RESPONSE( XZ_POLAR(-100), HUMAN_VOICE_0_100 )
	RESPONSE( XZ_POLAR(120), HUMAN_VOICE_0_120 ) RESPONSE( XZ_POLAR(-120), HUMAN_VOICE_0_120 )
	RESPONSE( XZ_POLAR(140), HUMAN_VOICE_0_140 ) RESPONSE( XZ_POLAR(-140), HUMAN_VOICE_0_140 )
	RESPONSE( XZ_POLAR(160), HUMAN_VOICE_0_160 ) RESPONSE( XZ_POLAR(-160), HUMAN_VOICE_0_160 )
	RESPONSE( XZ_POLAR(180), HUMAN_VOICE_0_180 )
	// Vertical, from front to back, overhead.
	RESPONSE( YZ_POLAR(-40), HUMAN_VOICE__40_0 )
	RESPONSE( YZ_POLAR(-20), HUMAN_VOICE__20_0 )
	RESPONSE( YZ_POLAR(20), HUMAN_VOICE_40_0 )
	RESPONSE( YZ_POLAR(40), HUMAN_VOICE_40_0 )
	RESPONSE( YZ_POLAR(60), HUMAN_VOICE_60_0 )
	RESPONSE( YZ_POLAR(90), HUMAN_VOICE_90_0 )
	RESPONSE( YZ_POLAR(120), HUMAN_VOICE_120_0 )
	RESPONSE( YZ_POLAR(140), HUMAN_VOICE_140_0 )
	RESPONSE( YZ_POLAR(160), HUMAN_VOICE_160_0 )
	RESPONSE( YZ_POLAR(200), HUMAN_VOICE_200_0 )
	RESPONSE( YZ_POLAR(220), HUMAN_VOICE_220_0 )
);

const SoundDirectivity SoundDirectivity:: HUMAN_VOICE = NEW_DIRECTIVITY( HumanVoice );




//##########################################################################################
//##########################################################################################
//############		
//############		Trumpet Directivity
//############		
//##########################################################################################
//##########################################################################################



// Data from "Acoustics and the Performance of Music", Jürgen Meyer 1978
// Around bell axis, from front to back.
#define TRUMPET_0_0 F(2000.0f,2.30f) F(6000.0f,4.40f) F(10000.0f,4.70f) F(15000.0f,6.60f)
#define TRUMPET_0_10 F(2000.0f,2.21f) F(6000.0f,3.85f) F(10000.0f,4.40f) F(15000.0f,4.40f)
#define TRUMPET_0_20 F(2000.0f,1.92f) F(6000.0f,3.18f) F(10000.0f,3.35f) F(15000.0f,3.05f)
#define TRUMPET_0_30 F(2000.0f,1.85f) F(6000.0f,2.35f) F(10000.0f,1.85f) F(15000.0f,1.60f)
#define TRUMPET_0_40 F(2000.0f,1.78f) F(6000.0f,1.30f) F(10000.0f,1.10f) F(15000.0f,0.87f)
#define TRUMPET_0_50 F(2000.0f,1.30f) F(6000.0f,0.86f) F(10000.0f,0.75f) F(15000.0f,0.65f)
#define TRUMPET_0_60 F(2000.0f,1.10f) F(6000.0f,0.60f) F(10000.0f,0.50f) F(15000.0f,0.56f)
#define TRUMPET_0_70 F(2000.0f,0.97f) F(6000.0f,0.39f) F(10000.0f,0.47f) F(15000.0f,0.51f)
#define TRUMPET_0_80 F(2000.0f,0.85f) F(6000.0f,0.24f) F(10000.0f,0.32f) F(15000.0f,0.46f)
#define TRUMPET_0_90 F(2000.0f,0.75f) F(6000.0f,0.15f) F(10000.0f,0.22f) F(15000.0f,0.28f)


DIRECTIVITY( Trumpet,
	// Horizontal, from front to back.
	RESPONSE( XZ_POLAR(0), TRUMPET_0_0 )
	RESPONSE( XZ_POLAR(10), TRUMPET_0_10 ) RESPONSE( XZ_POLAR(-10), TRUMPET_0_10 )
	RESPONSE( XZ_POLAR(20), TRUMPET_0_20 ) RESPONSE( XZ_POLAR(-20), TRUMPET_0_20 )
	RESPONSE( XZ_POLAR(30), TRUMPET_0_30 ) RESPONSE( XZ_POLAR(-30), TRUMPET_0_30 )
	RESPONSE( XZ_POLAR(40), TRUMPET_0_40 ) RESPONSE( XZ_POLAR(-40), TRUMPET_0_40 )
	RESPONSE( XZ_POLAR(50), TRUMPET_0_50 ) RESPONSE( XZ_POLAR(-50), TRUMPET_0_50 )
	RESPONSE( XZ_POLAR(60), TRUMPET_0_60 ) RESPONSE( XZ_POLAR(-60), TRUMPET_0_60 )
	RESPONSE( XZ_POLAR(70), TRUMPET_0_70 ) RESPONSE( XZ_POLAR(-70), TRUMPET_0_70 )
	RESPONSE( XZ_POLAR(80), TRUMPET_0_80 ) RESPONSE( XZ_POLAR(-80), TRUMPET_0_80 )
	RESPONSE( XZ_POLAR(90), TRUMPET_0_90 ) RESPONSE( XZ_POLAR(-90), TRUMPET_0_90 )
	// Vertical, from front to back, overhead and underneath.
	RESPONSE( YZ_POLAR(10), TRUMPET_0_10 ) RESPONSE( YZ_POLAR(-10), TRUMPET_0_10 )
	RESPONSE( YZ_POLAR(20), TRUMPET_0_20 ) RESPONSE( YZ_POLAR(-20), TRUMPET_0_20 )
	RESPONSE( YZ_POLAR(30), TRUMPET_0_30 ) RESPONSE( YZ_POLAR(-30), TRUMPET_0_30 )
	RESPONSE( YZ_POLAR(40), TRUMPET_0_40 ) RESPONSE( YZ_POLAR(-40), TRUMPET_0_40 )
	RESPONSE( YZ_POLAR(50), TRUMPET_0_50 ) RESPONSE( YZ_POLAR(-50), TRUMPET_0_50 )
	RESPONSE( YZ_POLAR(60), TRUMPET_0_60 ) RESPONSE( YZ_POLAR(-60), TRUMPET_0_60 )
	RESPONSE( YZ_POLAR(70), TRUMPET_0_70 ) RESPONSE( YZ_POLAR(-70), TRUMPET_0_70 )
	RESPONSE( YZ_POLAR(80), TRUMPET_0_80 ) RESPONSE( YZ_POLAR(-80), TRUMPET_0_80 )
	RESPONSE( YZ_POLAR(90), TRUMPET_0_90 ) RESPONSE( YZ_POLAR(-90), TRUMPET_0_90 )
);

const SoundDirectivity SoundDirectivity:: TRUMPET = NEW_DIRECTIVITY( Trumpet );




//##########################################################################################
//##########################################################################################
//############		
//############		Trombone Directivity
//############		
//##########################################################################################
//##########################################################################################



// Data from "Acoustics and the Performance of Music", Jürgen Meyer 1978
// Symmetric around bell axis, from front to back.
#define TROMBONE_0_0 F(500.0f,1.60f) F(1000.0f,2.10f) F(3000.0f,4.50f) F(10000.0f,6.10f)
#define TROMBONE_0_10 F(500.0f,1.59f) F(1000.0f,2.05f) F(3000.0f,3.90f) F(10000.0f,5.15f)
#define TROMBONE_0_20 F(500.0f,1.55f) F(1000.0f,1.85f) F(3000.0f,3.0f) F(10000.0f,3.20f)
#define TROMBONE_0_30 F(500.0f,1.51f) F(1000.0f,1.60f) F(3000.0f,2.0f) F(10000.0f,1.67f)
#define TROMBONE_0_40 F(500.0f,1.47f) F(1000.0f,1.36f) F(3000.0f,1.30f) F(10000.0f,1.21f)
#define TROMBONE_0_50 F(500.0f,1.32f) F(1000.0f,1.22f) F(3000.0f,0.95f) F(10000.0f,0.50f)
#define TROMBONE_0_60 F(500.0f,1.18f) F(1000.0f,1.00f) F(3000.0f,0.53f) F(10000.0f,0.27f)
#define TROMBONE_0_70 F(500.0f,1.05f) F(1000.0f,0.90f) F(3000.0f,0.53f) F(10000.0f,0.23f)
#define TROMBONE_0_80 F(500.0f,0.94f) F(1000.0f,0.84f) F(3000.0f,0.54f) F(10000.0f,0.29f)
#define TROMBONE_0_90 F(500.0f,0.84f) F(1000.0f,0.73f) F(3000.0f,0.44f) F(10000.0f,0.30f)


DIRECTIVITY( Trombone,
	// Horizontal, from front to back.
	RESPONSE( XZ_POLAR(0), TROMBONE_0_0 )
	RESPONSE( XZ_POLAR(10), TROMBONE_0_10 ) RESPONSE( XZ_POLAR(-10), TROMBONE_0_10 )
	RESPONSE( XZ_POLAR(20), TROMBONE_0_20 ) RESPONSE( XZ_POLAR(-20), TROMBONE_0_20 )
	RESPONSE( XZ_POLAR(30), TROMBONE_0_30 ) RESPONSE( XZ_POLAR(-30), TROMBONE_0_30 )
	RESPONSE( XZ_POLAR(40), TROMBONE_0_40 ) RESPONSE( XZ_POLAR(-40), TROMBONE_0_40 )
	RESPONSE( XZ_POLAR(50), TROMBONE_0_50 ) RESPONSE( XZ_POLAR(-50), TROMBONE_0_50 )
	RESPONSE( XZ_POLAR(60), TROMBONE_0_60 ) RESPONSE( XZ_POLAR(-60), TROMBONE_0_60 )
	RESPONSE( XZ_POLAR(70), TROMBONE_0_70 ) RESPONSE( XZ_POLAR(-70), TROMBONE_0_70 )
	RESPONSE( XZ_POLAR(80), TROMBONE_0_80 ) RESPONSE( XZ_POLAR(-80), TROMBONE_0_80 )
	RESPONSE( XZ_POLAR(90), TROMBONE_0_90 ) RESPONSE( XZ_POLAR(-90), TROMBONE_0_90 )
	// Vertical, from front to back, overhead and underneath.
	RESPONSE( YZ_POLAR(10), TROMBONE_0_10 ) RESPONSE( YZ_POLAR(-10), TROMBONE_0_10 )
	RESPONSE( YZ_POLAR(20), TROMBONE_0_20 ) RESPONSE( YZ_POLAR(-20), TROMBONE_0_20 )
	RESPONSE( YZ_POLAR(30), TROMBONE_0_30 ) RESPONSE( YZ_POLAR(-30), TROMBONE_0_30 )
	RESPONSE( YZ_POLAR(40), TROMBONE_0_40 ) RESPONSE( YZ_POLAR(-40), TROMBONE_0_40 )
	RESPONSE( YZ_POLAR(50), TROMBONE_0_50 ) RESPONSE( YZ_POLAR(-50), TROMBONE_0_50 )
	RESPONSE( YZ_POLAR(60), TROMBONE_0_60 ) RESPONSE( YZ_POLAR(-60), TROMBONE_0_60 )
	RESPONSE( YZ_POLAR(70), TROMBONE_0_70 ) RESPONSE( YZ_POLAR(-70), TROMBONE_0_70 )
	RESPONSE( YZ_POLAR(80), TROMBONE_0_80 ) RESPONSE( YZ_POLAR(-80), TROMBONE_0_80 )
	RESPONSE( YZ_POLAR(90), TROMBONE_0_90 ) RESPONSE( YZ_POLAR(-90), TROMBONE_0_90 )
);

const SoundDirectivity SoundDirectivity:: TROMBONE = NEW_DIRECTIVITY( Trombone );




//##########################################################################################
//##########################################################################################
//############		
//############		JBL LSR4328P Directivity
//############		
//##########################################################################################
//##########################################################################################




DIRECTIVITY( JBL_LSR_4328P,
	RESPONSE( SPHERICAL(170.0f,0.0f), F(63.5,0.26) F(125.0,0.27) F(250.0,0.30) F(500.0,0.21) F(1000.0,0.15) F(2000.0,0.14) F(4000.0,0.19) F(8000.0,0.10) )
	RESPONSE( SPHERICAL(170.0f,30.0f), F(63.5,0.23) F(125.0,0.25) F(250.0,0.29) F(500.0,0.25) F(1000.0,0.14) F(2000.0,0.10) F(4000.0,0.12) F(8000.0,0.06) )
	RESPONSE( SPHERICAL(170.0f,60.0f), F(63.5,0.19) F(125.0,0.21) F(250.0,0.24) F(500.0,0.18) F(1000.0,0.10) F(2000.0,0.07) F(4000.0,0.12) F(8000.0,0.06) )
	RESPONSE( SPHERICAL(170.0f,90.0f), F(63.5,0.18) F(125.0,0.17) F(250.0,0.15) F(500.0,0.08) F(1000.0,0.13) F(2000.0,0.09) F(4000.0,0.16) F(8000.0,0.15) )
	RESPONSE( SPHERICAL(170.0f,120.0f), F(63.5,0.25) F(125.0,0.21) F(250.0,0.11) F(500.0,0.19) F(1000.0,0.14) F(2000.0,0.06) F(4000.0,0.12) F(8000.0,0.10) )
	RESPONSE( SPHERICAL(170.0f,150.0f), F(63.5,0.30) F(125.0,0.50) F(250.0,0.69) F(500.0,0.38) F(1000.0,0.29) F(2000.0,0.18) F(4000.0,0.14) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(170.0f,180.0f), F(63.5,0.38) F(125.0,0.54) F(250.0,0.68) F(500.0,0.35) F(1000.0,0.28) F(2000.0,0.24) F(4000.0,0.25) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(170.0f,210.0f), F(63.5,0.37) F(125.0,0.53) F(250.0,0.69) F(500.0,0.36) F(1000.0,0.29) F(2000.0,0.22) F(4000.0,0.21) F(8000.0,0.08) )
	RESPONSE( SPHERICAL(170.0f,240.0f), F(63.5,0.26) F(125.0,0.47) F(250.0,0.66) F(500.0,0.37) F(1000.0,0.27) F(2000.0,0.16) F(4000.0,0.11) F(8000.0,0.06) )
	RESPONSE( SPHERICAL(170.0f,270.0f), F(63.5,0.03) F(125.0,0.05) F(250.0,0.10) F(500.0,0.10) F(1000.0,0.05) F(2000.0,0.06) F(4000.0,0.08) F(8000.0,0.05) )
	RESPONSE( SPHERICAL(170.0f,300.0f), F(63.5,0.26) F(125.0,0.24) F(250.0,0.17) F(500.0,0.05) F(1000.0,0.04) F(2000.0,0.06) F(4000.0,0.10) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(170.0f,330.0f), F(63.5,0.25) F(125.0,0.25) F(250.0,0.25) F(500.0,0.19) F(1000.0,0.16) F(2000.0,0.10) F(4000.0,0.16) F(8000.0,0.08) )
	RESPONSE( SPHERICAL(130.0f,0.0f), F(63.5,0.05) F(125.0,0.10) F(250.0,0.17) F(500.0,0.16) F(1000.0,0.15) F(2000.0,0.12) F(4000.0,0.11) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(130.0f,30.0f), F(63.5,0.08) F(125.0,0.10) F(250.0,0.13) F(500.0,0.08) F(1000.0,0.05) F(2000.0,0.05) F(4000.0,0.07) F(8000.0,0.08) )
	RESPONSE( SPHERICAL(130.0f,60.0f), F(63.5,0.21) F(125.0,0.20) F(250.0,0.16) F(500.0,0.13) F(1000.0,0.10) F(2000.0,0.05) F(4000.0,0.09) F(8000.0,0.05) )
	RESPONSE( SPHERICAL(130.0f,90.0f), F(63.5,0.08) F(125.0,0.07) F(250.0,0.07) F(500.0,0.19) F(1000.0,0.13) F(2000.0,0.15) F(4000.0,0.16) F(8000.0,0.14) )
	RESPONSE( SPHERICAL(130.0f,120.0f), F(63.5,0.31) F(125.0,0.65) F(250.0,0.98) F(500.0,0.67) F(1000.0,0.39) F(2000.0,0.24) F(4000.0,0.20) F(8000.0,0.09) )
	RESPONSE( SPHERICAL(130.0f,150.0f), F(63.5,0.43) F(125.0,0.85) F(250.0,1.26) F(500.0,0.90) F(1000.0,0.69) F(2000.0,0.43) F(4000.0,0.34) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(130.0f,180.0f), F(63.5,0.49) F(125.0,0.96) F(250.0,1.43) F(500.0,1.08) F(1000.0,0.93) F(2000.0,0.58) F(4000.0,0.32) F(8000.0,0.23) )
	RESPONSE( SPHERICAL(130.0f,210.0f), F(63.5,0.43) F(125.0,0.86) F(250.0,1.27) F(500.0,0.92) F(1000.0,0.71) F(2000.0,0.45) F(4000.0,0.33) F(8000.0,0.18) )
	RESPONSE( SPHERICAL(130.0f,240.0f), F(63.5,0.31) F(125.0,0.65) F(250.0,0.97) F(500.0,0.66) F(1000.0,0.40) F(2000.0,0.25) F(4000.0,0.22) F(8000.0,0.11) )
	RESPONSE( SPHERICAL(130.0f,270.0f), F(63.5,0.26) F(125.0,0.23) F(250.0,0.16) F(500.0,0.07) F(1000.0,0.06) F(2000.0,0.11) F(4000.0,0.10) F(8000.0,0.09) )
	RESPONSE( SPHERICAL(130.0f,300.0f), F(63.5,0.03) F(125.0,0.05) F(250.0,0.08) F(500.0,0.15) F(1000.0,0.10) F(2000.0,0.09) F(4000.0,0.12) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(130.0f,330.0f), F(63.5,0.09) F(125.0,0.07) F(250.0,0.04) F(500.0,0.13) F(1000.0,0.10) F(2000.0,0.07) F(4000.0,0.11) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(90.0f,0.0f), F(63.5,0.12) F(125.0,0.15) F(250.0,0.20) F(500.0,0.14) F(1000.0,0.09) F(2000.0,0.08) F(4000.0,0.12) F(8000.0,0.08) )
	RESPONSE( SPHERICAL(90.0f,30.0f), F(63.5,0.02) F(125.0,0.02) F(250.0,0.05) F(500.0,0.16) F(1000.0,0.17) F(2000.0,0.11) F(4000.0,0.16) F(8000.0,0.14) )
	RESPONSE( SPHERICAL(90.0f,60.0f), F(63.5,0.02) F(125.0,0.04) F(250.0,0.08) F(500.0,0.17) F(1000.0,0.13) F(2000.0,0.12) F(4000.0,0.12) F(8000.0,0.13) )
	RESPONSE( SPHERICAL(90.0f,90.0f), F(63.5,0.04) F(125.0,0.08) F(250.0,0.14) F(500.0,0.13) F(1000.0,0.10) F(2000.0,0.19) F(4000.0,0.21) F(8000.0,0.16) )
	RESPONSE( SPHERICAL(90.0f,120.0f), F(63.5,0.04) F(125.0,0.09) F(250.0,0.16) F(500.0,0.20) F(1000.0,0.16) F(2000.0,0.11) F(4000.0,0.22) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(90.0f,150.0f), F(63.5,0.26) F(125.0,0.61) F(250.0,1.03) F(500.0,1.07) F(1000.0,0.76) F(2000.0,0.70) F(4000.0,0.73) F(8000.0,0.50) )
	RESPONSE( SPHERICAL(90.0f,180.0f), F(63.5,0.29) F(125.0,0.69) F(250.0,1.15) F(500.0,1.24) F(1000.0,1.00) F(2000.0,0.97) F(4000.0,0.98) F(8000.0,0.92) )
	RESPONSE( SPHERICAL(90.0f,210.0f), F(63.5,0.24) F(125.0,0.55) F(250.0,0.93) F(500.0,0.97) F(1000.0,0.65) F(2000.0,0.58) F(4000.0,0.60) F(8000.0,0.38) )
	RESPONSE( SPHERICAL(90.0f,240.0f), F(63.5,0.19) F(125.0,0.42) F(250.0,0.69) F(500.0,0.68) F(1000.0,0.36) F(2000.0,0.26) F(4000.0,0.28) F(8000.0,0.12) )
	RESPONSE( SPHERICAL(90.0f,270.0f), F(63.5,0.15) F(125.0,0.19) F(250.0,0.22) F(500.0,0.13) F(1000.0,0.13) F(2000.0,0.13) F(4000.0,0.08) F(8000.0,0.11) )
	RESPONSE( SPHERICAL(90.0f,300.0f), F(63.5,0.18) F(125.0,0.18) F(250.0,0.17) F(500.0,0.07) F(1000.0,0.07) F(2000.0,0.07) F(4000.0,0.12) F(8000.0,0.09) )
	RESPONSE( SPHERICAL(90.0f,330.0f), F(63.5,0.14) F(125.0,0.19) F(250.0,0.25) F(500.0,0.23) F(1000.0,0.10) F(2000.0,0.10) F(4000.0,0.13) F(8000.0,0.08) )
	RESPONSE( SPHERICAL(50.0f,0.0f), F(63.5,0.50) F(125.0,0.45) F(250.0,0.32) F(500.0,0.37) F(1000.0,0.23) F(2000.0,0.23) F(4000.0,0.18) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(50.0f,30.0f), F(63.5,0.03) F(125.0,0.05) F(250.0,0.07) F(500.0,0.08) F(1000.0,0.18) F(2000.0,0.12) F(4000.0,0.12) F(8000.0,0.14) )
	RESPONSE( SPHERICAL(50.0f,60.0f), F(63.5,0.02) F(125.0,0.03) F(250.0,0.06) F(500.0,0.10) F(1000.0,0.21) F(2000.0,0.10) F(4000.0,0.11) F(8000.0,0.11) )
	RESPONSE( SPHERICAL(50.0f,90.0f), F(63.5,0.49) F(125.0,0.42) F(250.0,0.21) F(500.0,0.24) F(1000.0,0.25) F(2000.0,0.23) F(4000.0,0.20) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(50.0f,120.0f), F(63.5,0.47) F(125.0,0.41) F(250.0,0.23) F(500.0,0.20) F(1000.0,0.26) F(2000.0,0.17) F(4000.0,0.19) F(8000.0,0.13) )
	RESPONSE( SPHERICAL(50.0f,150.0f), F(63.5,0.27) F(125.0,0.36) F(250.0,0.53) F(500.0,0.67) F(1000.0,0.39) F(2000.0,0.32) F(4000.0,0.42) F(8000.0,0.19) )
	RESPONSE( SPHERICAL(50.0f,180.0f), F(63.5,0.28) F(125.0,0.38) F(250.0,0.57) F(500.0,0.74) F(1000.0,0.47) F(2000.0,0.41) F(4000.0,0.52) F(8000.0,0.28) )
	RESPONSE( SPHERICAL(50.0f,210.0f), F(63.5,0.25) F(125.0,0.35) F(250.0,0.53) F(500.0,0.67) F(1000.0,0.39) F(2000.0,0.31) F(4000.0,0.43) F(8000.0,0.20) )
	RESPONSE( SPHERICAL(50.0f,240.0f), F(63.5,0.19) F(125.0,0.19) F(250.0,0.20) F(500.0,0.11) F(1000.0,0.18) F(2000.0,0.10) F(4000.0,0.15) F(8000.0,0.09) )
	RESPONSE( SPHERICAL(50.0f,270.0f), F(63.5,0.07) F(125.0,0.13) F(250.0,0.21) F(500.0,0.27) F(1000.0,0.20) F(2000.0,0.10) F(4000.0,0.11) F(8000.0,0.10) )
	RESPONSE( SPHERICAL(50.0f,300.0f), F(63.5,0.07) F(125.0,0.07) F(250.0,0.07) F(500.0,0.05) F(1000.0,0.08) F(2000.0,0.11) F(4000.0,0.12) F(8000.0,0.12) )
	RESPONSE( SPHERICAL(50.0f,330.0f), F(63.5,0.48) F(125.0,0.43) F(250.0,0.32) F(500.0,0.29) F(1000.0,0.16) F(2000.0,0.15) F(4000.0,0.17) F(8000.0,0.12) )
	RESPONSE( SPHERICAL(10.0f,0.0f), F(63.5,0.64) F(125.0,0.59) F(250.0,0.43) F(500.0,0.14) F(1000.0,0.23) F(2000.0,0.20) F(4000.0,0.22) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(10.0f,30.0f), F(63.5,0.62) F(125.0,0.58) F(250.0,0.44) F(500.0,0.21) F(1000.0,0.27) F(2000.0,0.12) F(4000.0,0.23) F(8000.0,0.11) )
	RESPONSE( SPHERICAL(10.0f,60.0f), F(63.5,0.56) F(125.0,0.53) F(250.0,0.43) F(500.0,0.21) F(1000.0,0.29) F(2000.0,0.12) F(4000.0,0.10) F(8000.0,0.10) )
	RESPONSE( SPHERICAL(10.0f,90.0f), F(63.5,0.52) F(125.0,0.50) F(250.0,0.40) F(500.0,0.13) F(1000.0,0.23) F(2000.0,0.17) F(4000.0,0.21) F(8000.0,0.17) )
	RESPONSE( SPHERICAL(10.0f,120.0f), F(63.5,0.49) F(125.0,0.46) F(250.0,0.36) F(500.0,0.12) F(1000.0,0.24) F(2000.0,0.13) F(4000.0,0.17) F(8000.0,0.11) )
	RESPONSE( SPHERICAL(10.0f,150.0f), F(63.5,0.25) F(125.0,0.29) F(250.0,0.37) F(500.0,0.44) F(1000.0,0.20) F(2000.0,0.20) F(4000.0,0.19) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(10.0f,180.0f), F(63.5,0.19) F(125.0,0.24) F(250.0,0.30) F(500.0,0.28) F(1000.0,0.37) F(2000.0,0.16) F(4000.0,0.18) F(8000.0,0.16) )
	RESPONSE( SPHERICAL(10.0f,210.0f), F(63.5,0.24) F(125.0,0.28) F(250.0,0.38) F(500.0,0.45) F(1000.0,0.21) F(2000.0,0.21) F(4000.0,0.20) F(8000.0,0.07) )
	RESPONSE( SPHERICAL(10.0f,240.0f), F(63.5,0.24) F(125.0,0.27) F(250.0,0.35) F(500.0,0.42) F(1000.0,0.18) F(2000.0,0.16) F(4000.0,0.16) F(8000.0,0.06) )
	RESPONSE( SPHERICAL(10.0f,270.0f), F(63.5,0.04) F(125.0,0.10) F(250.0,0.18) F(500.0,0.21) F(1000.0,0.23) F(2000.0,0.12) F(4000.0,0.16) F(8000.0,0.10) )
	RESPONSE( SPHERICAL(10.0f,300.0f), F(63.5,0.13) F(125.0,0.12) F(250.0,0.12) F(500.0,0.11) F(1000.0,0.10) F(2000.0,0.13) F(4000.0,0.13) F(8000.0,0.09) )
	RESPONSE( SPHERICAL(10.0f,330.0f), F(63.5,0.61) F(125.0,0.56) F(250.0,0.40) F(500.0,0.14) F(1000.0,0.26) F(2000.0,0.14) F(4000.0,0.17) F(8000.0,0.12) )
);

const SoundDirectivity SoundDirectivity:: JBL_LSR_4328P = NEW_DIRECTIVITY( JBL_LSR_4328P );



//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
