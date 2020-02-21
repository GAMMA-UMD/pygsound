/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_OM_SOUND_FREQUENCY_BANDS_H
#define INCLUDE_OM_SOUND_FREQUENCY_BANDS_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that describes a partitioning of frequency space into frequency bands.
class FrequencyBands
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a frequency bands object with no bands.
			FrequencyBands();
			
			
			/// Create frequency bands with the specified number of logarithmically-distributed frequency bands across the human hearing range.
			FrequencyBands( Size numBands );
			
			
			/// Create a frequency bands object for the specified array of frequency band centers.
			/**
			  * The crossover points are chosen to be midway between the band centers in log space.
			  */
			FrequencyBands( const Float* bandCenters, Size numBands );
			
			
		//********************************************************************************
		//******	Band Accessor Methods
			
			
			/// Return the number of frequency bands there are.
			OM_INLINE Size getBandCount() const
			{
				return bands.getSize();
			}
			
			
			/// Return the center frequency for the frequency band with the specified index.
			/**
			  * The band index must be less than the total number of bands in the response.
			  */
			OM_INLINE Float getBandCenter( Index bandIndex ) const
			{
				return bands[bandIndex].center;
			}
			
			
			/// Return the frequency range in hertz for the frequency band with the specified index.
			/**
			  * The band index must be less than the total number of bands in the response.
			  */
			OM_INLINE AABB1f getBandRange( Index bandIndex ) const
			{
				return AABB1f( bandIndex == 0 ? 0.0f : bands[bandIndex-1].max, bands[bandIndex].max );
			}
			
			
		//********************************************************************************
		//******	Crossover Accessor Methods
			
			
			/// Return the number of crossover points in this frequency bands object.
			/**
			  * There is always one less crossover than the number of bands.
			  */
			OM_INLINE Size getCrossoverCount() const
			{
				return bands.getSize() == 0 ? 0 : bands.getSize() - 1;
			}
			
			
			/// Return the crossover point at the specified index.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			OM_INLINE Float getCrossover( Index crossoverIndex ) const
			{
				return bands[crossoverIndex].max;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not two frequency band objects are equal.
			OM_INLINE Bool operator == ( const FrequencyBands& other ) const
			{
				return bands == other.bands;
			}
			
			
			/// Return whether or not two frequency band objects are not equal.
			OM_INLINE Bool operator != ( const FrequencyBands& other ) const
			{
				return !(bands == other.bands);
			}
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores information for a frequency band.
			class Band
			{
				public:
					
					/// Create a new frequency band with the specified center frequency.
					OM_INLINE Band( Float newCenter )
						:	center( newCenter ),
							max( math::max<Float>() )
					{
					}
					
					/// Return whether or not two band objects are equal.
					OM_INLINE Bool operator == ( const Band& other ) const
					{
						return center == other.center && max == other.max;
					}
					
					/// The frequency for this frequency response point.
					Float center;
					
					/// The crossover point above this frequency band..
					Float max;
					
			};
			
			
		//********************************************************************************
		//******	Private Methods
			
			
			/// Update the crossover points based on the current frequency band centers.
			void updateCrossovers();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the frequency bands sorted in increasing order by frequency.
			ArrayList<Band> bands;
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FREQUENCY_BANDS_H
