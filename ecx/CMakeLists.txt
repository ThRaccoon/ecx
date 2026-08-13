add_library(ecx STATIC
    "src/ecx.cpp"
    "src/entity/entity_manager.cpp"
    "src/component/component_manager.cpp"
    "src/core/coordinator.cpp"
)

target_include_directories(ecx PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
