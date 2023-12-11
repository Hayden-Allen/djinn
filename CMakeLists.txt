cmake_minimum_required(VERSION 3.5)
project(myProject)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(lib/bullet3)

add_subdirectory(lib/libunistd)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(src/
                    lib/bullet3/src/
                    lib/glad/include/
                    lib/glfw/include/
		    lib/hapi/include
		    lib/hats/include
		    lib/imgui/
		    lib/mingl/include
		    lib/miniaudio/
		    lib/model3d
		    lib/nanovg/src
                    lib/quickjspp
                    lib/stb/)

file(GLOB VENDORS_SOURCES lib/glad/src/glad.c lib/imgui/*.cpp lib/imgui/backends/imgui_impl_glfw.cpp lib/imgui/backends/imgui_impl_opengl3.cpp lib/nanovg/src/nanovg.c lib/quickjspp/*.c)
set(HATS_SOURCES lib/hats/src/direction.cpp lib/hats/src/point.cpp lib/hats/src/vec.cpp)
set(PROJECT_HEADERS)
set(PROJECT_SOURCES "src/main.cpp")
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                         .gitattributes
                         .gitignore
                         .gitmodules)


add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -D_CRT_SECURE_NO_WARNINGS)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} ${HATS_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_link_libraries(${PROJECT_NAME} glfw libunistd
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    CXX_STANDARD 23)
