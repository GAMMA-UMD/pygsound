/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsWorldSpaceTriangle.h
 * Contents:    gsound::internal::WorldSpaceTriangle class declaration
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


#ifndef INCLUDE_GSOUND_WORLD_SPACE_TRIANGLE_H
#define INCLUDE_GSOUND_WORLD_SPACE_TRIANGLE_H


#include "gsInternalConfig.h"


#include "../gsSoundObject.h"
#include "gsObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a triangle after it has been transformed from object to world space.
/**
  * This class contains 3 3D vertices in world space for the triangle, plus an
  * object representing the plane equation of the triangle and pointers to the
  * original object-space triangle and its owning object.
  */
class WorldSpaceTriangle
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a world-space triangle that instances the specified object space triangle.
			GSOUND_INLINE WorldSpaceTriangle( const InternalSoundTriangle* newTriangle, const SoundObject* newObject )
				:	objectSpaceTriangle( newTriangle, newObject ),
					v1( newObject->getTransform().transformToWorld( *newTriangle->getVertex(0) ) ),
					v2( newObject->getTransform().transformToWorld( *newTriangle->getVertex(1) ) ),
					v3( newObject->getTransform().transformToWorld( *newTriangle->getVertex(2) ) ),
					plane( newObject->getTransform().transformToWorld( newTriangle->getPlane() ) )
			{
			}
			
			
			/// Create a world-space triangle that instances the specified object space triangle.
			GSOUND_INLINE WorldSpaceTriangle( const ObjectSpaceTriangle& t )
				:	objectSpaceTriangle( t ),
					v1( t.object->getTransform().transformToWorld( *t.triangle->getVertex(0) ) ),
					v2( t.object->getTransform().transformToWorld( *t.triangle->getVertex(1) ) ),
					v3( t.object->getTransform().transformToWorld( *t.triangle->getVertex(2) ) ),
					plane( t.object->getTransform().transformToWorld( t.triangle->getPlane() ) )
			{
			}
			
			
		//********************************************************************************
		//******	Vertex Accessor Methods
			
			
			/// Get pointers to the vertices which share the edge at the specified index.
			/**
			  * If the edge index is out of bounds [0-2], the output pointers are not set by
			  * the method and an assertion is raised.
			  */
			GSOUND_INLINE void getEdgeVertices( Index edgeIndex, const SoundVertex*& edgeV1, const SoundVertex*& edgeV2 ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access world space triangle edge with invalid index." );
				
				switch ( edgeIndex )
				{
					case 0:
						edgeV1 = &v1;
						edgeV2 = &v2;
						break;
						
					case 1:
						edgeV1 = &v1;
						edgeV2 = &v3;
						break;
						
					case 2:
						edgeV1 = &v2;
						edgeV2 = &v3;
						break;
				}
			}
			
			
			/// Return the vertex for this triangle that is not part of the specified edge.
			GSOUND_INLINE const SoundVertex* getEdgeFreeVertex( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access world space triangle edge with invalid index." );
				
				switch ( edgeIndex )
				{
					case 0:
						return &v3;
					case 1:
						return &v2;
					case 2:
						return &v1;
					default:
						return NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The first vertex of this world-space triangle.
			SoundVertex v1;
			
			
			/// The second vertex of this world-space triangle.
			SoundVertex v2;
			
			
			/// The third vertex of this world-space triangle.
			SoundVertex v3;
			
			
			/// An object representing the plane equation of the world space triangle.
			Plane3f plane;
			
			
			/// A pointer to the triangle which this world space triangle is an instance of.
			ObjectSpaceTriangle objectSpaceTriangle;
			
			
			
};





//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_WORLD_SPACE_TRIANGLE_H
