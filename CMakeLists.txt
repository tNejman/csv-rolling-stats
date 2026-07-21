cmake_minimum_required(VERSION 3.25)
project(CSV_Rolling_Stats LANGUAGES CXX)
set(APPLICATION_NAME csv-rolling-stats)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type is: ${CMAKE_BUILD_TYPE}")

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" --use-color --fix-errors)
endif()

function(apply_compiler_flags target)
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-g -O0
        -Wall -Wextra -Wpedantic -Werror 
        -Wcast-align -Wcast-qual 
        -Wconversion -Wsign-conversion -Wshadow 
        -Wdouble-promotion -Wnull-dereference 
        -Wformat=2 
        -Wzero-as-null-pointer-constant 
        -Wvla
        -fno-omit-frame-pointer
        -fsanitize=address
        -fsanitize=undefined>

        $<$<CONFIG:Release>:-march=native>
    )
    target_link_options(${target} PUBLIC $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endfunction()

add_subdirectory(app)
add_subdirectory(src/io)

enable_testing()
add_subdirectory(test)