/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsInternalSoundTriangle.h
 * Contents:    gsound::internal::InternalSoundTriangle class declaration
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


#ifndef INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H
#define INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H


#include "gsInternalConfig.h"


#include "../gsSoundMaterial.h"
#include "../gsSoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class DiffractionEdge;




//********************************************************************************
/// A class that is used internally to represent a triangle that is part of a mesh.
/**
  * Each triangle has pointers to the vertices and material that it shares and pointers
  * to any diffraction edges. This data is needed internally in order perform fast
  * sound propagation.
  */
class InternalSoundTriangle
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an internal sound triangle with the specified vertex and material pointers.
			GSOUND_INLINE InternalSoundTriangle( const SoundVertex* newV1, const SoundVertex* newV2, const SoundVertex* newV3,
												const SoundMaterial* newMaterial )
				:	plane( *newV1, *newV2, *newV3 ),
					material( newMaterial ),
					keyVertex( 0 ),
					numRows( 1 ),
					numColumns( 1 )
			{
				vertex[0] = newV1;
				vertex[1] = newV2;
				vertex[2] = newV3;
				edge[0] = NULL;
				edge[1] = NULL;
				edge[2] = NULL;
				area = Real(0.5)*math::cross( (Vector3f)(*newV3 - *newV1), (Vector3f)(*newV3 - *newV2) ).getMagnitude();
			}
			
			
		//********************************************************************************
		//******	Vertex Accessor Method
			
			
			/// Return a pointer to the vertex at the specified index in this triangle.
			GSOUND_FORCE_INLINE const SoundVertex* getVertex( Index index ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( index < 3, "Cannot access vertex at invalid index in triangle." );
				
				return vertex[index];
			}
			
			
			/// Set the triangle's vertex at the specified index.
			GSOUND_FORCE_INLINE void setVertex( Index index, const SoundVertex* newVertex )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( index < 3, "Cannot set vertex at invalid index in triangle." );
				
				vertex[index] = newVertex;
			}
			
			
		//********************************************************************************
		//******	Material Accessor Method
			
			
			/// Return a pointer to the SoundMaterial associated with this triangle.
			GSOUND_FORCE_INLINE const SoundMaterial* getMaterial() const
			{
				return material;
			}
			
			
			/// Set the SoundMaterial associated with this triangle.
			GSOUND_FORCE_INLINE void setMaterial( const SoundMaterial* newMaterial )
			{
				material = newMaterial;
			}
			
			
		//********************************************************************************
		//******	Plane Accessor Methods
			
			
			/// Return a reference to an object representing the plane of this triangle.
			GSOUND_FORCE_INLINE const Plane3f& getPlane() const
			{
				return plane;
			}
			
			
			/// Return a reference to the normal of this triangle.
			GSOUND_FORCE_INLINE const Vector3f& getNormal() const
			{
				return plane.normal;
			}
			
			
		//********************************************************************************
		//******	Area Accessor Methods
			
			
			/// Return the area of the triangle in square units.
			GSOUND_FORCE_INLINE Real getArea() const
			{
				return area;
			}
			
			
		//********************************************************************************
		//******	Neighbor Accessor Methods
			
			
			/// Return a pointer to the diffraction edge objects for this triangle for each edge.
			/**
			  * The edge pointer is NULL if that edge isn't a diffraction edge.
			  *
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE const DiffractionEdge* getDiffractionEdge( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access edge at invalid index in triangle." );
				
				return edge[edgeIndex];
			}
			
			
			/// Set a pointer to the diffraction edge objects for this triangle for each edge.
			/**
			  * Edge indices are denoted as follows:
			  * - 0: edge between vertices 0 and 1
			  * - 1: edge between vertices 0 and 2
			  * - 2: edge between vertices 1 and 2
			  */
			GSOUND_FORCE_INLINE void setDiffractionEdge( Index edgeIndex, const DiffractionEdge* newEdge )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot set edge at invalid index in triangle." );
				
				edge[edgeIndex] = newEdge;
			}
			
			
		//********************************************************************************
		//******	Diffuse Subdivision Info Accessor Methods
			
			
			/// Return the index of the key vertex for this triangle (0, 1, 2).
			GSOUND_FORCE_INLINE Index getKeyVertex() const
			{
				return keyVertex;
			}
			
			
			/// Set the index of the key vertex for this triangle (0, 1, 2).
			GSOUND_FORCE_INLINE void setKeyVertex( Index newKeyVertex )
			{
				keyVertex = (UInt8)math::clamp( newKeyVertex, Index(0), Index(2) );
			}
			
			
			/// Return the number of rows there are for this triangle's subdivision.
			GSOUND_FORCE_INLINE Size getRowCount() const
			{
				return numRows;
			}
			
			
			/// Set the number of rows there are for this triangle's subdivision.
			GSOUND_FORCE_INLINE void setRowCount( Size newNumRows )
			{
				numRows = (UInt8)math::min( newNumRows, (Size)math::max<UInt8>() );
			}
			
			
			/// Return the number of columns there are for this triangle's subdivision.
			GSOUND_FORCE_INLINE Size getColumnCount() const
			{
				return numColumns;
			}
			
			
			/// Set the number of columns there are for this triangle's subdivision.
			GSOUND_FORCE_INLINE void setColumnCount( Size newNumColumns )
			{
				numColumns = (UInt8)math::min( newNumColumns, (Size)math::max<UInt8>() );
			}
			
			
		//********************************************************************************
		//******	Edge Vertex Accessor Method
			
			
			/// Get the pointers to the vertices which share the edge at the specified index.
			GSOUND_INLINE void getEdgeVertices( Index edgeIndex, const SoundVertex*& v1, const SoundVertex*& v2 ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access triangle edge with invalid index." );
				
				if ( edgeIndex == 0 )
				{
					v1 = vertex[0];
					v2 = vertex[1];
				}
				else if ( edgeIndex == 1 )
				{
					v1 = vertex[0];
					v2 = vertex[2];
				}
				else if ( edgeIndex == 2 )
				{
					v1 = vertex[1];
					v2 = vertex[2];
				}
			}
			
			
			/// Return the vertex for this triangle that is not part of the specified edge.
			GSOUND_INLINE const SoundVertex* getEdgeFreeVertex( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access triangle edge with invalid index." );
				
				switch ( edgeIndex )
				{
					case 0:
						return vertex[2];
					case 1:
						return vertex[1];
					case 2:
						return vertex[0];
					default:
						return NULL;
				}
			}
			
			
			/// Return the vertex index for this triangle that is not part of the specified edge.
			GSOUND_INLINE static Index getEdgeFreeVertexIndex( Index edgeIndex )
			{
				GSOUND_DEBUG_ASSERT_MESSAGE( edgeIndex < 3, "Cannot access triangle edge with invalid index." );
				
				switch ( edgeIndex )
				{
					case 0:
						return 2;
					case 1:
						return 1;
					case 2:
						return 0;
					default:
						return 0;
				}
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Return a hash code for this triangle.
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return Hash(UInt64(this) >> 2)*Hash(0x2DA6B317);
			}
			
			
		//********************************************************************************
		//******	Static Helper Methods
			
			
			/// Get the index of the edge shared by the vertices with the specified indices within the triangle.
			GSOUND_FORCE_INLINE static Index getEdgeIndex( Index vIndex1, Index vIndex2 )
			{
				return vIndex1 + vIndex2 - 1;
			}
			
			
			/// Get the indices within the triangle of the vertices that that share the specified edge.
			GSOUND_INLINE static void getEdgeVertexIndices( Index edgeIndex, Index& vIndex1, Index& vIndex2 )
			{
				if ( edgeIndex == 0 )
				{
					vIndex1 = 0;
					vIndex2 = 1;
				}
				else if ( edgeIndex == 1 )
				{
					vIndex1 = 0;
					vIndex2 = 2;
				}
				else if ( edgeIndex == 2 )
				{
					vIndex1 = 1;
					vIndex2 = 2;
				}
			}
			
			
		//********************************************************************************
		//******	Triangle Intersection Method
			
			
			GSOUND_FORCE_INLINE Bool intersectRay( const Ray3f& ray, Real& distanceAlongRay ) const
			{
				// find the edge vectors of the triangle
				Vector3f v1ToV2 = *vertex[1] - *vertex[0];
				Vector3f v1ToV3 = *vertex[2] - *vertex[0];
				
				// the vector perpendicular to edge 2 and the ray's direction
				Vector3f pvec = math::cross( ray.direction, v1ToV3 );
				
				Real det = math::dot( v1ToV2, pvec );
				
				if ( math::abs(det) < math::epsilon<Real>() )
					return false;
				
				Real inverseDet = Real(1) / det;
				
				Vector3f v1ToSource = ray.origin - *vertex[0];
				
				Real u = math::dot( v1ToSource, pvec ) * inverseDet;
				
				if ( u < Real(0) || u > Real(1) )
					return false;
				
				Vector3f qvec = math::cross( v1ToSource, v1ToV2 );
				
				Real v = math::dot( ray.direction, qvec ) * inverseDet;
				
				if ( v < Real(0) || u + v > Real(1) )
					return false;
				
				distanceAlongRay = math::dot( v1ToV3, qvec ) * inverseDet;
				
				return distanceAlongRay > Real(0);
			}
			
			
			
	//private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// Pointers to the vertices of this triangle.
			const SoundVertex* vertex[3];
			
			
			/// A pointer to the triangle's material.
			const SoundMaterial* material;
			
			
			/// A pointer to the diffraction edge objects for this triangle for each edge.
			/**
			  * The edge pointer is NULL if that edge is not a diffraction edge.
			  *
			  * Edge indices are denoted as follows:
			  * 0 = edge between vertices 0 and 1
			  * 1 = edge between vertices 0 and 2
			  * 2 = edge between vertices 1 and 2
			  */
			const DiffractionEdge* edge[3];
			
			
			/// The plane in which the triangle lies.
			Plane3f plane;
			
			
			/// The area of the triangle in square units.
			Real area;
			
			
			/// The index of the subdivision key vertex for this triangle.
			UInt8 keyVertex;
			
			
			/// The number of rows there are for this triangle's subdivision.
			UInt8 numRows;
			
			
			/// The number of columns there are for this triangle's subdivision.
			UInt8 numColumns;
			
			
		//********************************************************************************
		//******	Private Friend Class Declaration
			
			
			/// Declare the SoundMeshLOD class as a friend so that it can access internal data.
			//friend class SoundMeshLOD;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_INTERNAL_SOUND_TRIANGLE_H
