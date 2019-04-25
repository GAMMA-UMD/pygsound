/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListenerIR.h
 * Contents:    gsound::SoundListenerIR class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_LISTENER_IR_H
#define INCLUDE_GSOUND_SOUND_LISTENER_IR_H


#include "gsConfig.h"


#include "gsSoundSourceIR.h"
#include "gsSoundListener.h"
#include "gsFrequencyBands.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores sound propagation output for a sound listener in a scene.
/**
  * A SoundListenerIR contains all of the sound propagation information for
  * a single SoundListener in a scene. The class itself contains a buffer of
  * SoundSourceIR objects which contain the sound propagation paths and sampled IRs for
  * each active source in the scene.
  *
  * In practice, the user does not need to directly access this class, just pass
  * it to the necessary functions. The manipulation of the data structure happens
  * automatically. However, the interface for querying sound paths is left
  * public in case one wishes to examine the output of the sound propagation
  * system.
  */
class SoundListenerIR
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty listener IR which holds data for 0 sound sources.
			SoundListenerIR();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound listener IR, releasing all internal resources.
			~SoundListenerIR();
			
			
		//********************************************************************************
		//******	Sound Listener Accessor Methods
			
			
			/// Return a pointer to the sound listener associated with this SoundListenerIR.
			GSOUND_INLINE const SoundListener* getListener() const
			{
				return listener;
			}
			
			
			/// Set the sound listener that this SoundListenerIR is associated with.
			GSOUND_INLINE void setListener( const SoundListener* newListener )
			{
				listener = newListener;
			}
			
			
			/// Set the sound listener that this SoundListenerIR is associated with to NULL.
			GSOUND_INLINE void clearListener()
			{
				listener = NULL;
			}
			
			
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			/// Return the number of sound sources that this SoundListenerIR can hold data for.
			GSOUND_INLINE Size getSourceCount() const
			{
				return numSources;
			}
			
			
			/// Set the number of sound sources that this SoundListenerIR can hold data for.
			/**
			  * If necessary, this method increases the number of SoundSourceIR objects
			  * that this propagation path buffer holds. A typical implementation will avoid deallocating
			  * existing SoundSourceIR objects if the number of sound sources is
			  * decreased.
			  * 
			  * @param newNumSources - the new number of sound sources that this buffer should hold data for.
			  */
			void setSourceCount( Size newNumSources );
			
			
		//********************************************************************************
		//******	Sound Source IR Accessor Methods
			
			
			/// Return a reference to the SoundSourceIR for the sound source at the specified index.
			/**
			  * If the specified sound source index is a valid index, a reference to the
			  * SoundSourceIR for the sound source at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param sourceIndex - the index of the sound source whose propagation path buffer should be accessed.
			  * @return a reference to the SoundSourceIR for the specified sound source.
			  */
			GSOUND_INLINE SoundSourceIR& getSourceIR( Index sourceIndex )
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < numSources );
				
				return sourceIRs[sourceIndex];
			}
			
			
			/// Return a const reference to the SoundSourceIR for the sound source at the specified index.
			/**
			  * If the specified sound source index is a valid index, a const reference to the
			  * SoundSourceIR for the sound source at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param sourceIndex - the index of the sound source whose propagation path buffer should be accessed.
			  * @return a const reference to the SoundSourceIR for the specified sound source.
			  */
			GSOUND_INLINE const SoundSourceIR& getSourceIR( Index sourceIndex ) const
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < numSources );
				
				return sourceIRs[sourceIndex];
			}
			
			
		//********************************************************************************
		//******	Frequency Bands Accessor Methods
			
			
			/// Return a reference to an object specifying which frequencies the bands in this IR correspond to.
			GSOUND_INLINE const FrequencyBands& getFrequencies() const
			{
				return frequencies;
			}
			
			
			/// Set an object specifying which frequencies the bands in this correspond to.
			GSOUND_INLINE void setFrequencies( const FrequencyBands& newFrequencies )
			{
				frequencies = newFrequencies;
			}
			
			
		//********************************************************************************
		//******	Impulse Response Clear Methods
			
			
			/// Clear all paths and impulses from this listener IR.
			/**
			  * This method does not change the number of sources that the IR supports.
			  * It only resets every SoundSourceIR to a default state with no paths or IR samples.
			  */
			void clear();
			
			
			/// Reset this listener IR, deallocating all internal storage and removing all sources.
			void reset();
			
			
		//********************************************************************************
		//******	Path Count Accessor Method
			
			
			/// Return the total number of propagation paths that this SoundListenerIR is holding.
			/**
			  * This method iterates over all internal SoundSourceIR objects
			  * and adds together the number of propagation paths for every buffer. This value
			  * is then returned.
			  * 
			  * @return the total number of propagation paths that this SoundListenerIR is holding.
			  */
			Size getPathCount() const;
			
			
		//********************************************************************************
		//******	IR Windowing Methods
			
			
			/// Trim the listener's source IR lengths based on the listener's threshold of hearing.
			void trim();
			
			
		//********************************************************************************
		//******	Storage Size Accessor Methods
			
			
			/// Compute and return the sound pressure in pascals at the listener's position.
			FrequencyBandResponse getPressure() const;
			
			
			/// Compute and return the sound pressure level (in dB SPL, per frequency band) at the listener's position.
			FrequencyBandResponse getPressureLevel() const;
			
			
		//********************************************************************************
		//******	Storage Size Accessor Methods
			
			
			/// Return the approximate size in bytes of the memory used for this IR.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the impulse responses for each sound source.
			Array<SoundSourceIR> sourceIRs;
			
			
			/// The number of sound sources that this IR holds data for.
			/**
			  * The number of sound sources is stored separately in order to not depend on
			  * the size of the SoundSourceIR array to indicate the number
			  * of sound sources. This results in less reallocations of the source IRs
			  * in typical use cases.
			  */
			Size numSources;
			
			
			/// A pointer to the listener associated with this IR.
			const SoundListener* listener;
			
			
			/// An object specifying which frequencies this IR corresponds to.
			FrequencyBands frequencies;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_LISTENER_IR_H
