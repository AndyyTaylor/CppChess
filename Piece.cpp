#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <vector>
#include <map>

#include "Piece.h"

Piece::Piece()
    : x(0)
    , y(0)
    , selected(false)
    , col("white") 
    , moved(false)
{
    
}
void Piece::Init(int _x, int _y, int _w, SDL_Renderer* renderer, std::string _type, std::string color, std::vector<Piece> pieces)
{
    col = color;
    type = _type;
    SetPos(_x, _y, _w, pieces);
    LoadTexture(renderer, type);
    moved = false;
}
void Piece::Render(SDL_Renderer* renderer)
{
    //selected = true;
    //std::cout << selected << std::endl;
    if (selected)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);  
        for (int i = 0; i < move_vector.size(); i+=2)
        {
            SDL_Color color = {220, 220, 220, 255};
            SDL_Point point;
            point.x = (x+move_vector[i]-1)*w+w/2;
            point.y = (8-(move_vector[i+1]+y))*w+w/2;
            RenderFillCircle(point, 10, color, renderer);
        }
    }
    if (texture != nullptr)
    {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    else 
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);    
    }
}
void Piece::RenderFillCircle(SDL_Point center, int radius, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}
void Piece::LoadMovementVector(std::vector<Piece> pieces)
{
    move_vector.clear();
    if (type == "king"){ // There literally has to be a better way to init std::vector
        int arr[16] = {1,1,1,0,1,-1,0,-1,-1,-1,-1,0,-1,1,0,1};
        for (int i = 0; i < 16; i++){ move_vector.push_back(arr[i]); }
    } else if (type == "rook"){
        AddVector(1, 8, 1, false, false, pieces);
        AddVector(1, 8, 1, true, false, pieces);
        AddVector(-1, -8, -1, false, false, pieces);
        AddVector(-1, -8, -1, true, false, pieces);
    } else if (type == "bishop"){
        AddVector(1, 8, 1, false, true, pieces);
        AddVector(-1, -8, -1, false, true, pieces);
        AddVector(-1, -8, -1, true, true, pieces);
        AddVector(1, 8, 1, true, true, pieces);
    } else if (type == "knight"){
        int arr[18] = {2,1,2,-1,-2,1,-2,-1,1,-2,-1,-2,1,2,-1,2};
        for (int i = 0; i < 16; i++){ move_vector.push_back(arr[i]); }
    } else if (type == "queen"){
        AddVector(1, 8, 1, false, true, pieces);
        AddVector(-1, -8, -1, false, true, pieces);
        AddVector(-1, -8, -1, true, true, pieces);
        AddVector(1, 8, 1, true, true, pieces);
        AddVector(1, 8, 1, false, false, pieces);
        AddVector(1, 8, 1, true, false, pieces);
        AddVector(-1, -8, -1, false, false, pieces);
        AddVector(-1, -8, -1, true, false, pieces);
    } else if (type == "pawn"){
        std::string cols = "wb";
        int found = 1-cols.find(col.at(0)); 
        if (!IsPieceAt(x, y+2*(found*2-1), pieces) && !moved)
        {
            move_vector.push_back(0); 
            move_vector.push_back(2*(found*2-1));
        } if (!IsPieceAt(x, y+found*2-1, pieces))
        {
            move_vector.push_back(0); 
            move_vector.push_back(found*2-1);
        } if (!CanMoveTo(x-1, y+found*2-1, pieces, false)){
            move_vector.push_back(-1);
            move_vector.push_back(found*2-1);
        } if (!CanMoveTo(x+1, y+found*2-1, pieces, false)){
            move_vector.push_back(1);
            move_vector.push_back(found*2-1);
        }
    }
}
void Piece::AddVector(int start, int end, int inc, bool flip, bool diag, std::vector<Piece> pieces)
{
    for (int _x = start; (end > 0 && _x < end) || (end < 0 && _x > end); _x+=inc){
        if (!flip){
            int x2 = _x;
            int y2 = _x;
            if (!diag) {
                y2 = 0;
            } 
            if (!CanMoveTo(x+x2, y+y2, pieces, true)) return;
            move_vector.push_back(x2);
            move_vector.push_back(y2);
            if (!CanMoveTo(x+x2, y+y2, pieces, false)) return;
        } else {
            int x2 = -_x;
            int y2 = _x;
            if (!diag) {
                x2 = 0;
            } 
            if (!CanMoveTo(x+x2, y+y2, pieces, true)) return;
            move_vector.push_back(x2);
            move_vector.push_back(y2);
            if (!CanMoveTo(x+x2, y+y2, pieces, false)) return;
        }
    } 
}
bool Piece::CanMoveTo(int _x, int _y, std::vector<Piece> pieces, bool eqlcol)
{
    //return true;
    for (int i = 0; i < pieces.size(); i++)
    {
        Piece& p = pieces[i];
        if (p.GetX() == x && p.GetY() == y) continue;
        if (p.GetX() == _x && p.GetY() == _y && (p.GetColor() == col) == eqlcol){
            return false;
        }
    }
    return true;
}
bool Piece::IsPieceAt(int _x, int _y, std::vector<Piece> pieces)
{
    for (int i = 0; i < pieces.size(); i++)
    {
        Piece& p = pieces[i];
        if (p.GetX() == x && p.GetY() == y) continue;
        if (p.GetX() == _x && p.GetY() == _y){
            return true;
        }
    }
    return false;
}
void Piece::LoadTexture(SDL_Renderer* renderer, std::string type)
{
    std::string source = "res/";
    source += col.at(0);
    if (type == "king"){
        source += "k.png"; 
    } else if (type == "rook"){
        source += "r.png";
    } else if (type == "bishop"){
        source += "b.png";
    } else if (type == "knight"){
        source += "n.png";
    } else if (type == "queen"){
        source += "q.png";
    } else if (type == "pawn"){
        source += "p.png";
    }
    texture = IMG_LoadTexture(renderer, source.c_str());
}
void Piece::SetPos(int _x, int _y, int _w, std::vector<Piece>& pieces)
{
    for (int i = 0; i < pieces.size(); i++)
    {
        Piece& p = pieces[i];
        if (p.GetX() == _x && p.GetY() == _y && p.GetColor() != col)
        {
            // Interesting, this avoids linear complexity
            std::swap(pieces[i], pieces.back());
            pieces.pop_back();
            break;
        }
    }
    x = _x; 
    y = _y; 
    w = _w;
    rect.x = w*(_x-1); 
    rect.y = w*(8-_y); 
    rect.w = w; 
    rect.h = w; 
    moved = true;
}