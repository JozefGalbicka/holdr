
add_executable(test_utils_strings
    utils/strings.test.c
)
add_executable(test_utils_file_reader
    utils/file_reader.test.c
)
add_executable(test_loaders_parser
    loaders/parser.test.c
)
add_executable(test_structures_array_list
    structures/list/array_list.test.c
)
add_executable(test_core_dns_packet
    core/dns_packet.test.c
)

target_link_libraries(test_utils_strings holdr_lib)
target_link_libraries(test_utils_file_reader holdr_lib)
target_link_libraries(test_loaders_parser holdr_lib)
target_link_libraries(test_structures_array_list holdr_lib)
target_link_libraries(test_core_dns_packet holdr_lib)

add_test (NAME utils-strings COMMAND test_utils_strings)
add_test (NAME utils-file-reader COMMAND test_utils_file_reader)
add_test (NAME loaders-parser COMMAND test_loaders_parser)
file(COPY loaders/zone_file.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/loaders)
add_test (NAME structures-array-list COMMAND test_structures_array_list)
file(COPY utils/foo.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/utils)
add_test (NAME core-dns-packet COMMAND test_core_dns_packet)
