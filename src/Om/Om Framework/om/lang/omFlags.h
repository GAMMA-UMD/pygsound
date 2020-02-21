/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com
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

#ifndef INCLUDE_OM_FLAGS_H
#define INCLUDE_OM_FLAGS_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




#define OM_DECLARE_FLAGS_CLASS( ClassName, CommentClassName, BaseType, ... ) \
/** These flags provide boolean information. Flags are indicated by setting a single bit of an unsigned integer to 1.
Enum values for the different flags are defined as members of the class.
Typically, the user would bitwise-OR the flag enum values together to produce a final set of flags. */\
class ClassName\
{\
	public:\
		\
		/** \brief An enum that specifies the different CommentClassName. */\
		enum Flag { __VA_ARGS__ };\
		\
		\
		/** \brief Create a new CommentClassName object with no flags set. */\
		OM_INLINE ClassName()\
			:	flags( BaseType(0) )\
		{\
		}\
		\
		\
		/** \brief Create a new CommentClassName object with the specified flag value initially set. */\
		OM_INLINE ClassName( Flag flag )\
			:	flags( flag )\
		{\
		}\
		\
		\
		/** \brief Create a new CommentClassName object with the specified initial combined flags value. */\
		OM_INLINE ClassName( BaseType newFlags )\
			:	flags( newFlags )\
		{\
		}\
		\
		\
		/** \brief Convert this CommentClassName object to an integer value. */\
		/** This operator is provided so that the ClassName object can be used as an integer value for bitwise logical operations. */\
		OM_INLINE operator BaseType () const\
		{\
			return flags;\
		}\
		\
		\
		/** \brief Return whether or not the specified flag value is set for this flags object. */\
		OM_INLINE Bool isSet( Flag flag ) const\
		{\
			return (flags & flag) == BaseType(flag);\
		}\
		\
		\
		/** \brief Return whether or not the any of the specified flag's bits are set for this flags object. */\
		OM_INLINE Bool isSetAny( Flag flag ) const\
		{\
			return (flags & flag) != BaseType(0);\
		}\
		\
		\
		/** \brief Set whether or not the specified flag value is set for this flags object. */\
		OM_INLINE void set( Flag flag, Bool newIsSet )\
		{\
			if ( newIsSet )\
				flags |= flag;\
			else\
				flags &= ~flag;\
		}\
		\
		\
	private:\
		\
		\
		/** \brief An integer value indicating the flags that are set. */\
		BaseType flags;\
		\
		\
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FLAGS_H
