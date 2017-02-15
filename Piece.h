#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <array>
#include <vector>

class Piece
{
public:
    Piece();
     
    void Init(int _x, int _y, int _w, SDL_Renderer* renderer, std::string type, std::string color, std::vector<Piece> pieces);
    void SetPos(int _x, int _y, int _w, std::vector<Piece>& pieces);
    void RenderFillCircle(SDL_Point center, int radius, SDL_Color color, SDL_Renderer* renderer);
    void LoadTexture(SDL_Renderer* renderer, std::string type);
    void LoadMovementVector(std::vector<Piece> pieces);
    void AddVector(int start, int end, int inc, bool flip, bool diag, std::vector<Piece> pieces);
    bool CanMoveTo(int _x, int _y, std::vector<Piece> pieces, bool eqlcol);
    bool IsPieceAt(int _x, int _y, std::vector<Piece> pieces);
    std::string GetColor(){ return col; }
    void Render(SDL_Renderer* renderer);
    void Select(){ selected = true; }
    void Deselect(){ selected = false; }
    bool isSelected(){ return selected; }
    int GetX(){ return x; } // SDL_Point
    int GetY(){ return y; }
    std::vector<int> move_vector;
    
private:
    int x, y, w;
    std::string col;
    std::string type;
    bool selected;
    bool moved;
    SDL_Rect rect;
    SDL_Texture* texture;
};