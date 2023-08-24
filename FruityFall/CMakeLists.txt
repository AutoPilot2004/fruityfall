# CMakeList.txt : CMake project for FruityFall, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (FruityFall "Application.cpp" "Application.h" "main.cpp" "Fruit.h" "Fruit.cpp" "Random.h" "Random.cpp" "Bucket.h" "Bucket.cpp" "Object.h" "Object.cpp")

target_link_libraries(FruityFall PRIVATE raylib)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/ ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FruityFall PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
