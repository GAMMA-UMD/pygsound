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

#ifndef INCLUDE_OM_SOUND_FILTER_VERSION_H
#define INCLUDE_OM_SOUND_FILTER_VERSION_H


#include "omSoundFiltersConfig.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the version number of a SoundFilter class.
class FilterVersion
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a version object representing the default version: 0.0.0
			OM_INLINE FilterVersion()
				:	majorVersion( 0 ),
					minorVersion( 0 ),
					revisionVersion( 0 )
			{
			}
			
			
			/// Create a version object representing the specified major/minor/revision version.
			OM_INLINE FilterVersion( UInt newMajorVersion, UInt newMinorVersion = 0,
											UInt newRevisionVersion = 0 )
				:	majorVersion( newMajorVersion ),
					minorVersion( newMinorVersion ),
					revisionVersion( newRevisionVersion )
			{
			}
			
			
			/// Create a version object that attempts to parse the specified version string.
			/**
			  * The string must be of the form "M.m.r" where 'M' is the major version
			  * number, 'm' is the minor version number, and 'r' is the revision number.
			  * If there is an error in parsing the string, the created version numbers are 0.0.0
			  */
			FilterVersion( const String& versionString );
			
			
		//********************************************************************************
		//******	Major Version Accessor Methods
			
			
			/// Return the major version number for this sound filter version object.
			OM_INLINE UInt getMajor() const
			{
				return majorVersion;
			}
			
			
			/// Set the major version number for this sound filter version object.
			OM_INLINE void setMajor( UInt newMajorVersion )
			{
				majorVersion = newMajorVersion;
			}
			
			
		//********************************************************************************
		//******	Minor Version Accessor Methods
			
			
			/// Return the minor version number for this sound filter version object.
			OM_INLINE UInt getMinor() const
			{
				return minorVersion;
			}
			
			
			/// Set the minor version number for this sound filter version object.
			OM_INLINE void setMinor( UInt newMinorVersion )
			{
				minorVersion = newMinorVersion;
			}
			
			
		//********************************************************************************
		//******	Minor Version Accessor Methods
			
			
			/// Return the revision version number for this sound filter version object.
			OM_INLINE UInt getRevision() const
			{
				return revisionVersion;
			}
			
			
			/// Set the revision version number for this sound filter version object.
			OM_INLINE void setRevision( UInt newRevisionVersion )
			{
				revisionVersion = newRevisionVersion;
			}
			
			
		//********************************************************************************
		//******	Version Comparison Operators
			
			
			/// Return whether or not this filter version number is exactly equal to another version number.
			OM_INLINE Bool operator == ( const FilterVersion& other ) const
			{
				return majorVersion == other.majorVersion && minorVersion == other.minorVersion && 
						revisionVersion == other.revisionVersion;
			}
			
			
			/// Return whether or not this filter version number is not equal to another version number.
			OM_INLINE Bool operator != ( const FilterVersion& other ) const
			{
				return majorVersion != other.majorVersion || minorVersion != other.minorVersion ||
						revisionVersion != other.revisionVersion;
			}
			
			
			/// Return whether or not this filter version number is earlier than another version number.
			Bool operator < ( const FilterVersion& other ) const;
			
			
			/// Return whether or not this filter version number is earlier than or equal to another version number.
			Bool operator <= ( const FilterVersion& other ) const;
			
			
			/// Return whether or not this filter version number is later than another version number.
			Bool operator > ( const FilterVersion& other ) const;
			
			
			/// Return whether or not this filter version number is later than or equal to another version number.
			Bool operator >= ( const FilterVersion& other ) const;
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this filter version to a human-readable string format.
			/**
			  * The returned string is of the form "M.m.r" where 'M' is the major version
			  * number, 'm' is the minor version number, and 'r' is the revision number.
			  */
			String toString() const;
			
			
			/// Convert this filter version to a human-readable string format.
			/**
			  * The returned string is of the form "M.m.r" where 'M' is the major version
			  * number, 'm' is the minor version number, and 'r' is the revision number.
			  */
			OM_INLINE operator String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An integer representing the major version number of a sound filter.
			UInt majorVersion;
			
			
			/// An integer representing the minor version number of a sound filter.
			UInt minorVersion;
			
			
			/// An integer representing the revision version number of a sound filter.
			UInt revisionVersion;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_VERSION_H
