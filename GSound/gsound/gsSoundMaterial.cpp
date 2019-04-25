/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMaterial.cpp
 * Contents:    gsound::SoundMaterial class implementation
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


#include "gsSoundMaterial.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



const FrequencyResponse SoundMaterial:: DEFAULT_REFLECTIVITY = Real(0.9);
const FrequencyResponse SoundMaterial:: DEFAULT_SCATTERING = Real(0.2);
const FrequencyResponse SoundMaterial:: DEFAULT_TRANSMISSION = Real(0);



//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundMaterial:: SoundMaterial()
	:	reflectivity( DEFAULT_REFLECTIVITY ),
		scattering( DEFAULT_SCATTERING ),
		transmission( DEFAULT_TRANSMISSION ),
		averageScattering( DEFAULT_SCATTERING.getAverage() ),
		color( 0.5f, 0.5f, 0.5f, 1.0f )
{
}




SoundMaterial:: SoundMaterial( const FrequencyResponse& newReflectivity,
								const FrequencyResponse& newScattering,
								const FrequencyResponse& newTransmission )
	:	reflectivity( newReflectivity ),
		scattering( newScattering ),
		transmission( newTransmission ),
		averageScattering( newScattering.getAverage() ),
		color( 0.5f, 0.5f, 0.5f, 1.0f )
{
}




SoundMaterial:: SoundMaterial( const FrequencyResponse& newReflectivity,
								const FrequencyResponse& newScattering,
								const FrequencyResponse& newTransmission, const UTF8String& newName )
	:	reflectivity( newReflectivity ),
		scattering( newScattering ),
		transmission( newTransmission ),
		averageScattering( newScattering.getAverage() ),
		name( newName ),
		color( 0.5f, 0.5f, 0.5f, 1.0f )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructors
//############		
//##########################################################################################
//##########################################################################################




