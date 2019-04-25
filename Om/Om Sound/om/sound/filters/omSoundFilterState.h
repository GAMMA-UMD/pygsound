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

#ifndef INCLUDE_OM_SOUND_FILTER_STATE_H
#define INCLUDE_OM_SOUND_FILTER_STATE_H


#include "omSoundFiltersConfig.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the entire serialized state of a SoundFilter instance.
/**
  * This class is a thin wrapper of a om::data::DataStore object and is used
  * as a dictionary to store common types of filter data (numbers, strings, bytes,
  * and other DataStore objects).
  */
class FilterState
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound filter state object with no data stored in it.
			OM_INLINE FilterState()
			{
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Return the total number of key-value pairs that are stored in this filter state.
			OM_INLINE Size getSize() const
			{
				return state.getSize();
			}
			
			
			/// Get a pointer to the value stored for the specified key string.
			/**
			  * The types of allowed values are: Bool, Int32, UInt32, Int64, UInt64, Float32, Float64,
			  * UTF8String, Data, and DataStore. This method is a thin wrapper around the
			  * DataStore::get() family of methods. See that class's documentation for more info.
			  * The method returns NULL if there is no value for the given key, or if
			  * the stored value has an incompatible type with the requested template type.
			  */
			template < typename V, typename K >
			OM_INLINE V* get( const K& key )
			{
				return state.get<V>( String(key) );
			}
			
			
			/// Get a const pointer to the value stored for the specified key string.
			/**
			  * The types of allowed values are: Int32, UInt32, Int64, UInt64, Float32, Float64,
			  * UTF8String, Data, and DataStore. This method is a thin wrapper around the
			  * DataStore::get() family of methods. See that class's documentation for more info.
			  * The method returns NULL if there is no value for the given key, or if
			  * the stored value has an incompatible type with the requested template type.
			  */
			template < typename V, typename K >
			OM_INLINE const V* get( const K& key ) const
			{
				return state.get<V>( String(key) );
			}
			
			
			/// Set the filter state to have a mapping from the specified key string to a template data value.
			/**
			  * The types of allowed values are: Int32, UInt32, Int64, UInt64, Float32, Float64,
			  * UTF8String, Data, and DataStore. This method is a thin wrapper around the
			  * DataStore::set() family of methods. See that class's documentation for more info.
			  * The method returns whether or not the operation was successful.
			  */
			template < typename K, typename V >
			OM_INLINE Bool set( const K& key, const V& value )
			{
				return state.set( String(key), value );
			}
			
			
			/// Remove the entry with the specified string key and return whether or not anything was removed.
			template < typename K >
			OM_INLINE Bool remove( const K& key )
			{
				return state.remove( String(key) );
			}
			
			
			/// Clear all stored data from this filter state, resulting in an empty state object.
			OM_INLINE void clear()
			{
				state.clear();
			}
			
			
		//********************************************************************************
		//******	Data Store Accessor Methods
			
			
			/// Return a reference to the DataStore object that contains this state's data entries.
			OM_INLINE const DataStore& getDataStore() const
			{
				return state;
			}
			
			
			/// Replace this filter state's internal data store with the specified data store, copying all entries.
			OM_INLINE void setDataStore( const DataStore& dataStore )
			{
				state = dataStore;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A DataStore object that stores the entire state of a SoundFilter instance.
			DataStore state;
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_STATE_H
