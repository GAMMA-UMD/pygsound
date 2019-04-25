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

#ifndef INCLUDE_OM_FUNCTION_H
#define INCLUDE_OM_FUNCTION_H


#include "omLanguageConfig.h"


#include "internal/omFunctionDefinition.h"
#include "internal/omMemberFunction.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Prototype for the function object template class.
template < typename Signature >
class Function;




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 0 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 0 parameters.
template < typename R >
class Function< R () >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)() )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)(),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)() const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () () const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)();
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer();
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)();
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R >
OM_INLINE Function<R ()> bind( R (*functionPointer)() )
{
	return Function<R ()>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R >
OM_INLINE Function<R ()> bind( R (ObjectType2::*functionPointer)(),
								ObjectType* objectPointer )
{
	return Function<R ()>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R >
OM_INLINE Function<R ()> bind( R (ObjectType2::*functionPointer)() const,
								const ObjectType* objectPointer )
{
	return Function<R ()>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 1 Parameter Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 1 parameter.
template < typename R, typename T1 >
class Function< R ( T1 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1 >
OM_INLINE Function<R ( T1 )> bind( R (*functionPointer)( T1 ) )
{
	return Function<R ( T1 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1 >
OM_INLINE Function<R ( T1 )> bind( R (ObjectType2::*functionPointer)( T1 ),
								ObjectType* objectPointer )
{
	return Function<R ( T1 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1 >
OM_INLINE Function<R ( T1 )> bind( R (ObjectType2::*functionPointer)( T1 ) const,
								const ObjectType* objectPointer )
{
	return Function<R ( T1 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 2 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 2 parameters.
template < typename R, typename T1, typename T2 >
class Function< R ( T1, T2 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2 >
OM_INLINE Function<R ( T1, T2 )> bind( R (*functionPointer)( T1, T2 ) )
{
	return Function<R ( T1, T2 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2 >
OM_INLINE Function<R ( T1, T2 )> bind( R (ObjectType2::*functionPointer)( T1, T2 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2 >
OM_INLINE Function<R ( T1, T2 )> bind( R (ObjectType2::*functionPointer)( T1, T2 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 3 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 3 parameters.
template < typename R, typename T1, typename T2, typename T3 >
class Function< R ( T1, T2, T3 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3 >
OM_INLINE Function<R ( T1, T2, T3 )> bind( R (*functionPointer)( T1, T2, T3 ) )
{
	return Function<R ( T1, T2, T3 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3 >
OM_INLINE Function<R ( T1, T2, T3 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3 >
OM_INLINE Function<R ( T1, T2, T3 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 4 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 4 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4 >
class Function< R ( T1, T2, T3, T4 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4 >
OM_INLINE Function<R ( T1, T2, T3, T4 )> bind( R (*functionPointer)( T1, T2, T3, T4 ) )
{
	return Function<R ( T1, T2, T3, T4 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4 >
OM_INLINE Function<R ( T1, T2, T3, T4 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4 >
OM_INLINE Function<R ( T1, T2, T3, T4 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 5 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 5 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
class Function< R ( T1, T2, T3, T4, T5 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5 ) )
{
	return Function<R ( T1, T2, T3, T4, T5 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 6 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 6 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
class Function< R ( T1, T2, T3, T4, T5, T6 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5,T6> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
			/// The type of the sixth parameter of this function object.
			typedef T6 ParameterType6;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5, T6 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5, p6 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5, p6 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5, T6 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5, T6 ) )
{
	return Function<R ( T1, T2, T3, T4, T5, T6 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 7 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 7 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
class Function< R ( T1, T2, T3, T4, T5, T6, T7 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
			/// The type of the sixth parameter of this function object.
			typedef T6 ParameterType6;
			
			
			/// The type of the seventh parameter of this function object.
			typedef T7 ParameterType7;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5, p6, p7 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5, p6, p7 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6, T7>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5, T6, T7 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ) )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 8 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 8 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
class Function< R ( T1, T2, T3, T4, T5, T6, T7, T8 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
			/// The type of the sixth parameter of this function object.
			typedef T6 ParameterType6;
			
			
			/// The type of the seventh parameter of this function object.
			typedef T7 ParameterType7;
			
			
			/// The type of the seventh parameter of this function object.
			typedef T8 ParameterType8;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5, p6, p7, p8 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5, p6, p7, p8 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6, T7, T8>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ) )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 9 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 8 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
class Function< R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
			/// The type of the sixth parameter of this function object.
			typedef T6 ParameterType6;
			
			
			/// The type of the seventh parameter of this function object.
			typedef T7 ParameterType7;
			
			
			/// The type of the eighth parameter of this function object.
			typedef T8 ParameterType8;
			
			
			/// The type of the nineth parameter of this function object.
			typedef T9 ParameterType9;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6, T7, T8, T9>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5,
						typename T6, typename T7, typename T8, typename T9 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8, typename T9 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8, typename T9 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( functionPointer, objectPointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Object With 10 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function template class for a method with 10 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
class Function< R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) >
{
	private:
		
		//********************************************************************************
		//******	Private Type Definitions
			
			
			/// The type of an internal function definition.
			typedef internal::FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> DefinitionType;
			
			
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The return type of this function object.
			typedef R ReturnType;
			
			
			/// The type of the first parameter of this function object.
			typedef T1 ParameterType1;
			
			
			/// The type of the second parameter of this function object.
			typedef T2 ParameterType2;
			
			
			/// The type of the third parameter of this function object.
			typedef T3 ParameterType3;
			
			
			/// The type of the fourth parameter of this function object.
			typedef T4 ParameterType4;
			
			
			/// The type of the fifth parameter of this function object.
			typedef T5 ParameterType5;
			
			
			/// The type of the sixth parameter of this function object.
			typedef T6 ParameterType6;
			
			
			/// The type of the seventh parameter of this function object.
			typedef T7 ParameterType7;
			
			
			/// The type of the eighth parameter of this function object.
			typedef T8 ParameterType8;
			
			
			/// The type of the nineth parameter of this function object.
			typedef T9 ParameterType9;
			
			
			/// The type of the tenth parameter of this function object.
			typedef T10 ParameterType10;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default function object with a NULL function pointer.
			OM_INLINE Function()
				:	object( NULL ),
					staticFunctionPointer( NULL )
			{
			}
			
			
			/// Create a new function object that wraps the specified non-member function pointer.
			OM_INLINE Function( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) )
				:	object( NULL ),
					staticFunctionPointer( functionPointer )
			{
			}
			
			
			/// Create a new function object that wraps the specified object and member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
								ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
			/// Create a new function object that wraps the specified object and const member function pointer.
			template < typename ObjectType, typename ObjectType2 >
			OM_INLINE Function( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) const,
								const ObjectType* objectPointer )
			{
				constructMemberFunction( functionPointer, objectPointer );
			}
			
			
		//********************************************************************************
		//******	Function Call Method
			
			
			/// Call this function and return its return value.
			/**
			  * Calling a NULL function object will cause an assertion to be raised.
			  */
			OM_INLINE ReturnType operator () ( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10 ) const
			{
				if ( object != NULL )
					return (*(const DefinitionType*)memberFunctionStorage)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
				else
				{
					OM_DEBUG_ASSERT_MESSAGE( staticFunctionPointer != NULL, "Cannot call NULL function object." );
					return staticFunctionPointer( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
				}
			}
			
			
		//********************************************************************************
		//******	Function Equality Comparison Operators
			
			
			/// Return whether or not this function references the same function as another function object.
			OM_INLINE Bool operator == ( const Function& other ) const
			{
				if ( object != other.object )
					return false;
				else if ( object == NULL )
					return staticFunctionPointer == other.staticFunctionPointer;
				else
					return ((const DefinitionType*)memberFunctionStorage)->equals( *(const DefinitionType*)other.memberFunctionStorage );
			}
			
			
			/// Return whether or not this function references a different function as onother function object.
			OM_INLINE Bool operator != ( const Function& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Function Status Methods
			
			
			/// Return whether or not this function object has a function/object pointer that is NULL.
			/**
			  * If this method returns TRUE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isNull() const
			{
				return object == NULL && staticFunctionPointer == NULL;
			}
			
			
			/// Return whether or not this function object has a function/object pointer that is not NULL.
			/**
			  * If this method returns FALSE, calling the function will result in
			  * an assertion being raised.
			  */
			OM_INLINE Bool isSet() const
			{
				return object != NULL || staticFunctionPointer != NULL;
			}
			
			
			/// Return whether or not this function object refers to a member function of a class object.
			OM_INLINE Bool isMember() const
			{
				return object != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename SignatureType, typename ObjectType >
			OM_INLINE void constructMemberFunction( SignatureType functionPointer, ObjectType* newObject )
			{
				if ( newObject == NULL || functionPointer == NULL )
				{
					staticFunctionPointer = NULL;
					object = NULL;
				}
				else
				{
					new (memberFunctionStorage) internal::MemberFunction<
									ObjectType, SignatureType, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>( functionPointer, newObject );
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The storage space required for a member function object.
			static const Size MEMBER_FUNCTION_STORAGE_SIZE = sizeof(&DefinitionType::equals) + 2*sizeof(void*);
			
			
			/// A union which saves space when storing the static and member function pointers.
			union
			{
				struct
				{
					/// A pointer to the object that receives the member function call.
					void* object;
					
					/// A non-member function pointer to use when the function object represents a non-member function.
					R (*staticFunctionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 );
				};
				
				/// A local buffer of bytes that stores an opaque member function.
				UByte memberFunctionStorage[MEMBER_FUNCTION_STORAGE_SIZE];
			};
			
			
			
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bind( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( functionPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
															ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( functionPointer, objectPointer );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bind( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) const,
															const ObjectType* objectPointer )
{
	return Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( functionPointer, objectPointer );
}




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FUNCTION_H
