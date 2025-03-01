#include <string>
#include <cmath>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_power.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include "render.hpp"

const int dgWidth{100};
const int dgHeight{10};

bool gIsCharging{true};
int gBatteryPercentage{0};
int gWidth = dgWidth;
int gHeight = dgHeight;

SDL_Color gBatteryColor{0x33, 0xff, 0x33, 0x50};
SDL_Window* gWindow{nullptr};
SDL_Renderer* gRenderer{nullptr};

SDL_Color getColorFromPercentage(int percentage) {
    if (percentage <= 1 || percentage >= 100) {
        // Handle invalid input (e.g., return a default color)
        SDL_Color defaultColor = {0, 0, 0, 0x50}; // Black with your opacity
        return defaultColor;
    }

    SDL_Color color;
    color.a = 0x50; // Constant opacity

    if (percentage <= 50) {
        // Red to purple (red decreasing, blue increasing)
        color.r = 255 - (percentage * 255 / 50);
        color.g = 0;
        color.b = (percentage * 255 / 50);

    } else {
        // Purple to blue (red decreasing, blue increasing)
        color.r = 255 - (percentage * 255 / 50);
        color.g = 0;
        color.b = 255;
    }

    return color;
}

bool init(){
  if(!SDL_Init(SDL_INIT_VIDEO)){
    SDL_Log("SDL could not initialize! Error: %s\n", SDL_GetError());
    return false;
  }
  gWindow = SDL_CreateWindow(
      "Simple Battery status",
      gWidth, gHeight,
      SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_UTILITY | SDL_WINDOW_BORDERLESS);
  if(gWindow == nullptr){
    SDL_Log("Window could not be created! Error: %s\n", SDL_GetError());
    return false;
  }
  gRenderer = SDL_CreateRenderer(gWindow, nullptr);
  if(gRenderer == nullptr){
    SDL_Log("Renderer could not be created! Error: %s\n", SDL_GetError());
    return false;
  }
  SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_ADD);
  return true;
}

bool isPointInsideCircle(float clickX, float clickY, float circleX, float circleY, float radius) {
    double distance = std::sqrt(std::pow(clickX - circleX, 2) + std::pow(clickY - circleY, 2));
    return distance <= radius;
}   

void close(){
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gRenderer = nullptr;
  gWindow = nullptr;
  SDL_Quit();
}

int main(int argc, char* args[]){
    if(!init()){
      SDL_Log("Failed to initialize!\n");
      return -1;
    }
        bool quit{false};
        SDL_Event e;
        SDL_zero(e);

        while(!quit){
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_EVENT_QUIT){
                    quit = true;
                }
            }

            if(SDL_GetPowerInfo(nullptr, &gBatteryPercentage) != SDL_POWERSTATE_CHARGING){
                gIsCharging = false;
            }else{
                gIsCharging = true;
            }

          if(gIsCharging && gBatteryPercentage > 90){
            gHeight = 1;
            gWidth = 1;
          }else if(gBatteryPercentage > 90){
            gHeight = 1;
            gWidth = 1;
          }else{
            gHeight = dgHeight;
            gWidth = dgWidth;
          }
          SDL_SetWindowSize(gWindow, gWidth, gHeight);
          SDL_Delay(10);
          RENDER::renderBackground(gRenderer, {0x00, 0x00, 0x00, 0x55}); 
          if(!gIsCharging || (gIsCharging && gBatteryPercentage < 90)){
          RENDER::drawRectangle(gRenderer, 0, 0, (gWidth/100)*gBatteryPercentage, gHeight,getColorFromPercentage(gBatteryPercentage) , true);
          RENDER::drawRectangle(gRenderer, 0, 0, (gWidth/100)*gBatteryPercentage, gHeight/5, {0xff, 0x00, 0x00, 0x50}, true);
          }
          SDL_RenderPresent(gRenderer);
          SDL_Delay(5000);
        }
    close();
    return 0;
}
