cmake_minimum_required(VERSION 3.16)
project(quoridor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "This project targets the Windows console (windows.h, conio.h).")
endif()

add_executable(quoridor
        src/main.c
        src/jeu.c
        src/affichage.c
        src/interaction.c
        src/utile.c)

target_include_directories(quoridor PRIVATE include)

if(MSVC)
    target_compile_definitions(quoridor PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(quoridor PRIVATE /W3 /utf-8)
else()
    target_compile_options(quoridor PRIVATE -Wall -Wextra)
endif()
