set(NAME ftdi)

add_library(${NAME} STATIC)
target_include_directories(${NAME} PUBLIC
    ${FTDI_DRIVER_PATH}
)

target_sources(${NAME} PRIVATE
    ${FTDI_DRIVER_PATH}/ftd2xx.h
    src/dummy.c
)
#target_include_directories(${NAME} PRIVATE include)