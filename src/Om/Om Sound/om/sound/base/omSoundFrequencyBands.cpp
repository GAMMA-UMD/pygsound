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

#include "omSoundFrequencyBands.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




FrequencyBands:: FrequencyBands()
{
}




FrequencyBands:: FrequencyBands( Size numBands )
	:	bands( numBands )
{
	// Logarithmically distribute the frequencies in the useable human hearing range.
	const Float minF = 40.0f;
	const Float maxF = 15000.0f;
	const Float lnMinF = math::ln( minF );
	const Float lnMaxF = math::ln( maxF );
	
	for ( Index i = 0; i < numBands; i++ )
	{
		const Float a = (Float(i) + Float(0.5)) / Float(numBands);
		const Float lnF = lnMinF + a*(lnMaxF - lnMinF);
		
		bands.add( math::exp( lnF ) );
	}
	
	this->updateCrossovers();
}




FrequencyBands:: FrequencyBands( const Float* bandCenters, Size numBands )
{
	for ( Index i = 0; i < numBands; i++ )
		bands.add( bandCenters[i] );
	
	this->updateCrossovers();
}




//##########################################################################################
//##########################################################################################
//############
//############		Crossover Update Method
//############
//##########################################################################################
//##########################################################################################




void FrequencyBands:: updateCrossovers()
{
	const Size numCrossovers = this->getCrossoverCount();
	
	// Compute the crossover points, midway between the frequencies in log space.
	for ( Index i = 0; i < numCrossovers; i++ )
		bands[i].max = math::exp( Float(0.5)*(math::ln(bands[i].center) + math::ln(bands[i+1].center)) );
	
	bands[numCrossovers].max = math::max( bands[numCrossovers].center, 20000.0f );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
