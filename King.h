#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>

#include "Piece.h"

class King : public Piece
{
public:
    King();
    void Init(int _x, int _y, SDL_Renderer* renderer, int SQUARE_SIZE);
    
private:
    int z; //placeholder
};