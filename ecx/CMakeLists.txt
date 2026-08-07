add_library(ecx STATIC
    "src/ecx.cpp"
)

target_include_directories(ecx PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
