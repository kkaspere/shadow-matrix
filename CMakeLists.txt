cmake_minimum_required(VERSION 3.16)
project(matrix_shadow C)
set(CMAKE_C_STANDARD 99)
set(SOURCES src/matrix_shd.c main.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(PROJ_INCLUDE "./inc")
add_executable(shadow_matrix ${SOURCES})
target_include_directories(shadow_matrix PUBLIC ${PROJ_INCLUDE})

#find_package(cmocka 1.1.0 REQUIRED)

# testing binary
set(TESTING_SOURCES src/matrix_shd.c test.c)
add_executable(shadow_matrix_test ${TESTING_SOURCES})
target_include_directories(shadow_matrix_test PUBLIC ${PROJ_INCLUDE})
target_link_libraries(shadow_matrix_test PRIVATE cmocka)
# enable testing functionality
enable_testing()

# define tests
add_test(
        NAME shadow_matrix_test
        COMMAND $<TARGET_FILE:shadow_matrix_test>
)
