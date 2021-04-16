find_package(Qt5Core REQUIRED)
set(TARGET libquex)

include_directories(include)
set(libquex_include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL "libquex_include_dir")
set(INTERFACES
    include/quextool.h
    include/hFileInfo.h
    include/classInfo.h
    )
    
set(HEADERS
	${INTERFACES}
    quexNode.h
    QuexNodeFile/hFileQuexNode.h
    )

#.cpp .c文件列表
set(SOURCES
    quextool.cpp
    hFileInfo.cpp
    classInfo.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lexical_analysis/LexerForC.cpp
    cpp.qx
    )

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET}  
    Qt5::Core
)

add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/lexical_analysis/LexerForC.cpp
    COMMAND quex -i ${CMAKE_CURRENT_SOURCE_DIR}/cpp.qx -o LexerForC --odir ${CMAKE_CURRENT_BINARY_DIR}/lexical_analysis
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cpp.qx 
)
