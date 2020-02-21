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

#ifndef INCLUDE_OM_MEMBER_FUNCTION_H
#define INCLUDE_OM_MEMBER_FUNCTION_H


#include "../omLanguageConfig.h"


#include "omFunctionDefinition.h"


//##########################################################################################
//************************  Start Om Language Internal Namespace  **************************
OM_LANGUAGE_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that wraps a member function pointer and object pointer of runtime type.
/**
  * A MemberFunction object inherits from FunctionDefinition which defines a generic
  * interface for functions. This allows a MemberFunction to wrap objects of runtime
  * or virtual type and allow them to be handled as a standard non-member function.
  */
template <	typename ObjectType,
			typename SignatureType,
			typename R, 
			typename T1 = Void, 
			typename T2 = Void, 
			typename T3 = Void, 
			typename T4 = Void, 
			typename T5 = Void, 
			typename T6 = Void, 
			typename T7 = Void, 
			typename T8 = Void, 
			typename T9 = Void, 
			typename T10 = Void >
class MemberFunction : public FunctionDefinition< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of this member function's superclass.
			typedef FunctionDefinition< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 > DefinitionType;
			
			
			/// The type of this member function class, defined here for brevity elsewhere.
			typedef MemberFunction< ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 > ThisType;
			
			
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new member function object with the specified object and function pointer.
			OM_INLINE MemberFunction( SignatureType newFunctionPointer, ObjectType* objectPointer )
				:	object( objectPointer ),
					functionPointer( newFunctionPointer )
			{
			}
			
			
		//********************************************************************************
		//******	Function Call Operators
			
			
			/// Call a member function object with 0 parameters.
			OM_INLINE R operator () () const
			{
				return (object->*functionPointer)();
			}
			
			
			/// Call a member function object with 1 parameter.
			OM_INLINE R operator () ( T1 p1 ) const
			{
				return (object->*functionPointer)( p1 );
			}
			
			
			/// Call a member function object with 2 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2 ) const
			{
				return (object->*functionPointer)( p1, p2 );
			}
			
			
			/// Call a member function object with 3 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3 ) const
			{
				return (object->*functionPointer)( p1, p2, p3 );
			}
			
			
			/// Call a member function object with 4 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4 );
			}
			
			
			/// Call a member function object with 5 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5 );
			}
			
			
			/// Call a member function object with 6 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5, p6 );
			}
			
			
			/// Call a member function object with 7 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5, p6, p7 );
			}
			
			
			/// Call a member function object with 8 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5, p6, p7, p8 );
			}
			
			
			/// Call a member function object with 9 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
			}
			
			
			/// Call a member function object with 10 parameters.
			OM_INLINE R operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10 ) const
			{
				return (object->*functionPointer)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operator
			
			
			/// Return whether or not this member function is exactly equal to another member function.
			/**
			  * This method compares both the destination object pointer and the function
			  * pointer when testing for equality. If both are equal for both objects,
			  * the member functions are considered equal.
			  */
			virtual Bool equals( const DefinitionType& other ) const
			{
				const ThisType* memberFunction = (const ThisType*)&other;
				
				return functionPointer == memberFunction->functionPointer && object == memberFunction->object;
			}
			
			
		//********************************************************************************
		//******	Clone Method
			
			
			/// Create and return a deep copy of this member function object.
			/**
			  * This allows member functions to be copied constructed without knowing
			  * the type of the destination object or function signature.
			  */
			virtual DefinitionType* clone() const
			{
				return util::construct<ThisType>( functionPointer, object );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the object for which the member function pointer will be called.
			ObjectType* object;
			
			
			/// A pointer to the member function that this object is wrapping.
			SignatureType functionPointer;
			
			
};




//##########################################################################################
//************************  End Om Language Internal Namespace  ****************************
OM_LANGUAGE_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MEMBER_FUNCTION_H
