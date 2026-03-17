add_executable(pixelstorm_demo
    src/main.cpp
)

target_link_libraries(pixelstorm_demo PRIVATE pixelstorm_engine)

if (WIN32)
    target_link_options(pixelstorm_demo PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
    )
endif()
