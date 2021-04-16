find_package(Qt5Core REQUIRED)
set(TARGET libtool)

include_directories(include)
set(libtool_include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL "libquex_include_dir")
set(INTERFACES
    include/fileTool.h
    )
    
set(HEADERS
	${INTERFACES}
    )

set(SOURCES
    fileTool.cpp
    )

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET}  
    Qt5::Core
)
