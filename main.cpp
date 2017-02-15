#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>

#include "Piece.h"

using namespace std::chrono;
// Would be 'const int' but I want to make the board resizeable
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;
int BOARD_WIDTH, BOARD_HEIGHT, SQUARE_SIZE;

SDL_Window* window;
SDL_Renderer* renderer;

std::vector<Piece> pieces;
std::string turn = "white"; // Should use Enums for this stuff

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();
void Quit();

void RunGame();
void Render();
void HandleInput();
void UpdateDimensions();
void SetupBoard();

double GetDelta();
void RenderGameBoard(); 

bool loop = true;
auto timePrev = high_resolution_clock::now();

int main(int argc, char* args[])
{
    if (!InitEverything())
        return -1;
    
    SetupBoard();
    RunGame();
    
    Quit();
    return 0;
}
void RunGame()
{
    while (loop)
    {
        for (int i = 0; i < pieces.size(); i++)
        {
            Piece& p = pieces[i];
            p.LoadMovementVector(pieces);
        }
        HandleInput();
        Render();
        
        double delta = GetDelta();
        
    }
}
void Render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    RenderGameBoard();
    
    for (int i = 0; i < pieces.size(); i++)
    {
        pieces[i].Render(renderer);
    }
    
    SDL_RenderPresent(renderer);
}
void SetupBoard()
{
    std::string piecePos[] = {"pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "pawn", "rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"};
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            Piece p;
            std::string col = "white";
            int y;
            int x;
            if (piecePos[i] != "pawn")
            {
                x = i-7;
                y = 1;
                if (j > 0){ 
                    col = "black"; 
                    y = 8;
                }
            } else {
                x = i+1;
                y = 2;
                if (j > 0){ 
                    col = "black"; 
                    y = 7;
                }
            }
            p.Init(x, y, SQUARE_SIZE, renderer, piecePos[i], col, pieces);
            pieces.push_back(p); 
        } 
    }
      
}
void RenderGameBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((j%2==0&&i%2==0)||(j%2!=0&&i%2!=0))
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            SDL_Rect r = {i*SQUARE_SIZE, j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &r);
        }
    }
}
void HandleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            loop = false;
        else if (event.type == SDL_KEYDOWN)
        {
            
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int x = floor(event.button.x/SQUARE_SIZE)+1;
                int y = 8-floor(event.button.y/SQUARE_SIZE);
                bool selected = false;
                for (int p = 0; p < pieces.size(); p++)
                {
                    if (pieces[p].isSelected())
                    {
                        selected = true;
                        bool moved = false;
                        Piece& spiece = pieces[p];
                        for (int i = 0; i < spiece.move_vector.size(); i+=2)
                        {
                            int px = spiece.move_vector[i]+spiece.GetX();
                            int py = spiece.move_vector[i+1]+spiece.GetY();
                            if (px == x && py == y)
                            {
                                spiece.SetPos(x, y, SQUARE_SIZE, pieces);
                                moved = true;
                                if (turn == "white")
                                    turn = "black";
                                else
                                    turn = "white";
                                break;
                            }
                        }
                        //std::cout << spiece.GetX() << std::endl;
                        spiece.Deselect();
                    }
                }
                
                if (!selected)
                {
                    for (int i = 0; i < pieces.size(); i++)
                    {
                        Piece& p = pieces[i];
                        p.Deselect();
                    }
                    for (int i = 0; i < pieces.size(); i++)
                    {
                        Piece& p = pieces[i];
                        if (p.GetX() == x && p.GetY() == y && p.GetColor() == turn)
                        {
                            p.Select();
                            break;
                        }
                    }
                }
            }
        }
    }
}
void UpdateDimensions()
{
    BOARD_WIDTH = SCREEN_WIDTH;
    BOARD_HEIGHT = SCREEN_HEIGHT;
    SQUARE_SIZE = BOARD_WIDTH/8;
}
double GetDelta()
{
	auto timeCurrent = high_resolution_clock::now();

	auto timeDiff = duration_cast< nanoseconds >( timeCurrent - timePrev );

	double delta = timeDiff.count();

	delta /= 1000000000;

	timePrev = timeCurrent;
	return delta;
}
bool InitEverything()
{
    if (!InitSDL())
        return false;
    
    if (!CreateWindow())
        return false;
    
    if (!CreateRenderer())
        return false;
    
    SetupRenderer();
    UpdateDimensions();
    
    return true;
}
bool InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL failed to initialize : " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}
bool CreateWindow()
{
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Failed to create window : " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}
bool CreateRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}
void SetupRenderer()
{
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void Quit()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}
