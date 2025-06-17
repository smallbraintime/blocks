cmake_minimum_required(VERSION 3.16)

project(blocks VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)

qt_add_resources(PROJECT_RESOURCES resources.qrc)

set(PROJECT_SOURCES
    src/main.cpp
    src/app.h
    src/app.cpp
    src/menu.h
    src/menu.cpp
    src/editor.h
    src/editor.cpp
    src/serializer.h
    src/serializer.cpp
    src/cameracontroller.h
    src/cameracontroller.cpp
    src/camera.h
    src/camera.cpp
    src/renderer.h
    src/renderpass.h
    src/renderpass.cpp
    src/renderer.cpp
    src/inputmanager.h
    src/inputmanager.cpp

    # resources.qrc
    # resources/shaders/base.frag
    # resources/shaders/base.vert
    # resources/shaders/fxaa.frag
    # resources/shaders/fxaa.vert
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(blocks
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
    )
endif()

target_link_libraries(blocks PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.blocks)
endif()
set_target_properties(blocks PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS blocks
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(blocks)
endif()
