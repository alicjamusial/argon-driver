set(NAME argon)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static -static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
endif ()

add_executable(${NAME}
    src/main.cpp
)

target_link_libraries(${NAME}
    PUBLIC
        flash_controller
        mpsse
)

target_include_directories(${NAME} PUBLIC
    include
)