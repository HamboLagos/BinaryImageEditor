project(unit_tests)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/vendor/googletest/googletest/include
    )

add_executable(
    quad_node_tests
    quad_node_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/quad_node.cpp
    )
target_link_libraries(quad_node_tests gmock gtest gmock_main)
add_test(NAME quad_node COMMAND quad_node_tests)

add_executable(
    quad_tree_tests
    quad_tree_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/quad_tree.cpp
    )
target_link_libraries(quad_tree_tests gmock gtest gmock_main)
add_test(NAME quad_tree COMMAND quad_tree_tests)
