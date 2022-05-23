#ifndef _GAMESCENE
#define _GAMESCENE

#include <SDL2/SDL.h>


typedef enum
{
    gamescene_null = 0,       /** an empty scene witch is the default if no scene has been pushed **/

    gamescene_startanim,      /** this scene plays the starting animation **/
    gamescene_startmenu,      /** this is the scene where the main menu is **/
    gamescene_arena,          /** This is where the game plays out **/
    gamescene_settingsmenu,   /** this is where the you can change settings in the game **/
    gamescene_gameover,       /** this is what scene you switch to after you die **/

    gamescene_server, /** this scene is used for servers **/
    gamescene_error, /** this scene is pushed when a soft error is made **/

} gamescene_t;



/** \brief Pushes a Scene onto the scene stack
 *
 * \param scene The scene you would like to push
 *
 * \sa popscene()
 * \sa getscene()
 **/
void pushscene(gamescene_t scene);


/** \brief pops the scene off the scene stack
 *
 * \sa pushscene()
 * \sa getscene()
 **/
void popscene(void);

/** \brief Gets the current scene
 *
 * \sa pushscene()
 * \sa popscene()
 **/
gamescene_t getscene(void);

/** \brief Gets the current scene stack size
 *
 * \sa pushscene()
 * \sa popscene()
 **/
int getscenestacksize(void);

/** \brief Goes to the scene. popping and pushing scenes if neccessary
 *
 * \sa pushscene()
 * \sa popscene()
 **/
void gotoscene(gamescene_t gamescene);

/** \brief checks if scene has been pushed.
 *
 * \sa pushscene()
 * \sa popscene()
 **/
SDL_bool scenepushed(gamescene_t gamescene);



typedef void (*scene_setup_t)(void);
typedef void (*scene_update_t)(void);
typedef void (*scene_draw_t)(void);
typedef void (*scene_delete_t)(void);

/** \brief registers a scene.
 * 
 * \param id the id of the scene you would like to register
 * \param setupfunc function called when the scene is loaded
 * \param updatefunc function called when the scene should be updated
 * \param drawfunc function called when the scene should be drawn
 * \param deletefunc function called when the scene is deleted
 **/
void registerscene(int id, scene_setup_t setupfunc, scene_update_t updatefunc, scene_update_t drawfunc, scene_update_t deletefunc);



#define MAX_REGISTEREDSCENE_COUNT 256




#endif
