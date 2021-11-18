project(om-framework)

if(APPLE)
	file(GLOB SOURCES om/*/*.cpp om/*/*.mm)
else()
	file(GLOB SOURCES om/*/*.cpp)
endif()
set( SOURCEFILES ${SOURCES} )

add_library( om-framework STATIC ${SOURCEFILES})

