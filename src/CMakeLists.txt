
add_library (holdr_lib
    structures/list/array_list.h
    structures/list/array_list.c

    structures/tree/red_black_tree.h
    structures/tree/red_black_tree.c
    structures/tree/red_black_tree_node.h
    structures/tree/red_black_tree_node.c

    structures/tree/trie.h
    structures/tree/trie.c
    structures/tree/trie_node.h
    structures/tree/trie_node.c

    structures/table/murmur_hash.h
    structures/table/murmur_hash.c
    structures/table/mersenne_twister.h
    structures/table/mersenne_twister.c
    structures/table/hash_table.h
    structures/table/hash_table.c
    structures/table/hash_table_node.h
    structures/table/hash_table_node.c

    utils/strings.h
    utils/strings.c
    utils/prints.h
    utils/prints.c
    utils/file_reader.h
    utils/file_reader.c
    utils/macros.h
    loaders/config.h
    loaders/config.c
    loaders/parser.h
    loaders/parser.c
    core/dns_packet.h
    core/dns_packet.c
    core/server.h
    core/server.c
    ../external/tomlc/toml.h
    ../external/tomlc/toml.c
)

add_executable(holdr
    main.c
)

target_link_libraries(holdr holdr_lib)
