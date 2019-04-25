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

#ifndef INCLUDE_OM_LOG_H
#define INCLUDE_OM_LOG_H


#include "omIOConfig.h"


#include "omStringOutputStream.h"
#include "omPrintStream.h"


#include "../lang/omShared.h"
#include "../util/omArray.h"
#include "../util/omStaticArray.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows the user to print messages to an abstract string output stream.
class Log : public StringOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a Log which prints its output to standard output.
			Log();
			
			
			/// Create a Log which sends its output to the specified StringOutputStream.
			/**
			  * If the stream is NULL, the log outputs to the standard output.
			  */
			Log( StringOutputStream* newStream );
			
			
		//********************************************************************************
		//******	Stream Accessor Methods
			
			
			/// Return a pointer to the StringOutputStream which this log is outputing to.
			/**
			  * If the stream is NULL, the log outputs to the standard output.
			  */
			OM_INLINE StringOutputStream* getStream() const
			{
				return stream;
			}
			
			
			/// Set the StringOutputStream which this log should output to.
			/**
			  * If the stream is NULL, the log outputs to the standard output.
			  */
			OM_INLINE void setStream( StringOutputStream* newStream )
			{
				stream = newStream;
			}
			
			
		//********************************************************************************
		//******	Single Character Buffer Output Operators
			
			
			OM_INLINE Log& operator < ( Char character )
			{
				if ( stream )
					stream->writeASCII( character );
				else
					fputc( character, stdout );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Null-Terminated Character Buffer Output Operators
			
			
			OM_INLINE Log& operator < ( const Char* characters )
			{
				if ( stream )
					stream->writeASCII( characters );
				else
					fwrite( characters, sizeof(Char), data::String::getLength(characters), stdout );
					
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Output Operators
			
			
			OM_INLINE Log& operator < ( const data::String& string )
			{
				this->writeASCII( string );
				
				return *this;
			}
			
			
			OM_INLINE Log& operator < ( const data::UTF8String& string )
			{
				this->writeUTF8( string );
				
				return *this;
			}
			
			
			OM_INLINE Log& operator < ( const data::UTF16String& string )
			{
				this->writeUTF8( data::UTF8String(string) );
				
				return *this;
			}
			
			
			OM_INLINE Log& operator < ( const data::UTF32String& string )
			{
				this->writeUTF8( data::UTF8String(string) );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Primitive Type Output Operators
			
			
			/// Write the specified boolean value to this output log.
			OM_INLINE Log& operator < ( Bool value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified short integer value to this output log.
			OM_INLINE Log& operator < ( Short value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified unsigned short integer value to this output log.
			OM_INLINE Log& operator < ( UShort value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified integer value to this output log.
			OM_INLINE Log& operator < ( Int value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified unsigned integer value to this output log.
			OM_INLINE Log& operator < ( UInt value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified long integer value to this output log.
			OM_INLINE Log& operator < ( Long value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified unsigned long integer value to this output log.
			OM_INLINE Log& operator < ( ULong value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified long long integer value to this output log.
			OM_INLINE Log& operator < ( LongLong value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified unsigned long long integer value to this output log.
			OM_INLINE Log& operator < ( ULongLong value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified floating point value to this output log.
			OM_INLINE Log& operator < ( Float value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
			/// Write the specified double floating point value to this output log.
			OM_INLINE Log& operator < ( Double value )
			{
				this->writeASCII( data::String(value) );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Pointer Type Output Operators
			
			
			/// Write the specified pointer to this output log.
			/**
			  * The basic implementation for this method writes either 'NULL'
			  * if the pointer is NULL, or converts the pointer to a hexadecimal
			  * integer representation and writes that.
			  */
			Log& operator < ( const void* pointer );
			
			
			/// Write the specified pointer to this output log.
			/**
			  * The basic implementation for this method writes either 'NULL'
			  * if the pointer is NULL, or converts the pointer to a hexadecimal
			  * integer representation and writes that.
			  */
			OM_INLINE Log& operator < ( void* pointer )
			{
				return *this < (const void*)pointer;
			}
			
			
			/// Write the specified pointer to this output log.
			/**
			  * The basic implementation for this method writes either 'NULL'
			  * if the pointer is NULL, or converts the pointer to a hexadecimal
			  * integer representation and writes that.
			  */
			template < typename T >
			OM_INLINE Log& operator < ( T* pointer )
			{
				return *this < (const void*)pointer;
			}
			
			
			/// Write the specified pointer to this output log.
			/**
			  * The basic implementation for this method writes either 'NULL'
			  * if the pointer is NULL, or converts the pointer to a hexadecimal
			  * integer representation and writes that.
			  */
			template < typename T >
			OM_INLINE Log& operator < ( const T* pointer )
			{
				return *this < (const void*)pointer;
			}
			
			
			/// Write the specified pointer to this output log.
			/**
			  * The basic implementation for this method writes either 'NULL'
			  * if the pointer is NULL, or converts the pointer to a hexadecimal
			  * integer representation and writes that.
			  */
			template < typename T >
			OM_INLINE Log& operator < ( const typename lang::Shared<T>& pointer )
			{
				return *this < (const void*)pointer.getPointer();
			}
			
			
		//********************************************************************************
		//******	Array Type Output Operators
			
			
			/// Write the specified array to this output log.
			/**
			  * The basic implementation for this method writes either '[NULL]'
			  * if the array is NULL, or writes an array representation of every element
			  * in the array.
			  */
			template < typename T >
			OM_NO_INLINE Log& operator < ( const typename util::Array<T>& array )
			{
				if ( array.isNull() )
					this->writeASCII( "[NULL]" );
				else
				{
					this->writeASCII( "[ " );
					
					Index lastIndex = array.getSize() - 1;
					
					for ( Index i = 0; i < array.getSize(); i++ )
					{
						*this < array[i];
						
						if ( i != lastIndex )
							this->writeASCII( ", " );
					}
					
					this->writeASCII( " ]" );
				}
				
				return *this;
			}
			
			
			/// Write the specified static array to this output log.
			/**
			  * The basic implementation for this method writes either '[NULL]'
			  * if the array is NULL, or writes an array representation of every element
			  * in the array.
			  */
			template < typename T, Size size >
			OM_NO_INLINE Log& operator < ( const typename util::StaticArray<T,size>& array )
			{
				this->writeASCII( "[ " );
				
				Index lastIndex = size - 1;
				
				for ( Index i = 0; i < size; i++ )
				{
					*this < array[i];
					
					if ( i != lastIndex )
						this->writeASCII( ", " );
				}
				
				this->writeASCII( " ]" );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Other Type Output Operators
			
			
			/// Convert the specified templated object to a string and write it to the output log.
			/**
			  * This method uses the specified object's string cast operator to convert it
			  * to a string.
			  */
			template < typename T >
			OM_INLINE Log& operator < ( const T& object )
			{
				this->writeASCII( object.operator data::String() );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	New Line Output Operator
			
			
			/// Write a new line character and then log the specified object, using one of the standard '<' operators.
			template < typename T >
			OM_INLINE Log& operator << ( const T& object )
			{
				*this < '\n' < object;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush the output stream, sending all internally buffered output to it's destination.
			virtual void flush();
			
			
	private:
		
		//********************************************************************************
		//******	String Write Methods
			
			
			/// Write the specified number of characters from the character buffer and return the number written.
			virtual Size writeChars( const Char* characters, Size number );
			
			
			/// Write the specified number of UTF-8 characters from the character buffer and return the number written.
			virtual Size writeUTF8Chars( const UTF8Char* characters, Size number );
			
			
			/// Write the specified number of UTF-16 characters from the character buffer and return the number written.
			virtual Size writeUTF16Chars( const UTF16Char* characters, Size number );
			
			
			/// Write the specified number of UTF-32 characters from the character buffer and return the number written.
			virtual Size writeUTF32Chars( const UTF32Char* characters, Size number );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the StringOutputStream object that is being logged to.
			StringOutputStream* stream;
			
			
			
};




/// The standard output log object, connected to standard output.
extern Log Console;




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_LOG_H
