cmake_minimum_required(VERSION 3.13.1)
project( pysound )

# Need this when building on RHEL
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
message( "Python dir: " ${Python3_INCLUDE_DIRS})
message( "Python lib: " ${Python3_LIBRARY_DIRS})

find_package(pybind11 CONFIG REQUIRED)
message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIR}")

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include
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
		src/MicrophoneArrays.cpp
		src/ARDUtils.cpp
		src/Metrics.cpp)

add_executable( gsound-ir src/main.cpp )

set( CMAKE_SHARED_LIBRARY_PREFIX "" )
add_library( pysound SHARED ${SOURCEFILES}
		src/module.cpp )

target_link_libraries( pysound gsound)
target_link_libraries( gsound-ir pysound
				${Python3_LIBRARIES}
)

set_target_properties( gsound-ir PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}/bin)
set_target_properties( pysound PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}/lib)
