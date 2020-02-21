/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMedium.cpp
 * Contents:    gsound::SoundMedium class implementation
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


#include "gsSoundMedium.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




const SoundMedium SoundMedium:: AIR = getAirMedium( Real(20), Real(101.325), Real(50), FrequencyBands() );




//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




SoundMedium:: SoundMedium()
	:	speedOfSound( getAirSpeedOfSound( Real(20), Real(101.325), Real(50) ) ),
		absorption( getAbsorption( Real(20), Real(101.325), Real(50), FrequencyBands() ) )
{
}




SoundMedium:: SoundMedium( Real newSpeedOfSound, const FrequencyBandResponse& newAirAbsorption )
	:	speedOfSound( math::max( newSpeedOfSound, Real(0) ) ),
		absorption( newAirAbsorption )
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Air Medium Computation Method
//############
//##########################################################################################
//##########################################################################################




SoundMedium SoundMedium:: getAirMedium( Real tempC, Real pressurekPa, Real relativeHumidity,
										const FrequencyBands& frequencies )
{
	return SoundMedium( getAirSpeedOfSound( tempC, pressurekPa, relativeHumidity ),
						getAbsorption( tempC, pressurekPa, relativeHumidity, frequencies ) );
}




//##########################################################################################
//##########################################################################################
//############
//############		Air Speed of Sound Computation Method
//############
//##########################################################################################
//##########################################################################################





Real SoundMedium:: getAirSpeedOfSound( Real tempC, Real pressurekPa, Real relativeHumidity )
{
	// http://www.sengpielaudio.com/calculator-airpressure.htm
	// Calculation of speed of sound in humid air
	// Code from: Dr Richard Lord - http://www.npl.co.uk/acoustics/techguides/speedair
	// Based on the approximate formula found in
	// Owen Cramer, "The variation of the specific heat ratio and the speed of sound in air with temperature, pressure, humidity, and CO2 concentration",
	// The Journal of the Acoustical Society of America (JASA), J. Acoust. Soc. Am. 93(5) p. 2510-2516; formula at p. 2514.
	// Saturation vapour pressure found in 
	// Richard S. Davis, "Equation for the Determination of the Density of Moist Air (1981/91)", Metrologia, 29, p. 67-70, 1992,
	// and a mole fraction of carbon dioxide of 0.0004.
	// The mole fraction is simply an expression of the number of moles of a compound divided by the total number of moles of all the compounds present in the gas.
	
	// temperature deg C
	Real T = math::clamp( tempC, Real(-273.15), Real(100000.0) );
	
	// pressure in pascals
	Real P = Real(1000)*math::clamp( pressurekPa, Real(0), Real(10000) );
	
	// relative humidity
	Real Rh = math::clamp( relativeHumidity, Real(0), Real(100) );
	
	const Real Kelvin = Real(273.15);// For converting to Kelvin
	const Real e = Real(2.71828182845904523536);
	
	// ambient temperature (Kelvin)
	Real T_kel = Kelvin + T;// Measured ambient temp
	
	// Molecular concentration of water vapour calculated from Rh
	// using Giacomos method by Davis (1991) as implemented in DTU report 11b-1997
	Real ENH = Real(3.141593e-8)*P + Real(1.00062) + math::square(T)*Real(5.6e-7);
	
	// These commented lines correspond to values used in Cramer (Appendix)
	// Real PSV1 = math::square(T_kel)*Real(1.2811805e-5) - Real(1.9509874e-2)*T_kel;
	// Real PSV2 = Real(34.04926034) - Real(6.3536311e3) / T_kel;
	Real PSV1 = math::square(T_kel)*Real(1.2378847e-5) - Real(1.9121316e-2)*T_kel;
	Real PSV2 = Real(33.93711047) - Real(6.3431645e3) / T_kel;
	Real PSV = math::pow( e, PSV1 )*math::pow( e, PSV2 );
	
	// Molecular concentration of water vapour
	Real H = Rh*ENH*PSV/P;
	
	// Mole fraction of carbon dioxide and water vapour respectively
	Real Xw = H / Real(100.0);
	// Real Xc = Real(314.0e-6);
	Real Xc = Real(400.0e-6);
	
	// Speed calculated using the method of Cramer from JASA vol 93 p. 2510
	Real C1 = Real(0.603055)*T + Real(331.5024) - math::square(T)*Real(5.28e-4) + (Real(0.1495874)*T + Real(51.471935) -math::square(T)*Real(7.82e-4))*Xw;
	Real C2 = (Real(-1.82e-7) + Real(3.73e-8)*T - math::square(T)*Real(2.93e-10))*P+(Real(-85.20931) - Real(0.228525)*T + math::square(T)*Real(5.91e-5))*Xc;
	Real C3 = math::square(Xw)*Real(2.835149) - math::square(P)*Real(2.15e-13) + math::square(Xc)*Real(29.179762) + Real(4.86e-4)*Xw*P*Xc;
	Real C = C1 + C2 - C3;
	
	return C;
}




//##########################################################################################
//##########################################################################################
//############
//############		Air Absorption Computation Method
//############
//##########################################################################################
//##########################################################################################





FrequencyBandResponse SoundMedium:: getAbsorption( Real tempC, Real pressurekPa, Real relativeHumidity,
													const FrequencyBands& frequencies )
{
	// http://www.sengpielaudio.com/calculator-air.htm
	Real pres = Real(1000)*math::clamp( pressurekPa, Real(0), Real(1000) ); // atmospheric pressure in pascals
	Real temp = math::clamp( tempC, Real(-273.15), Real(100000.0) ); // temp in C
	Real relh = math::clamp( relativeHumidity, Real(0), Real(100) ); // relative humidity.
	
	temp += Real(273.15); // Convert to Kelvin.
	pres /= Real(101325); // Convert to relative pressure.
	
	Real C_humid = Real(4.6151) - Real(6.8346)*math::pow( Real(273.15)/temp, Real(1.261) );
	Real hum = relh * math::pow( Real(10), C_humid ) * pres;
	
	// Convert to relative air temp (re 20 deg C)
	Real tempr = temp / Real(293.15);
	
	Real frO = pres*(Real(24) + Real(4.04e4)*hum*(Real(0.02) + hum)/(Real(0.391) + hum));
	Real frN = pres*math::pow(tempr,Real(-0.5))*(9+280*hum*math::exp(Real(-4.17)*(math::pow(tempr,Real(-1/3))-1)));
	FrequencyBandResponse result;
	
	for ( Index b = 0; b < result.getBandCount(); b++ )
	{
		// Frequency in Hertz
		Real freq_hum = frequencies[b];
		
		// Attenuation in dB/meter
		Real alpha = Real(8.686)*freq_hum*freq_hum*(Real(1.84e-11)*(1/pres)*math::sqrt(tempr) +
					math::pow(tempr,Real(-2.5))*(Real(0.01275)*(math::exp(Real(-2239.1)/temp)*1/(frO+freq_hum*freq_hum/frO)) + 
					Real(0.1068)*(math::exp(Real(-3352)/temp)*1/(frN+freq_hum*freq_hum/frN))));
		
		// Convert sound pressure dB/meter to sound intensity dB/meter (different definition of dB).
		result[b] = Real(0.5)*alpha;
	}
	
	return result;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
