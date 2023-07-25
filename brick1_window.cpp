#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SPEED (150)

int main(void){
  //initialize SDL_Init
  if (SDL_Init(SDL_INIT_VIDEO)!= 0){

    printf("error initializing SD: %s\n", SDL_GetError());
    return 1;

  }

//creating SDL_Window

  SDL_Window *win = SDL_CreateWindow("Hello!",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);

  if(!win){
    printf("error creating window%s\n",SDL_GetError() );
    SDL_Quit();
    return 1;
  }

//now we make a renderer?

  unsigned int render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

  SDL_Renderer *rend = SDL_CreateRenderer(win,-1,render_flags);

  if(!rend){
    printf("error creating renderer: %s\n",SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

//hopefully that worked, now we load image into memory using SDL_image library function
  SDL_Surface *surf = IMG_Load("resources/cameron.png");

  if(!surf){
    printf("error creating surface\n");
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }
//create texture from surface
  SDL_Texture *tex = SDL_CreateTextureFromSurface(rend,surf);
  SDL_FreeSurface(surf);

  if(!tex){
    printf("error creating surface: %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }
  //clear window note: turns window black
  SDL_RenderClear(rend);



  //draw image to SDL_Window

  //SDL_RenderCopy(rend,tex,NULL,NULL);
  //SDL_RenderPresent(rend);


//instead of doing above and rendering to window, we will rend to rectangle using SDL_Rect
  SDL_Rect dest; //rect structure
//get dimensions of the texture
  SDL_QueryTexture(tex,NULL,NULL, &dest.w,&dest.h);

  //make image smaller
  dest.w /= 4;
  dest.h /= 8;

  //position rectangle in middle of screen
  float xpos = (WINDOW_WIDTH - dest.w)/2;
  float ypos = (WINDOW_HEIGHT - dest.h)/2;

  //give rectangle initial velocity
  float xvel = SPEED;
  float yvel = SPEED;

  //animation loop
  bool isquit = false;
  SDL_Event event;

    int left,right = 0;

  while(!isquit){


    //poll for quit and keyboard events
    while(SDL_PollEvent( & event)) {
      switch(event.type){
        case SDL_QUIT:
          isquit = true;
          break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.scancode){
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
              left = 1;
              break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
              right = 1;
              break;
          }
          break;
          case SDL_KEYUP:
            switch(event.key.keysym.scancode){
              case SDL_SCANCODE_LEFT:
              case SDL_SCANCODE_A:
                left = 0;
                break;
              case SDL_SCANCODE_RIGHT:
              case SDL_SCANCODE_D:
                right = 0;
                break;
            }
            break;
      }
    }

    if ( xpos <= 0 ){
      xpos = 0;
      xvel = -xvel;
    }
    if ( ypos <= 0 ){
      ypos = 0;
      yvel = -yvel;
    }
    if ( xpos >= WINDOW_WIDTH - dest.w ){
      xpos = WINDOW_WIDTH - dest.w;
      xvel = -xvel;
    }
    if ( ypos >= WINDOW_HEIGHT - dest.h ){
      ypos = WINDOW_HEIGHT - dest.h;
      yvel = -yvel;
    }

    /*
    if( left && !right )
      xvel = -SPEED;
    if( right && !left )
      xvel = SPEED;
*/

    xpos += xvel/120;
    ypos += yvel/120;

    dest.x = (int) xpos;
    dest.y = (int) ypos;

    SDL_RenderClear(rend);
    SDL_RenderCopy(rend,tex,NULL,&dest);
    SDL_RenderPresent(rend);

    SDL_Delay(1000/120);
  }

  /*
  bool isquit = false;
  SDL_Event event;
  while (!isquit) {
      if (SDL_PollEvent( & event)) {
          if (event.type == SDL_QUIT) {
              isquit = true;
          }
      }

  }
*/
  //clean up
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
