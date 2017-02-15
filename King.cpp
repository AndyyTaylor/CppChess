#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>

#include "King.h"

King::King()
{
    
}
void King::Init(int _x, int _y, SDL_Renderer* renderer, int SQUARE_SIZE)
{
    SetPos(_x, _y, SQUARE_SIZE);
    texture = IMG_LoadTexture(renderer, "res/wk.png");
}