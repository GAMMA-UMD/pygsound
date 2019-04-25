/*
 * Project:     GSound
 * 
 * File:        gsound/gsFrequencyBands.h
 * Contents:    gsound::FrequencyBands class declaration
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


#ifndef INCLUDE_GSOUND_FREQUENCY_BANDS_H
#define INCLUDE_GSOUND_FREQUENCY_BANDS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies a fixed number of frequency bands for which sound should be propagated and rendered.
/**
  * The frequency bands determine the frequencies for the values stored in
  * in a FrequencyBandResponse and are a global simulation parameter.
  * It allows the user to specify per-simulation the frequencies that are
  * being simulated, thus enabling simulation of independent frequency bands.
  *
  * By default, the frequency bands equally covers the human hearing range, 20 Hz to 20 kHz,
  * with logarithmic spacing of frequencies.
  *
  * The number of frequency bands used by the simulation is determined at compile time
  * and can be changed by setting the value of GSOUND_FREQUENCY_COUNT in gsConfig.h to a multiple
  * of the SIMD width (e.g. 4).
  */
class FrequencyBands
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default frequency band object with the default frequency band centers.
			/**
			  * By default, the frequency bands equally divide the human hearing range
			  * with logarithmic spacing of frequencies.
			  */
			FrequencyBands();
			
			
			/// Create a frequency band object with frequency band centers from the specified array.
			/**
			  * The array must not be NULL and must have at least the correct number of elements.
			  * The constructor reads GSOUND_FREQUENCY_COUNT values from the input array
			  * and uses them to initialize the frequency band centers. The crossover points
			  * are automatically computed from these frequencies.
			  *
			  * @param array - the array of frequency band center frequencies.
			  */
			GSOUND_INLINE FrequencyBands( const Real array[GSOUND_FREQUENCY_COUNT] )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					frequencies[i] = array[i];
				
				updateCrossovers();
			}
			
			
		//********************************************************************************
		//******	Frequency Band Accessor Methods
			
			
			/// Return the number of bands in this frequency bands object.
			/**
			  * The value returned by this method is GSOUND_FREQUENCY_COUNT and is fixed at compilation time
			  * in order to allow simple multiplication of FrequencyBandResponse objects
			  * and to reduce the number of allocations performed.
			  */
			GSOUND_INLINE Size getBandCount() const
			{
				return numFrequencyBands;
			}
			
			
			/// Return the range of frequencies corresponding to the specified band index.
			GSOUND_INLINE AABB1f getBandRange( Index bandIndex ) const
			{
				AABB1f range( 0, math::max<Real>() );
				
				if ( bandIndex > 0 )
					range.min = crossovers[bandIndex - 1];
				
				if ( bandIndex < numCrossovers )
					range.max = crossovers[bandIndex];
				
				return range;
			}
			
			
			/// Return the center frequency of the band with the specified index.
			GSOUND_INLINE Real getBand( Index bandIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return frequencies[bandIndex];
			}
			
			
			/// Return a reference to the center frequency of the band with the specified index.
			GSOUND_INLINE Real& operator [] ( Index bandIndex )
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return frequencies[bandIndex];
			}
			
			
			/// Return the center frequency of the band with the specified index.
			GSOUND_INLINE Real operator [] ( Index bandIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return frequencies[bandIndex];
			}
			
			
		//********************************************************************************
		//******	Crossover Accessor Methods
			
			
			/// Return the number of crossover points there are that separate the frequency bands.
			/**
			  * This value is always 1 less than the number of frequency bands.
			  */
			GSOUND_INLINE Size getCrossoverCount() const
			{
				return numFrequencyBands - 1;
			}
			
			
			/// Return the frequency of the crossover point at the specified crossover index.
			GSOUND_INLINE Real getCrossover( Index crossoverIndex ) const
			{
				return crossovers[crossoverIndex];
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this frequency band object is the same as another.
			GSOUND_INLINE Bool operator == ( const FrequencyBands& other ) const
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
				{
					if ( frequencies[i] != other.frequencies[i] )
						return false;
				}
				
				for ( Index i = 0; i < numCrossovers; i++ )
				{
					if ( crossovers[i] != other.crossovers[i] )
						return false;
				}
				
				return true;
			}
			
			
			/// Return whether or not this frequency band object is different than another.
			GSOUND_INLINE Bool operator != ( const FrequencyBands& other ) const
			{
				return !((*this) == other);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The number of frequency bands in this frequency band object.
			static const Size numFrequencyBands = GSOUND_FREQUENCY_COUNT;
			
			
			/// The number of crossover points in this frequency band object.
			static const Size numCrossovers = GSOUND_FREQUENCY_COUNT - 1;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Update the crossover points based on the current frequency band centers.
			void updateCrossovers();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The center frequencies of these frequency bands.
			Real frequencies[numFrequencyBands];
			
			
			/// The split points (crossover frequencies) for the frequency bands.
			Real crossovers[numCrossovers];
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FREQUENCY_BANDS_H
