
#include <Runtime/Multiplayer/multiplayer.h>

#include <Runtime/Scenes/scene.h>


struct connection_t connection;

void server_connect(const char *host, int port)
{

    if (getscene() == gamescene_server)
        err("Already Connected");




    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);




    for (int i = 0; i < CONNECTION_ATTEMPTS; i++)
    {
        if (i != 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "connection attempt %d failed", i);
            SDL_Delay(1000);
        }


        SDL_Log("starting connection attempt %d", i + 1);
        struct hostent *h = gethostbyname(host);

        if (!h)
        {

            SDL_LogError(0, "Could Not Resolve host");

            continue;
        }
        memcpy(&addr.sin_addr, h->h_addr, h->h_length);






        connection.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connection.sock == INVALID_SOCKET) err("Could not create Socket");

        if (connect(connection.sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        {
            closesocket(connection.sock);

            memset(&connection, 0, sizeof(connection));
            connection.sock = INVALID_SOCKET;

            SDL_LogError(0, "Connection refused");

            continue;
        }
        break;
    }


    if (connection.sock == INVALID_SOCKET)
    {
        SDL_LogError(0, "All connection attempt failed");
        soft_err("Could not connect to host %s:%d", host, port);
        return;
    }



    SDL_Log("Successfully connected to %s on port %d", host, port);
    pushscene(gamescene_server);





}


void server_disconnect(void)
{
    if (getscene() != gamescene_server)
        err("Not connected to anything");

    popscene();

}

#include <Runtime/Player/Player.h>
#include <stdlib.h>



player_in_t *inplayers;
int playercount;




playerinput_t lastinput;
player_t *myplayer;
extern int playerskin;



extern SDL_Cursor *cursor;

void setup_server(void)
{

    if (connection.sock == INVALID_SOCKET)
        err("Attempted to push the server scene manually");


    SDL_Surface *surface = loadbmp("data/sprites/crosshair.bmp");
    if (cursor)
        SDL_FreeCursor(cursor);
    cursor = SDL_CreateColorCursor(surface, (surface->w / 2), (surface->h / 2));
    SDL_FreeSurface(surface);
    SDL_SetCursor(cursor);

    alloc_player(0,0, 0, &myplayer);



    inplayers = 0;
    playercount = 0;




    Mix_FadeOutMusic(2000);

}



void update_server(void)
{
        fd_set fdset;
        struct timeval tv = {0};
        tv.tv_usec = 1;
        FD_ZERO(&fdset);
        FD_SET(connection.sock, &fdset);
        select(0, &fdset, 0,0, &tv);

        if (FD_ISSET(connection.sock, &fdset))
        {
            char packet[MAX_PACKETSIZE];
            int packetsize;


            packetsize = recv(connection.sock, packet, MAX_PACKETSIZE, 0);
            if (!packetsize || packetsize == SOCKET_ERROR)
            {
                /**Kicked from server or server closed**/
                popscene();
                soft_err("Connection Closed");
                return;
            }



            player_in_t p = unpack_player(packet);

            int index = -1;

            for (int i = 0; i < playercount; i++)
            {
                if (p.id == inplayers[i].id)
                {
                    index = i;
                    break;
                }
            }



            if (index == -1)
            {
                playercount++;
                realloc(inplayers,  sizeof(player_in_t) * playercount);
                inplayers[playercount - 1] = p;
            }
            else
            {
                inplayers[index] = p;
            }


        }






        playerinput_t input = {0};
        get_playerinput(myplayer, inputtype_keyboard_and_mouse, 0, &input);
        update_player(&input, myplayer);



        SDL_bool sendpacket = SDL_FALSE;


        /** Out input changed **/

        SDL_bool inputchanged;


            if (input.horisontalaxis != lastinput.horisontalaxis)
                inputchanged = SDL_TRUE;
            if (input.verticalaxis != lastinput.verticalaxis)
                inputchanged = SDL_TRUE;

            if (input.shootingdir.x != lastinput.shootingdir.x)
                inputchanged = SDL_TRUE;
            if (input.shootingdir.y != lastinput.shootingdir.y)
                inputchanged = SDL_TRUE;

            if (input.shooting != lastinput.shooting)
                inputchanged = SDL_TRUE;



        /** if input changed **/
        if (inputchanged)
        {
            lastinput = input;
            sendpacket = SDL_TRUE;
        }


        if (sendpacket)
        {
            player_in_t p; get_playerin(myplayer, &p);
            char *packet = pack_player(&p);
            send(connection.sock, packet, strlen(packet) + 1, 0);
            free(packet);
        }




}

void draw_server(void)
{
    for (int i = 0; i < playercount; i++)
    {

            player_in_t player = inplayers[i];
            playerskin_t *skin = playerskins[player.skin];


            rect_t  r = {player.rect.x, player.rect.y, player.rect.w, player.rect.h};
            rect_t  sr = {player.snowballrect.x, player.snowballrect.y, player.snowballrect.w, player.snowballrect.h};
            rect_t  er = {player.eyesrect.x, player.eyesrect.y, player.eyesrect.w, player.eyesrect.h};


            drawtexture(skin->player[player.frame], &r, player.rotation);
            drawtexture(skin->eyes, &er, 0);
            drawtexture(skin->snowball, &sr, 0);

    }


    draw_player(myplayer);
}

void delete_server(void)
{
    closesocket(connection.sock);

    memset(&connection, 0, sizeof(connection));
    connection.sock = INVALID_SOCKET;


    delete_player(myplayer);

    if (inplayers)
        free(inplayers);

    inplayers = 0;
    playercount = 0;


    SDL_Log("Disconnected from current server");

}


