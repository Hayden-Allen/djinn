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

option(DJINN_DIST OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    add_subdirectory(lib/libunistd)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
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
		    lib/nanovg/src
                    lib/quickjspp
                    lib/)

file(GLOB VENDORS_SOURCES lib/glad/src/glad.c lib/imgui/*.cpp lib/imgui/backends/imgui_impl_glfw.cpp lib/imgui/backends/imgui_impl_opengl3.cpp lib/nanovg/src/nanovg.c lib/glad/src/glad.cpp
        lib/quickjspp/cutils.c
        lib/quickjspp/libbf.c
        lib/quickjspp/libregexp.c
        lib/quickjspp/libunicode.c
        lib/quickjspp/quickjs.c
        lib/quickjspp/quickjs-bjson.c
        lib/quickjspp/quickjs-libc.c)
set(HATS_SOURCES lib/hats/src/direction.cpp lib/hats/src/point.cpp lib/hats/src/vec.cpp )
set(PROJECT_HEADERS "src/core/constants.h" "src/core/util.h" "src/scene/entity/entity.h"  "src/script/service/asset_service.h" "src/script/js.h" "src/manager/manager.h" "src/manager/asset/custom_mesh_manager.h" "src/pch.h" "src/script/service/service.h" "src/manager/asset/shader_manager.h" "src/script/service/render_service.h" "src/manager/asset/texture_manager.h" "src/script/service/nanovg_service.h" "src/manager/asset/cubemap_manager.h" "src/script/service/util_service.h" "src/scene/entity/camera_entity.h" "src/script/service/scene_service.h"  "src/manager/scene/entity_manager.h"  "src/script/service/input_service.h" "src/manager/scene/entity_manager_base.h" "src/manager/scene/camera_entity_manager.h" "src/script/service/imgui_service.h" "src/asset/mesh/custom_mesh.h" "src/scene/scene_object.h" "src/scene/mesh/mesh_instance.h" "src/core/identifiable.h" "src/scene/mesh/mesh_instance_batch.h" "src/manager/scene/mesh_instance_manager.h" "src/asset/shaders.h" "src/script/service/sound_service.h" "src/scene/sound_emitter.h" "src/manager/asset/sound_source_manager.h" "src/manager/scene/sound_emitter_manager.h" "src/asset/sound_source.h"  "src/asset/mesh/mesh.h" "src/asset/mesh/static_mesh.h" "src/asset/mesh/animated_mesh.h" "src/manager/asset/loaded_mesh_manager.h" "src/asset/mesh/loaded_mesh.h" "src/scene/mesh/animated_mesh_instance.h"  "src/manager/scene/physics_object_manager.h" "src/scene/physics/physics_object.h"  "src/scene/xport/phorm.h" "src/manager/xport/phorm_manager.h" "src/scene/visibility_scene_object.h" "src/scene/xport/light.h" "src/manager/xport/light_manager.h"   "src/scene/xport/waypoint.h" "src/manager/xport/waypoint_manager.h" "src/scene/xport/xport.h" "src/asset/xport/material.h" "src/manager/xport/material_manager.h" "src/scene/physics/bvh_physics_object.h" "src/scene/physics/box_physics_object.h" "src/scene/physics/sphere_physics_object.h" "src/scene/physics/capsule_physics_object.h" "src/scene/physics/primitive_physics_object.h" "src/core/m3d.h" "src/scene/physics/cylinder_physics_object.h" "src/debug/physics_debug_drawer.h" "src/scene/tagged.h" "src/script/service/event_service.h")
set(PROJECT_SOURCES "src/core/util.cpp"   "src/scene/entity/entity.cpp"  "src/script/service/asset_service.cpp" "src/script/js.cpp" "src/manager/asset/custom_mesh_manager.cpp" "src/manager/asset/shader_manager.cpp" "src/main.cpp" "src/script/service/render_service.cpp"  "src/manager/asset/texture_manager.cpp"  "src/script/service/nanovg_service.cpp" "src/manager/asset/cubemap_manager.cpp" "src/script/service/util_service.cpp" "src/scene/entity/camera_entity.cpp" "src/script/service/scene_service.cpp" "src/manager/scene/entity_manager.cpp"  "src/script/service/input_service.cpp"  "src/manager/scene/camera_entity_manager.cpp" "src/script/service/imgui_service.cpp" "src/scene/scene_object.cpp" "src/scene/mesh/mesh_instance.cpp" "src/scene/mesh/mesh_instance_batch.cpp" "src/asset/mesh/custom_mesh.cpp" "src/manager/scene/mesh_instance_manager.cpp" "src/asset/shaders.cpp" "src/script/service/sound_service.cpp"  "src/scene/sound_emitter.cpp" "src/manager/asset/sound_source_manager.cpp" "src/manager/scene/sound_emitter_manager.cpp" "src/asset/sound_source.cpp" "src/asset/mesh/mesh.cpp"  "src/asset/mesh/static_mesh.cpp" "src/core/m3d.cpp"  "src/asset/mesh/animated_mesh.cpp" "src/scene/mesh/animated_mesh_instance.cpp" "src/scene/physics/physics_object.cpp" "src/manager/scene/physics_object_manager.cpp"   "src/scene/xport/phorm.cpp" "src/manager/xport/phorm_manager.cpp" "src/scene/visibility_scene_object.cpp" "src/scene/xport/light.cpp" "src/manager/xport/light_manager.cpp" "src/scene/xport/waypoint.cpp" "src/manager/xport/waypoint_manager.cpp" "src/scene/xport/xport.cpp" "src/asset/xport/material.cpp" "src/manager/xport/material_manager.cpp" "src/scene/physics/bvh_physics_object.cpp" "src/scene/physics/box_physics_object.cpp" "src/scene/physics/sphere_physics_object.cpp" "src/scene/physics/capsule_physics_object.cpp" "src/scene/physics/primitive_physics_object.cpp" "src/scene/physics/cylinder_physics_object.cpp"  "src/debug/physics_debug_drawer.cpp" "src/scene/tagged.cpp" "src/script/service/event_service.cpp")
if(MSVC)
    set(WATCHER_HEADERS "src/debug/directory_watcher.h" "src/debug/script_watcher.h" "src/debug/shader_watcher.h" "src/debug/texture_watcher.h" "src/debug/cubemap_watcher.h" "src/debug/sound_source_watcher.h")
    set(WATCHER_SOURCES "src/debug/texture_watcher.cpp" "src/debug/cubemap_watcher.cpp" "src/debug/sound_source_watcher.cpp" "src/debug/shader_watcher.cpp")
endif()
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                         .gitattributes
                         .gitignore
                         .gitmodules)


add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -D_CRT_SECURE_NO_WARNINGS
                -DNVG_NO_STB)
if(DJINN_DIST)
        add_definitions(-DDJINN_DIST)
endif()
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${WATCHER_SOURCES} ${WATCHER_HEADERS} ${PROJECT_CONFIGS} ${VENDORS_SOURCES} ${HATS_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
if(MSVC)
        target_link_libraries(${PROJECT_NAME} libunistd)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    CXX_STANDARD 23)
