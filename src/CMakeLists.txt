# Defs header file configuration
set(LASSIE_DEFS_OUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/LASsieDefs.h")
configure_file("LASsieDefs.h.in" "${LASSIE_DEFS_OUT_FILE}" @ONLY)

# Headers/sources
set(LIB_HEADER_FILES
		"${LASSIE_DEFS_OUT_FILE}"
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
		DEBUG_OUTPUT_NAME   "${PROJECT_NAME}${LASSIE_LIB_DEBUG_SUFFIX}"
		RELEASE_OUTPUT_NAME "${PROJECT_NAME}"
	)

# Installation
install(TARGETS "${PROJECT_NAME}" DESTINATION "lib${LASSIE_LIB_DIR_SUFFIX}")
install(FILES ${LIB_HEADER_FILES} DESTINATION "include/${PROJECT_NAME}")
