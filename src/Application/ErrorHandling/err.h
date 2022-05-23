#ifndef _ERR
#define _ERR


#include <SDL2/SDL.h>




SDL_AssertState assert_handle(const SDL_AssertData* data, void* userdata);

#ifdef assert
#undef assert
#endif // assert
#define assert(condition, error) { SDL_SetAssertionHandler( assert_handle, error ); SDL_assert( condition ); }



void soft_err(const char* MSG, ...);
void err(const char* MSG, ...);


#endif
