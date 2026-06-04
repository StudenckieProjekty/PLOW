#include "ui.h"
#include <iostream>

void initAllegro() {
    al_init();
    al_init_image_addon();
    al_install_mouse();
}

void drawBackground(ALLEGRO_BITMAP* bgImage) {
    al_draw_scaled_bitmap(
        bgImage,
        0, 0, al_get_bitmap_width(bgImage), al_get_bitmap_height(bgImage),
        0, 0, 1280.0f, 720.0f,
        0
    );
}

bool drawButtonCentered(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* hoverImage, float yCoordinate, float scale, ALLEGRO_MOUSE_STATE& mouseState) {
    float imageWidth = al_get_bitmap_width(image);
    float imageHeight = al_get_bitmap_height(image);

    float targetWidth = imageWidth * scale;
    float targetHeight = imageHeight * scale;

    float xCoordinate = (1280.0f - targetWidth) / 2.0f;

    bool bIsHovered = (mouseState.x >= xCoordinate && mouseState.x <= xCoordinate + targetWidth && mouseState.y >= yCoordinate && mouseState.y <= yCoordinate + targetHeight);

    ALLEGRO_BITMAP* bmpToDraw = bIsHovered ? hoverImage : image;

    al_draw_scaled_bitmap(
        bmpToDraw,
        0, 0, imageWidth, imageHeight,
        xCoordinate, yCoordinate, targetWidth, targetHeight,
        0
    );

    return bIsHovered;
}

void drawMenu(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* titleImage, ALLEGRO_BITMAP* playBtn, ALLEGRO_BITMAP* playSelBtn, ALLEGRO_BITMAP* quitBtn, ALLEGRO_BITMAP* quitSelBtn) {
    drawBackground(bgImage);

    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);

    float titleScale = 0.5f;
    float titleWidth = al_get_bitmap_width(titleImage) * titleScale;
    float titleHeight = al_get_bitmap_height(titleImage) * titleScale;
    float titleCoordinateX = (1280.0f - titleWidth) / 2.0f;
    float titleCoordinateY = 15.0f;

    al_draw_scaled_bitmap(titleImage, 0, 0, al_get_bitmap_width(titleImage), al_get_bitmap_height(titleImage),
        titleCoordinateX, titleCoordinateY, titleWidth, titleHeight, 0);

    float buttonScale = 0.3f;
    float playCoordinateY = titleCoordinateY + titleHeight;
    bool bIsPlayHovered = drawButtonCentered(playBtn, playSelBtn, playCoordinateY, buttonScale, mouseState);
    float quitCoordinateY = playCoordinateY + (al_get_bitmap_height(playBtn) * buttonScale) + 30.0f;
    bool bIsQuitHovered = drawButtonCentered(quitBtn, quitSelBtn, quitCoordinateY, buttonScale, mouseState);

    if (mouseState.buttons & 1) {
        if (bIsPlayHovered) {
            board->state = GameState::Running;
        }
        else if (bIsQuitHovered) {
            board->state = GameState::Exit;
        }
    }
}

void drawBoard(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* mineTile) {
    drawBackground(bgImage);

    float tileSize = 64.0f;

    float boardWidthPx = board->width * tileSize;
    float boardHeightPx = board->height * tileSize;

    float startX = (1280.0f - boardWidthPx) / 2.0f;
    float startY = 720.0f - boardHeightPx - 20.0f;

    float srcW = al_get_bitmap_width(hiddenTile);
    float srcH = al_get_bitmap_height(hiddenTile);

    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {

            float currentX = startX + (j * tileSize);
            float currentY = startY + (i * tileSize);

            ALLEGRO_BITMAP* bmpToDraw = nullptr;

            if (board->matrix[i][j].state == TileState::Hidden) {
                bmpToDraw = hiddenTile;
            }
            else if (board->matrix[i][j].state == TileState::Revealed) {
                if (board->matrix[i][j].bHasMine) {
                    bmpToDraw = mineTile;
                }
                else {
                    bmpToDraw = revealedTiles[board->matrix[i][j].adjacentMinesNum];
                }
            }

            if (bmpToDraw != nullptr) {
                al_draw_scaled_bitmap(
                    bmpToDraw, 0, 0, srcW, srcH,
                    currentX, currentY, tileSize, tileSize, 0
                );
            }
        }
    }
}

void handleBoardClick(struct board* board, float mouseX, float mouseY, bool* bIsFirstClick) {
    float tileSize = 64.0f;
    float boardWidthPx = board->width * tileSize;
    float boardHeightPx = board->height * tileSize;

    float startX = (1280.0f - boardWidthPx) / 2.0f;
    float startY = 720.0f - boardHeightPx - 20.0f;

    if (mouseX >= startX && mouseX < startX + boardWidthPx && mouseY >= startY && mouseY < startY + boardHeightPx) {

        int j = (mouseX - startX) / tileSize;
        int i = (mouseY - startY) / tileSize;

        if (*bIsFirstClick) {
            initBoardAfterFirstClick(board, i, j);
            *bIsFirstClick = false;
        }

        revealTile(board, i, j);
    }
}
