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

#include "omMemory.h"


#if defined(OM_PLATFORM_APPLE)
	#include <sys/sysctl.h>
	#include <mach/vm_statistics.h>
	#include <mach/mach_types.h>
	#include <mach/mach_init.h>
	#include <mach/mach_host.h>
	#include <sys/param.h>
	#include <sys/mount.h>
#elif defined(OM_PLATFORM_ANDROID)
	#include <sys/types.h>
    #include <sys/sysinfo.h>
	#include <unistd.h>
#elif defined(OM_PLATFORM_LINUX)
	#include <sys/types.h>
    #include <sys/sysinfo.h>
	#include <unistd.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
#else
	#error Unsupported platform
#endif


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		RAM Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeSize Memory:: getRAMSize()
{
#if defined(OM_PLATFORM_APPLE)
	int mib[2];
	int64_t physicalMemory;
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	size_t length = sizeof(int64_t);
	sysctl( mib, 2, &physicalMemory, &length, NULL, 0 );
	
	return (LargeSize)physicalMemory;
	
#elif defined(OM_PLATFORM_LINUX)
	struct sysinfo memoryInfo;
	
	if ( sysinfo( &memoryInfo ) != 0 )
		return LargeSize(0);
	
	return LargeSize(memoryInfo.totalram)*LargeSize(memoryInfo.mem_unit);
	
#elif defined(OM_PLATFORM_WINDOWS)
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(MEMORYSTATUSEX);
	
	// If the function fails, return a size of 0.
	if ( GlobalMemoryStatusEx( &status ) == 0 )
		return Size(0);
	
	return (LargeSize)status.ullTotalPhys;
#endif
}




LargeSize Memory:: getRAMAvailable()
{
#if defined(OM_PLATFORM_APPLE)
	vm_size_t page_size;
	vm_statistics64_data_t vm_stats;
	mach_port_t mach_port = mach_host_self();
	mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);
	
	// If getting memory attributes fails, return a size of 0.
	if ( host_page_size( mach_port, &page_size ) != KERN_SUCCESS ||
		host_statistics64( mach_port, HOST_VM_INFO, (host_info64_t)&vm_stats, &count ) != KERN_SUCCESS )
		return LargeSize(0);
	
	return LargeSize(vm_stats.free_count) * LargeSize(page_size);
	
#elif defined(OM_PLATFORM_LINUX)
	struct sysinfo memoryInfo;
	
	if ( sysinfo( &memoryInfo ) != 0 )
		return LargeSize(0);
	
	return LargeSize(memoryInfo.freeram)*LargeSize(memoryInfo.mem_unit);
	
#elif defined(OM_PLATFORM_WINDOWS)
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(MEMORYSTATUSEX);
	
	// If the function fails, return a size of 0.
	if ( GlobalMemoryStatusEx( &status ) == 0 )
		return Size(0);
	
	return (LargeSize)status.ullAvailPhys;
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Virtual Memory Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeSize Memory:: getVMSize()
{
#if defined(OM_PLATFORM_APPLE)
	struct statfs memoryInfo;
	
	// Get the space available for swap at the root directory.
	if ( statfs("/", &memoryInfo) != 0 )
		return LargeSize(0);
	
	return LargeSize(memoryInfo.f_bsize)*LargeSize(memoryInfo.f_bfree);
	
#elif defined(OM_PLATFORM_LINUX)
	struct sysinfo memoryInfo;
	
	if ( sysinfo( &memoryInfo ) != 0 )
		return LargeSize(0);
	
	return (LargeSize(memoryInfo.totalram) + LargeSize(memoryInfo.totalswap))*LargeSize(memoryInfo.mem_unit);
	
#elif defined(OM_PLATFORM_WINDOWS)
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(MEMORYSTATUSEX);
	
	// If the function fails, return a size of 0.
	if ( GlobalMemoryStatusEx( &status ) == 0 )
		return Size(0);
	
	return (LargeSize)status.ullTotalPageFile;
#endif
}




LargeSize Memory:: getVMAvailable()
{
#if defined(OM_PLATFORM_APPLE)
	xsw_usage memoryInfo = {0};
	size_t size = sizeof(xsw_usage);
	
	if ( sysctlbyname("vm.swapusage", &memoryInfo, &size, NULL, 0) != 0 )
		return LargeSize(0);
	
	return LargeSize(memoryInfo.xsu_avail)*LargeSize(memoryInfo.xsu_pagesize);
	
#elif defined(OM_PLATFORM_LINUX)
	struct sysinfo memoryInfo;
	
	if ( sysinfo( &memoryInfo ) != 0 )
		return LargeSize(0);
	
	return (LargeSize(memoryInfo.freeram) + LargeSize(memoryInfo.freeswap))*LargeSize(memoryInfo.mem_unit);
	
#elif defined(OM_PLATFORM_WINDOWS)
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(MEMORYSTATUSEX);
	
	// If the function fails, return a size of 0.
	if ( GlobalMemoryStatusEx( &status ) == 0 )
		return Size(0);
	
	return (LargeSize)status.ullAvailPageFile;
#endif
}




Size Memory:: getVMPageSize()
{
#if defined(OM_PLATFORM_ANDROID)
	return (Size)getpagesize();
#elif defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	return (Size)sysconf(_SC_PAGE_SIZE);
#elif defined(OM_PLATFORM_WINDOWS)
	// Get information about the system.
	SYSTEM_INFO systemInfo;
	GetSystemInfo( &systemInfo );
	
	return (Size)systemInfo.dwPageSize;
#endif
}




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
