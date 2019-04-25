/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundSourceClusterer.h
 * Contents:    gsound::internal::SoundSourceClusterer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_CLUSTERER_H
#define INCLUDE_GSOUND_SOUND_SOURCE_CLUSTERER_H


#include "gsInternalConfig.h"


#include "gsSoundSourceCluster.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundScene;


//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class SoundSourceClusterer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty sound source clusterer.
			SoundSourceClusterer();
			
			
		//********************************************************************************
		//******	Source Accessor Methods
			
			
			/// Return the total number of sound sources that are in this source clusterer.
			GSOUND_FORCE_INLINE Size getSourceCount() const
			{
				return numSources;
			}
			
			
			/// Add a new sound source to this clusterer.
			Bool addSource( SoundSource* newSource );
			
			
			/// Remove a sound source from this clusterer.
			Bool removeSource( SoundSource* newSource );
			
			
		//********************************************************************************
		//******	Source Cluster Accessor Methods
			
			
			/// Return the number of sound source clusters that are in this clusterer.
			GSOUND_FORCE_INLINE Size getClusterCount() const
			{
				return numClusters;
			}
			
			
			/// Return a pointer to the sound source cluster at the specified index in this clusterer.
			GSOUND_FORCE_INLINE const SoundSourceCluster* getCluster( Index clusterIndex ) const
			{
				return &sourceClusters[clusterIndex];
			}
			
			
		//********************************************************************************
		//******	Cluster Update Method
			
			
			/// Update the source clusters in this clusterer for the specified listener.
			/**
			  * The clusterer uses the scene to do ray-based occlusion queries.
			  */
			void updateClusters( const SoundListener& listener, const SoundScene& scene,
								Real innerClusteringAngle, Real outerClusteringAngle );
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that encapsulates data needed for an octree leaf node.
			class LeafData
			{
				public:
					
					
					/// A list containing the sources for this leaf node.
					ArrayList<SoundSource*> sources;
					
					
			};
			
			
			/// A node in the source octree.
			class Node
			{
				public:
					
					GSOUND_INLINE Node( const Vector3f& newPosition, Real newRadius, Node* newParent = NULL )
						:	position( newPosition ),
							radius( newRadius ),
							parent( newParent ),
							children( NULL ),
							leafData( NULL )
					{
					}
					
					GSOUND_INLINE ~Node()
					{
						if ( children )
						{
							for ( Index i = 0; i < 8; i++ )
							{
								if ( children[i] )
									util::destruct( children[i] );
							}
							
							util::deallocate( children );
						}
						
						if ( leafData )
							util::destruct( leafData );
					}
					
					/// Return whether or not this node is a leaf node.
					GSOUND_INLINE Bool isLeaf() const
					{
						return children == NULL;
					}
					
					
					/// Return the index of the child of this node that contains the specified query point.
					GSOUND_INLINE Index getChildIndex( const Vector3f& query ) const
					{
						return ((query.x < position.x) << 0) |
								((query.y < position.y) << 1) |
								((query.z < position.z) << 2);
					}
					
					
					/// Return the position of the child of this node that contains the specified query point.
					GSOUND_INLINE Vector3f getChildPosition( Index childIndex ) const
					{
						Real halfRadius = Real(0.5)*radius;
						Vector3f c = position;
						c.x -= (childIndex & (1 << 0)) ? halfRadius : -halfRadius;
						c.y -= (childIndex & (1 << 1)) ? halfRadius : -halfRadius;
						c.z -= (childIndex & (1 << 2)) ? halfRadius : -halfRadius;
						
						return c;
					}
					
					
					/// Return the index of the child of this node that contains the specified query point.
					GSOUND_INLINE Bool contains( const Vector3f& query ) const
					{
						Float slop = 0.0001f;
						Float slopRadius = radius + slop;
						return query.x >= position.x - slopRadius &&
								query.x <= position.x + slopRadius &&
								query.y >= position.y - slopRadius &&
								query.y <= position.y + slopRadius &&
								query.z >= position.z - slopRadius &&
								query.z <= position.z + slopRadius;
					}
					
					
					GSOUND_INLINE AABB3f getAABB() const
					{
						return AABB3f( position - radius, position + radius );
					}
					
					
					/// The position of the center of this octree node in world space.
					Vector3f position;
					
					/// The half-size of this (cubic) octree node in world space.
					Real radius;
					
					/// The parent node of this node.
					Node* parent;
					
					/// A pointer to the block-allocated children of this node, or NULL if this is a leaf node.
					Node** children;
					
					/// A pointer to a list containing the sources for this node if it is a leaf.
					LeafData* leafData;
					
			};
			
			
		//********************************************************************************
		//******	Octree Update Methods
			
			
			/// Update the source octree so that it clusters sources for the specified listener and clustering parameters.
			void updateOctree( const SoundListener& listener, Real outerClusteringAngle );
			
			
			void updateOctreeRecursive( const SoundListener& listener, Real outerClusteringAngle, Node* node );
			
			
			void insertSourceRecursive( const SoundListener& listener, Real outerClusteringAngle, SoundSource* source, Node* node );
			
			
			Bool addSourceToNode( SoundSource* source, const SoundListener& listener, Real outerClusteringAngle, Node* node );
			
			
			Bool removeSourceRecursive( SoundSource* source, Node* node );
			
			Bool removeSourceFromNode( SoundSource* source, Node* node );
			Bool removeSourceAtIndexInNode( Index sourceIndex, Node* node );
			
			
			void getNodeSourcesRecursive( Node* node, ArrayList<SoundSource*>& sources );
			
			
		//********************************************************************************
		//******	Cluster Update Methods
			
			
			void clusterSourcesRecursive( const SoundListener& listener, const SoundScene& scene,
										Real innerClusteringAngleRadians, Real outerClusteringAngleRadians, Node* node );
			
			
			GSOUND_INLINE SoundSourceCluster& getNewSourceCluster()
			{
				if ( numClusters == sourceClusters.getSize() )
					sourceClusters.add( SoundSourceCluster() );
				
				SoundSourceCluster& sourceCluster = sourceClusters[numClusters];
				sourceCluster.clearSources();
				numClusters++;
				
				return sourceCluster;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The root node of the source octree.
			Node* root;
			
			
			/// A list of the sound source clusters that have been created.
			ArrayList<SoundSourceCluster> sourceClusters;
			
			
			/// The number of clusters that are valid in the list of clusters.
			Size numClusters;
			
			
			/// A list of sources that have not yet been inserted into the octree.
			ArrayList<SoundSource*> newSources;
			
			
			/// A temporary array used to keep track of which sources are clustered.
			Array<Bool> leafSourcesClustered;
			
			
			/// The total number of sources that are currently in this sound source clusterer.
			Size numSources;
			
			
			
};





//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_CLUSTERER_H
