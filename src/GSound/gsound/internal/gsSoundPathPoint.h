/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundPathPoint.h
 * Contents:    gsound::SoundPathPoint class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PATH_POINT_H
#define INCLUDE_GSOUND_SOUND_PATH_POINT_H


#include "gsInternalConfig.h"


#include "gsObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class used to uniquely identify a point on a sound sound path.
/**
  * A point represents a single interaction of sound with the environment,
  * and a sequence of path points makes up a sound path from the source to
  * listener.
  */
class SoundPathPoint
{
	public:
		
		//********************************************************************************
		//******	Sound Path Point Type Enum
			
			
			/// An enum specifying the type of a point on a sound path.
			enum Type
			{
				/// The path point represents a specular triangle reflection.
				SPECULAR_REFLECTION = 0x8DA6B343,
				
				/// The path point represents a diffuse triangle reflection.
				DIFFUSE_REFLECTION = 0xD8163841,
				
				/// The path point represents sound diffraction over a triangle's edge.
				EDGE_DIFFRACTION = 0xCB1AB31F,
				
				/// The path point represents sound tranmission through a medium boundary.
				TRANSMISSION = 0x724FAFCF
			};
			
			
			/// The type to use for a sound path point ID.
			typedef UInt32 IDType;
			
			
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new sound path point with the specified type, triangle, and ID.
			GSOUND_INLINE SoundPathPoint( Type newType, const ObjectSpaceTriangle& newTriangle, IDType newID = IDType(0) )
				:	triangle( newTriangle ),
					type( newType ),
					id( newID )
			{
			}
			
			
		//********************************************************************************
		//******	Type Accessor Methods
			
			
			/// Return an enum value indicating the type of this sound path point.
			GSOUND_INLINE Type getType() const
			{
				return type;
			}
			
			
			/// Set an enum value indicating the type of this sound path point.
			GSOUND_INLINE void setType( Type newType )
			{
				type = newType;
			}
			
			
		//********************************************************************************
		//******	Triangle Accessor Methods
			
			
			/// Return a reference to the triangle where this path point occurred.
			GSOUND_INLINE const ObjectSpaceTriangle& getTriangle() const
			{
				return triangle;
			}
			
			
			/// Set the triangle where this path point occurred.
			GSOUND_INLINE void setTriangle( const ObjectSpaceTriangle& newTriangle )
			{
				triangle = newTriangle;
			}
			
			
		//********************************************************************************
		//******	ID Accessor Methods
			
			
			/// Return an integer ID that specifies the place on the triangle where the path point is.
			/**
			  * For points with type EDGE_DIFFRACTION, the ID indicates the edge index on the
			  * triangle where the diffraction occurred.
			  *
			  * For points with type DIFFUSE_REFLECTION, the ID indicates the diffuse patch ID
			  * for the triangle where the reflection occurred.
			  */
			GSOUND_INLINE IDType getID() const
			{
				return id;
			}
			
			
			/// Set an integer ID that specifies the place on the triangle where the path point is.
			/**
			  * For points with type EDGE_DIFFRACTION, the ID indicates the edge index on the
			  * triangle where the diffraction occurred.
			  *
			  * For points with type DIFFUSE_REFLECTION, the ID indicates the diffuse patch ID
			  * for the triangle where the reflection occurred.
			  */
			GSOUND_INLINE void setID( IDType newID )
			{
				id = newID;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integral hash code for this sound path point.
			GSOUND_INLINE SoundPathHash getHashCode() const
			{
				return type ^ triangle.getHashCode() + id;
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this sound path is equal to another.
			GSOUND_INLINE Bool operator == ( const SoundPathPoint& other ) const
			{
				return type == other.type && triangle == other.triangle && id == other.id;
			}
			
			
			/// Return whether or not this sound path is not equal to another.
			GSOUND_INLINE Bool operator != ( const SoundPathPoint& other ) const
			{
				return type != other.type || triangle != other.triangle || id != other.id;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The triangle where this path point occurred.
			ObjectSpaceTriangle triangle;
			
			
			/// The type of this sound path point.
			Type type;
			
			
			/// An integer ID that specifies the place on the triangle where the path point is.
			/**
			  * For points with type EDGE_DIFFRACTION, the ID indicates the edge index on the
			  * triangle where the diffraction occurred.
			  *
			  * For points with type DIFFUSE_REFLECTION, the ID indicates the diffuse patch ID
			  * for the triangle where the reflection occurred.
			  */
			UInt32 id;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PATH_POINT_H
