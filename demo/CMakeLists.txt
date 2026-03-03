file(GLOB_RECURSE DEMO_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(pixelstorm-demo ${DEMO_SOURCES})

target_link_libraries(pixelstorm-demo
    PRIVATE
        pixelstorm::engine
)

target_compile_features(pixelstorm-demo PRIVATE cxx_std_17)
