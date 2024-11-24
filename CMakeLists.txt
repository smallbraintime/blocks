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
    src/main.cpp
    src/app.h
    src/app.cpp
    src/menu.h
    src/menu.cpp
    src/game.h
    src/game.cpp
    src/style.h
)

file(READ "${CMAKE_SOURCE_DIR}/src/styles/menu.qss" QSS_CONTENT)
string(REPLACE "\n" " " QSS_CONTENT "${QSS_CONTENT}")
string(REPLACE "\"" "\\\"" QSS_CONTENT "${QSS_CONTENT}")
set(STYLESHEET "${QSS_CONTENT}")
configure_file(${CMAKE_SOURCE_DIR}/src/styles/style_template.h.in ${CMAKE_SOURCE_DIR}/src/style.h @ONLY)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(blocks
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/styles/menu.qss
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
