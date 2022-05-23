#include <Runtime/Scenes/scene.h>
#include <Graphics/graphics.h>


static gamescene_t *scene;
static Uint8 top;



typedef struct regscene_t
{
     scene_setup_t setupfunc;
     scene_update_t updatefunc;
     scene_update_t drawfunc;
     scene_update_t deletefunc;
} regscene_t;

static regscene_t *reg_scenes[MAX_REGISTEREDSCENE_COUNT] = {0};

/** This Function is called when the scene changes **/
void load_scene(gamescene_t scene)
{
    if (!reg_scenes[scene])
        err("scene not registered");
    reg_scenes[scene]->setupfunc();
    return;
}

void delete_scene(gamescene_t scene)
{
    if (!reg_scenes[scene])
        err("scene not registered");
    reg_scenes[scene]->deletefunc();
    return;
}

void popscene(void)
{

    if (top <= 0)
    {
        err("Unexpected scene pop");
        return;
    }


    register gamescene_t backup = scene[top];


    scene = realloc(scene, sizeof(gamescene_t) * ((--top) + 1));

    SDL_Log("scene %d has been poped off the scene stack.", backup);
    SDL_Log("the current scene is now %d", scene[top]);



}



void pushscene(gamescene_t gamescene)
{
    if (!scene)
    {
        scene = calloc(1, sizeof(gamescene_t));
        top = 0;
        scene[top] = gamescene;
        load_scene(gamescene);
        return;
    }

    if (gamescene == gamescene_null)
        err("the null scene may not be pushed");

        for (int i = 0; i < (top + 1); i++)
        {
            if  (scene[i] == gamescene)
            {
                gotoscene(gamescene);
            }
        }



        if (((int)top + (int)1) > 255)
            err("Scene stack overflow");


        ++top;
        scene = realloc(scene, sizeof(gamescene_t) * ((top) + 1));
        scene[top] = gamescene;

    SDL_Log("scene %d has been pushed onto the scene stack.\n", scene[top]);



}


void gotoscene(gamescene_t gamescene)
{


        if (!scenepushed(gamescene))
        {
            pushscene(gamescene);
            return;
        }




        popscene();
         if (getscene() == gamescene)
            return;
        popscene();


         while (getscene() != gamescene)
         {
             delete_scene(getscene());
             popscene();
         }
}

SDL_bool scenepushed(gamescene_t gamescene)
{
    SDL_bool ret =  SDL_FALSE;

    for (int i = 0; i < (top + 1); i++)
    {
        if  (scene[i] == gamescene)
            ret = SDL_TRUE;
    }

    return ret;
}

gamescene_t getscene()/** this scene plays the starting animation **/
{
    if (!scene)
        return gamescene_null;
    else
        return scene[top];
}

int getscenestacksize(void)
{
    return top + 1;
}


void registerscene(int id, scene_setup_t setupfunc, scene_update_t updatefunc, scene_update_t drawfunc, scene_update_t deletefunc)
{
    if (id == -1)
    {
        for (int i = 0; i < MAX_REGISTEREDSCENE_COUNT; i++)
        {
            if (!reg_scenes[i])
            {
                id = i;
                break;
            }

        }
    }

    if (reg_scenes[id])
        err("scene has already been registered");

    reg_scenes[id] = calloc(1, sizeof(regscene_t));
    reg_scenes[id]->setupfunc   = setupfunc;
    reg_scenes[id]->updatefunc  = updatefunc;
    reg_scenes[id]->drawfunc    = drawfunc;
    reg_scenes[id]->deletefunc  = deletefunc;

    SDL_Log("scene %d has been registered", id);
}



void update_scene(gamescene_t scene)
{
    if (!reg_scenes[scene])
        err("scene not registered");
    reg_scenes[scene]->updatefunc();
    return;
}

void draw_scene(gamescene_t scene)
{
    if (!reg_scenes[scene])
        err("scene not registered");
    reg_scenes[scene]->drawfunc();
    return;
}


