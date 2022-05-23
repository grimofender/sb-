#ifndef _Player_
#define _Player_

#include <Graphics/graphics.h>
#include <Runtime/Multiplayer/multiplayer.h>

#define PLAYER_MAXFRAMES 3




typedef struct playerskin_t
{

	SDL_Texture *player[PLAYER_MAXFRAMES];
	SDL_Texture *eyes;
	SDL_Texture *snowball;


}  playerskin_t;


typedef struct snowball_t
{
	union
	{
		struct {

			vector2_t pos;
			vector2_t scale;
		};

		rect_t  rect;
	};


	double angle;
	double rotation;
	vector2_t velocity;


	SDL_bool shot;
} snowball_t;

typedef struct player_t
{


	int skinid;
	union
	{
		struct {

			vector2_t pos;
			vector2_t scale;
		};

		rect_t  rect;
	};

	int frame;
	snowball_t snowball;

	union
	{
		struct {

			vector2_t pos;
			vector2_t scale;
		};

		rect_t  rect;
	} eyes;

	struct
	{
	   SDL_bool dying;
	   vector2_t vel;
	   double deathrotation;
	   SDL_bool dead;
	} death;



	int frametimer;


	double rotation;
} player_t;

typedef enum inputtype_t
{
   inputtype_keyboard_and_mouse = 0,
   inputtype_controller

} inputtype_t;

typedef struct playerinput_t
{
    vector2_t shootingdir;

    float verticalaxis;
    float horisontalaxis;

    SDL_bool shooting;

} playerinput_t;




/** \brief Gets input for player
 *
 * \param player the player you want to get input for
 * \param inputtype type of input
 * \param joystick joystick
 * \param haptic haptic
 * \param input the input
 *
 * \sa update_player
 **/
void get_playerinput(player_t *player, inputtype_t inputtype, SDL_Joystick *joystick, playerinput_t *input);

/**
    Macros
**/

#define PLAYERDEFAULT_WIDTH (14 * 15.3)
#define PLAYERDEFAULT_HEIGHT (20 * 15.3)

#define PLAYER_EYESRADIUS  28
#define PLAYER_EYESDEFAULT_WIDTH  (5 * 15)
#define PLAYER_EYESDEFAULT_HEIGHT (3 * 15)


#define SNOWBALLDEFAULT_WIDTH  (8 * 9)
#define SNOWBALLDEFAULT_HEIGHT (8 * 9)
#define PLAYERSPEED 0.54
#define SNOWBALLSPEED 1.2
#define SNOWBALLRADIUS 219



#define PLAYERSKIN_COUNT 4

/**
    Externals
**/

extern playerskin_t *playerskins[PLAYERSKIN_COUNT];




/**
    Convinence functions
**/

/** \brief Updates a player
 *
 * \param input  The input of the player
 * \param player The player you want update
 *
 * \sa get_playerinput
 * \sa draw_player
 **/

void update_player(playerinput_t *input, player_t* player);

/** \brief Draws a player onto the screen
 *
 * \param player The player you want draw
 *
 * \sa update_player
 **/
void draw_player(player_t *player);

/** \brief murders a player like a psychopath.
 *
 * \param player The player you want kill
 * \param player The velocity you would like to throw him
 *
 * \sa update_player
 * \sa draw_player
 **/
void kill_player(player_t *player, vector2_t vel);

/** \brief gets the internet version of a player
 *
 * \param player The player you want to get.
 *
 * \sa alloc_player
 **/
void get_playerin(player_t *player, player_in_t *playerin);

/**
    Allocation functions
**/

/** NOTE: I amt no going to describe these because there boring. **/

void alloc_player(int x, int y, int skinid, player_t **player);
void alloc_playerskin(const char* skinname, playerskin_t * *skin);

void delete_player(player_t *player);
void delete_playerskin(playerskin_t *skin);

/**

This function registers a player so that other game objects will treat them as players.

**/


#endif
