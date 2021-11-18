project( gsound )

file(GLOB_RECURSE SOURCES gsound/*.cpp)
set( SOURCEFILES ${SOURCES} )

add_library( gsound STATIC ${SOURCEFILES})

set( EXTERNAL_LIBS
		om-sound
		om-resources
		om-bvh
		om-framework
		fftw3_threads
		fftw3f
		fftw3
)

target_link_libraries( gsound ${EXTERNAL_LIBS} )


