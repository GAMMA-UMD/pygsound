project(om-resources)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Framework
)

file(GLOB SOURCES om/*/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( om-resources STATIC ${SOURCEFILES})

target_link_libraries( om-resources om-framework )

