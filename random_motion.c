#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region DESCRIPTION
/*
 * Random Motion
 * Really basic use of SDL2 and stdlib's rand function.
 * Use of SDL2's RenderSetLogicalSize - resize the window
 * 
 */
#pragma endregion DESCRIPTION

#pragma region INCLUDES
#include <math.h>
#include <time.h>
//local headers
#include "helper.h"
#pragma endregion INCLUDES

#pragma region CPP DEFINITIONS
#pragma endregion CPP DEFINITIONS

#pragma region GLOBALS
#pragma endregion GLOBALS

#pragma region FUNCTION PROTOTYPES
#pragma endregion FUNCTION PROTOTYPES

/* DEFINED PROGRESS GOALS
 * 
 */

#pragma region MAIN FUNCTION
int main(int argc, char *argv[])
{

	(void)argc;
	(void)argv;

#pragma region INIT
	init();

	SDL_SetWindowSize(Window, 640, 400);
	SDL_SetWindowTitle(Window, "SDL 2 - Random Motion");
	SDL_RenderSetLogicalSize(Renderer, 1920, 1080);

#pragma region Load BG
	SDL_Surface *temp_surface = NULL;
	temp_surface = IMG_Load("./earth.jpg");
	SDL_Texture *Texture;
	Texture = SDL_CreateTextureFromSurface(Renderer, temp_surface);
#pragma endregion Load BG

#pragma region Player Sprite
	temp_surface = IMG_Load("./sat.png");
	SDL_Texture *Player_tex;
	Player_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	SDL_Rect player_dst;

	player_dst.w = 200;
	player_dst.h = 142;
	player_dst.x = 500;
	player_dst.y = 500;

	int velocity_x = 0;
	int velocity_y = 0;
	int ship_counter_1 = 0;
	int ship_threshold_1 = 25;
#pragma endregion Player Sprite

	SDL_FreeSurface(temp_surface);
	srand(time(NULL)); // randomize seed
	SDL_ShowWindow(Window);

	SDL_Event event;
	int running = 1; // event loop exit flag
#pragma endregion INIT

#pragma region MAIN LOOP
	while (running)
	{
#pragma region INPUT LOOP
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					running = 0;
				}
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = 0;
					break;
				default:
					break;
				}
			}
		}
#pragma endregion INPUT LOOP

#pragma region RANDOM MOTION LOGIC
		if (++ship_counter_1 > ship_threshold_1)
		{

			// select new direction vector
			velocity_x = -3 + rand() % 6;
			velocity_y = -3 + rand() % 6;

			// select a random number of frames to stay on new heading
			ship_threshold_1 = 50 + rand() % 500;
			SDL_Log("x: %d y: %d", player_dst.x, player_dst.y);
			// reset counter
			ship_counter_1 = 0;
		}

		// move ship
		player_dst.x += velocity_x;
		player_dst.y += velocity_y;
		//SDL_Log("y: %d", player_dst.y);

		// test if ship went beyond screen edges
		if (player_dst.x >= 1920 - player_dst.w)
			velocity_x = -rand() % 3; //velocity_x = !velocity_x;//player_dst.x = -player_dst.w;
		else if (player_dst.x < 0)
			velocity_x = rand() % 3; //!velocity_x;//player_dst.x = 1920;

		if ((player_dst.y + player_dst.h) >= 1080)
		{
			//SDL_Log("velocity_y: %d", velocity_y);
			velocity_y = -rand() % 3; //!velocity_y;//player_dst.y = -player_dst.h;
		}
		else
		{
			if (player_dst.y < 1)
				velocity_y = rand() % 3; //velocity_y = !velocity_y;//player_dst.y = 1080;
		}
#pragma endregion RANDOM MOTION LOGIC

#pragma region RENDERING
		SDL_RenderCopy(Renderer, Texture, NULL, NULL);
		SDL_RenderCopy(Renderer, Player_tex, NULL, &player_dst);
		SDL_RenderPresent(Renderer);
#pragma endregion RENDERING
	}
#pragma endregion MAIN LOOP

#pragma region CLEAN UP
	SDL_DestroyTexture(Texture);
	SDL_DestroyTexture(Player_tex);
	exit_();
#pragma endregion CLEAN UP
	return EXIT_SUCCESS;
}
#pragma endregion MAIN FUNCTION

#pragma region FUNCTIONS
#pragma endregion FUNCTIONS
