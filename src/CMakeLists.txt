# Debug lib suffix
set(LIB_DEBUG_SUFFIX "d")

# Headers/sources
set(LIB_HEADER_FILES
		"Types.h"
		"LASsie.h"
	)
set(LIB_SOURCE_FILES
		"LASsie.cpp"
    )

# Target builds
add_library("${PROJECT_NAME}" STATIC ${LIB_SOURCE_FILES})
set_target_properties(
		"${PROJECT_NAME}"
		PROPERTIES
		DEBUG_OUTPUT_NAME   "${PROJECT_NAME}${LIB_DEBUG_SUFFIX}"
		RELEASE_OUTPUT_NAME "${PROJECT_NAME}"
	)

# Installation
install(TARGETS "${PROJECT_NAME}" DESTINATION "lib")
install(FILES ${LIB_HEADER_FILES} DESTINATION "include/${PROJECT_NAME}")
