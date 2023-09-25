add_executable (FruityFall "Application.cpp" "Application.h" "main.cpp" "Fruit.h" "Fruit.cpp" "Random.h" "Random.cpp" "Bucket.h" "Bucket.cpp" "Object.h" "Object.cpp")

target_link_libraries(FruityFall PRIVATE raylib)

add_custom_command(TARGET FruityFall POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:FruityFall>/assets
  COMMAND_EXPAND_LISTS
  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FruityFall PROPERTY CXX_STANDARD 20)
endif()