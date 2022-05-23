#ifndef _MULTIPLAYER
#define _MULTIPLAYER

#if defined(_WIN32) && _WIN32 == 1
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define SD_RECEIVE SHUT_RD
#define SD_SEND SHUT_WR
#define SD_BOTH SHUT_RDWR

#define closesocket(sock) close( sock );



#endif // _WIN32





typedef struct player_in_t
{
    int id;


    struct
    {
        float x, y;
        float w, h;

    } rect;

    struct
    {
        float x, y;
        float w, h;

    } snowballrect;

    struct
    {
        float x, y;
        float w, h;

    } eyesrect;

    int skin;
    int frame;
    double rotation;
    double snowballrotation;


} player_in_t;

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>

static char *pack_player(player_in_t *player)
{
    char data[1000] = {0};

    sprintf(data, "%d %f %f %f %f %f %f %f %f %f %f %f %f %d %d %lf %lf",
             player->id,
             player->rect.x, player->rect.y, player->rect.w, player->rect.h,
             player->snowballrect.x, player->snowballrect.y, player->snowballrect.w, player->snowballrect.h,
             player->eyesrect.x, player->eyesrect.y, player->eyesrect.w, player->eyesrect.h,
             player->skin, player->frame, player->rotation, player->snowballrotation);


    return strdup(data);
}

static player_in_t unpack_player(char *data)
{
        player_in_t player = {0};


        sscanf(data, "%d %f %f %f %f %f %f %f %f %f %f %f %f %d %d %lf %lf",
                    &player.id,
                    &player.rect.x, &player.rect.y, &player.rect.w, &player.rect.h,
                    &player.snowballrect.x, &player.snowballrect.y, &player.snowballrect.w, &player.snowballrect.h,
                    &player.eyesrect.x, &player.eyesrect.y, &player.eyesrect.w, &player.eyesrect.h,
                    &player.skin, &player.frame, &player.rotation, &player.snowballrotation);


        return player;
}



#define MAX_PACKETSIZE 100000




/** Don't use any of this if were the server**/
#ifndef _SERVER

#include <Application/appdata.h>
#include <SDL2/SDL.h>

#define CONNECTION_ATTEMPTS 5




extern struct connection_t
{
    SOCKET sock;
    int id;
} connection;

void server_connect(const char *host, int port);





#endif // !_SERVER


#endif // _MULTIPLAYER
