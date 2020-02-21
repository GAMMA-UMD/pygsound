/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSceneIR.h
 * Contents:    gsound::SoundSceneIR class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SCENE_IR_H
#define INCLUDE_GSOUND_SOUND_SCENE_IR_H


#include "gsConfig.h"


#include "gsSoundListenerIR.h"
#include "gsSoundListener.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates the propagation path information for one frame of sound propagation for many listeners.
/**
  * A SoundSceneIR contains all of the sound propagation information for
  * all of the listeners in a scene. The class itself contains a buffer of
  * SoundListenerIR objects which contain the propagation paths for
  * each listener in the scene.
  *
  * In practice, one only needs to create a SoundSceneIR object and pass that
  * object between SoundPropagator and SoundListenerRenderer objects. The manipulation
  * of the data structure happens automatically in these classes. However, the interface
  * for querying propagation paths is left public in case one wishes to examine the output
  * of the sound propagation system rather than render it.
  */
class SoundSceneIR
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty listener IR which holds data for 0 sound listeners.
			SoundSceneIR();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy the sound scene IR, releasing all child listener IRs.
			~SoundSceneIR();
			
			
		//********************************************************************************
		//******	Listener Count Accessor Methods
			
			
			/// Return the number of sound listeners that this SoundSceneIR can hold data for.
			GSOUND_INLINE Size getListenerCount() const
			{
				return numListeners;
			}
			
			
			/// Set the number of sound listeners that this SoundSceneIR can hold data for.
			/**
			  * If necessary, this method increases the number of SoundListenerIR objects
			  * that this propagation path buffer holds. This method avoid deallocating
			  * existing SoundListenerIR objects if the number of sound listeners is
			  * decreased.
			  * 
			  * @param newNumListeners - the new number of sound listeners that this buffer should hold data for.
			  */
			void setListenerCount( Size newNumListeners );
			
			
		//********************************************************************************
		//******	Listener IR Accessor Methods
			
			
			/// Return the SoundListenerIR for the sound listener at the specified index.
			/**
			  * If the specified sound listener index is a valid index, a reference to the
			  * SoundListenerIR for the sound listener at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param listenerIndex - the index of the sound listener whose propagation path buffer should be accessed.
			  * @return a reference to the SoundListenerIR for the specified sound listener.
			  */
			GSOUND_INLINE SoundListenerIR& getListenerIR( Index listenerIndex )
			{
				GSOUND_DEBUG_ASSERT( listenerIndex < numListeners );
				
				return listenerIRs[listenerIndex];
			}
			
			
			/// Return the SoundListenerIR for the sound listener at the specified index.
			/**
			  * If the specified sound listener index is a valid index, a const reference to the
			  * SoundListenerIR for the sound listener at that index is returned.
			  * Otherwise, an assertion is raised.
			  * 
			  * @param listenerIndex - the index of the sound listener whose propagation path buffer should be accessed.
			  * @return a const reference to the SoundListenerIR for the specified sound listener.
			  */
			GSOUND_INLINE const SoundListenerIR& getListenerIR( Index listenerIndex ) const
			{
				GSOUND_DEBUG_ASSERT( listenerIndex < numListeners );
				
				return listenerIRs[listenerIndex];
			}
			
			
			/// Return a pointer to the listener IR for the specified listener, or NULL if the listener IR is not found.
			SoundListenerIR* findListenerIR( const SoundListener* listener );
			
			
			/// Return a pointer to the listener IR for the specified listener, or NULL if the listener IR is not found.
			const SoundListenerIR* findListenerIR( const SoundListener* listener ) const;
			
			
		//********************************************************************************
		//******	Clear Methods
			
			
			/// Clear all paths and impulses from this scene IR.
			/**
			  * This method does not change the number of listeners and sources that the IR supports.
			  * It only resets every SoundListenerIR to a default state with no paths or IR samples.
			  */
			void clear();
			
			
			/// Reset this scene IR, deallocating all internal storage and removing all listeners.
			void reset();
			
			
		//********************************************************************************
		//******	Path Count Accessor Method
			
			
			/// Return the total number of propagation paths that this SoundSceneIR is holding.
			/**
			  * This method iterates over all internal SoundListenerIR objects
			  * and adds together the number of propagation paths for every buffer. This value
			  * is then returned.
			  * 
			  * @return the total number of propagation paths that this SoundSceneIR is holding.
			  */
			Size getPathCount() const;
			
			
		//********************************************************************************
		//******	Storage Size Accessor Methods
			
			
			/// Return the approximate size in bytes of the memory used for this IR.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the propagation buffers for each sound listener.
			Array<SoundListenerIR> listenerIRs;
			
			
			/// The number of sound listeners that this buffer holds propagation paths for.
			/**
			  * The number of sound listeners is stored separately in order to not depend on
			  * the size of the SoundListenerIR array to indicate the number
			  * of sound listeners. This results in less reallocations of the propagation path bufffers
			  * in typical use cases.
			  */
			Size numListeners;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SCENE_IR_H
