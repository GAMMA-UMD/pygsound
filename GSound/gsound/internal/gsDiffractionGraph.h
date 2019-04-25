/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsDiffractionGraph.h
 * Contents:    gsound::internal::DiffractionGraph class declaration
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


#ifndef INCLUDE_GSOUND_DIFFRACTION_GRAPH_H
#define INCLUDE_GSOUND_DIFFRACTION_GRAPH_H


#include "gsInternalConfig.h"


#include "gsDiffractionEdge.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains edge-to-edge visibility information for diffraction.
class DiffractionGraph
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default empty diffraction graph with no edges or connections.
			GSOUND_INLINE DiffractionGraph()
			{
			}
			
			
			/// Create a default empty diffraction graph with the specified edges but no edge neighbors.
			GSOUND_INLINE DiffractionGraph( const Shared<ArrayList<DiffractionEdge> >& newEdges )
				:	edges( newEdges ),
					edgeNeighbors()
			{
			}
			
			
			/// Create a default empty diffraction graph with no edges or connections.
			GSOUND_INLINE DiffractionGraph( const Shared<ArrayList<DiffractionEdge> >& newEdges,
											const ArrayList<UInt32>& newEdgeNeighbors )
				:	edges( newEdges ),
					edgeNeighbors( newEdgeNeighbors )
			{
			}
			
			
		//********************************************************************************
		//******	Diffraction Edge Accessor Methods
			
			
			/// Return the diffraction edge at the specified index in this SoundMesh.
			GSOUND_FORCE_INLINE const internal::DiffractionEdge& getEdge( Index edgeIndex ) const
			{
				GSOUND_DEBUG_ASSERT( edgeIndex < edges->getSize() );
				
				return (*edges)[edgeIndex];
			}
			
			
			/// Get the number of diffraction edges in this SoundMesh.
			GSOUND_FORCE_INLINE Size getEdgeCount() const
			{
				return edges->getSize();
			}
			
			
		//********************************************************************************
		//******	Diffraction Edge Neighbor Accessor Methods
			
			
			/// Return the diffraction edge which is the neighbor at the specified index in this mesh.
			/**
			  * Neighboring edges are stored as a packed list of indices. Each DiffractionEdge object
			  * has an offset into the list of edge neighbors which indicates the first edge neighbor
			  * index for that edge. Edges are neighboring if they can potentially diffract with each other.
			  */
			GSOUND_FORCE_INLINE const internal::DiffractionEdge& getEdgeNeighbor( Index edgeNeighborIndex ) const
			{
				GSOUND_DEBUG_ASSERT( edgeNeighborIndex < edgeNeighbors.getSize() );
				
				UInt32 edgeIndex = edgeNeighbors[edgeNeighborIndex];
				
				GSOUND_DEBUG_ASSERT( edgeIndex < edges->getSize() );
				
				return (*edges)[edgeIndex];
			}
			
			
			/// Return the diffraction edge which is the neighbor at the specified index in this mesh.
			/**
			  * Neighboring edges are stored as a packed list of indices. Each DiffractionEdge object
			  * has an offset into the list of edge neighbors which indicates the first edge neighbor
			  * index for that edge. Edges are neighboring if they can potentially diffract with each other.
			  */
			GSOUND_FORCE_INLINE Index getEdgeNeighborIndex( Index edgeNeighborIndex ) const
			{
				GSOUND_DEBUG_ASSERT( edgeNeighborIndex < edgeNeighbors.getSize() );
				
				return edgeNeighbors[edgeNeighborIndex];
			}
			
			
			/// Get the number of visible edges in this SoundMesh.
			GSOUND_FORCE_INLINE Size getEdgeNeighborCount() const
			{
				return edgeNeighbors.getSize();
			}
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Method
			
			
			/// Return the approximate size in bytes of this diffraction graph's allocated memory.
			GSOUND_FORCE_INLINE Size getSizeInBytes() const
			{
				return edges->getCapacity()*sizeof(DiffractionEdge) + edgeNeighbors.getCapacity()*sizeof(UInt32);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of all of the diffraction edges in this mesh.
			Shared< ArrayList<DiffractionEdge> > edges;
			
			
			/// A list of packed indices which contain the diffraction edge connections for this graph.
			ArrayList<UInt32> edgeNeighbors;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DIFFRACTION_GRAPH_H
