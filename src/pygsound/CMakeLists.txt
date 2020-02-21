cmake_minimum_required(VERSION 3.13.1)
project( pygsound )

# May need this when building on RHEL
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
message( "Python dir: " ${Python3_INCLUDE_DIRS})
message( "Python lib: " ${Python3_LIBRARY_DIRS})

find_package(pybind11 CONFIG REQUIRED)
message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIR}")

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include
		            ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${Python3_INCLUDE_DIRS}
                    ${pybind11_INCLUDE_DIR}
)

# Add link directoris
link_directories( 
                ${Python3_LIBRARY_DIRS}
		    ${VORBIS_LIBRARY_DIRS}
		    ${OGG_LIBRARY_DIRS}
)

set( SOURCEFILES src/Context.cpp
		src/SoundMesh.cpp
		src/Scene.cpp
		src/SoundSource.cpp
		src/Listener.cpp
		src/MicrophoneArrays.cpp)

pybind11_add_module(pygsound SHARED ${SOURCEFILES} src/module.cpp)

target_link_libraries( pygsound PRIVATE gsound)

