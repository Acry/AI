#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION
/*
 * Simple Tracking Techniques
 * Chasing and Evasion
 * 
 */
#pragma endregion DESCRIPTION

#pragma region INCLUDES
#include <math.h>
//local headers
#include "helper.h"
#pragma endregion INCLUDES

#pragma region CPP DEFINITIONS
#define LogicalSizeH 1920
#define LogicalSizeV 1080

#define TEXT_COLOR 106, 87, 87, 255
#pragma endregion CPP DEFINITIONS

#pragma region GLOBALS
#pragma endregion GLOBALS

#pragma region FUNCTION PROTOTYPES
#pragma endregion FUNCTION PROTOTYPES

/* DEFINED PROGRESS GOALS
 * 
 * Render Background
 * Set vitual render size
 * render player
 * render monster
 * Player control
 * tracking logic
 * Render Status Text
 * 
 */

/* ANNOTATION
 * rotation/facing will be done in a seprate demo. That isn't an AI-Problem
 */

#pragma endregion HEAD

#pragma region MAIN FUNCTION
int main(int argc, char *argv[])
{

	(void)argc;
	(void)argv;

#pragma region INIT
	init();

	SDL_SetWindowSize(Window, 640, 400);
	SDL_SetWindowTitle(Window, "SDL 2 - Tracking");
	SDL_RenderSetLogicalSize(Renderer, LogicalSizeH, LogicalSizeV);

	SDL_Surface *temp_surface = NULL;

#pragma region TEXT STATUS
	char buffer[64]; // used to print strings
	TTF_Font *Font = NULL;

	Font = TTF_OpenFont("FunSized.ttf", 60);
	sprintf(buffer, "Zombie is chasing! Space to switch");

	SDL_Color text_color = {TEXT_COLOR};

	temp_surface = TTF_RenderText_Blended(Font, buffer, text_color);
	SDL_Texture *chase_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_Rect chase_dst;
	SDL_QueryTexture(chase_tex, NULL, NULL, &chase_dst.w, &chase_dst.h);

	chase_dst.x = LogicalSizeH / 2 - chase_dst.w / 2;
	chase_dst.y = 900;

	sprintf(buffer, "Zombie is evading! Space to switch");
	temp_surface = TTF_RenderText_Blended(Font, buffer, text_color);
	SDL_Texture *evade_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_Rect evade_dst;
	SDL_QueryTexture(evade_tex, NULL, NULL, &evade_dst.w, &evade_dst.h);
	evade_dst.x = LogicalSizeH / 2 - evade_dst.w / 2;
	evade_dst.y = 900;

	TTF_CloseFont(Font);
#pragma endregion TEXT STATUS

#pragma region Load BG
	temp_surface = IMG_Load("bg.jpg");
	SDL_Texture *Texture;
	Texture = SDL_CreateTextureFromSurface(Renderer, temp_surface);
#pragma endregion Load BG

#pragma region Player
	temp_surface = IMG_Load("./player.png");
	SDL_Texture *Player_tex;
	Player_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_Rect player_dst;
	SDL_QueryTexture(Player_tex, NULL, NULL, &player_dst.w, &player_dst.h);
	player_dst.x = LogicalSizeH / 2 - player_dst.w / 2;
	player_dst.y = 500;
#pragma endregion Player

#pragma region Monster
	temp_surface = IMG_Load("zombie.png");
	SDL_SetColorKey(temp_surface,
									SDL_TRUE,
									SDL_MapRGB(temp_surface->format, 0, 0, 0));
	SDL_Texture *Monster_tex;
	Monster_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_Rect monster_dst;
	SDL_QueryTexture(Monster_tex, NULL, NULL, &monster_dst.w, &monster_dst.h);
	monster_dst.x = 0;
	monster_dst.y = 500;
#pragma endregion Monster
	SDL_FreeSurface(temp_surface);
	int chase = 1; // used to select creatures mode of operation, 1=chase, 0=evade
	SDL_ShowWindow(Window);
	SDL_Event event;
	int running = 1; // event loop exit flag
#pragma endregion INIT

#pragma region MAIN LOOP
	while (running)
	{

		Uint8 *keys = (Uint8 *)SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_ESCAPE])
			running = 0;
		if (keys[SDL_SCANCODE_LEFT])
			player_dst.x -= 8;
		if (keys[SDL_SCANCODE_RIGHT])
			player_dst.x += 8;
		if (keys[SDL_SCANCODE_UP])
			player_dst.y -= 8;
		if (keys[SDL_SCANCODE_DOWN])
			player_dst.y += 8;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					chase = chase ^ 1;
					break;
				default:
					break;
				}
			}
		}

#pragma region TRACKING LOGIC
		if (chase == 1)
		{
			// track on x coordinate
			if (player_dst.x >= monster_dst.x)
				monster_dst.x++;
			else if (player_dst.x <= monster_dst.x)
				monster_dst.x--;

			// now track on y coordinate

			if (player_dst.y >= monster_dst.y)
				monster_dst.y++;
			else if (player_dst.y <= monster_dst.y)
				monster_dst.y--;
		}
		else
		{
			// must be evading
			// evade on x coordinate
			if (player_dst.x <= monster_dst.x)
				monster_dst.x++;
			else if (player_dst.x >= monster_dst.x)
				monster_dst.x--;

			// now evade on y coordinate
			if (player_dst.y <= monster_dst.y)
				monster_dst.y++;
			else if (player_dst.y >= monster_dst.y)
				monster_dst.y--;
		}

		// check bounds monster
		if (monster_dst.x >= LogicalSizeH - monster_dst.w)
			monster_dst.x = LogicalSizeH - monster_dst.w;
		else if (monster_dst.x <= 0)
			monster_dst.x = 0;

		if (monster_dst.y >= LogicalSizeV - monster_dst.h)
			monster_dst.y = LogicalSizeV - monster_dst.h;
		else if (monster_dst.y <= 0)
			monster_dst.y = 0;

		// check bounds player
		if (player_dst.x >= LogicalSizeH - player_dst.w)
			player_dst.x = LogicalSizeH - player_dst.w;
		else if (player_dst.x <= 0)
			player_dst.x = 0;

		if (player_dst.y >= LogicalSizeV - player_dst.h)
			player_dst.y = LogicalSizeV - player_dst.h;
		else if (player_dst.y <= 0)
			player_dst.y = 0;
#pragma endregion TRACKING LOGIC

#pragma region RENDERING
		SDL_SetRenderDrawColor(Renderer, 70, 70, 70, 255);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, Texture, NULL, NULL);

		if (chase)
			SDL_RenderCopy(Renderer, chase_tex, NULL, &chase_dst);
		else
			SDL_RenderCopy(Renderer, evade_tex, NULL, &evade_dst);

		SDL_RenderCopy(Renderer, Player_tex, NULL, &player_dst);
		SDL_RenderCopy(Renderer, Monster_tex, NULL, &monster_dst);
		SDL_RenderPresent(Renderer);
#pragma endregion RENDERING
	}
#pragma endregion MAIN LOOP

#pragma region CLEAN UP
	SDL_DestroyTexture(Texture);
	SDL_DestroyTexture(chase_tex);
	SDL_DestroyTexture(evade_tex);
	SDL_DestroyTexture(Player_tex);
	SDL_DestroyTexture(Monster_tex);

	exit_();
#pragma endregion CLEAN UP
	return EXIT_SUCCESS;
}
#pragma endregion MAIN FUNCTION

#pragma region FUNCTIONS
#pragma endregion FUNCTIONS
