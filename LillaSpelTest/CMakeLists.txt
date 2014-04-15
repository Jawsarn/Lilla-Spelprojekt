# CMake settings
cmake_minimum_required(VERSION 2.8)

# Root project settings
set(PROJECT_NAME LillaSpelTest)
project(${PROJECT_NAME})

# Set the files used in the target
file(GLOB_RECURSE SOURCES Source/*.cpp)
file(GLOB_RECURSE HEADERS Include/*.h)


# Set search paths
include_directories(Include)		 # No idea what this is

# Set library search path
link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LillaSpel/Debug)

# Add the target
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})



# Set SUBSYSTEM:WINDOWS
set_target_properties(${PROJECT_NAME} )