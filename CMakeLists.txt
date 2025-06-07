cmake_minimum_required(VERSION 3.16)

project(blocks VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)

set(PROJECT_SOURCES
    src/app/main.cpp
    src/app/app.h
    src/app/app.cpp
    src/app/menu.h
    src/app/menu.cpp
    src/app/editor.h
    src/app/editor.cpp
    src/app/serializer.h
    src/app/serializer.cpp
    src/app/cameracontroller.h
    src/app/cameracontroller.cpp
    src/app/camera.h
    src/app/camera.cpp
    src/app/renderer.h
    src/app/renderpass.h
    src/app/renderpass.cpp
    src/app/renderer.cpp
    src/app/scene.h

    resources/shaders/zpass.vert
    resources/shaders/zpass.frag
    resources/shaders/base.frag
    resources/shaders/base.vert
    resources/shaders/lighting.frag
    resources/shaders/lighting.vert
    resources/shaders/postprocess.frag
    resources/shaders/postprocess.vert
)

qt_add_resources(PROJECT_SOURCES
    PREFIX "/resources"
    FILES
        shaders/base.frag
        shaders/base.vert
        shaders/lighting.frag
        shaders/lighting.vert
        shaders/postprocess.frag
        shaders/postprocess.vert
        shaders/zpass.frag
        shaders/zpass.vert
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(blocks
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(blocks PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    core
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
