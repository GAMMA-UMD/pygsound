/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsPropagationData.h
 * Contents:    gsound::internal::PropagationData class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_DATA_H
#define INCLUDE_GSOUND_PROPAGATION_DATA_H


#include "gsInternalConfig.h"


#include "../gsSoundDetector.h"
#include "../gsSoundSource.h"
#include "gsSoundPathCache.h"
#include "gsDiffusePathCache.h"
#include "gsIRCache.h"
#include "gsVisibilityCache.h"
#include "gsSoundBandDirectivity.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundPropagator;


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




//********************************************************************************
/// A class that stores internal propagation data for a scene.
class PropagationData
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound propagation data object with no data stored.
			PropagationData();
			
			
			/// Create a new sound propagation data object that is a deep copy of another.
			PropagationData( const PropagationData& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound propagation data object, releasing all internal propagation data.
			~PropagationData();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the sound propagation data from another object to this one, performing a deep copy.
			PropagationData& operator = ( const PropagationData& other );
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Remove old cache data from this propagation data object.
			/**
			  * This method iterates through the stored data for each listener
			  * and source and removes that data if it was not updated on
			  * the last frame.
			  */
			void removeOldData();
			
			
			/// Remove all cached data from this propagation data object.
			void reset();
			
			
			/// Reset only the cached path data from this propagation data object.
			void resetPaths();
			
			
		//********************************************************************************
		//******	Data Size Accessor Method
			
			
			/// Return the approximate number of bytes of memory occupied by this sound propagation data.
			Size getSizeInBytes() const;
			
			
			
	//private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that stores internal sound propagation data for a source.
			class SourceData
			{
				public:
					
					GSOUND_INLINE SourceData()
						:	timeStamp( 0 ),
							irLength( 0 ),
							maxIRLength( 0 )
					{
					}
					
					
					/// The index of the most recent propagation frame for this source data.
					/**
					  * This value is used to determine whether or not the source has been
					  * recently updated.
					  */
					Index timeStamp;
					
					
					/// A cache of the IR for this sound source.
					IRCache irCache;
					
					
					/// A cache of diffuse paths for this sound source.
					DiffusePathCache diffusePathCache;
					
					
					/// A cache of the triangle visibility for this sound source.
					VisibilityCache visibilityCache;
					
					
					/// An object that stores the directivity for the sound source for the global simulation frequency bands.
					SoundBandDirectivity directivity;
					
					
					/// The length of this source's impulse response on the last frame.
					Float irLength;
					
					
					/// The current max allowed length of this source's source impulse responses on the last frame.
					Float maxIRLength;
					
					
			};
			
			
			/// A class that stores internal sound propagation data for a listener.
			class ListenerData
			{
				public:
					
					/// Construct a default-initialized listener data object.
					GSOUND_INLINE ListenerData()
						:	timeStamp( 0 ),
							irLength( 0 ),
							maxIRLength( 0 )
					{
					}
					
					
					/// Construct a deep copy of another listener data object.
					GSOUND_INLINE ListenerData( const ListenerData& other )
						:	timeStamp( other.timeStamp ),
							soundPathCache( other.soundPathCache ),
							irLength( other.irLength ),
							maxIRLength( other.maxIRLength )
					{
						HashMap< const SoundSource*, Shared<SourceData> >::ConstIterator iterator = other.sources.getIterator();
						
						while ( iterator )
						{
							const SoundSource* source = iterator.getKey();
							sources.add( source->getHashCode(), source, Shared<SourceData>::construct( *iterator.getValue() ) );
							
							iterator++;
						}
					}
					
					
					/// The index of the most recent propagation frame for this listener data.
					/**
					  * This value is used to determine whether or not the listener has been
					  * recently updated.
					  */
					Index timeStamp;
					
					
					/// A cache containing the set of previously valid propagation paths for this listener.
					SoundPathCache soundPathCache;
					
					
					/// A map from sound sources to the (possibly shared) source data for those sources.
					HashMap< const SoundSource*, Shared<SourceData> > sources;
					
					
					/// The length of this source's impulse response on the last frame.
					Float irLength;
					
					
					/// The current max allowed length of this listener's source impulse responses on the last frame.
					Float maxIRLength;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Friend Class Declarations
			
			
			/// Declare the sound propagation class as a friend so that it can update internal data.
			friend class gsound::SoundPropagator;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A map from sound listeners to the (possibly shared) listener data for those listeners.
			HashMap< const SoundListener*, Shared<ListenerData> > listeners;
			
			
			/// The current frame timestamp, used to determine the age of cached information.
			/**
			  * In order for the caching of sound information to function properly, it is
			  * necessary for the user to increment this value for each successive frame.
			  */
			Index timeStamp;
			
			
			/// The total time for this propagation data since the start of the simulation.
			Time time;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_DATA_H
