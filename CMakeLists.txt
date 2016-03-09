project(LASsie C CXX)
cmake_minimum_required(VERSION 2.8)

# No in-source building
if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	file(REMOVE_RECURSE "CMakeFiles" "CMakeCache.txt")
	message(FATAL_ERROR "In-source building not allowed.")
endif()

# Sub-directories
add_subdirectory(src)
add_subdirectory(testsrc)
