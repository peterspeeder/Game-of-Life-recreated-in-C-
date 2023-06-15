#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600
#define BLOCK_SIZE 10

typedef struct
{
    int width;
    int height;
    bool **gameBoard;
} Game;

Game game;

void InitializeGame(int width, int height);
void UpdateGame(void);
void DrawGame(void);
void ShutdownGame(void);
void ToggleCell(int x, int y);
void RandomlyFillBoard(int percent);
void ResetBoard(void);
void GameLoop(void);

int main(void)
{
    InitializeGame(WIDTH / BLOCK_SIZE, HEIGHT / BLOCK_SIZE);
    RandomlyFillBoard(50);
    GameLoop();
    ShutdownGame();

    return 0;
}

void InitializeGame(int width, int height)
{
    game.width = width;
    game.height = height;

    // Allocate memory for the game board
    game.gameBoard = (bool **)malloc(sizeof(bool *) * game.width);
    for (int i = 0; i < game.width; i++)
    {
        game.gameBoard[i] = (bool *)malloc(sizeof(bool) * game.height);
    }
}

void UpdateGame(void)
{
    bool **newBoard = (bool **)malloc(sizeof(bool *) * game.width);
    for (int i = 0; i < game.width; i++)
    {
        newBoard[i] = (bool *)malloc(sizeof(bool) * game.height);
    }

    for (int i = 0; i < game.width; i++)
    {
        for (int j = 0; j < game.height; j++)
        {
            int surrounding = 0;

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (!(x == 0 && y == 0) && i + x >= 0 && i + x < game.width && j + y >= 0 && j + y < game.height)
                    {
                        if (game.gameBoard[i + x][j + y])
                            surrounding++;
                    }
                }
            }

            if (game.gameBoard[i][j])
            {
                // Cell is alive, will the cell survive?
                if (surrounding < 2 || surrounding > 3)
                    newBoard[i][j] = false;
                else
                    newBoard[i][j] = true;
            }
            else
            {
                // Cell is dead, will the cell be given birth?
                if (surrounding == 3)
                    newBoard[i][j] = true;
                else
                    newBoard[i][j] = false;
            }
        }
    }

    // Copy new board to game board
    for (int i = 0; i < game.width; i++)
    {
        for (int j = 0; j < game.height; j++)
        {
            game.gameBoard[i][j] = newBoard[i][j];
        }
    }

    // Free memory allocated for new board
    for (int i = 0; i < game.width; i++)
    {
        free(newBoard[i]);
    }
    free(newBoard);
}

void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < game.width; i++)
    {
        for (int j = 0; j < game.height; j++)
        {
            if (game.gameBoard[i][j])
                DrawRectangle(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, WHITE);
        }
    }

    EndDrawing();
}

void ShutdownGame(void)
{
    for (int i = 0; i < game.width; i++)
    {
        free(game.gameBoard[i]);
    }
    free(game.gameBoard);
}

void ToggleCell(int x, int y)
{
    if (x >= 0 && x < game.width && y >= 0 && y < game.height)
        game.gameBoard[x][y] = !game.gameBoard[x][y];
}

void RandomlyFillBoard(int percent)
{
    for (int i = 0; i < game.width; i++)
    {
        for (int j = 0; j < game.height; j++)
        {
            int randomValue = rand() % 100;
            game.gameBoard[i][j] = (randomValue < percent);
        }
    }
}

void ResetBoard(void)
{
    for (int i = 0; i < game.width; i++)
    {
        for (int j = 0; j < game.height; j++)
        {
            game.gameBoard[i][j] = false;
        }
    }
}

void GameLoop(void)
{
    InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }

    CloseWindow();
}

