set(NAME argon_cli)

add_executable(${NAME})

target_sources(${NAME} PRIVATE
    src/main.cpp
    )

target_link_libraries(${NAME} PRIVATE
    CLI11::CLI11
)