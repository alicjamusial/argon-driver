set(NAME flash_controller)

add_library(${NAME} STATIC)

target_include_directories(${NAME} PUBLIC
    include
)

target_link_libraries(${NAME} PUBLIC
    mpsse
    ftdi
    ${FTDI_DRIVER_PATH}/amd64/ftd2xx.lib
)

target_sources(${NAME} PRIVATE
    include/flash_controller/FlashController.hpp
    include/flash_controller/FlashDriver.hpp
    include/flash_controller/SPI.hpp
    include/flash_controller/helpers.hpp

    src/FlashController.cpp
    src/FlashDriver.cpp
    src/SPI.cpp
)