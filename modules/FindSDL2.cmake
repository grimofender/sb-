# This script is written by Noah Hewitt for SMG
# SDL2_INCLUDE_DIR - include dirs for SDL2
# SDL2_LIBRARY - the SDL2 library
# SDL2_MAIN_LIBRARY - the SDL2main library
# SDL2_LIBRARIES - ALL libraries required to use SDL2
# SDL2_SHARED - The SDL2 shared library

# If using the SDL2 devel you should define these
# SDL2_PATH - defines the path to sdl2
# SDL2_PATH(ENV) - defines the path to sdl2

if (NOT APPLE)
    find_package(Threads QUIET)
    if (NOT THREADS_FOUND)
        if (SDL2_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find Threads (Threads is required by SDL2)")
        elseif (NOT SDL2_FIND_QUIETLY)
            message(WARNING "Could NOT find Threads (Threads is required by SDL2)")
        endif()
    endif ()
endif ()

# Set search Paths
SET( SDL2_SEARCH_PATHS
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt
    ${SDL2_PATH}
    $ENV{SDL2_PATH}
)

set (SDL2_INCLUDE_PREFIXES
    include/SDL2 
    include
)

set (SDL2_LIBRARY_PREFIXES
    lib
    bin
)

if (WIN32)
    if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        list(APPEND SDL2_LIBRARY_PREFIXES lib/x64)
    elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        list(APPEND SDL2_LIBRARY_PREFIXES lib/x86)
    endif()
endif()

find_path(SDL2_INCLUDE_DIR SDL2/SDL.h
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFFIXES ${SDL2_INCLUDE_PREFIXES}
)

find_library(SDL2_LIBRARY
    NAMES SDL2
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFFIXES ${SDL2_LIBRARY_PREFIXES}
)

find_library(SDL2_MAIN_LIBRARY
    NAMES SDL2main
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFFIXES ${SDL2_LIBRARY_PREFIXES}
)


find_file(SDL2_SHARED
    NAMES SDL2.dll SDL2.so SDL2.dylib
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFIXES ${SDL2_LIBRARY_PREFIXES}
)

if (SDL2_INCLUDE_DIR)
    file(READ "${SDL2_INCLUDE_DIR}/SDL2/SDL_version.h" SDL2_VERSION_HEADER)
        # Get Major version
        string(REGEX MATCH "#define SDL_MAJOR_VERSION[ ]*[0-9]+" SDL2_VERSION_MAJOR ${SDL2_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_VERSION_MAJOR ${SDL2_VERSION_MAJOR})
        # Get Minor version
        string(REGEX MATCH "#define SDL_MINOR_VERSION[ ]*[0-9]+" SDL2_VERSION_MINOR ${SDL2_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_VERSION_MINOR ${SDL2_VERSION_MINOR})
        # Get Patch version
        string(REGEX MATCH "#define SDL_PATCHLEVEL[ ]*[0-9]+" SDL2_VERSION_PATCH ${SDL2_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_VERSION_PATCH ${SDL2_VERSION_PATCH})
    unset(SDL2_VERSION_HEADER)
    set (SDL2_VERSION "${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_SHARED SDL2_LIBRARY SDL2_INCLUDE_DIR VERSION_VAR SDL2_VERSION)

if (SDL2_FOUND)
    set(SDL2_LIBRARIES ${SDL2_MAIN_LIBRARY} ${SDL2_LIBRARY})
    if (MINGW)
        set(SDL2_LIBRARIES -lmingw32 ${SDL2_LIBRARIES})
    elseif (UNIX AND NOT APPLE)
        set(SDL2_LIBRARIES ${SDL2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    elseif (APPLE)
        set(SDL2_LIBRARIES ${SDL2_LIBRARIES} "-framework Cocoa")
    endif()
endif()

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2_MAIN_LIBRARY)