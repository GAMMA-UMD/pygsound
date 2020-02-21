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

#ifndef INCLUDE_OM_TENSOR_H
#define INCLUDE_OM_TENSOR_H


#include "omMathConfig.h"


#include "omTensorType.h"
#include "omTensorMath.h"
#include "omMatrix.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a generalized mathematical value of arbitrary dimension and type.
/**
  * A tensor is a generalized matrix. This class supports tensors up to dimension 3.
  */
class Tensor
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default tensor with no value set.
			OM_INLINE Tensor()
				:	value( localBuffer ),
					capacityInBytes( LOCAL_BUFFER_SIZE ),
					type()
			{
			}
			
			
			/// Create a tensor with the specified type and an undefined value.
			Tensor( const TensorType& newType );
			
			
			/// Create a tensor with the specified type and a value from the specified opaque data source.
			Tensor( const TensorType& newType, const UByte* valueData );
			
			
			/// Create a tensor with the type and value of the specified parameter.
			/**
			  * If the new tensor value's type is undefined, the tensor is
			  * constructed with no value stored.
			  */
			template < typename T >
			OM_INLINE explicit Tensor( const T& newValue )
				:	type( TensorType::of<T>() )
			{
				// Statically verify the attribute type.
				TensorType::check<T>();
				const Size typeSize = type.getSizeInBytes();
				
				// Check to see if we should allocate a buffer for the value or use the local buffer.
				if ( typeSize > LOCAL_BUFFER_SIZE )
				{
					value = util::allocate<UByte>( typeSize );
					capacityInBytes = typeSize;
				}
				else
				{
					value = localBuffer;
					capacityInBytes = LOCAL_BUFFER_SIZE;
				}
				
				*((T*)value) = newValue;
			}
			
			
			/// Create a copy of the specified tensor and its value.
			Tensor( const Tensor& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a Tensor object and deallocate the internal storage.
			OM_INLINE ~Tensor()
			{
				if ( value != localBuffer )
					util::deallocate( value );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the value and type of another tensor to this tensor.
			Tensor& operator = ( const Tensor& other );
			
			
		//********************************************************************************
		//******	Type Accessor Methods
			
			
			/// Return the type of value that this tensor stores.
			OM_INLINE const TensorType& getType() const
			{
				return type;
			}
			
			
			/// Set the type of value that this tensor stores.
			/**
			  * The internal memory for the tensor is enlarged if necessary to
			  * contain the specified type. The contents of the
			  * value are undefined after this method is called.
			  */
			void setType( const TensorType& newType );
			
			
		//********************************************************************************
		//******	Value Accessor Methods
			
			
			/// Set this tensor to have the specified type and value from an opaque data source.
			/**
			  * If the value data is NULL, the type of the tensor is changed but it is uninitialized.
			  */
			void set( const TensorType& newType, const UByte* valueData );
			
			
			/// Set the value of this tensor to be a copy of the specified object.
			/**
			  * This method is for tensor types that have a size that is known at compile time.
			  */
			template < typename T >
			OM_INLINE void set( const T& newValue )
			{
				if ( sizeof(T) > capacityInBytes )
					reallocate( sizeof(T) );
				
				// Statically verify the input type.
				TensorType::check<T>();
				
				// Assign the value to the tensor.
				*((T*)value) = newValue;
				type = TensorType::of<T>();
			}
			
			
			/// Get the value of this tensor as the type of the template output parameter.
			/**
			  * This method is for tensor types that have a size that is known at compile time.
			  * The method returns whether or not the tensor's type was able to be converted to the
			  * template type.
			  */
			template < typename T >
			OM_INLINE Bool get( T& outputValue ) const
			{
				if ( value == NULL || TensorType::of<T>() != type )
					return false;
				
				// Statically verify the output type.
				TensorType::check<T>();
				
				// Assign the value to the output parameter.
				outputValue = *((T*)value);
				
				return true;
			}
			
			
			/// Set the entire contents of this tensor to zero.
			OM_INLINE Bool zero()
			{
				if ( value == NULL )
					return false;
				
				// Zero the internal buffer.
				util::zeroPOD( value, type.getSizeInBytes() );
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Component Accessor Methods
			
			
			/// Return the number of components that are in this attribute value.
			OM_INLINE Size getScalarCount() const
			{
				return type.getScalarCount();
			}
			
			
			/// Get the components of the AttributeValue in the output pointer.
			/**
			  * If the templated type of the output pointer matches the type of the 
			  * shader attribute's component type, TRUE is returned and the attribute is
			  * placed in the location specified by the output pointer. Otherwise FALSE is
			  * returned and no value is retrieved.
			  */
			template < typename T >
			OM_INLINE Bool getScalars( T* output ) const
			{
				// Statically check the output component type.
				ScalarType::check<T>();
				
				if ( value == NULL || ScalarType::of<T>() != type.getScalarType() )
					return false;
				
				util::copyPOD( output, (T*)value, type.getScalarCount() );
				
				return true;
			}
			
			
			/// Return a pointer to the components of this Shader attribute, stored in column-major order.
			/**
			  * If the templated type of the function matches the type of the 
			  * shader attribute's component type, the function is successful and
			  * the component pointer is returned. Otherwise, NULL is
			  * returned indicating failure.
			  */
			template < typename T >
			OM_INLINE T* getScalars()
			{
				// Statically check the output component type.
				ScalarType::check<T>();
				
				if ( ScalarType::of<T>() != type.getScalarType() )
					return NULL;
				
				return (T*)value;
			}
			
			
			/// Return a pointer to the components of this tensor.
			/**
			  * If the templated type of the function matches the type of the 
			  * tensor's scalar type, the function is successful and
			  * the component pointer is returned. Otherwise, NULL is
			  * returned indicating failure.
			  */
			template < typename T >
			OM_INLINE const T* getScalars() const
			{
				// Statically check the output component type.
				ScalarType::check<T>();
				
				if ( ScalarType::of<T>() != type.getScalarType() )
					return NULL;
				
				return (T*)value;
			}
			
			
			/// Return a raw pointer to the underlying data storage for this tensor's value.
			OM_INLINE UByte* getPointer()
			{
				return value;
			}
			
			
			/// Return a raw pointer to the underlying data storage for this tensor's value.
			OM_INLINE const UByte* getPointer() const
			{
				return value;
			}
			
			
		//********************************************************************************
		//******	Status Accessor Method
			
			
			/// Return whether or not the tensor's value is not set.
			OM_INLINE Bool isNull() const
			{
				return type.getScalarType() == ScalarType::UNDEFINED;
			}
			
			
			/// Return whether or not the tensor has a value set.
			OM_INLINE Bool isSet() const
			{
				return type.getScalarType() != ScalarType::UNDEFINED;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the tensor's value.
			data::String toString() const;
			
			
			/// Convert this tensor's value into a string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add another tensor to this one and return the result.
			OM_FORCE_INLINE Tensor operator + ( const Tensor& other ) const
			{
				Tensor result;
				add( result, *this, other );
				return result;
			}
			
			
			/// Add another tensor to this one and return the result.
			OM_FORCE_INLINE Tensor operator - ( const Tensor& other ) const
			{
				Tensor result;
				subtract( result, *this, other );
				return result;
			}
			
			
			/// Multiply this tensor and another one and return the result.
			OM_FORCE_INLINE Tensor operator * ( const Tensor& other ) const
			{
				Tensor result;
				multiply( result, *this, other );
				return result;
			}
			
			
			/// Divide this tensor by another one and return the result.
			OM_FORCE_INLINE Tensor operator / ( const Tensor& other ) const
			{
				Tensor result;
				divide( result, *this, other );
				return result;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignmet Operators
			
			
			/// Add another tensor to this one, modifying this tensor.
			OM_FORCE_INLINE Tensor& operator += ( const Tensor& other )
			{
				add( *this, other );
				return *this;
			}
			
			
			/// Subtract another tensor from this one, modifying this tensor.
			OM_FORCE_INLINE Tensor& operator -= ( const Tensor& other )
			{
				subtract( *this, other );
				return *this;
			}
			
			
			/// Multiply this tensor by another one, modifying this tensor.
			OM_FORCE_INLINE Tensor& operator *= ( const Tensor& other )
			{
				multiply( *this, other );
				return *this;
			}
			
			
			/// Divide this tensor by another one, modifying this tensor.
			OM_FORCE_INLINE Tensor& operator /= ( const Tensor& other )
			{
				divide( *this, other );
				return *this;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Functions
			
			
			/// Resize the internal storage for this tensor to the specified capacity in bytes.
			void reallocate( Size newCapacity );
			
			
			/// Convert a tensor with the specified scalar components and sizes to a string representation.
			template < typename T >
			OM_FORCE_INLINE static data::String convertToString( const T* scalars, Size size0, Size size1, Size size2 );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The size of the local storage buffer for a tensor.
			static const Size LOCAL_BUFFER_SIZE = 16;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the storage for this tensor's value.
			UByte* value;
			
			
			/// The capacity in bytes of this tensor's storage buffer.
			Size capacityInBytes;
			
			
			/// The type of the value stored by this tensor.
			TensorType type;
			
			
			/// A local storage buffer that reduces allocations for small tensors.
			UByte localBuffer[LOCAL_BUFFER_SIZE];
			
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TENSOR_H
