cmake_minimum_required(VERSION 3.16)
project(djinn)

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


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    add_subdirectory(lib/libunistd)
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
                    lib/haul/include
		    lib/imgui/
		    lib/mingl/include
		    lib/miniaudio/
		    lib/model3d
		    lib/nanovg/src
                    lib/quickjspp
                    lib/stb/)

file(GLOB VENDORS_SOURCES lib/glad/src/glad.c lib/imgui/*.cpp lib/imgui/backends/imgui_impl_glfw.cpp lib/imgui/backends/imgui_impl_opengl3.cpp lib/nanovg/src/nanovg.c lib/glad/src/glad.cpp
        lib/quickjspp/cutils.c
        lib/quickjspp/libbf.c
        lib/quickjspp/libregexp.c
        lib/quickjspp/libunicode.c
        lib/quickjspp/quickjs.c
        lib/quickjspp/quickjs-bjson.c
        lib/quickjspp/quickjs-libc.c)
set(HATS_SOURCES lib/hats/src/direction.cpp lib/hats/src/point.cpp lib/hats/src/vec.cpp)
set(PROJECT_HEADERS "src/constants.h" "src/script_manager.h" "src/util.h" "src/mesh_manager.h" "src/manager.h" "src/asset_service.h" "src/js_global.h")
set(PROJECT_SOURCES "src/main.cpp"   "src/directory_watcher.h" "src/directory_watcher.cpp" "src/script_watcher.h" "src/script_watcher.cpp" "src/entity.h" "src/entity.cpp" "src/util.cpp" "src/script_manager.cpp" "src/mesh_manager.cpp" "src/asset_service.cpp" "src/js_global.cpp")
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                         .gitattributes
                         .gitignore
                         .gitmodules)


add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -D_CRT_SECURE_NO_WARNINGS)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} ${HATS_SOURCES} "src/constants.h")
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
if(MSVC)
        target_link_libraries(${PROJECT_NAME} libunistd)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    CXX_STANDARD 23)
