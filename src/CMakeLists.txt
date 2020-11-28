set(NAME argon)

set(FTDI_DRIVER_PATH C:/sterowniczki/ftdi_driver)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static -static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
endif ()

add_executable(${NAME}
    main.cpp
    SPIMemory.cpp
    SPI.cpp
    N25QDriver.cpp



)

add_subdirectory(mpsse)
add_subdirectory(ftdi)

target_link_libraries(${NAME}
    PUBLIC
        mpsse
        ftdi
        ${FTDI_DRIVER_PATH}/amd64/ftd2xx.lib
)

#target_include_directories(${NAME} PRIVATE include)