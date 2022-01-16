cmake_minimum_required(VERSION 3.10)

project(torus)

set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

add_executable(torus src/torus.c deps/jack2/simple_client.c)
# add_executable(torus src/torus.c deps/jack2/capture_client.c)
# add_executable(torus src/torus.c deps/jack2/simple_session_client.c)
# add_executable(torus src/torus.c deps/jack2/inprocess.c)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

find_package(JACK REQUIRED)


# # libsndfile
# find_package(PkgConfig QUIET)
# if(PKG_CONFIG_FOUND)
#     pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
# endif(PKG_CONFIG_FOUND)

# # Include dir
# find_path(LIBSNDFILE_INCLUDE_DIR
#         NAMES sndfile.h
#         PATHS ${LIBSNDFILE_PKGCONF_INCLUDE_DIRS}
#         )

# # Library
# find_library(LIBSNDFILE_LIBRARY
#         NAMES sndfile libsndfile-1
#         PATHS ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}
#         )

# find_package(PackageHandleStandardArgs)
# find_package_handle_standard_args(LibSndFile  DEFAULT_MSG  LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)

# if(LIBSNDFILE_FOUND)
#     set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
#     set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
# endif(LIBSNDFILE_FOUND)

# mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)

# include(FindPkgConfig)
# pkg_search_module(SndFile REQUIRED sndfile)




if (MINGW)
  
  set (GLUT_ROOT_PATH ${CMAKE_SOURCE_DIR}/deps/freeglut-mingw/freeglut)

  find_library( GLUT_glut_LIBRARY_RELEASE NAMES glut glut32 freeglut
    PATHS
    ${OPENGL_LIBRARY_DIR}
    ${GLUT_ROOT_PATH}/Release
    ${GLUT_ROOT_PATH}/lib
    ${GLUT_ROOT_PATH}/lib/x64
    ${GLUT_ROOT_PATH}/bin
    ${GLUT_ROOT_PATH}/bin/x64
  )

  find_library( GLUT_glut_LIBRARY_DEBUG NAMES freeglutd
    PATHS
    ${OPENGL_LIBRARY_DIR}
    ${GLUT_ROOT_PATH}/Debug
    ${GLUT_ROOT_PATH}/lib
    ${GLUT_ROOT_PATH}/lib/x64
    ${GLUT_ROOT_PATH}/bin
    ${GLUT_ROOT_PATH}/bin/x64
  )

  set(
    GLUT_glut_LIBRARY_RELEASE
  
    ${GLUT_ROOT_PATH}/lib/x64/libfreeglut.a
  )

  set(
    GLUT_glut_LIBRARY_DEBUG
  
    ${GLUT_ROOT_PATH}/lib/x64/libfreeglut.a
  )

  set(
    GLUT_glut_LIBRARY
  
    ${GLUT_ROOT_PATH}/lib/x64/libfreeglut.a
  )

  set(
    GLUT_LIBRARY
    
    ${GLUT_glut_LIBRARY}
  )

  target_link_libraries(
    torus
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${JACK_LIBRARIES}
    # ${LIBSNDFILE_LIBRARIES}
  )

  include_directories(
    "deps"
    ${GLUT_INCLUDE_DIRS}
    ${GLUT_ROOT_PATH}/include
    ${JACK_INCLUDE_DIRS}
    # ${LIBSNDFILE_INCLUDE_DIRS}
  )

elseif (WIN32)

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
    ${JACK_LIBRARIES}
    # ${LIBSNDFILE_LIBRARIES}
  )

  include_directories(
    "deps"
    ${GLUT_INCLUDE_DIRS}
    ${GLUT_ROOT_PATH}/include
    ${JACK_INCLUDE_DIRS}
    # ${LIBSNDFILE_INCLUDE_DIRS}
  )

else ()

  find_package(GLUT REQUIRED)
  find_package(OpenGL REQUIRED)

  set (CURSES_NEED_WIDE TRUE)
    
  find_package(Curses REQUIRED)

  target_link_libraries(
    torus

    pthread
    m
    ${CURSES_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${JACK_LIBRARIES}
    # ${LIBSNDFILE_LIBRARIES}
  )

  include_directories(
    ${OpenGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${JACK_INCLUDE_DIRS}
    "deps"
    # ${LIBSNDFILE_INCLUDE_DIRS}
  )

endif ()
