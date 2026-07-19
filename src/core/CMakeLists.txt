add_library(Core STATIC
    StatProcessor.cc
)
apply_compiler_flags(Core)
target_include_directories(Core PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src

)
target_link_libraries(Core PRIVATE
)