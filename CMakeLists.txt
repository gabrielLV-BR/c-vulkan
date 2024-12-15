cmake_minimum_required(VERSION 3.17)

project(mygame VERSION 1.0)

set (C_STANDARD 99)

# temp
set(GLFW_BUILD_WAYLAND 1)
set(GLFW_BUILD_X11 0)

add_subdirectory(modules/glfw)

find_package(Vulkan REQUIRED)

add_executable(
	${PROJECT_NAME}
	src/main.c
	src/window.c

	src/utils/strings.c

	src/renderer/renderer.c

	src/renderer/backend/vulkan_backend.c
	src/renderer/backend/vulkan_command_buffer.c
	src/renderer/backend/vulkan_utils.c

)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan)
target_include_directories(
	${PROJECT_NAME} 
	PRIVATE 
	"${CMAKE_SOURCE_DIR}/src/"
	"${CMAKE_SOURCE_DIR}/vendor/"
	"${GLFW_SOURCE_DIR}/include/"
)
