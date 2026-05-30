file(GLOB_RECURSE DEMO_SCENE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scenes/*.cpp
)

add_executable(pixelstorm_demo
    src/main.cpp
    ${DEMO_SCENE_SOURCES}
)

target_link_libraries(pixelstorm_demo PRIVATE pixelstorm_engine)

add_custom_command(TARGET pixelstorm_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/engine/assets
            $<TARGET_FILE_DIR:pixelstorm_demo>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/demo/assets
            $<TARGET_FILE_DIR:pixelstorm_demo>/assets
)

if (WIN32)
    target_link_options(pixelstorm_demo PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
    )
endif()
