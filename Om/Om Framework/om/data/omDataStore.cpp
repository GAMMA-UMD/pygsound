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

#include "omDataStore.h"


#define OM_MAX( X, Y ) (X < Y ? Y : X)


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Entry Class Definition
//############		
//##########################################################################################
//##########################################################################################




class DataStore:: Entry
{
	public:
		
		//********************************************************************************
		//******	Type Enum Declaration
			
			
			/// An enum which describes the various allowed types of data store entries.
			enum EntryType
			{
				/// An entry which stores a boolean value.
				BOOLEAN,
				
				/// An entry which stores a signed 32-bit integer value.
				INT32,
				
				/// An entry which stores an unsigned 32-bit integer value.
				UINT32,
				
				/// An entry which stores a signed 64-bit integer value.
				INT64,
				
				/// An entry which stores an unsigned 64-bit integer value.
				UINT64,
				
				/// An entry which stores a 32-bit floating-point value.
				FLOAT32,
				
				/// An entry which stores a 64-bit floating-point value.
				FLOAT64,
				
				/// An entry which stores a UTF-8 encoded string of characters.
				UTF8_STRING,
				
				/// An entry which stores an array of bytes in the form of a Data object.
				DATA,
				
				/// An entry which store another DataStore object, to allow hierarchical structures.
				DATA_STORE
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a data store entry for the specified boolean value.
			OM_INLINE Entry( Bool value )
				:	type( BOOLEAN ),
					data( localStorage )
			{
				dataBool = value;
			}
			
			
			/// Create a data store entry for the specified 32-bit signed integer value.
			OM_INLINE Entry( Int32 value )
				:	type( INT32 ),
					data( localStorage )
			{
				dataInt32 = value;
			}
			
			
			/// Create a data store entry for the specified 32-bit unsigned integer value.
			OM_INLINE Entry( UInt32 value )
				:	type( UINT32 ),
					data( localStorage )
			{
				dataUInt32 = value;
			}
			
			
			/// Create a data store entry for the specified 64-bit signed integer value.
			OM_INLINE Entry( Int64 value )
				:	type( INT64 ),
					data( localStorage )
			{
				dataInt64 = value;
			}
			
			
			/// Create a data store entry for the specified 64-bit unsigned integer value.
			OM_INLINE Entry( UInt64 value )
				:	type( UINT64 ),
					data( localStorage )
			{
				dataUInt64 = value;
			}
			
			
			/// Create a data store entry for the specified 32-bit floating point value.
			OM_INLINE Entry( Float32 value )
				:	type( FLOAT32 ),
					data( localStorage )
			{
				dataFloat32 = value;
			}
			
			
			/// Create a data store entry for the specified 64-bit floating point value.
			OM_INLINE Entry( Float64 value )
				:	type( FLOAT64 ),
					data( localStorage )
			{
				dataFloat64 = value;
			}
			
			
			/// Create a data store entry for the specified UTF-8 encoded string value.
			OM_INLINE Entry( const UTF8String& value )
				:	type( UTF8_STRING ),
					data( localStorage )
			{
				// Copy the string into the local data store.
				new ((UTF8String*)data) UTF8String( value );
			}
			
			
			/// Create a data store entry for the specified bytes.
			OM_INLINE Entry( const UByte* bytes, Size numBytes )
				:	type( DATA ),
					data( localStorage )
			{
				// Copy the data into the local data store.
				new ((Data*)data) Data( bytes, numBytes );
			}
			
			
			/// Create a data store entry for the specified data store object.
			OM_INLINE Entry( const DataStore& dataStore )
				:	type( DATA ),
					data( localStorage )
			{
				// Copy the data into the local data store.
				new ((DataStore*)data) DataStore( dataStore );
			}
			
			
			/// Create a copy of the specified data store entry, deep-copying its stored value.
			OM_INLINE Entry( const Entry& other )
				:	type( other.type ),
					data( localStorage )
			{
				switch ( type )
				{
					case BOOLEAN:	dataBool = other.dataBool;			break;
					case INT32:		dataInt32 = other.dataInt32;		break;
					case UINT32:	dataUInt32 = other.dataUInt32;		break;
					case INT64:		dataInt64 = other.dataInt64;		break;
					case UINT64:	dataUInt64 = other.dataUInt64;		break;
					case FLOAT32:	dataFloat32 = other.dataFloat32;	break;
					case FLOAT64:	dataFloat64 = other.dataFloat64;	break;
					case UTF8_STRING:
						new ((UTF8String*)data) UTF8String( *(UTF8String*)other.data );
						break;
					case DATA:
						new ((Data*)data) Data( *(Data*)other.data );
						break;
					case DATA_STORE:
						new ((DataStore*)data) DataStore( *(DataStore*)other.data );
						break;
				}
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a data store entry, releasing any internal resources.
			OM_INLINE ~Entry()
			{
				switch ( type )
				{
					case UTF8_STRING:
						((UTF8String*)data)->~UTF8String();
						break;
					case DATA:
						((Data*)data)->~Data();
						break;
					case DATA_STORE:
						((DataStore*)data)->~DataStore();
						break;
					default:
						break;
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents of one data store entry to another, replacing any previous contents.
			OM_INLINE Entry& operator = ( const Entry& other )
			{
				if ( this != &other )
				{
					// Destroy the old stored value if necessary.
					switch ( type )
					{
						case UTF8_STRING:
							((UTF8String*)data)->~UTF8String();
							break;
						case DATA:
							((Data*)data)->~Data();
							break;
						case DATA_STORE:
							((DataStore*)data)->~DataStore();
							break;
						default:
							break;
					}
					
					type = other.type;
					data = localStorage;
					
					switch ( type )
					{
						case BOOLEAN:	dataBool = other.dataBool;			break;
						case INT32:		dataInt32 = other.dataInt32;		break;
						case UINT32:	dataUInt32 = other.dataUInt32;		break;
						case INT64:		dataInt64 = other.dataInt64;		break;
						case UINT64:	dataUInt64 = other.dataUInt64;		break;
						case FLOAT32:	dataFloat32 = other.dataFloat32;	break;
						case FLOAT64:	dataFloat64 = other.dataFloat64;	break;
						case UTF8_STRING:
							new ((UTF8String*)data) UTF8String( *(UTF8String*)other.data );
							break;
						case DATA:
							new ((Data*)data) Data( *(Data*)other.data );
							break;
						case DATA_STORE:
							new ((DataStore*)data) DataStore( *(DataStore*)other.data );
							break;
					}
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Return the type of the data stored in this entry.
			OM_INLINE EntryType getType() const
			{
				return type;
			}
			
			
			/// Return an opaque pointer to the data stored in this entry.
			OM_INLINE void* getData() const
			{
				return data;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The number of bytes of entry-local storage that an entry should have.
			static const Size MAX_LOCAL_STORAGE_SIZE = OM_MAX( sizeof(UTF8String), OM_MAX( sizeof(Data), sizeof(DataStore) ) );
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// An enum value that denotes the type of data stored by this entry.
			EntryType type;
			
			
			/// A pointer to the data element that is stored.
			void* data;
			
			
			/// An anonymous union used to guarantee alignment requirements are met for various standard types.
			union
			{
				UByte localStorage[MAX_LOCAL_STORAGE_SIZE];
				Int32 dataBool;
				Int32 dataInt32;
				UInt32 dataUInt32;
				Int64 dataInt64;
				UInt64 dataUInt64;
				Float32 dataFloat32;
				Float64 dataFloat64;
			};
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




DataStore:: DataStore()
	:	entries()
{
}




DataStore:: DataStore( const DataStore& other )
	:	entries( other.entries )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




DataStore:: ~DataStore()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




DataStore& DataStore:: operator = ( const DataStore& other )
{
	if ( this != &other )
	{
		entries = other.entries;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Entry Get Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool* DataStore:: get<Bool>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::BOOLEAN )
		return NULL;
	else
		return (Bool*)entry->getData();
}




template <>
const Bool* DataStore:: get<Bool>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::BOOLEAN )
		return NULL;
	else
		return (const Bool*)entry->getData();
}




template <>
Int32* DataStore:: get<Int32>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::INT32 )
		return NULL;
	else
		return (Int32*)entry->getData();
}




template <>
const Int32* DataStore:: get<Int32>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::INT32 )
		return NULL;
	else
		return (const Int32*)entry->getData();
}




template <>
UInt32* DataStore:: get<UInt32>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UINT32 )
		return NULL;
	else
		return (UInt32*)entry->getData();
}




