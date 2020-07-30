#ifndef _HELPER_H_
#define _HELPER_H_
#include <stdlib.h>
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

extern SDL_Window *Window;
extern SDL_Renderer *Renderer;

void init(void);
void exit_(void);

#endif
