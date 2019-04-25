project(om-framework)

file(GLOB SOURCES om/*/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( om-framework STATIC ${SOURCEFILES})

