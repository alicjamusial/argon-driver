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

    include/commands/Erase.hpp
    src/commands/Erase.cpp

    include/commands/RawSPI.hpp
    src/commands/RawSPI.cpp

    include/commands/Read.hpp
    src/commands/Read.cpp

    include/commands/ReadStatus.hpp
    src/commands/ReadStatus.cpp

    include/commands/Write.hpp
    src/commands/Write.cpp
    )

target_link_libraries(${NAME} PRIVATE
    CLI11::CLI11
    flash_controller
)