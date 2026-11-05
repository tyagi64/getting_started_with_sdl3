# SDL3
find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
)
