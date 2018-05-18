//BEGIN HEAD
//BEGIN DESCRIPTION
/*
 * 
 * Simple Tracking Techniques
 * Chasing and Evasion
 * 
 */
//END   DESCRIPTION

//BEGIN INCLUDES
#include <math.h>
//local headers
#include "helper.h"
//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define LogicalSizeH 1920
#define LogicalSizeV 1080

#define TEXT_COLOR 106, 87, 87, 255
//END   CPP DEFINITIONS

//BEGIN GLOBALS
//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
//END	FUNCTION PROTOTYPES

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

//END 	HEAD

//BEGIN MAIN FUNCTION
int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT
init();

SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,640,400);
SDL_SetWindowTitle(Window, "SDL 2 - Tracking");
SDL_RenderSetLogicalSize(Renderer, LogicalSizeH, LogicalSizeV);

SDL_Surface *temp_surface = NULL;

//BEGIN TEXT STATUS
char 		buffer[64]; // used to print srtrings
TTF_Font       *Font       = NULL;

Font = TTF_OpenFont("FunSized.ttf", 60);
sprintf(buffer,"Zombie is chasing! Space to switch");

SDL_Color 	text_color = {TEXT_COLOR};

temp_surface = TTF_RenderText_Blended(Font, buffer, text_color);
SDL_Texture *chase_tex=SDL_CreateTextureFromSurface(Renderer, temp_surface);
SDL_Rect chase_dst;
SDL_QueryTexture(chase_tex, NULL, NULL, &chase_dst.w, &chase_dst.h);

chase_dst.x=LogicalSizeH/2-chase_dst.w/2;
chase_dst.y=900;

sprintf(buffer,"Zombie is evading! Space to switch");
temp_surface = TTF_RenderText_Blended(Font, buffer, text_color);
SDL_Texture *evade_tex=SDL_CreateTextureFromSurface(Renderer, temp_surface);
SDL_Rect evade_dst;
SDL_QueryTexture(evade_tex, NULL, NULL, &evade_dst.w, &evade_dst.h);
evade_dst.x=LogicalSizeH/2-evade_dst.w/2;
evade_dst.y=900;

TTF_CloseFont(Font);
//END 	TEXT STATUS

//BEGIN Load BG
temp_surface = IMG_Load("bg.jpg");
SDL_Texture *Texture;
Texture = SDL_CreateTextureFromSurface(Renderer, temp_surface);
//END Load BG

//BEGIN Player
temp_surface = IMG_Load("./player.png");
SDL_Texture *Player_tex;
Player_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
SDL_Rect player_dst;
SDL_QueryTexture(Player_tex, NULL, NULL, &player_dst.w, &player_dst.h);
player_dst.x=LogicalSizeH/2-player_dst.w/2;
player_dst.y=500;
//END Player

//BEGIN Monster
temp_surface = IMG_Load("zombie.png");
SDL_SetColorKey(temp_surface,
		SDL_TRUE,
		SDL_MapRGB( temp_surface->format, 0, 0, 0 ));
SDL_Texture 	*Monster_tex;
Monster_tex = SDL_CreateTextureFromSurface(Renderer, temp_surface);
SDL_Rect monster_dst;
SDL_QueryTexture(Monster_tex, NULL, NULL, &monster_dst.w, &monster_dst.h);
monster_dst.x=0;
monster_dst.y=500;
//END Monster
SDL_FreeSurface(temp_surface);
int chase = 1; 	// used to select creatures mode of operation, 1=chase, 0=evade
SDL_ShowWindow(Window);
SDL_Event event;
int running = 1;// event loop exit flag
//END   INIT

//BEGIN MAIN LOOP
while(running){
	//BEGIN INPUT LOOP
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			running = 0;
		if(event.type == SDL_MOUSEBUTTONDOWN)
			if(event.button.button == SDL_BUTTON_RIGHT)
				running=0;
		if (event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_UP:
					if ((player_dst.y-=16) < -player_dst.h)
						player_dst.y=LogicalSizeV+player_dst.h;
					break;
				case SDLK_DOWN:
					if ((player_dst.y+=16)>LogicalSizeV)
						player_dst.y=-player_dst.h;
					break;
				case SDLK_RIGHT:
					if ((player_dst.x+=16)>LogicalSizeH)
						player_dst.x=-player_dst.w;
					break;
				case SDLK_LEFT:
					if ((player_dst.x-=16)<-player_dst.w)
						player_dst.x=LogicalSizeH;
					break;
				case SDLK_ESCAPE:
					running = 0;
					break;
				case SDLK_SPACE:
					chase=chase^1;
					break;
				default:
					break;
			}
		}
	}
	//END   INPUT LOOP
	
	//BEGIN TRACKING LOGIC
	if (chase==1){
		// track on x coordinate
		if (player_dst.x > monster_dst.x)
			monster_dst.x++;
		else
		if (player_dst.x < monster_dst.x)
			monster_dst.x--;
			
			// now track on y coordinate
			
		if (player_dst.y > monster_dst.y)
			monster_dst.y++;
		else
		if (player_dst.y < monster_dst.y)
			monster_dst.y--;
				
	} else{
		// must be evading
		// evade on x coordinate
		if (player_dst.x < monster_dst.x)
			monster_dst.x++;
		else
		if (player_dst.x > monster_dst.x)
			monster_dst.x--;
			
		// now evade on y coordinate
		if (player_dst.y < monster_dst.y)
			monster_dst.y++;
		else
		if (player_dst.y > monster_dst.y)
			monster_dst.y--;
				
	} // end else
	
	// test of creature has moved off screen
	if (monster_dst.x>LogicalSizeH-monster_dst.w)
		monster_dst.x = 920-monster_dst.w;
	else
	if (monster_dst.x<-monster_dst.w)
		monster_dst.x = -monster_dst.w;
		
	if (monster_dst.y>LogicalSizeV)
		monster_dst.y = LogicalSizeV;
	else
	if (monster_dst.y<-2)
		monster_dst.y = -2;
	//END TRACKING LOGIC



//BEGIN ANIMATION

//END 	ANIMATION

//BEGIN RENDERING
SDL_SetRenderDrawColor(Renderer, 70, 70, 70, 255);
SDL_RenderClear(Renderer);
SDL_Delay(8);
SDL_RenderCopy(Renderer, Texture    , NULL, NULL);

if (chase)
	SDL_RenderCopy(Renderer, chase_tex, NULL, &chase_dst);
else
	SDL_RenderCopy(Renderer, evade_tex, NULL, &evade_dst);

SDL_RenderCopy(Renderer, Player_tex , NULL, &player_dst);
SDL_RenderCopy(Renderer, Monster_tex, NULL, &monster_dst);
SDL_RenderPresent(Renderer);
//END   RENDERING
}
//END MAIN LOOP

//BEGIN CLEAN UP
SDL_DestroyTexture(Texture);
SDL_DestroyTexture(chase_tex);
SDL_DestroyTexture(evade_tex);
SDL_DestroyTexture(Player_tex);
SDL_DestroyTexture(Monster_tex);

exit_();
//END   CLEAN UP
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
//END   FUNCTIONS
