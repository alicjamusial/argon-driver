set(NAME mpsse)

add_library(${NAME} STATIC)
target_include_directories(${NAME} PUBLIC
    include
)

target_link_libraries(${NAME} PUBLIC
        ftdi
        )

target_sources(${NAME} PRIVATE
    include/mpsse/ftdi_common.h
    include/mpsse/ftdi_infra.h
    include/mpsse/ftdi_mid.h
    include/mpsse/ftdi_spi.h
    include/mpsse/libMPSSE_spi.h

    src/ftdi_common.c
    src/ftdi_infra.c
    src/ftdi_mid.c
    src/ftdi_spi.c
)
#target_include_directories(${NAME} PRIVATE include)