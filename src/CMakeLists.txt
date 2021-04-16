find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC TRUE) # 设置QT自动查找需要moc的头文件
#目标名称
set(TARGET notepad)

#源文件列表
set(SOURCES
    main.cpp
    mainwindow.cpp
    )

#头文件列表
set(HEADERS
    mainwindow.h
    )

#Qt UI文件及预处理
qt5_wrap_ui(FORMS 
    mainwindow.ui
    )
	
#Qt资源文件及预处理
qt5_add_resources(RESOURCES )

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${TARGET}
    ${FORMS}
    ${RESOURCES}
    ${HEADERS}
    ${SOURCES})

include_directories(${libquex_include_dir})
include_directories(${libtool_include_dir})
target_link_libraries(${TARGET}  
    libquex
    libtool
    Qt5::Widgets
)