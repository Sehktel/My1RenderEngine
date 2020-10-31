# Project settings
cmake_minimum_required(VERSION 3.1)
project(My1GameEngine)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

# The main executable

add_executable(game WIN32
  src/main.cpp
  src/FileSystem.cpp
  src/RenderSystem.cpp
  src/Gameplay/GraphicsObject.cpp
)

set_property(TARGET game PROPERTY CXX_STANDARD 11)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
include_directories(lib/glfw/include)
target_link_libraries(game PRIVATE glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

# glad
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)
include_directories(lib/glad/include)
target_link_libraries(game PRIVATE glad)

#stb
include_directories(${CMAKE_SOURCE_DIR}/lib/stb_image)
link_directories(${CMAKE_SOURCE_DIR}/lib/stb_image)

#glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
include_directories(lib/glm)