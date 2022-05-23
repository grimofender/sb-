# This script is written by Noah Hewitt for SMG
# SDL2_TTF_LIBRARY - the SDL2_TTF library
# SDL2_TTF_LIBRARIES - ALL libraries required to use SDL2_TTF

# If using the SDL2_TTF devel you should define these
# SDL2_PATH - defines the path to sdl2
# SDL2_PATH(ENV) - defines the path to sdl2


if (NOT APPLE)
    find_package(SDL2 QUIET)
    if (NOT SDL2_FOUND)
        if (SDL2_TTF_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find SDL2 (SDL2 is required by SDL2_TTF)")
        elseif (NOT SDL2_TTF_FIND_QUIETLY)
            message(WARNING "Could NOT find SDL2 (SDL2 is required by SDL2_TTF)")
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

find_library(SDL2_TTF_LIBRARY
    NAMES SDL2_TTF
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFFIXES ${SDL2_LIBRARY_PREFIXES}
)


find_file(SDL2_TTF_SHARED
    NAMES SDL2_TTF.dll SDL2_TTF.so SDL2_TTF.dylib
    PATHS ${SDL2_SEARCH_PATHS}
    PATH_SUFIXES ${SDL2_LIBRARY_PREFIXES}
)

if (SDL2_INCLUDE_DIR)
    file(READ "${SDL2_INCLUDE_DIR}/SDL2/SDL_TTF.h" SDL2_TTF_VERSION_HEADER)
        # Get Major version
        string(REGEX MATCH "#define SDL_TTF_MAJOR_VERSION[ ]*[0-9]+" SDL2_TTF_VERSION_MAJOR ${SDL2_TTF_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_TTF_VERSION_MAJOR ${SDL2_TTF_VERSION_MAJOR})
        # Get Minor version
        string(REGEX MATCH "#define SDL_TTF_MINOR_VERSION[ ]*[0-9]+" SDL2_TTF_VERSION_MINOR ${SDL2_TTF_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_TTF_VERSION_MINOR ${SDL2_TTF_VERSION_MINOR})
        # Get Patch version
        string(REGEX MATCH "#define SDL_TTF_PATCHLEVEL[ ]*[0-9]+" SDL2_TTF_VERSION_PATCH ${SDL2_TTF_VERSION_HEADER})
        string(REGEX MATCH "[0-9]+" SDL2_TTF_VERSION_PATCH ${SDL2_TTF_VERSION_PATCH})
    unset(SDL2_VERSION_HEADER)
    set (SDL2_TTF_VERSION "${SDL2_TTF_VERSION_MAJOR}.${SDL2_TTF_VERSION_MINOR}.${SDL2_TTF_VERSION_PATCH}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_ttf REQUIRED_VARS SDL2_TTF_LIBRARY VERSION_VAR SDL2_TTF_VERSION)

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_TTF_LIBRARY)