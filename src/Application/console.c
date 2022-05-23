
#include <stdio.h>
#include <string.h>

#define MAXCOMMAND 255

#include <Graphics/graphics.h>
#include <Runtime/Multiplayer/multiplayer.h>



#include <stdlib.h>
#include <SDL2/SDL_platform.h>

#include <Runtime/Scenes/scene.h>

extern int fps;
extern int capfps;
extern SDL_cond *cond;
extern SDL_mutex *mutex;




int console(void* data)
{
    char* input = calloc(MAXCOMMAND + 1, sizeof(char));

	char* command = calloc(MAXCOMMAND, sizeof(char));
	char* arg1 = calloc(MAXCOMMAND, sizeof(char));
	char* arg2 = calloc(MAXCOMMAND, sizeof(char));


	while (1)
	{
		while (fgets(input, 255, stdin) == NULL);





		int argcount = sscanf(input, "%s%s%s\n", command, arg1, arg2) - 1;


		if (!strcmp(command, "help"))
		{
			printf("***command list***\n");
			printf("diag                            (shows diagnostics)\n");
			printf("pushscene <sceneindex>          (pushes a scene)\n");
            printf("connect <IP address>            (connects to a server)\n");
			printf("popscene                        (pushes a scene)\n");
			printf("***command list***\n");
		} else if (!strcmp(command, "diag"))
        {
            printf("***diagnostics***\n");
			printf("fps: %d\n", fps);
			printf("CPU count: %d\n", SDL_GetCPUCount());
			printf("sys RAM count: %dmb\n", SDL_GetSystemRAM());

            SDL_Rect r;
			SDL_GetDisplayUsableBounds(SDL_GetWindowDisplayIndex(window), &r);

            SDL_DisplayMode mode = {0};
            SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);


			printf("Monitor Display: %s %dx%d %dhz\n", SDL_GetDisplayName(SDL_GetWindowDisplayIndex(window)), r.w, r.h, mode.refresh_rate);

			printf("Platform: %s\n", SDL_GetPlatform());


			printf("***diagnostics***");

        }
		else if (!strcmp(command, "pushscene"))
        {
            if (argcount < 1)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Not enough Arguments");
                goto endcommand;
            }


            int scene = SDL_atoi(arg1);
            if (SDL_GetError()[0] != 0) err(SDL_GetError());
            pushscene(scene);
        }
        else if (!strcmp(command, "popscene"))
            popscene();
        else if (!strcmp(command, "cap"))
        {
            if (argcount < 1)
            {
                printf("%d\n", capfps);
                goto endcommand;
            }

            if (sscanf(arg1, "%d", &capfps) != 1) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid Argument");
        }
        else if (!strcmp(command, "assert_test"))
        {
            if (argcount < 1)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Not enough Arguments");
                goto endcommand;
            }

            assert(0, input + strlen("assert_test "));

        }
        else
            printf("Command not recognized.");




        printf("\n");

        endcommand:


		memset(command, 0,strlen(command) + 1);
		memset(arg1, 0,strlen(arg1) + 1);
		memset(arg2, 0,strlen(arg2) + 1);


	}


	return 0;
}
