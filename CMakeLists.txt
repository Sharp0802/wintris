cmake_minimum_required(VERSION 3.26)
project(wintris LANGUAGES C)

include_directories(inc)
add_executable(wintris
        src/blkctl.c
        src/main.c
        src/winstd.c
        src/error.c
)
target_compile_features(wintris PRIVATE c_std_99)
