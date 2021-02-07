set(NAME argon)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static -static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
endif ()

add_executable(${NAME}
    include/argon/FlashController.hpp

    src/main.cpp
    src/FlashController.cpp
)

target_link_libraries(${NAME}
    PUBLIC
        flash_controller
        mpsse
)

target_include_directories(${NAME} PUBLIC
    include
)