SoundMaterial:: ~SoundMaterial()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Absorption Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMaterial:: setAbsorption( const FrequencyResponse& newAbsorption )
{
	reflectivity = newAbsorption;
	
	// Convert absorption to reflectivity.
	for ( Index i = 0; i < reflectivity.getFrequencyCount(); i++ )
	{
		const Float gain = reflectivity.getFrequencyGain(i);
		reflectivity.setFrequencyGain( i, math::sqrt( Real(1) - math::clamp( gain, Real(0), Real(1) ) ) );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Frequency Bands Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMaterial:: setFrequencyBands( const FrequencyBands& newBands ) const
{
	reflectivityBands = reflectivity.getBandResponse( newBands );
	scatteringBands = scattering.getBandResponse( newBands );
	transmissionBands = transmission.getBandResponse( newBands );
	averageScattering = scattering.getAverage();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Predefined Sound Material Initializers.
//############		
//##########################################################################################
//##########################################################################################


#define MATERIAL( functionName, reflectivity, scattering ) \
GSOUND_INLINE static SoundMaterial new##functionName##Material()\
{\
	FrequencyResponse r, s, t(0);\
	reflectivity;\
	scattering;\
	return SoundMaterial( r, s, t, #functionName );\
}\

#define NEW_MATERIAL( functionName ) new##functionName##Material()


#define R( f, a ) r.setFrequency( f, a );
#define A( f, a ) r.setFrequency( f, math::sqrt( 1 - a ) );
#define S( f, a ) s.setFrequency( f, a );


//##########################################################################################
// Unpainted brick.

// Architectural Acoustics - M. David Egan
#define BRICK_R A( 125.0f, 0.02f ) A( 250.0f, 0.02f ) A( 500.0f, 0.03f ) A( 1000.0f, 0.04f ) A( 2000.0f, 0.05f ) A( 4000.0f, 0.07f )
// Estimated, stolen from stone.
#define BRICK_S S( 125.0f, 0.20f ) S( 250.0f, 0.25f ) S( 500.0f, 0.30f ) S( 1000.0f, 0.35f ) S( 2000.0f, 0.40f ) S( 4000.0f, 0.45f )

MATERIAL( Brick, BRICK_R, BRICK_S );
const SoundMaterial SoundMaterial:: BRICK = NEW_MATERIAL( Brick );

//##########################################################################################
// Painted brick.

// Architectural Acoustics - M. David Egan
#define BRICK_PAINTED_R A( 125.0f, 0.01f ) A( 250.0f, 0.01f ) A( 500.0f, 0.02f ) A( 1000.0f, 0.02f ) A( 2000.0f, 0.02f ) A( 4000.0f, 0.03f )
// Estimated
#define BRICK_PAINTED_S S( 125.0f, 0.15f ) S( 250.0f, 0.15f ) S( 500.0f, 0.20f ) S( 1000.0f, 0.20f ) S( 2000.0f, 0.20f ) S( 4000.0f, 0.25f )

MATERIAL( PaintedBrick, BRICK_PAINTED_R, BRICK_PAINTED_S );
const SoundMaterial SoundMaterial:: BRICK_PAINTED = NEW_MATERIAL( PaintedBrick );

//##########################################################################################
// Light carpet on a solid backing.

// Architectural Acoustics - M. David Egan
#define CARPET_R R( 125.0f, 0.99f ) R( 250.0f, 0.97f ) R( 500.0f, 0.95f ) R( 1000.0f, 0.89f ) R( 2000.0f, 0.74f ) R( 4000.0f, 0.59f )
// Estimated
#define CARPET_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.15f ) S( 1000.0f, 0.20f ) S( 2000.0f, 0.30f ) S( 4000.0f, 0.45f )

MATERIAL( Carpet, CARPET_R, CARPET_S );
const SoundMaterial SoundMaterial:: CARPET = NEW_MATERIAL( Carpet );

//##########################################################################################
// Heavy carpet on a solid backing.

// Architectural Acoustics - M. David Egan
#define CARPET_HEAVY_R R( 125.0f, 0.99f ) R( 250.0f, 0.97f ) R( 500.0f, 0.93f ) R( 1000.0f, 0.79f ) R( 2000.0f, 0.63f ) R( 4000.0f, 0.59f )
// Estimated
#define CARPET_HEAVY_S S( 125.0f, 0.10f ) S( 250.0f, 0.15f ) S( 500.0f, 0.20f ) S( 1000.0f, 0.25f ) S( 2000.0f, 0.35f ) S( 4000.0f, 0.50f )

MATERIAL( CarpetHeavy, CARPET_HEAVY_R, CARPET_HEAVY_S );
const SoundMaterial SoundMaterial:: CARPET_HEAVY = NEW_MATERIAL( CarpetHeavy );

//##########################################################################################
// Heavy carpet on a padded foam rubber backing.

// Architectural Acoustics - M. David Egan
#define CARPET_HEAVY_PADDED_R R( 125.0f, 0.96f ) R( 250.0f, 0.87f ) R( 500.0f, 0.66f ) R( 1000.0f, 0.56f ) R( 2000.0f, 0.54f ) R( 4000.0f, 0.52f )
// Estimated
#define CARPET_HEAVY_PADDED_S S( 125.0f, 0.10f ) S( 250.0f, 0.15f ) S( 500.0f, 0.20f ) S( 1000.0f, 0.25f ) S( 2000.0f, 0.35f ) S( 4000.0f, 0.50f )

MATERIAL( CarpetHeavyPadded, CARPET_HEAVY_PADDED_R, CARPET_HEAVY_PADDED_S );
const SoundMaterial SoundMaterial:: CARPET_HEAVY_PADDED = NEW_MATERIAL( CarpetHeavyPadded );

//##########################################################################################
// Smooth concrete.

// Architectural Acoustics - M. David Egan
#define CONCRETE_R R( 125.0f, 0.99f ) R( 250.0f, 0.99f ) R( 500.0f, 0.99f ) R( 1000.0f, 0.99f ) R( 2000.0f, 0.99f ) R( 4000.0f, 0.99f )
// Estimated
#define CONCRETE_S S( 125.0f, 0.10f ) S( 250.0f, 0.11f ) S( 500.0f, 0.12f ) S( 1000.0f, 0.13f ) S( 2000.0f, 0.14f ) S( 4000.0f, 0.15f )

MATERIAL( Concrete, CONCRETE_R, CONCRETE_S );
const SoundMaterial SoundMaterial:: CONCRETE = NEW_MATERIAL( Concrete );

//##########################################################################################
// Rough concrete.

// Architectural Acoustics - M. David Egan
#define CONCRETE_ROUGH_R R( 125.0f, 0.99f ) R( 250.0f, 0.99f ) R( 500.0f, 0.98f ) R( 1000.0f, 0.97f ) R( 2000.0f, 0.96f ) R( 4000.0f, 0.95f )
// Estimated
#define CONCRETE_ROUGH_S S( 125.0f, 0.10f ) S( 250.0f, 0.12f ) S( 500.0f, 0.15f ) S( 1000.0f, 0.20f ) S( 2000.0f, 0.25f ) S( 4000.0f, 0.30f )

MATERIAL( ConcreteRough, CONCRETE_ROUGH_R, CONCRETE_ROUGH_S );
const SoundMaterial SoundMaterial:: CONCRETE_ROUGH = NEW_MATERIAL( ConcreteRough );

//##########################################################################################
// Concrete block, unpainted.

// Architectural Acoustics - M. David Egan
#define CONCRETE_BLOCK_R R( 125.0f, 0.80f ) R( 250.0f, 0.75f ) R( 500.0f, 0.83f ) R( 1000.0f, 0.84f ) R( 2000.0f, 0.78f ) R( 4000.0f, 0.87f )
// Estimated
#define CONCRETE_BLOCK_S S( 125.0f, 0.10f ) S( 250.0f, 0.12f ) S( 500.0f, 0.15f ) S( 1000.0f, 0.20f ) S( 2000.0f, 0.25f ) S( 4000.0f, 0.30f )

MATERIAL( ConcreteBlock, CONCRETE_BLOCK_R, CONCRETE_BLOCK_S );
const SoundMaterial SoundMaterial:: CONCRETE_BLOCK = NEW_MATERIAL( ConcreteBlock );

//##########################################################################################
// Concrete block, painted.

// Architectural Acoustics - M. David Egan
#define CONCRETE_BLOCK_PAINTED_R R( 125.0f, 0.95f ) R( 250.0f, 0.97f ) R( 500.0f, 0.97f ) R( 1000.0f, 0.96f ) R( 2000.0f, 0.95f ) R( 4000.0f, 0.96f )
// Estimated
#define CONCRETE_BLOCK_PAINTED_S S( 125.0f, 0.10f ) S( 250.0f, 0.11f ) S( 500.0f, 0.13f ) S( 1000.0f, 0.15f ) S( 2000.0f, 0.16f ) S( 4000.0f, 0.20f )

MATERIAL( ConcreteBlockPainted, CONCRETE_BLOCK_PAINTED_R, CONCRETE_BLOCK_PAINTED_S );
const SoundMaterial SoundMaterial:: CONCRETE_BLOCK_PAINTED = NEW_MATERIAL( ConcreteBlockPainted );

//##########################################################################################
// Glass, normal window.

// Architectural Acoustics - M. David Egan
#define GLASS_R R( 125.0f, 0.81f ) R( 250.0f, 0.87f ) R( 500.0f, 0.91f ) R( 1000.0f, 0.94f ) R( 2000.0f, 0.96f ) R( 4000.0f, 0.98f )
// Estimated
#define GLASS_S S( 125.0f, 0.05f ) S( 250.0f, 0.05f ) S( 500.0f, 0.05f ) S( 1000.0f, 0.05f ) S( 2000.0f, 0.05f ) S( 4000.0f, 0.05f )

MATERIAL( Glass, GLASS_R, GLASS_S );
const SoundMaterial SoundMaterial:: GLASS = NEW_MATERIAL( Glass );

//##########################################################################################
// Glass, heavy window.

// Architectural Acoustics - M. David Egan
#define GLASS_HEAVY_R R( 125.0f, 0.91f ) R( 250.0f, 0.97f ) R( 500.0f, 0.98f ) R( 1000.0f, 0.98f ) R( 2000.0f, 0.99f ) R( 4000.0f, 0.99f )
// Estimated
#define GLASS_HEAVY_S S( 125.0f, 0.05f ) S( 250.0f, 0.05f ) S( 500.0f, 0.05f ) S( 1000.0f, 0.05f ) S( 2000.0f, 0.05f ) S( 4000.0f, 0.05f )

MATERIAL( GlassHeavy, GLASS_HEAVY_R, GLASS_HEAVY_S );
const SoundMaterial SoundMaterial:: GLASS_HEAVY = NEW_MATERIAL( GlassHeavy );

//##########################################################################################
// Grass, 2" high.

// Architectural Acoustics - M. David Egan
#define GRASS_R R( 125.0f, 0.94f ) R( 250.0f, 0.86f ) R( 500.0f, 0.63f ) R( 1000.0f, 0.56f ) R( 2000.0f, 0.28f ) R( 4000.0f, 0.10f )
// Estimated
#define GRASS_S S( 125.0f, 0.30f ) S( 250.0f, 0.30f ) S( 500.0f, 0.40f ) S( 1000.0f, 0.50f ) S( 2000.0f, 0.60f ) S( 4000.0f, 0.70f )

MATERIAL( Grass, GRASS_R, GRASS_S );
const SoundMaterial SoundMaterial:: GRASS = NEW_MATERIAL( Grass );

//##########################################################################################
// Gravel, 4" deep, moist.

// Architectural Acoustics - M. David Egan
#define GRAVEL_R R( 125.0f, 0.87f ) R( 250.0f, 0.63f ) R( 500.0f, 0.59f ) R( 1000.0f, 0.55f ) R( 2000.0f, 0.50f ) R( 4000.0f, 0.45f )
// Estimated
#define GRAVEL_S S( 125.0f, 0.20f ) S( 250.0f, 0.30f ) S( 500.0f, 0.40f ) S( 1000.0f, 0.50f ) S( 2000.0f, 0.60f ) S( 4000.0f, 0.70f )

MATERIAL( Gravel, GRAVEL_R, GRAVEL_S );
const SoundMaterial SoundMaterial:: GRAVEL = NEW_MATERIAL( Gravel );

//##########################################################################################
// 1/2" gypsum board, nailed to 2x4s with air cavities.

// Architectural Acoustics - M. David Egan
#define GYPSUM_BOARD_R R( 125.0f, 0.84f ) R( 250.0f, 0.95f ) R( 500.0f, 0.97f ) R( 1000.0f, 0.98f ) R( 2000.0f, 0.96f ) R( 4000.0f, 0.95f )
// Estimated
#define GYPSUM_BOARD_S S( 125.0f, 0.10f ) S( 250.0f, 0.11f ) S( 500.0f, 0.12f ) S( 1000.0f, 0.13f ) S( 2000.0f, 0.14f ) S( 4000.0f, 0.15f )

MATERIAL( GypsumBoard, GYPSUM_BOARD_R, GYPSUM_BOARD_S );
const SoundMaterial SoundMaterial:: GYPSUM_BOARD = NEW_MATERIAL( GypsumBoard );

//##########################################################################################
// Ceramic or marble tile.

// Architectural Acoustics - M. David Egan
#define CERAMIC_TILE_R R( 125.0f, 0.99f ) R( 250.0f, 0.99f ) R( 500.0f, 0.99f ) R( 1000.0f, 0.99f ) R( 2000.0f, 0.99f ) R( 4000.0f, 0.99f )
// http://www.bembook.ibpsa.us/index.php?title=Scattering_Coefficient_in_Building_Acoustics
#define CERAMIC_TILE_S S( 125.0f, 0.10f ) S( 250.0f, 0.12f ) S( 500.0f, 0.14f ) S( 1000.0f, 0.16f ) S( 2000.0f, 0.18f ) S( 4000.0f, 0.20f )

MATERIAL( CeramicTile, CERAMIC_TILE_R, CERAMIC_TILE_S );
const SoundMaterial SoundMaterial:: CERAMIC_TILE = NEW_MATERIAL( CeramicTile );

//##########################################################################################
// Plaster on brick.

// Architectural Acoustics - M. David Egan
#define PLASTER_ON_BRICK_R R( 125.0f, 0.99f ) R( 250.0f, 0.99f ) R( 500.0f, 0.99f ) R( 1000.0f, 0.98f ) R( 2000.0f, 0.98f ) R( 4000.0f, 0.97f )
// Estimated, stolen from stone.
#define PLASTER_ON_BRICK_S S( 125.0f, 0.20f ) S( 250.0f, 0.25f ) S( 500.0f, 0.30f ) S( 1000.0f, 0.35f ) S( 2000.0f, 0.40f ) S( 4000.0f, 0.45f )

MATERIAL( PlasterOnBrick, PLASTER_ON_BRICK_R, PLASTER_ON_BRICK_S );
const SoundMaterial SoundMaterial:: PLASTER_ON_BRICK = NEW_MATERIAL( PlasterOnBrick );

//##########################################################################################
// Plaster on concrete block.

// Architectural Acoustics - M. David Egan
#define PLASTER_ON_CONCRETE_BLOCK_R R( 125.0f, 0.94f ) R( 250.0f, 0.95f ) R( 500.0f, 0.96f ) R( 1000.0f, 0.97f ) R( 2000.0f, 0.97f ) R( 4000.0f, 0.98f )
// Estimated, stolen from stone.
#define PLASTER_ON_CONCRETE_BLOCK_S S( 125.0f, 0.20f ) S( 250.0f, 0.25f ) S( 500.0f, 0.30f ) S( 1000.0f, 0.35f ) S( 2000.0f, 0.40f ) S( 4000.0f, 0.45f )

MATERIAL( PlasterOnConcreteBlock, PLASTER_ON_CONCRETE_BLOCK_R, PLASTER_ON_CONCRETE_BLOCK_S );
const SoundMaterial SoundMaterial:: PLASTER_ON_CONCRETE_BLOCK = NEW_MATERIAL( PlasterOnConcreteBlock );

//##########################################################################################
// Snow, fresh, 4" deep.

// Architectural Acoustics - M. David Egan
#define SNOW_R R( 125.0f, 0.74f ) R( 250.0f, 0.50f ) R( 500.0f, 0.32f ) R( 1000.0f, 0.22f ) R( 2000.0f, 0.22f ) R( 4000.0f, 0.22f )
// Estimated
#define SNOW_S S( 125.0f, 0.20f ) S( 250.0f, 0.30f ) S( 500.0f, 0.40f ) S( 1000.0f, 0.50f ) S( 2000.0f, 0.60f ) S( 4000.0f, 0.75f )

MATERIAL( Snow, SNOW_R, SNOW_S );
const SoundMaterial SoundMaterial:: SNOW = NEW_MATERIAL( Snow );

//##########################################################################################
// Steel, smooth.

// Architectural Acoustics - M. David Egan
#define STEEL_R R( 125.0f, 0.97f ) R( 250.0f, 0.95f ) R( 500.0f, 0.95f ) R( 1000.0f, 0.95f ) R( 2000.0f, 0.96f ) R( 4000.0f, 0.99f )
// Estimated
#define STEEL_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.10f ) S( 2000.0f, 0.10f ) S( 4000.0f, 0.10f )

MATERIAL( Steel, STEEL_R, STEEL_S );
const SoundMaterial SoundMaterial:: STEEL = NEW_MATERIAL( Steel );

//##########################################################################################
// Water surface.

// Architectural Acoustics - M. David Egan
#define WATER_R R( 125.0f, 0.99f ) R( 250.0f, 0.99f ) R( 500.0f, 0.99f ) R( 1000.0f, 0.99f ) R( 2000.0f, 0.99f ) R( 4000.0f, 0.98f )
// Estimated
#define WATER_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.07f ) S( 2000.0f, 0.05f ) S( 4000.0f, 0.05f )

MATERIAL( Water, WATER_R, WATER_S );
const SoundMaterial SoundMaterial:: WATER = NEW_MATERIAL( Water );

//##########################################################################################
// Thin (1/4") wood panneling with an air space behind.

// Architectural Acoustics - M. David Egan
#define WOOD_THIN_R R( 125.0f, 0.76f ) R( 250.0f, 0.89f ) R( 500.0f, 0.95f ) R( 1000.0f, 0.96f ) R( 2000.0f, 0.97f ) R( 4000.0f, 0.97f )
// Estimated
#define WOOD_THIN_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.10f ) S( 2000.0f, 0.10f ) S( 4000.0f, 0.15f )

MATERIAL( WoodThin, WOOD_THIN_R, WOOD_THIN_S );
const SoundMaterial SoundMaterial:: WOOD_THIN = NEW_MATERIAL( WoodThin );

//##########################################################################################
// Thick (1") wood panneling with an air space behind.

// Architectural Acoustics - M. David Egan
#define WOOD_THICK_R R( 125.0f, 0.90f ) R( 250.0f, 0.93f ) R( 500.0f, 0.95f ) R( 1000.0f, 0.97f ) R( 2000.0f, 0.97f ) R( 4000.0f, 0.97f )
// Estimated
#define WOOD_THICK_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.10f ) S( 2000.0f, 0.10f ) S( 4000.0f, 0.15f )

MATERIAL( WoodThick, WOOD_THICK_R, WOOD_THICK_S );
const SoundMaterial SoundMaterial:: WOOD_THICK = NEW_MATERIAL( WoodThick );

//##########################################################################################
// Wood floor.

// Architectural Acoustics - M. David Egan
#define WOOD_FLOOR_R R( 125.0f, 0.92f ) R( 250.0f, 0.94f ) R( 500.0f, 0.95f ) R( 1000.0f, 0.96f ) R( 2000.0f, 0.97f ) R( 4000.0f, 0.96f )
// Estimated
#define WOOD_FLOOR_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.10f ) S( 2000.0f, 0.10f ) S( 4000.0f, 0.15f )

MATERIAL( WoodFloor, WOOD_FLOOR_R, WOOD_FLOOR_S );
const SoundMaterial SoundMaterial:: WOOD_FLOOR = NEW_MATERIAL( WoodFloor );

//##########################################################################################
// Wood floor on concrete backing.

// Architectural Acoustics - M. David Egan
#define WOOD_ON_CONRETE_R R( 125.0f, 0.98f ) R( 250.0f, 0.98f ) R( 500.0f, 0.96f ) R( 1000.0f, 0.97f ) R( 2000.0f, 0.97f ) R( 4000.0f, 0.96f )
// Estimated
#define WOOD_ON_CONRETE_S S( 125.0f, 0.10f ) S( 250.0f, 0.10f ) S( 500.0f, 0.10f ) S( 1000.0f, 0.10f ) S( 2000.0f, 0.10f ) S( 4000.0f, 0.15f )

MATERIAL( WoodOnConcrete, WOOD_ON_CONRETE_R, WOOD_ON_CONRETE_S );
const SoundMaterial SoundMaterial:: WOOD_ON_CONRETE = NEW_MATERIAL( WoodOnConcrete );




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
