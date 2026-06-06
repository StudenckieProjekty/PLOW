#pragma once
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "logic.h"

void initAllegro();

struct UIButton {
    ALLEGRO_BITMAP* image;
    ALLEGRO_BITMAP* hoverImage;
    float x;
    float y;
    float width;
    float height;
};

struct UIButton createButton(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* hoverImage, float xCoordinate, float yCoordinate, float scale);

struct UIButton createCenteredButton(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* hoverImage, float yCoordinate, float scale);

void drawBackground(ALLEGRO_BITMAP* backgroundImage);

void drawMenu(struct board* board, ALLEGRO_BITMAP* backgroundImage, ALLEGRO_BITMAP* titleImage, struct UIButton* playButton, struct UIButton* quitButton);

void handleMenuClick(struct board* board, int mouseButton, float mouseX, float mouseY, struct UIButton* playButton, struct UIButton* quitButton, bool* bIsFirstClick);

void drawBoard(struct board* board, ALLEGRO_BITMAP* backgroundImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* questionTile, ALLEGRO_BITMAP* flaggedTile, ALLEGRO_BITMAP* mineTile, ALLEGRO_BITMAP* wonText, ALLEGRO_BITMAP* lostText, ALLEGRO_BITMAP* subMessage, ALLEGRO_BITMAP* clockTile, ALLEGRO_FONT* pixelFont);

void handleBoardClick(struct board* board, int mouseButton, float mouseX, float mouseY, bool* bIsFirstClick);
