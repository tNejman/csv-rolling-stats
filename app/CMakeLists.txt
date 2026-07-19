add_executable(${APPLICATION_NAME} Main.cc)
apply_compiler_flags(${APPLICATION_NAME})
target_include_directories(${APPLICATION_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include

)
target_link_libraries(${APPLICATION_NAME} PRIVATE
)