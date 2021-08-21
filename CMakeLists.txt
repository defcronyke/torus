cmake_minimum_required(VERSION 3.10)

project(torus)

add_executable(torus torus.c)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if (WIN32)
  set (GLUT_ROOT_PATH $ENV{USERPROFILE}/freeglut)

  find_library( GLUT_glut_LIBRARY_RELEASE NAMES glut glut32 freeglut
    PATHS
    ${OPENGL_LIBRARY_DIR}
    ${GLUT_ROOT_PATH}/Release
    ${GLUT_ROOT_PATH}/lib/x64
    ${GLUT_ROOT_PATH}/bin
  )

find_library( GLUT_glut_LIBRARY_DEBUG NAMES freeglutd
  PATHS
  ${OPENGL_LIBRARY_DIR}
  ${GLUT_ROOT_PATH}/Debug
  ${GLUT_ROOT_PATH}/lib/x64
  ${GLUT_ROOT_PATH}/bin
  )
  
  mark_as_advanced(GLUT_glut_LIBRARY_RELEASE GLUT_glut_LIBRARY_DEBUG)

  find_package(GLUT REQUIRED)

  target_link_libraries(
    torus
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
  )

  include_directories(
    ${GLUT_INCLUDE_DIRS}
    ${GLUT_ROOT_PATH}/include
  )
    
else ()

  set (CURSES_NEED_WIDE TRUE)
  
  find_package(Curses REQUIRED)

  find_package(GLUT REQUIRED)
  
  target_link_libraries(
    torus
    m
    ${CURSES_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
  )

  include_directories(
    ${GLUT_INCLUDE_DIRS}
  )
  
endif ()


# add_dependencies(
#   torus

#   OpenGL::OpenGL
#   OpenGL::GLU
# )
