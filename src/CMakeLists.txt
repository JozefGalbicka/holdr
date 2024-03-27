
add_library (holdr_lib
    structures/list/array_list.h
    structures/list/array_list.c
    utils/strings.h
    utils/strings.c
    utils/prints.h
    utils/prints.c
    utils/macros.h
    loaders/config.h
    loaders/config.c
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
