project(om-sound)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Framework
	${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Resources
	${CMAKE_CURRENT_SOURCE_DIR}/../platform
	${VORBIS_INCLUDE_DIRS}
	${OGG_INCLUDE_DIRS}
)


file(GLOB SOURCES om/*/*/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( om-sound STATIC ${SOURCEFILES})


set( EXTERNAL_LIBS
		ogg
		vorbis
		om-resources
		om-framework
		Threads::Threads )

target_link_libraries( om-sound ${EXTERNAL_LIBS})

