project(LASsie C CXX)
cmake_minimum_required(VERSION 2.8)

# No in-source building
if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	file(REMOVE_RECURSE "CMakeFiles" "CMakeCache.txt")
	message(FATAL_ERROR "In-source building not allowed.")
endif()

# CMake module paths
if (WIN32 AND NOT CYGWIN)
	set(LASSIE_INSTALL_CMAKE_SUBDIR "CMake")
	set(LASSIE_INSTALL_DIR_REL2CMAKE "..")
else()
	set(LASSIE_INSTALL_CMAKE_SUBDIR "lib/CMake/${PROJECT_NAME}")
	set(LASSIE_INSTALL_DIR_REL2CMAKE "../../..")
endif()

# Version vars
set(LASSIE_VERSION_MAJOR 1)
set(LASSIE_VERSION_MINOR 0)
set(LASSIE_VERSION_PATCH 0)
set(LASSIE_VERSION "${LASSIE_VERSION_MAJOR}.${LASSIE_VERSION_MINOR}.${LASSIE_VERSION_PATCH}")

# Compiler defines
add_definitions("-DLASSIE_VERSION_MAJOR=${LASSIE_VERSION_MAJOR}")
add_definitions("-DLASSIE_VERSION_MINOR=${LASSIE_VERSION_MINOR}")
add_definitions("-DLASSIE_VERSION_PATCH=${LASSIE_VERSION_PATCH}")

# Suffixes
set(LASSIE_LIB_DEBUG_SUFFIX "d")
if("${CMAKE_SIZEOF_VOID_P}" MATCHES "8")
	set(LASSIE_LIB_DIR_SUFFIX "64")
else()
    set(LASSIE_LIB_DIR_SUFFIX "")
endif()

# Sub-directories
add_subdirectory(src)
add_subdirectory(testsrc)

# Configure ${PROJECT_NAME}Config{,Version}.cmake files
configure_file("${PROJECT_NAME}Config.cmake.in"        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"        @ONLY)
configure_file("${PROJECT_NAME}ConfigVersion.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Installation
install(FILES
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/${LASSIE_INSTALL_CMAKE_SUBDIR}")
