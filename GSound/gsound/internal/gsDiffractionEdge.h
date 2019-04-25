/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsDiffractionEdge.h
 * Contents:    gsound::internal::DiffractionEdge class declaration
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


#ifndef INCLUDE_GSOUND_DIFFRACTION_EDGE_H
#define INCLUDE_GSOUND_DIFFRACTION_EDGE_H


#include "gsInternalConfig.h"


#include "gsInternalSoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains information about a single diffraction edge.
class DiffractionEdge
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default diffraction edge with no internal volume.
			GSOUND_INLINE DiffractionEdge()
			{
			}
			
			
			/// Create a diffraction edge for the specified edge ID.
			GSOUND_INLINE DiffractionEdge( const internal::InternalSoundTriangle* newTriangle1,
											Index newEdgeIndex1,
											const internal::InternalSoundTriangle* newTriangle2,
											Index newEdgeIndex2,
											const SoundVertex* newV1,
											const SoundVertex* newV2,
											const Plane3f& newPlane1,
											const Plane3f& newPlane2 )
				:	triangle1( newTriangle1 ),
					triangle2( newTriangle2 ),
					plane1( newPlane1 ),
					plane2( newPlane2 ),
					v1( newV1 ),
					v2( newV2 ),
					neighborListOffset( 0 ),
					numNeighbors( 0 ),
					edgeIndex1( (UInt16)newEdgeIndex1 ),
					edgeIndex2( (UInt16)newEdgeIndex2 )
			{
				triangle1->getEdgeVertices( edgeIndex1, v1, v2 );
				
				// Make sure that the planes point towards the outside of the diffraction edge.
				const SoundVertex* freeVertex1 = triangle1->getEdgeFreeVertex( edgeIndex1 );
				const SoundVertex* freeVertex2 = triangle2->getEdgeFreeVertex( edgeIndex2 );
				
				if ( plane1.getSignedDistanceTo( *freeVertex2 ) > Real(0) )
					plane1 = -plane1;
				
				if ( plane2.getSignedDistanceTo( *freeVertex1 ) > Real(0) )
					plane2 = -plane2;
			}
			
			
		//********************************************************************************
		//******	Direction Accessor Method
			
			
			/// Return the normalized direction of this diffraction edge.
			/**
			  * The direction is the vector from the first edge vertex in the direction
			  * of the second vertex.
			  */
			GSOUND_INLINE Vector3f getDirection() const
			{
				return (*v2 - *v1).normalize();
			}
			
			
			/// Return the unnormalized vector from the first edge vertex to the second vertex.
			GSOUND_INLINE Vector3f getExtent() const
			{
				return *v2 - *v1;
			}
			
			
		//********************************************************************************
		//******	Endpoint Accessor Methods
			
			
			/// Return the first vertex of this diffraction edge.
			GSOUND_INLINE const Vector3f& getStart() const
			{
				return *v1;
			}
			
			
			/// Return the second vertex of this diffraction edge.
			GSOUND_INLINE const Vector3f& getEnd() const
			{
				return *v2;
			}
			
			
		//********************************************************************************
		//******	Edge Normal Accessor Method
			
			
			/// Return a normalized vector which biases points on this edge out and away from the edge.
			/**
			  * This vector is used to avoid numerical issues that would occur when tracing rays.
			  */
			GSOUND_FORCE_INLINE Vector3f getNormal() const
			{
				if ( triangle1 != triangle2 )
				{
					Vector3f normalSum = plane1.normal + plane2.normal;
					Real normalSumMagnitude = normalSum.getMagnitude();
					
					if ( normalSumMagnitude > math::epsilon<Real>() )
						return normalSum / normalSumMagnitude;
				}
				
				// If the edge uses the same triangle or triangle normals point in opposite directions.
				return math::cross( plane1.normal, *v2 - *v1 ).normalize();
			}
			
			
		//********************************************************************************
		//******	Length Accessor Method
			
			
			/// Return the length in units of this diffraction edge.
			GSOUND_INLINE Real getLength() const
			{
				return (*v2 - *v1).getMagnitude();
			}
			
			
		//********************************************************************************
		//******	Edge Orientation Test Method
			
			
			/// Return whether or not the specified point is in this edge's diffraction regions.
			GSOUND_FORCE_INLINE Bool testOrientation( const Vector3f& point, Real offset ) const
			{
				Real d1 = plane1.getSignedDistanceTo( point );
				Real d2 = plane2.getSignedDistanceTo( point );
				
				Bool front1 = d1 > offset;
				Bool front2 = d2 > offset;
				Bool behind1 = d1 < -offset;
				Bool behind2 = d2 < -offset;
				
				return !((front1 & front2) || (behind1 & behind2));
			}
			
			
		//********************************************************************************
		//******	Free Vertex Accessor Method
			
			
			/// Return a pointer to the vertex on the first triangle that is not part of this edge.
			GSOUND_INLINE const SoundVertex* getFreeVertex1() const
			{
				return triangle1->getEdgeFreeVertex( edgeIndex1 );
			}
			
			
			/// Return a pointer to the vertex on the second triangle that is not part of this edge.
			GSOUND_INLINE const SoundVertex* getFreeVertex2() const
			{
				return triangle2->getEdgeFreeVertex( edgeIndex2 );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The first endpoint of the edge.
			const SoundVertex* v1;
			
			
			/// The second endpoint of the edge.
			const SoundVertex* v2;
			
			
			/// The first triangle that forms this diffraction edge.
			const InternalSoundTriangle* triangle1;
			
			
			/// The second triangle that forms this diffraction edge.
			const InternalSoundTriangle* triangle2;
			
			
			/// The plane of the first triangle that has a normal pointing towards the outside (concave side) of the edge.
			Plane3f plane1;
			
			
			/// The plane of the second triangle that has a normal pointing towards the outside (concave side) of the edge.
			Plane3f plane2;
			
			
			/// An offset into a global list of edge neighbor connections.
			UInt32 neighborListOffset;
			
			
			/// The number of diffraction edges that could diffract with this edge.
			UInt32 numNeighbors;
			
			
			/// The edge index for the first triangle of this diffraction edge.
			UInt16 edgeIndex1;
			
			
			/// The edge index for the second triangle of this diffraction edge.
			UInt16 edgeIndex2;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DIFFRACTION_EDGE_H
