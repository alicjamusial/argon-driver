set(NAME argon_cli)

add_executable(${NAME})

target_include_directories(${NAME} PRIVATE include)

target_sources(${NAME} PRIVATE
    src/main.cpp

    include/GlobalOptions.hpp
    src/GlobalOptions.cpp

    include/commands/ListDevices.hpp
    src/commands/ListDevices.cpp

    include/commands/ReadId.hpp
    src/commands/ReadId.cpp
    )

target_link_libraries(${NAME} PRIVATE
    CLI11::CLI11
)