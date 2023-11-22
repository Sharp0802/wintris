cmake_minimum_required(VERSION 3.26)
project(wintris LANGUAGES C)

include_directories(inc)
add_executable(wintris
        src/blkctl.c
        src/main.c
        src/winstd.c
        src/error.c
)
