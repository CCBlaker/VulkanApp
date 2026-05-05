cmake_minimum_required(VERSION 3.20)
project(VulkanApp)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Project headers
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/def/*.cpp"
)

add_executable(vkApp ${SRC_FILES} src/main.cpp)

target_link_libraries(vkApp ${Vulkan_LIBRARIES} glfw)