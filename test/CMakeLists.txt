include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.2
    SYSTEM
)
FetchContent_MakeAvailable(Catch2)
set_target_properties(Catch2 Catch2WithMain PROPERTIES CXX_CLANG_TIDY "")

function(f_add_test_executable target_name file_name) 
    add_executable(${target_name} ${file_name})
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(${target_name} PRIVATE
        Catch2::Catch2WithMain
        # Core
    )
    target_compile_options(${target_name} PRIVATE -g -O0)
    catch_discover_tests(${target_name})
    set_target_properties(${target_name} PROPERTIES CXX_CLANG_TIDY "") # required clear for c++26 syntax
endfunction()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

f_add_test_executable(TestCsvRollingStats TestCsvRollingStats.cc)
f_add_test_executable(TestHelperTest TestHelperTest.cc)