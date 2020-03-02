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

#ifndef INCLUDE_OM_FRAMEWORK_H
#define INCLUDE_OM_FRAMEWORK_H


#include "omConfig.h"

#include "omUtilities.h"
#include "omLanguage.h"
#include "omData.h"
#include "omIO.h"
#include "omMath.h"
#include "omThreads.h"
#include "omTime.h"
#include "omSystem.h"


//##########################################################################################
//*********************************  Start Om Namespace  ***********************************
OM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//********************************************************************************
// Utilities:


using om::util::Allocator;
using om::util::AlignedAllocator;
using om::util::Array;
using om::util::StaticArray;
using om::util::ShortArray;
using om::util::PODArray;
using om::util::ArrayList;
using om::util::ShortArrayList;
using om::util::HashMap;
using om::util::PriorityQueue;


//********************************************************************************
// Language:


using om::lang::Function;
using om::lang::bind;
using om::lang::FunctionCall;
using om::lang::bindCall;
using om::lang::Shared;
using om::lang::Optional;
using om::lang::HalfFloat;
using om::lang::Float16;
using om::lang::UserData;


//********************************************************************************
// Math:


using om::math::Size2D;
using om::math::Size3D;
using om::math::ScalarType;


//********************************************************************************
// Data:



using om::data::String;
using om::data::UTF8String;
using om::data::UTF16String;
using om::data::UTF32String;


using om::data::StringIterator;
using om::data::UTF8StringIterator;
using om::data::UTF16StringIterator;
using om::data::UTF32StringIterator;

using om::data::StringBuffer;
using om::data::UTF8StringBuffer;
using om::data::UTF16StringBuffer;
using om::data::UTF32StringBuffer;

using om::data::Data;
using om::data::DataBuffer;
using om::data::DataStore;

using om::data::HashCode;

using om::data::Endianness;

//********************************************************************************
// File System:


using om::fs::Path;
using om::fs::File;
using om::fs::Directory;


//********************************************************************************
// Input and Output:


using om::io::DataInputStream;
using om::io::DataOutputStream;
using om::io::StringInputStream;
using om::io::StringOutputStream;

using om::io::FileReader;
using om::io::FileWriter;
using om::io::PrintStream;
using om::io::Log;
using om::io::BinaryEncoder;
using om::io::BinaryDecoder;


using om::io::Console;


//********************************************************************************
// Time:


using om::time::Time;
using om::time::Timer;


//********************************************************************************
// Threads:


using namespace om::threads;


//********************************************************************************
// System:


using namespace om::system;


//##########################################################################################
//*********************************  End Om Namespace  *************************************
OM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FRAMEWORK_H
