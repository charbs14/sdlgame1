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
  SDL_Rect ball;
  SDL_Rect intersect;
//get dimensions of the texture
  SDL_QueryTexture(tex,NULL,NULL, &dest.w,&dest.h);

  //make image smaller
  dest.w /= 4;
  dest.h /= 28;

  ball.w = 12;
  ball.h = 12;

  //position rectangle in middle bottom of screen
  float xpos = (WINDOW_WIDTH - dest.w)/2;
  float ypos = (WINDOW_HEIGHT - 3*dest.h);

  //position for the ball with start at the middle of the screen

  float bposx = (WINDOW_WIDTH - ball.w)/2;
  float bposy = (WINDOW_HEIGHT - ball.h)/2;

  //give BALL initial velocity
  float xvel = SPEED*2;
  float yvel = SPEED*2;
  //paddle velocity
  float xvelp = 0;
  float yvelp = 0;

  //animation loop
  bool isquit = false;
  SDL_Event event;

    int left,right = 0;
  float area = dest.w*dest.h;

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

    //collision detection for window
    if ( bposx <= 0 ){
      bposx = 0;
      xvel = -xvel;
    }
    if ( bposy <= 0 ){
      bposy = 0;
      yvel = -yvel;
    }
    if ( bposx >= WINDOW_WIDTH - ball.w ){
      bposx = WINDOW_WIDTH - ball.w;
      xvel = -xvel;
    }
    if ( bposy >= WINDOW_HEIGHT - ball.h ){
      bposy = WINDOW_HEIGHT - ball.h;
      yvel = -yvel;
    }
    //collision detection for paddle
    if(SDL_IntersectRect(&ball,&dest,&intersect) == SDL_TRUE){
      if(intersect.y == dest.y){ //hits top of paddle
        bposy -= intersect.h+2;
        yvel = -yvel;
      }
      if(intersect.y + intersect.h == dest.y + dest.h){ //hits bottom of paddle
        bposy += intersect.h+2;
        yvel = -yvel;
      }
      if(intersect.x == dest.x){//hits left side of paddle
        bposx -= intersect.w+2;
        xvel = -xvel;
      }
      if(intersect.x + intersect.w == dest.x + dest.w){ //hits right side of paddle
        bposx += intersect.w+2;
        xvel = -xvel;
      }
    }


    if( left && !right )
      xvelp = -SPEED*3;
    if( right && !left )
      xvelp = SPEED*3;
    if( !left && !right )
      xvelp = 0;

    xpos += xvelp/60;

    bposx += xvel/60;
    bposy += yvel/60;

    dest.x = (int) xpos;
    dest.y = (int) ypos;

    ball.x = (int) bposx;
    ball.y = (int) bposy;

    SDL_SetRenderDrawColor(rend,0,0,0,1);;//set render draw color to black to clear screen
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend,tex,NULL,&dest);

    SDL_RenderDrawRect(rend,&ball);
    SDL_SetRenderDrawColor(rend,255,255,255,1);
    SDL_RenderFillRect(rend,&ball);
    SDL_RenderPresent(rend);

    SDL_Delay(1000/60);
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
