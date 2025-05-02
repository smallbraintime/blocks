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
    src/app/voxel.h
    src/app/voxel.cpp
    src/app/style.h
    resources.qrc
    shaders/basic.frag
    shaders/basic.vert
)

add_library(core STATIC
    src/core/camera.h
    src/core/renderer.h
    src/core/cameracontroller.h
    src/core/scene.h
    src/core/camera.cpp
    src/core/renderer.cpp
    src/core/cameracontroller.cpp
    src/core/scene.cpp
    src/core/entity.h
    src/core/entity.cpp
    src/core/graphicscomponent.h
    src/core/graphicscomponents.h
    src/core/graphicscomponents.cpp
    src/core/gameenginewidget.h
    src/core/gameenginewidget.cpp
)

target_link_libraries(core PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/src/core
)

file(READ "${CMAKE_SOURCE_DIR}/styles/menu.qss" QSS_CONTENT)
string(REPLACE "\n" " " QSS_CONTENT "${QSS_CONTENT}")
string(REPLACE "\"" "\\\"" QSS_CONTENT "${QSS_CONTENT}")
set(STYLESHEET "${QSS_CONTENT}")
configure_file(${CMAKE_SOURCE_DIR}/styles/style_template.h.in ${CMAKE_SOURCE_DIR}/src/style.h @ONLY)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(blocks
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(blocks PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
)

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
