# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME SpelProjekt)
project(${PROJECT_NAME})

# Set the files used in the target
file(GLOB_RECURSE SOURCES Source/*.cpp)
file(GLOB_RECURSE HEADERS Include/*.h)


# Set search paths
include_directories(Include)		 # No idea what this is

# Set library search path
#link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SpelProjekt/Debug)

# Add the target
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} d3d11.lib)
target_link_libraries(${PROJECT_NAME} d3dcompiler.lib)
target_link_libraries(${PROJECT_NAME} dxguid.lib)
target_link_libraries(${PROJECT_NAME} Xinput9_1_0.lib)
target_link_libraries(${PROJECT_NAME} dxguid.lib)
target_link_libraries(${PROJECT_NAME} winmm.lib)
target_link_libraries(${PROJECT_NAME} comctl32.lib)


# Set SUBSYSTEM:WINDOWS
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

add_custom_target(_resourceCopy ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/LillaSpelTest/Shaders ${CMAKE_BINARY_DIR}/LillaSpelTest
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/LillaSpelTest/Objs ${CMAKE_BINARY_DIR}/LillaSpelTest
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/LillaSpelTest/Textures ${CMAKE_BINARY_DIR}/LillaSpelTest
)