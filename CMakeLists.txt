cmake_minimum_required(VERSION 3.16)
project(pixelstorm-engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PIXELSTORM_BUILD_DEMO "Build the demo executable" ON)
option(PIXELSTORM_BUILD_IMGUI "Build Dear ImGui from external/imgui" ON)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

# GLAD
add_library(pixelstorm_glad STATIC external/glad/src/glad.c)
add_library(pixelstorm::glad ALIAS pixelstorm_glad)
target_include_directories(pixelstorm_glad PUBLIC external/glad/include)
set_target_properties(pixelstorm_glad PROPERTIES FOLDER "external")

# GLM
add_library(pixelstorm_glm INTERFACE)
add_library(pixelstorm::glm ALIAS pixelstorm_glm)
target_include_directories(pixelstorm_glm INTERFACE external/glm)

# ImGui
if (PIXELSTORM_BUILD_IMGUI)
    add_library(pixelstorm_imgui STATIC
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
    )
    add_library(pixelstorm::imgui ALIAS pixelstorm_imgui)
    target_include_directories(pixelstorm_imgui
        PUBLIC
            external/imgui
            external/imgui/backends
    )
    target_link_libraries(pixelstorm_imgui PUBLIC glfw pixelstorm::glad)
    target_compile_definitions(pixelstorm_imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
    set_target_properties(pixelstorm_imgui PROPERTIES FOLDER "external")
endif()

add_subdirectory(engine)

if (PIXELSTORM_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if (TARGET pixelstorm-demo)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pixelstorm-demo)
endif()
