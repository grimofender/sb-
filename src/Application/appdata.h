#ifndef APP_DATA
#define APP_DATA



#include <SDL2/SDL_version.h>
const static SDL_version appversion = {
    1,  /** major version **/
    0,  /** minor version **/
    0,   /** patch version **/
};

#ifndef DEBUG
#define DEBUG 1
#endif // !DEBUG

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 25322
#endif // !DEFAULT_PORT

/** Defined so the assert system reports everything**/
#define SDL_ASSERT_LEVEL 3

/** Make sure were on Windows 7 or higher **/
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <SDL2/SDL_assert.h>
#include <Application/ErrorHandling/err.h>


#endif // APP_DATA
