/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundPathID.h
 * Contents:    gsound::internal::SoundPathID class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PATH_ID_H
#define INCLUDE_GSOUND_SOUND_PATH_ID_H


#include "gsInternalConfig.h"


#include "gsSoundPathPoint.h"
#include "../gsSoundDetector.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to uniquely identify and quickly compare sound sound paths.
/**
  * It contains an object which describes each point of a sound path, as well as
  * an integer hash code for that sound path. Two equal sound paths will
  * have the same hash code. This fact is used to speed up path equality comparisons.
  *
  * If two sound path IDs are equal, then it can be said that 
  * the two sound paths that they describe are the same. By convention, 
  * path points should be placed in the path description in the order in which
  * the sound propagates: source, scene interaction 1, scene interaction 2, ... , listener.
  */
class SoundPathID
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a sound path description object with no path points.
			GSOUND_INLINE SoundPathID()
				:	source( NULL ),
					listener( NULL ),
					hashCode( 1 )
			{
			}
			
			
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			/// Return a pointer to the sound source that starts this sound path ID.
			GSOUND_FORCE_INLINE const SoundDetector* getSource() const
			{
				return source;
			}
			
			
			/// Set a pointer to the sound source that starts this sound path ID.
			GSOUND_FORCE_INLINE void setSource( const SoundDetector* newSource )
			{
				source = newSource;
			}
			
			
		//********************************************************************************
		//******	Sound Listener Accessor Methods
			
			
			/// Return a pointer to the sound listener that ends this sound path ID.
			GSOUND_FORCE_INLINE const SoundDetector* getListener() const
			{
				return listener;
			}
			
			
			/// Set a pointer to the sound listener that ends this sound path ID.
			GSOUND_FORCE_INLINE void setListener( const SoundDetector* newListener )
			{
				listener = newListener;
			}
			
			
		//********************************************************************************
		//******	Path Point Accessor Methods
			
			
			/// Return the number of points along the propagaion path.
			GSOUND_FORCE_INLINE Size getPointCount() const
			{
				return points.getSize();
			}
			
			
			/// Return a reference to the sound path point at the specified index.
			GSOUND_FORCE_INLINE const SoundPathPoint& getPoint( Index i ) const
			{
				return points[i];
			}
			
			
			/// Add a new sound path point to the end of the path ID.
			GSOUND_FORCE_INLINE void addPoint( const SoundPathPoint& newPoint )
			{
				points.add( newPoint );
				hashCode += newPoint.getHashCode()*(SoundPathHash)points.getSize();
			}
			
			
			/// Insert a new sound path point at the specified index.
			GSOUND_FORCE_INLINE void insertPoint( Index index, const SoundPathPoint& newPoint )
			{
				points.insert( index, newPoint );
				recalculateHashCode();
			}
			
			
			/// Remove the last sound path point from this path ID.
			GSOUND_FORCE_INLINE void removeLastPoint()
			{
				// Update the hash code.
				hashCode -= points.getLast().getHashCode()*(SoundPathHash)points.getSize();
				points.removeLast();
			}
			
			
			/// Remove the specified number of sound path points from the end of this path ID.
			GSOUND_FORCE_INLINE void removeLastPoints( Size numPoints )
			{
				points.removeLast( numPoints );
				recalculateHashCode();
			}
			
			
			/// Clear all sound path points from this path ID.
			GSOUND_FORCE_INLINE void clearPoints()
			{
				points.clear();
				hashCode = 1;
			}
			
			
		//********************************************************************************
		//******	Path Description Equality Comparison Methods
			
			
			/// Return whether or not this path description has the same path points as another.
			/**
			  * Path descriptions with the same points but in a different order are
			  * defined as different.
			  */
			GSOUND_FORCE_INLINE Bool operator == ( const SoundPathID& other ) const
			{
				return hashCode == other.hashCode && source == other.source && listener == other.listener && points == other.points;
			}
			
			
			/// Return whether or not this path description has path points that differ from another's.
			/**
			  * Path descriptions with the same points but in a different order are
			  * defined as different.
			  */
			GSOUND_FORCE_INLINE Bool operator != ( const SoundPathID& other ) const
			{
				return hashCode != other.hashCode || source != other.source || listener != other.listener || points != other.points;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Methods
			
			
			/// Return an integer hash code for this sound path ID.
			GSOUND_FORCE_INLINE SoundPathHash getHashCode() const
			{
				return (PointerInt(source) >> 2)*0xD8163841 ^ hashCode;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Method
			
			
			/// Return the size in bytes allocated by this path ID.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return sizeof(SoundPathID) + sizeof(SoundPathPoint)*points.getCapacity();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Recompute the hash code for this path description.
			void recalculateHashCode()
			{
				const Size numPoints = points.getSize();
				hashCode = 1;
				
				for ( Index i = 0; i < numPoints; i++ )
					hashCode += points[i].getHashCode()*SoundPathHash(i + 1);
			}
			
			
		//********************************************************************************
		//******	Private Static Data Methods
			
			
			/// The default capacity of a sound path ID point array.
			/**
			  * Increasing this value will reduce the number of dynamic allocations that need
			  * to be performed, but at the cost of a larger memory footprint for some paths. This value determines
			  * the storage available in the local path point array. Paths exceeding this value require
			  * dynamic memory to be allocated.
			  */
			static const Size DEFAULT_PATH_CAPACITY = 4;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the sound source that starts this sound path ID.
			const SoundDetector* source;
			
			
			/// A pointer to the sound listener that end this sound path ID.
			const SoundDetector* listener;
			
			
			/// A list of the points on the sound path.
			ShortArrayList<SoundPathPoint,DEFAULT_PATH_CAPACITY> points;
			
			
			/// An integral hash code generated for the sound path ID.
			SoundPathHash hashCode;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PATH_ID_H
