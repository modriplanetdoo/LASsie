# Sources
set(TEST_SOURCE_FILES
        "main.cpp"
    )

add_executable("test${PROJECT_NAME}" "${TEST_SOURCE_FILES}")
target_link_libraries("test${PROJECT_NAME}" "${PROJECT_NAME}")
