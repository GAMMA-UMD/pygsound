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

#include "omSoundFilterVersion.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FilterVersion:: FilterVersion( const String& versionString )
{
	int result = std::sscanf( versionString.getCString(), "%u.%u.%u",
							&majorVersion, &minorVersion, &revisionVersion );
	
	if ( result != 3 )
		majorVersion = minorVersion = revisionVersion = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version Comparison Operators
//############		
//##########################################################################################
//##########################################################################################




Bool FilterVersion:: operator < ( const FilterVersion& other ) const
{
	if ( majorVersion < other.majorVersion )
		return true;
	else if ( majorVersion == other.majorVersion )
	{
		if ( minorVersion < other.minorVersion )
			return true;
		else if ( minorVersion == other.minorVersion )
			return revisionVersion < other.revisionVersion;
	}
	
	return false;
}




Bool FilterVersion:: operator <= ( const FilterVersion& other ) const
{
	if ( majorVersion < other.majorVersion )
		return true;
	else if ( majorVersion == other.majorVersion )
	{
		if ( minorVersion < other.minorVersion )
			return true;
		else if ( minorVersion == other.minorVersion )
			return revisionVersion <= other.revisionVersion;
	}
	
	return false;
}




Bool FilterVersion:: operator > ( const FilterVersion& other ) const
{
	if ( majorVersion > other.majorVersion )
		return true;
	else if ( majorVersion == other.majorVersion )
	{
		if ( minorVersion > other.minorVersion )
			return true;
		else if ( minorVersion == other.minorVersion )
			return revisionVersion > other.revisionVersion;
	}
	
	return false;
}




Bool FilterVersion:: operator >= ( const FilterVersion& other ) const
{
	if ( majorVersion > other.majorVersion )
		return true;
	else if ( majorVersion == other.majorVersion )
	{
		if ( minorVersion > other.minorVersion )
			return true;
		else if ( minorVersion == other.minorVersion )
			return revisionVersion >= other.revisionVersion;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version String Conversion Method
//############		
//##########################################################################################
//##########################################################################################




String FilterVersion:: toString() const
{
	return String(majorVersion) + "." + String(minorVersion) + "." + String(revisionVersion);
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