template <>
const UInt32* DataStore:: get<UInt32>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UINT32 )
		return NULL;
	else
		return (const UInt32*)entry->getData();
}




template <>
Int64* DataStore:: get<Int64>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::INT64 )
		return NULL;
	else
		return (Int64*)entry->getData();
}




template <>
const Int64* DataStore:: get<Int64>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::INT64 )
		return NULL;
	else
		return (const Int64*)entry->getData();
}




template <>
UInt64* DataStore:: get<UInt64>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UINT64 )
		return NULL;
	else
		return (UInt64*)entry->getData();
}




template <>
const UInt64* DataStore:: get<UInt64>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UINT64 )
		return NULL;
	else
		return (const UInt64*)entry->getData();
}




template <>
Float32* DataStore:: get<Float32>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::FLOAT32 )
		return NULL;
	else
		return (Float32*)entry->getData();
}




template <>
const Float32* DataStore:: get<Float32>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::FLOAT32 )
		return NULL;
	else
		return (const Float32*)entry->getData();
}




template <>
Float64* DataStore:: get<Float64>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::FLOAT64 )
		return NULL;
	else
		return (Float64*)entry->getData();
}




template <>
const Float64* DataStore:: get<Float64>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::FLOAT64 )
		return NULL;
	else
		return (const Float64*)entry->getData();
}




