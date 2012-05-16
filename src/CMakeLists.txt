# Cmake minimum version: 2.8
cmake_minimum_required(VERSION 2.8)

# Set the source files
set(SRCS main.cpp App.cpp Vector3.cpp Mesh.cpp Matrix4.cpp Shader.cpp)

# Set the include directories
set(INCLUDES ${CMAKE_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIR} 
    ${GLEW_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

# Set the libraries needed by this project
set(LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SFML_LIBRARIES})
# We need to do some funny stuff on windows.
if(WIN32)
set(LIBS ${LIBS} opengl32 glu32 winmm)
endif(WIN32)

# Add the sources, include dirs and libs
add_executable(Simple3DModelRenderer ${SRCS})
include_directories(${INCLUDES})
target_link_libraries(Simple3DModelRenderer ${LIBS})
