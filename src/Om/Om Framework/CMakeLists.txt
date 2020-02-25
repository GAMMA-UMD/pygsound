project(om-framework)

file(GLOB SOURCES om/*/*.cpp om/*/*.mm)
set( SOURCEFILES ${SOURCES} )

add_library( om-framework STATIC ${SOURCEFILES})

