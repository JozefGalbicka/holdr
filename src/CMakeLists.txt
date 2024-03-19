
add_library (holdr_lib
    utils/strings.h
    utils/strings.c
    utils/prints.h
    utils/prints.c
    utils/macros.h
)

add_executable(holdr
    main.c
)

target_link_libraries(holdr holdr_lib)
