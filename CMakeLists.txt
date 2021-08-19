cmake_minimum_required(VERSION 3.10)

project(torus)

set (CURSES_NEED_WIDE TRUE)

add_executable(torus torus.c)

find_package(Curses REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_dependencies(
  torus

  OpenGL::OpenGL
  OpenGL::GLU
)

include_directories(
  ${GLUT_INCLUDE_DIRS}
)

target_link_libraries(
  torus 
  
  m
  ${CURSES_LIBRARIES}
  OpenGL::OpenGL
  OpenGL::GLU
  ${GLUT_LIBRARY}
)
