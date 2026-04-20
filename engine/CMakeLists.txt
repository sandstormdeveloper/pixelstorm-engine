file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(pixelstorm_engine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_compile_features(pixelstorm_engine PUBLIC cxx_std_17)
target_compile_definitions(pixelstorm_engine PUBLIC GLFW_INCLUDE_NONE)

target_include_directories(pixelstorm_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/external/glm
)

target_link_libraries(pixelstorm_engine PUBLIC glfw glad)

if (WIN32)
    target_link_libraries(pixelstorm_engine PUBLIC opengl32)
endif()
