#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "logic.h"

void initAllegro();

void drawBackground(ALLEGRO_BITMAP* bgImage);

void drawMenu(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* titleImage,
    ALLEGRO_BITMAP* playBtn, ALLEGRO_BITMAP* playSelBtn,
    ALLEGRO_BITMAP* quitBtn, ALLEGRO_BITMAP* quitSelBtn);

void drawBoard(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* mineTile);

void handleBoardClick(struct board* board, float mouseX, float mouseY, bool* bIsFirstClick);
