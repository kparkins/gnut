cmake_minimum_required(VERSION 3.1.0)
project(gnut)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT WIN32) 
  set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
  set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif(NOT WIN32)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

if(WIN32)
  include_directories("lib/glfw/include" "lib/glm" "lib/glew/include" "lib/soil/include")
  link_directories("lib/glew/lib/Release/x64" "lib/glfw/lib-mingw-w64" "lib/soil")
else(WIN32)
  find_package(glfw3 REQUIRED)
  find_package(GLM REQUIRED)
  find_package(GLEW REQUIRED)
endif(WIN32)

if(APPLE)
  include_directories("/usr/local/include")
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif(APPLE)

include_directories("src/gfx" "src/util" "src/net" "src/" "src/tools" "src/tools/log" "src/tools/script")
file(GLOB_RECURSE SOURCES "src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)


if(WIN32)
target_link_libraries(${PROJECT_NAME}
	${OPENGL_LIBRARIES} 
	glfw3 
	glew32s
	glu32
    libSOIL.a
	opengl32)
endif(WIN32)

if(APPLE)
target_link_libraries(${PROJECT_NAME}
      ${GLFW3_LIBRARY}
      ${OPENGL_LIBRARIES}
      ${GLEW_LIBRARY}
      ${COCOA_LIBRARY}
      ${COREVID_LIBRARY}
      ${IOKIT_LIBRARY})
endif(APPLE)

if(UNIX)
target_link_libraries(${PROJECT_NAME}
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
	${GLEW_LIBRARY}
	pthread)
endif(UNIX)