template <>
UTF8String* DataStore:: get<UTF8String>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UTF8_STRING )
		return NULL;
	else
		return (UTF8String*)entry->getData();
}




template <>
const UTF8String* DataStore:: get<UTF8String>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::UTF8_STRING )
		return NULL;
	else
		return (const UTF8String*)entry->getData();
}




template <>
Data* DataStore:: get<Data>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::DATA )
		return NULL;
	else
		return (Data*)entry->getData();
}




template <>
const Data* DataStore:: get<Data>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::DATA )
		return NULL;
	else
		return (const Data*)entry->getData();
}




template <>
DataStore* DataStore:: get<DataStore>( const String& key )
{
	Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::DATA_STORE )
		return NULL;
	else
		return (DataStore*)entry->getData();
}




template <>
const DataStore* DataStore:: get<DataStore>( const String& key ) const
{
	const Entry* entry;
	
	if ( !entries.find( key.getHashCode(), key, entry ) )
		return NULL;
	else if ( entry->getType() != Entry::DATA_STORE )
		return NULL;
	else
		return (const DataStore*)entry->getData();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Entry Set Methods
//############		
//##########################################################################################
//##########################################################################################




Bool DataStore:: set( const String& key, Bool value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, Int32 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, UInt32 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, Int64 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, UInt64 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, Float32 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, Float64 value )
{
	entries.set( key.getHashCode(), key, Entry( value ) );
	
	return true;
}




Bool DataStore:: set( const String& key, const UTF8String& string )
{
	entries.set( key.getHashCode(), key, Entry( string ) );
	
	return true;
}




Bool DataStore:: set( const String& key, const Data& data )
{
	entries.set( key.getHashCode(), key, Entry( data.getPointer(), data.getSizeInBytes() ) );
	
	return true;
}




Bool DataStore:: set( const String& key, const UByte* bytes, Size numBytes )
{
	if ( bytes == NULL || numBytes == 0 )
		return false;
	
	entries.set( key.getHashCode(), key, Entry( bytes, numBytes ) );
	
	return true;
}




Bool DataStore:: set( const String& key, const DataStore& dataStore )
{
	entries.set( key.getHashCode(), key, Entry( dataStore ) );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Entry Remove Methods
//############		
//##########################################################################################
//##########################################################################################




Bool DataStore:: remove( const String& key )
{
	return entries.remove( key.getHashCode(), key );
}




void DataStore:: clear()
{
	entries.clear();
}




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
