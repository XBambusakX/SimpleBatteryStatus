#include <iostream>
#include <string>
#include <cmath>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include "render.hpp"

void RENDER::renderBackground(SDL_Renderer* renderer, SDL_Color color){
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer);
}

void RENDER::drawRectangle(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Color color, bool filled){
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_FRect rect{x, y, w, h};
  if(filled){
    SDL_RenderFillRect(renderer, &rect);
  } else {
    SDL_RenderRect(renderer, &rect);
  }
}
