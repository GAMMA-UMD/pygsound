project( pygsound )

# May need this when building on RHEL
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(Python3_FIND_VIRTUALENV "FIRST")
find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
message( "Python dir: " ${Python3_INCLUDE_DIRS})
message( "Python lib: " ${Python3_LIBRARY_DIRS})

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

# Detect build type and determine output target (pybind11 cannot build both lib and bin in one pass)
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_low)
string(FIND ${CMAKE_BUILD_TYPE_low} "debug" DEBUG_Found)
if (DEBUG_Found GREATER -1)
	message("Building pygsound debug binary")
	add_executable(test_ir_compute tests/test_ir_compute.cpp ${SOURCEFILES} src/module.cpp)
	target_link_libraries(test_ir_compute PRIVATE gsound pybind11::embed)
else()
	message("Building pygsound release library")
	pybind11_add_module(pygsound SHARED ${SOURCEFILES} src/module.cpp)
	target_link_libraries( pygsound PRIVATE gsound)
endif()





if (APPLE)
	find_library(CARBON_LIBRARY Carbon)
 	find_library(FOUNDATION_LIBRARY Foundation)
 	find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
 	find_library(CORE_SERVICES_LIBRARY CoreServices)
	target_link_libraries( pygsound PRIVATE ${CARBON_LIBRARY} ${FOUNDATION_LIBRARY}
  							${CORE_FOUNDATION_LIBRARY} ${CORE_SERVICES_LIBRARY})
endif()

