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

#include "omType.h"


#if defined(OM_COMPILER_GCC)
	#include <cxxabi.h>
#elif defined(OM_COMPILER_MSVC)
	#include <Windows.h>
	#include <DbgHelp.h>
#else
	#error "Unsupported Compiler"
#endif


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




String Type:: unmangle( const char* mangledName )
{
#if defined(OM_COMPILER_GCC)
	
	int status;
	char* name = abi::__cxa_demangle( mangledName, NULL, 0, &status );
	
	String typeString( name );
	
	std::free( name );
	
	return typeString;
	
#elif defined(OM_COMPILER_MSVC)
	
	return String( mangledName );
	/*
	static Bool initialized = false;
	
	if ( !initialized )
	{
		SymSetOptions( SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS );
		
		if ( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) )
			return String( mangledName );
	}
	
	const Size bufferSize = 1024;
	char unmangledName[bufferSize];
	
	if ( !UnDecorateSymbolName( mangledName, unmangledName, bufferSize, UNDNAME_COMPLETE ) )
		return String( mangledName );
	
	return String( unmangledName );
	*/
#else
	
	return String( mangledName );
	
#endif
}




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

