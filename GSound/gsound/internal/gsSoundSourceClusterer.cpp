/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundSourceClusterer.cpp
 * Contents:    gsound::internal::SoundSourceClusterer class implementation
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


#include "gsSoundSourceClusterer.h"


#include "../gsSoundScene.h"
#include "../gsSoundListener.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundSourceClusterer:: SoundSourceClusterer()
	:	root( NULL ),
		numClusters( 0 ),
		numSources( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundSourceClusterer:: addSource( SoundSource* newSource )
{
	if ( newSource == NULL )
		return false;
	
	newSources.add( newSource );
	numSources++;
	
	return true;
}




Bool SoundSourceClusterer:: removeSource( SoundSource* newSource )
{
	if ( newSource == NULL || root == NULL )
		return false;
	
	if ( removeSourceRecursive( newSource, root ) )
	{
		numSources--;
		return true;
	}
	else
		return false;
}




Bool SoundSourceClusterer:: removeSourceRecursive( SoundSource* source, Node* node )
{
	if ( node->isLeaf() )
		return removeSourceFromNode( source, node );
	else
	{
		Index childIndex = node->getChildIndex( source->getPosition() );
		
		if ( node->children[childIndex] )
			return removeSourceRecursive( source, node->children[childIndex] );
		else
			return false;
	}
}




Bool SoundSourceClusterer:: removeSourceFromNode( SoundSource* source, Node* node )
{
	return node->leafData->sources.removeUnordered( source );
}




Bool SoundSourceClusterer:: removeSourceAtIndexInNode( Index sourceIndex, Node* node )
{
	return node->leafData->sources.removeAtIndexUnordered( sourceIndex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundSourceClusterer:: updateClusters( const SoundListener& listener, const SoundScene& scene,
											Real innerClusteringAngle, Real outerClusteringAngle )
{
	//***********************************************************************************
	// Update the octree with the new source positions.
	
	Real innerClusteringAngleRadians = math::degreesToRadians( innerClusteringAngle );
	Real outerClusteringAngleRadians = math::degreesToRadians( outerClusteringAngle );
	
	updateOctree( listener, outerClusteringAngleRadians );
	
	//***********************************************************************************
	// Cluster the sources in their current configuration in the octree.
	
	// Reset the cluster list.
	numClusters = 0;
	
	if ( root )
		clusterSourcesRecursive( listener, scene, innerClusteringAngleRadians, outerClusteringAngleRadians, root );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Octree Update Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundSourceClusterer:: updateOctree( const SoundListener& listener, Real outerClusteringAngle )
{
	// Update if the root node is not NULL.
	if ( root )
		updateOctreeRecursive( listener, outerClusteringAngle, root );
	
	//***********************************************************************************
	
	// Insert the new sources.
	const Size numNewSources = newSources.getSize();
	
	for ( Index s = 0; s < numNewSources; s++ )
	{
		SoundSource* newSource = newSources[s];
		
		if ( root == NULL )
		{
			Vector3f v = newSource->getPosition() - listener.getPosition();
			Real nodeDistance;
			v.normalize( nodeDistance );
			
			// Compute the minimum node radius.
			Real minRadius = nodeDistance*math::tan( Real(0.5)*outerClusteringAngle );
			
			root = util::construct<Node>( newSource->getPosition(), minRadius );
		}
		
		insertSourceRecursive( listener, outerClusteringAngle, newSource, root );
	}
	
	newSources.clear();
}




void SoundSourceClusterer:: updateOctreeRecursive( const SoundListener& listener, Real outerClusteringAngle, Node* node )
{
	if ( node->isLeaf() )
	{
		// Check to make sure that this leaf node is the right size and update the sources.
		Vector3f v = node->position - listener.getPosition();
		Real nodeDistance;
		v.normalize( nodeDistance );
		
		// Compute the minimum node radius.
		Real minRadius = nodeDistance*math::tan( Real(0.5)*outerClusteringAngle );
		
		// Reorganize the node if it is too small or too large.
		if ( node->radius < minRadius*Real(0.5) )
		{
			// This node is too small, turn its parent into a leaf node containing all child sources.
			Node* parent = node->parent;
			parent->leafData = util::construct<LeafData>();
			
			for ( Index i = 0; i < 8; i++ )
			{
				if ( parent->children[i] )
				{
					// Add the sources from the child node to the parent.
					getNodeSourcesRecursive( parent->children[i], parent->leafData->sources );
					
					// Destroy the child node.
					util::destruct( parent->children[i] );
				}
			}
			
			// Deallocate the child pointer array.
			util::deallocate( parent->children );
			parent->children = NULL;
		}
		else if ( Real(0.5)*node->radius > minRadius )
		{
			// This node is too big. Remove its sources and reinsert them later.
			// It will be split when inserting them.
			if ( node->leafData )
			{
				newSources.addAll( node->leafData->sources );
				util::destruct( node->leafData );
				node->leafData = NULL;
			}
		}
		else if ( node->leafData )
		{
			// Make sure that the sources are all contained in the node
			for ( Index s = 0; s < node->leafData->sources.getSize(); )
			{
				SoundSource* source = node->leafData->sources[s];
				
				if ( !node->contains( source->getPosition() ) )
				{
					// Source can't be in this node, remove it from the node and re-insert into the tree.
					removeSourceAtIndexInNode( s, node );
					
					// Add it to the list of new sources to be inserted.
					newSources.add( source );
					continue;
				}
				
				s++;
			}
		}
	}
	else
	{
		// Update child nodes.
		for ( Index i = 0; i < 8; i++ )
		{
			if ( node->children && node->children[i] )
				updateOctreeRecursive( listener, outerClusteringAngle, node->children[i] );
		}
	}
}




void SoundSourceClusterer:: insertSourceRecursive( const SoundListener& listener, Real outerClusteringAngle, SoundSource* source, Node* node )
{
	const Vector3f& sourcePosition = source->getPosition();
	
	if ( node->contains( sourcePosition ) )
	{
		if ( node->isLeaf() )
		{
			Vector3f v = node->position - listener.getPosition();
			Real nodeDistance;
			v.normalize( nodeDistance );
			
			// Compute the minimum node radius.
			Real minRadius = nodeDistance*math::tan( Real(0.5)*outerClusteringAngle );
			
			if ( Real(0.5)*node->radius <= minRadius )
			{
				// This leaf node is the right size based on the clustering angle.
				
				// Create the leaf data for this node if it has not been created yet.
				if ( node->leafData == NULL )
					node->leafData = util::construct<LeafData>();
				
				// Add this source to the node.
				node->leafData->sources.add( source );
			}
			else
			{
				// Add a child to this leaf node, splitting it.
				Index childIndex = node->getChildIndex( sourcePosition );
				
				// Determine the position and radius of the node.
				Real radius = Real(0.5)*node->radius;
				node->children = om::util::constructArray<Node*>( 8, (Node*)NULL );
				node->children[childIndex] = util::construct<Node>( node->getChildPosition(childIndex), radius, node );
				
				// Insert into the child.
				return insertSourceRecursive( listener, outerClusteringAngle, source, node->children[childIndex] );
			}
		}
		else
		{
			Index childIndex = node->getChildIndex( sourcePosition );
			
			if ( node->children[childIndex] == NULL )
			{
				// Create a new child node.
				// Determine the position and radius of the node.
				Real radius = Real(0.5)*node->radius;
				node->children[childIndex] = util::construct<Node>( node->getChildPosition(childIndex), radius, node );
			}
			
			// Insert into the correct child.
			return insertSourceRecursive( listener, outerClusteringAngle, source, node->children[childIndex] );
		}
	}
	else
	{
		if ( node->parent )
		{
			insertSourceRecursive( listener, outerClusteringAngle, source, node->parent );
		}
		else
		{
			// Create a parent node for this (root) node.
			// Determine the position and radius of the node.
			Real radius = Real(2)*node->radius;
			Vector3f position = node->position;
			position.x += sourcePosition.x > node->position.x ? node->radius : -node->radius;
			position.y += sourcePosition.y > node->position.y ? node->radius : -node->radius;
			position.z += sourcePosition.z > node->position.z ? node->radius : -node->radius;
			
			// Create the new root node and set the child pointer.
			root = util::construct<Node>( position, radius );
			node->parent = root;
			root->children = om::util::constructArray<Node*>( 8, (Node*)NULL );
			root->children[root->getChildIndex( node->position )] = node;
			
			// Insert recursively into the root node.
			insertSourceRecursive( listener, outerClusteringAngle, source, root );
		}
	}
}




void SoundSourceClusterer:: getNodeSourcesRecursive( Node* node, ArrayList<SoundSource*>& sources )
{
	if ( node->isLeaf() )
	{
		if ( node->leafData )
			sources.addAll( node->leafData->sources );
	}
	else
	{
		for ( Index i = 0; i < 8; i++ )
		{
			if ( node->children[i] )
				getNodeSourcesRecursive( node->children[i], sources );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Clustering Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundSourceClusterer:: clusterSourcesRecursive( const SoundListener& listener, const SoundScene& scene,
													Real innerClusteringAngleRadians, Real outerClusteringAngleRadians, Node* node )
{
	if ( node->isLeaf() )
	{
		if ( node->leafData )
		{
			const ArrayList<SoundSource*>& leafSources = node->leafData->sources;
			const Size numLeafSources = leafSources.getSize();
			
			if ( leafSourcesClustered.getSize() < numLeafSources )
				leafSourcesClustered.setSize( numLeafSources );
			
			for ( Index s = 0; s < numLeafSources; s++ )
				leafSourcesClustered[s] = false;
			
			// For each source that has not yet been clustered,
			// find all other sources that could be in a cluster with that source.
			for ( Index s = 0; s < numLeafSources; s++ )
			{
				// Skip sources that have already been clustered.
				if ( leafSourcesClustered[s] )
					continue;
				
				SoundSource* source = leafSources[s];
				
				// Skip disabled sources.
				if ( !source->getIsEnabled() )
					continue;
				
				Vector3f sourceVector = (source->getPosition() - listener.getPosition()).normalize();
				
				// Mark this source as clustered.
				leafSourcesClustered[s] = true;
				
				// Create a new cluster for this source.
				SoundSourceCluster& sourceCluster = getNewSourceCluster();
				sourceCluster.addSource( source );
				Real maxD = 0;
				
				for ( Index s2 = s + 1; s2 < numLeafSources; s2++ )
				{
					// Skip other sources that have already been clustered.
					if ( leafSourcesClustered[s2] )
						continue;
					
					SoundSource* source2 = leafSources[s2];
					
					// Skip disabled sources.
					if ( !source2->getIsEnabled() )
						continue;
					
					Vector3f source2Vector = (source2->getPosition() - listener.getPosition()).normalize();
					
					// Determine the distance between the sources.
					Vector3f sToS2 = source2->getPosition() - source->getPosition();
					Real d;
					sToS2.normalize( d );
					maxD = math::max( d, maxD );
					
					// Compute the max clustering distances for the source midpoint.
					Vector3f midpoint = math::midpoint( source->getPosition(), source2->getPosition() );
					Real midD = (midpoint - listener.getPosition()).getMagnitude();
					Real outerD = 2*midD*math::tan( Real(0.5)*innerClusteringAngleRadians );
					
					// Compute the angle between the sources from the listener's perspective.
					Real angle = math::acos( math::dot( sourceVector, source2Vector ) );
					
					if ( angle < innerClusteringAngleRadians && d < outerD )
					{
						Ray3f testRay( source->getPosition() + sToS2*source->getRadius(), sToS2 );
						Real rayDistance = math::max( d - source->getRadius() - source2->getRadius(), Real(0) );
						
						// Skip this source if it is not visible from the first one.
						if ( !scene.intersectRay( testRay, rayDistance ) )
							continue;
						
						// Cluster the sources.
						leafSourcesClustered[s2] = true;
						sourceCluster.addSource( source2 );
					}
				}
				
				// After the cluster has been finalized, determine whether or not the sources should be merged.
				// Sources are merged when the angular size of the cluster is less than the inner clustering angle.
				
				Vector3f centroid = sourceCluster.getCentroid();
				const Size numClusteredSources = sourceCluster.getSourceCount();
				
				// Compute the bounding sphere of the sources in the cluster, centered at the centroid.
				Sphere3f bs( centroid, sourceCluster.getSource(0)->getRadius() );
				
				for ( Index i = 0; i < numClusteredSources; i++ )
				{
					Sphere3f sourceBS( sourceCluster.getSource(i)->getPosition(), sourceCluster.getSource(i)->getRadius() );
					Real maxR = bs.position.getDistanceTo( sourceBS.position );// + sourceBS.radius;
					
					if ( maxR > bs.radius )
						bs.radius = maxR;
				}
				
				// Set the cluster's position and radius.
				sourceCluster.setPosition( bs.position );
				sourceCluster.setRadius( bs.radius );
				
				// Compute the angular size of the cluster from the listener's perspective.
				
				Sphere3f bs2( sourceCluster.getSource(0)->getPosition(), sourceCluster.getSource(0)->getRadius() );
				
				for ( Index i = 1; i < numClusteredSources; i++ )
					bs2.enlargeFor( sourceCluster.getSource(i)->getPosition() );
				
				maxD = math::max( maxD, sourceCluster.getSource(0)->getRadius()*Real(2) );
				
				Real angularSize = Real(2)*math::atan( math::min( Real(0.5)*maxD, bs2.radius ) / listener.getPosition().getDistanceTo( bs2.position ) );
				
				// If the max spread is less than the inner clustering angle, merge the sources.
				//if ( angularSize < Real(1.1)*innerClusteringAngleRadians )
					sourceCluster.setIsMerged( true );
				/*else
				{
					sourceCluster.setIsMerged( false );
					
					// Determine how much to interpolate the source positions with the merged position.
					Real interp = math::clamp( math::max( (outerClusteringAngleRadians - angularSize), Real(0) )/
												(outerClusteringAngleRadians - innerClusteringAngleRadians),
												Real(0), Real(1) );
					
					// Set the interpolated source positions.
					for ( Index i = 0; i < numClusteredSources; i++ )
					{
						sourceCluster.setSourcePosition( i, centroid*interp +
															sourceCluster.getSource(i)->getPosition()*(Real(1) - interp) );
					}
				}*/
			}
		}
	}
	else
	{
		for ( Index i = 0; i < 8; i++ )
		{
			if ( node->children[i] )
				clusterSourcesRecursive( listener, scene, innerClusteringAngleRadians,
										outerClusteringAngleRadians, node->children[i] );
		}
	}
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
