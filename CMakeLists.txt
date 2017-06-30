cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#for FindGLM.cmake, FindGLFW3.cmake, other helpers
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

FIND_PACKAGE(OpenGL REQUIRED) #for graphics
FIND_PACKAGE(GLEW REQUIRED)

#required CMake options for QT Apps
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#for window widgets
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Multimedia REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}  ${Qt5Multimedia_INCLUDE_DIRS})

#default OpenGL libs
SET(EXT_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})

#get symlink_data macro
include(${CMAKE_SOURCE_DIR}/cmake_modules/helpers.cmake)

INCLUDE_DIRECTORIES(
  ${GLEW_INCLUDE_DIR}
  ./
  )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

#QT headers requiring MOC (meta object compiler)
set(QTHDRS
  mainwindow.h
  mypanelopengl.h
  kiss_fft130/kiss_fft.h
  kiss_fft130/tools/kiss_fftr.h

  )

#window design files
set(QTFORMS mainwindow.ui)

#Run qt tools to auto generate C++ compatible files
QT5_WRAP_CPP(QTHDRS_MOC ${QTHDRS})
QT5_WRAP_UI(QTFORMS_HDRS ${QTFORMS})
SET(MYQTFILES ${QTHDRS_MOC} ${QTFORMS_HDRS})

#normal source files
set(SRC
  main.cpp
  mainwindow.cpp
  mypanelopengl.cpp
  kiss_fft130/kiss_fft.c
)

# Allow shaders to show up in QT Creator
set(SHADERS
  shaders/vshader.glsl
  shaders/fshader.glsl
)

# main executable
ADD_EXECUTABLE(final ${SRC} ${MYQTFILES} ${SHADERS})

TARGET_LINK_LIBRARIES(final ${EXT_LIBS} Qt5::Widgets Qt5::OpenGL -lrtaudio Qt5::OpenGL Qt5::Multimedia)
#qt5_use_modules(final OpenGL Widgets)

#link shaders folder to build directory
symlink_data(final_shaders shaders)
