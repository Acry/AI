//BEGIN DESCRIPTION
/*
 * Random Motion
 * Really basic use of 
 */
//END   DESCRIPTION

//BEGIN INCLUDES
#include <math.h>
#include <time.h>
//local headers
#include "helper.h"
//END   INCLUDES

//BEGIN CPP DEFINITIONS
//END   CPP DEFINITIONS

//BEGIN GLOBALS
//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
//END	FUNCTION PROTOTYPES

/* DEFINED PROGRESS GOALS
 * 
 */

//BEGIN MAIN FUNCTION
int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT
init();

SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,640,400);
SDL_SetWindowTitle(Window, "SDL 2 - Random Motion");
SDL_RenderSetLogicalSize(Renderer, 1920, 1080);

//BEGIN Load BG
SDL_Surface *temp_surface = NULL;
temp_surface 		  = IMG_Load("./earth.jpg");
SDL_Texture *Texture;
Texture 		  = SDL_CreateTextureFromSurface(Renderer, temp_surface);
//END 	Load BG

//BEGIN Player Sprite
temp_surface = IMG_Load("./sat.png");
SDL_Texture *Player_tex;
Player_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);

SDL_Rect player_dst;

player_dst.w		=200;
player_dst.h		=142;
player_dst.x		=500;
player_dst.y		=500;

int velocity_x		=0;
int velocity_y		=0;
int ship_counter_1   	=0;
int ship_threshold_1 	=25;
//END 	Player Sprite

SDL_FreeSurface(temp_surface);
srand(time(NULL)); 	// randomize seed
SDL_ShowWindow(Window);

SDL_Event event;
int running = 1;	// event loop exit flag
//END   INIT

//BEGIN MAIN LOOP
while(running){
	//BEGIN INPUT LOOP
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running = 0;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_RIGHT){
				running=0;
			}
		}
		if (event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					running = 0;
					break;
				default:
					break;
			}
		}
	}
	//END   INPUT LOOP
	
	//BEGIN RANDOM MOTION LOGIC
	if (++ship_counter_1 > ship_threshold_1){
		
		// select new direction vector
		velocity_x = -3 + rand()%6;
		velocity_y = -3 + rand()%6;
		
		// select a random number of frames to stay on new heading
		ship_threshold_1 = 50 + rand()%500;
		SDL_Log("x: %d y: %d", player_dst.x, player_dst.y);
		// reset counter
		ship_counter_1   = 0;
		
	}
	
	// move ship
	player_dst.x+=velocity_x;
	player_dst.y+=velocity_y;
	//SDL_Log("y: %d", player_dst.y);
	
	// test if ship went beyond screen edges
	if (player_dst.x >= 1920-player_dst.w)
		velocity_x = -rand()%3;//velocity_x = !velocity_x;//player_dst.x = -player_dst.w;
	else if (player_dst.x < 0)
		velocity_x = rand()%3;//!velocity_x;//player_dst.x = 1920;
		
	if ((player_dst.y+player_dst.h) >= 1080){
		//SDL_Log("velocity_y: %d", velocity_y);
		velocity_y = -rand()%3;//!velocity_y;//player_dst.y = -player_dst.h;
	} else{
		if (player_dst.y < 1)
			velocity_y = rand()%3;//velocity_y = !velocity_y;//player_dst.y = 1080;
	}
	//SDL_Log("y: %d", player_dst.y);
	//END RANDOM MOTION LOGIC
	
	
	//BEGIN RENDERING
	SDL_SetRenderDrawColor(Renderer, 70, 70, 70, 255);
	SDL_RenderClear(Renderer);
	SDL_Delay(16);
	SDL_RenderCopy(Renderer, Texture, NULL, NULL);
	SDL_RenderCopy(Renderer, Player_tex, NULL, &player_dst);
	SDL_RenderPresent(Renderer);
	//END   RENDERING
}
//END MAIN LOOP

//BEGIN CLEAN UP
SDL_DestroyTexture(Texture);
SDL_DestroyTexture(Player_tex);
exit_();
//END   CLEAN UP
return EXIT_SUCCESS;
	
}
//END   MAIN FUNCTION


//BEGIN FUNCTIONS
//END   FUNCTIONS
