#ifndef _GRAPHICS
#define _GRAPHICS

#include <Application/appdata.h>


#include <SDL2/SDL_ttf.h>

#ifdef _WIN32S
#include <windows.h>
#endif // _WIN32S


#include <SDL2/SDL_opengl.h>

#include <Application/ErrorHandling/err.h>

#define WINDOW_WIDTH  (1000)
#define WINDOW_HEIGHT (920)


#define DEFAULTRESELUTION_WIDTH  (2250)
#define DEFAULTRESELUTION_HEIGHT (2070)



#define min(x, y) ((y)<(x)? (y) : (x))
#define max(x, y) ((x)>(y)? (x) : (y))

typedef struct vector2_t
{
    float x, y;
} vector2_t;



typedef struct rect_t
{
    float x, y;
    float w, h;
} rect_t;

extern int reselution_width;
extern int reselution_height;


extern long double deltatime;


extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *rendertarget;

void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void center(rect_t  *rt, vector2_t* po);

inline static vector2_t middle(vector2_t *point,rect_t * rect)
{

    point->x = (rect->x + (rect->w / 2));
	point->y = (rect->y + (rect->h / 2));
}


void getrightcorner(vector2_t* pt, rect_t  *rt);

SDL_Surface *loadbmp(const char* file);

static const SDL_Color transperent = {255, 191, 221, 0};

void getmousepos(float *x, float *y);

SDL_bool hasintersection(rect_t *r1, rect_t *r2);

void drawtexture(SDL_Texture *tex, rect_t  *r, const double angle);

SDL_Texture *loadtexture(const char* file);





void draw_text(float x, float y, char* text, TTF_Font *font, SDL_Color color, SDL_bool centered);



#endif
