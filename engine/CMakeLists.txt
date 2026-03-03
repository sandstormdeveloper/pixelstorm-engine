file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(pixelstorm STATIC ${ENGINE_SOURCES})
add_library(pixelstorm::engine ALIAS pixelstorm)

target_include_directories(pixelstorm
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(pixelstorm
    PUBLIC
        glfw
        pixelstorm::glad
        pixelstorm::glm
)

if (TARGET pixelstorm::imgui)
    target_link_libraries(pixelstorm PUBLIC pixelstorm::imgui)
endif()

target_compile_features(pixelstorm PUBLIC cxx_std_17)
