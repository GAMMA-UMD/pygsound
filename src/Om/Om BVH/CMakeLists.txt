project(om-bvh)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Om\ Framework
)

file(GLOB SOURCES om/*/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( om-bvh STATIC ${SOURCEFILES})

target_link_libraries( om-bvh om-framework )

