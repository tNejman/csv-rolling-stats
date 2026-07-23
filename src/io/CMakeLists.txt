add_library(Io STATIC
    CsvReader.cc
    CsvWriter.cc
)
apply_compiler_flags(Io)
target_include_directories(Io PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src

)
target_link_libraries(Io PRIVATE
)