set(NAME ftdi)

include(lookup_ftdi.cmake)

lookup_ftdi()

message(STATUS "[FTDI] URL: ${FTDI_DRIVER_URL}")
message(STATUS "[FTDI] Library dir: ${FTDI_LIB_DIR}")

include(FetchContent)

FetchContent_Declare(
    ftdi_zip
    URL ${FTDI_DRIVER_URL}
)

FetchContent_MakeAvailable(ftdi_zip)

add_library(${NAME} STATIC IMPORTED GLOBAL)

set_target_properties(${NAME} PROPERTIES
    IMPORTED_LOCATION ${ftdi_zip_SOURCE_DIR}/${FTDI_LIB_DIR}/ftd2xx.lib
)

target_include_directories(${NAME} INTERFACE ${ftdi_zip_SOURCE_DIR})
