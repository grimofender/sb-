cmake_minimum_required(VERSION 3.0)
project(sb+ VERSION 1.0.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
file(GLOB_RECURSE SRC "src/*.c" "src/*.rc")

include_directories("src/")
configure_file("${CMAKE_SOURCE_DIR}/src/Application/appdata.h.in" "${CMAKE_SOURCE_DIR}/src/Application/appdata.h")

if (CMAKE_SYSTEM_NAME EQUAL "Emscripten")
    set ( USE_FLAGS "-s USE_SDL=2")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}" CACHE STRING "" )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}" CACHE STRING "" )
    set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} " CACHE STRING "" )
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(sbplus ${SRC}) 

set(DEPENDENCIES "${CMAKE_CURRENT_SOURCE_DIR}/src/resources")
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    include_directories()
    add_compile_definitions(BUILDING_FOR_WEB)
    set(CMAKE_EXE_LINKER_FLAGS " -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 --preload-file bin/data@data --preload-file /bin/Icon.bmp@Icon.bmp")
    set(DEPENDENCIES
        ${DEPENDENCIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/index.html
    )
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    target_link_libraries(sbplus
        winmm
        ws2_32
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_LIBRARIES}
    )
    set(DEPENDENCIES
        ${DEPENDENCIES}
        ${SDL2_MIXER_SHARED}
        ${SDL2_TTF_SHARED}
        ${SDL2_SHARED}
    )
endif()






foreach(DEPENDENCY ${DEPENDENCIES})
    get_target_property(OUTPUT_DIR sbplus RUNTIME_OUTPUT_DIRECTORY)
    if (EXISTS ${DEPENDENCY})
        if (IS_DIRECTORY ${DEPENDENCY})
            add_custom_command(TARGET sbplus PRE_BUILD 
            COMMAND cmake -E copy_directory ${DEPENDENCY} "$<TARGET_FILE_DIR:sbplus>"
            )
        else()
            add_custom_command(TARGET sbplus PRE_BUILD 
            COMMAND cmake -E copy ${DEPENDENCY} "$<TARGET_FILE_DIR:sbplus>"
            )
        endif()
    else ()
        message (WARNING "Could not copy the contents of ${DEPENDENCY}. \n this won't effect building though.")
    endif()
endforeach()