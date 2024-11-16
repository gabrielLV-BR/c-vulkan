cmake_minimum_required(VERSION 3.17)

project(mygame VERSION 1.0)

set(GLFW_BUILD_WAYLAND 1)
set(GLFW_BUILD_X11 0)

add_subdirectory(modules/glfw)

add_executable(
	${PROJECT_NAME}
	src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/vendor/" "${GLFW_SOURCE_DIR}/include/")
