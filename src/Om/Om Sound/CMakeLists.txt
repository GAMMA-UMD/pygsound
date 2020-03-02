project(om-sound)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Framework
	${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Resources
	${CMAKE_CURRENT_SOURCE_DIR}/../platform
)


file(GLOB SOURCES om/*/*/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( om-sound STATIC ${SOURCEFILES})


set( EXTERNAL_LIBS
		om-resources
		om-framework
		Threads::Threads )

target_link_libraries( om-sound ${EXTERNAL_LIBS})

