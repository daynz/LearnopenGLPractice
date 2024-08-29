cmake_minimum_required(VERSION 3.20)

project(2DGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
add_subdirectory(lib/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)
add_subdirectory(lib/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
add_subdirectory(lib/assimp)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)