#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
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

void drawBackground(ALLEGRO_BITMAP* bgImage);

void drawMenu(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* titleImage, struct UIButton* playButton, struct UIButton* quitButton);

void handleMenuClick(struct board* board, int mouseButton, float mouseX, float mouseY, struct UIButton* playBtn, struct UIButton* quitBtn);

void drawBoard(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* questionTile, ALLEGRO_BITMAP* flaggedTile, ALLEGRO_BITMAP* mineTile, ALLEGRO_BITMAP* wonText, ALLEGRO_BITMAP* lostText, ALLEGRO_BITMAP* subMessage);

void handleBoardClick(struct board* board, int mouseButton, float mouseX, float mouseY, bool* bIsFirstClick);
