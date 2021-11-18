project( pygsound )

# May need this when building on RHEL
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include
		            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set( SOURCEFILES src/Context.cpp
		src/SoundMesh.cpp
		src/Scene.cpp
		src/SoundSource.cpp
		src/Listener.cpp
		src/MicrophoneArrays.cpp)

message("Building pygsound release library")
pybind11_add_module(pygsound SHARED ${SOURCEFILES} src/module.cpp)
target_link_libraries( pygsound PRIVATE gsound)


# Platform specific libraries
if (APPLE)
	find_library(CARBON_LIBRARY Carbon)
 	find_library(FOUNDATION_LIBRARY Foundation)
 	find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
 	find_library(CORE_SERVICES_LIBRARY CoreServices)

	target_link_libraries( pygsound PRIVATE ${CARBON_LIBRARY} ${FOUNDATION_LIBRARY}
				${CORE_FOUNDATION_LIBRARY} ${CORE_SERVICES_LIBRARY})
endif()