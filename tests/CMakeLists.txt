
add_executable(test_utils_strings
    utils/strings.test.c
)
target_link_libraries(test_utils_strings holdr_lib)

add_test (NAME utils-strings COMMAND test_utils_strings)
