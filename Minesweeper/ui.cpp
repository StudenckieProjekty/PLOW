#include "ui.h"
#include <iostream>

void initAllegro() {
    al_init();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
}

void drawTextWithOutline(ALLEGRO_FONT* font, ALLEGRO_COLOR textColor, ALLEGRO_COLOR outlineColor, float x, float y, int flags, const char* text) {
    al_draw_text(font, outlineColor, x - 3, y - 3, flags, text);
    al_draw_text(font, outlineColor, x, y - 3, flags, text);
    al_draw_text(font, outlineColor, x + 3, y - 3, flags, text);
    al_draw_text(font, outlineColor, x - 3, y, flags, text);
    al_draw_text(font, outlineColor, x + 3, y, flags, text);
    al_draw_text(font, outlineColor, x - 3, y + 3, flags, text);
    al_draw_text(font, outlineColor, x, y + 3, flags, text);
    al_draw_text(font, outlineColor, x + 3, y + 3, flags, text);
    al_draw_text(font, textColor, x, y, flags, text);
}

void drawBackground(ALLEGRO_BITMAP* bgImage) {
    al_draw_scaled_bitmap(
        bgImage,
        0, 0, al_get_bitmap_width(bgImage), al_get_bitmap_height(bgImage),
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        0
    );
}

struct UIButton createButton(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* hoverImage, float xCoordinate, float yCoordinate, float scale) {
    struct UIButton btn;
    btn.image = image;
    btn.hoverImage = hoverImage;

    btn.width = al_get_bitmap_width(image) * scale;
    btn.height = al_get_bitmap_height(image) * scale;

    btn.x = xCoordinate;
    btn.y = yCoordinate;

    return btn;
}

struct UIButton createCenteredButton(ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* hoverImage, float yCoordinate, float scale) {
    float targetWidth = al_get_bitmap_width(image) * scale;
    float xCoordinate = (SCREEN_WIDTH - targetWidth) / 2.0f;

    return createButton(image, hoverImage, xCoordinate, yCoordinate, scale);
}

bool bIsMouseWithinButton(float mouseX, float mouseY, struct UIButton* button) {
    return (mouseX >= button->x && mouseX <= button->x + button->width && mouseY >= button->y && mouseY <= button->y + button->height);
}

void drawButton(struct UIButton* button, float mouseX, float mouseY) {

    bool bIsHovered = bIsMouseWithinButton(mouseX, mouseY, button);

    ALLEGRO_BITMAP* bmpToDraw = bIsHovered ? button->hoverImage : button->image;

    al_draw_scaled_bitmap(bmpToDraw, 0, 0, al_get_bitmap_width(bmpToDraw), al_get_bitmap_height(bmpToDraw), button->x, button->y, button->width, button->height, 0);
}

void drawMenu(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* titleImage, struct UIButton* playButton, struct UIButton* quitButton) {
    drawBackground(bgImage);

    float titleScale = 0.5f;
    float titleWidth = al_get_bitmap_width(titleImage) * titleScale;
    float titleHeight = al_get_bitmap_height(titleImage) * titleScale;
    float titleCoordinateX = (SCREEN_WIDTH - titleWidth) / 2.0f;
    float titleCoordinateY = 15.0f;

    al_draw_scaled_bitmap(titleImage, 0, 0, al_get_bitmap_width(titleImage), al_get_bitmap_height(titleImage), titleCoordinateX, titleCoordinateY, titleWidth, titleHeight, 0);

    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);

    drawButton(playButton, mouseState.x, mouseState.y);
    drawButton(quitButton, mouseState.x, mouseState.y);
}

void handleMenuClick(struct board* board, int mouseButton, float mouseX, float mouseY, struct UIButton* playBtn, struct UIButton* quitBtn) {
    if (mouseButton == 1 && bIsMouseWithinButton(mouseX, mouseY, playBtn)) {
        board->state = GameState::Running;
    }
    else if (mouseButton == 1 && bIsMouseWithinButton(mouseX, mouseY, quitBtn)) {
        board->state = GameState::Exit;
    }
}

void drawBoard(struct board* board, ALLEGRO_BITMAP* bgImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* questionTile, ALLEGRO_BITMAP* flaggedTile, ALLEGRO_BITMAP* mineTile, ALLEGRO_BITMAP* wonText, ALLEGRO_BITMAP* lostText, ALLEGRO_BITMAP* subMessage, ALLEGRO_BITMAP* clockIcon, ALLEGRO_FONT* mcFont) {
    drawBackground(bgImage);

    float boardWidthPx = board->width * TILE_SIZE;
    float boardHeightPx = board->height * TILE_SIZE;

    float startX = (SCREEN_WIDTH - boardWidthPx) / 2.0f;
    float startY = SCREEN_HEIGHT - boardHeightPx - 20.0f;

    float srcW = al_get_bitmap_width(hiddenTile);
    float srcH = al_get_bitmap_height(hiddenTile);

    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {

            float currentX = startX + (j * TILE_SIZE);
            float currentY = startY + (i * TILE_SIZE);

            ALLEGRO_BITMAP* bmpToDraw = nullptr;

            switch (board->matrix[i][j].state) {
                case TileState::Hidden:
                    bmpToDraw = hiddenTile;
                    break;

                case TileState::Flagged:
                    bmpToDraw = flaggedTile;
                    break;

                case TileState::QuestionMarked:
                    bmpToDraw = questionTile;
                    break;

                case TileState::Revealed:
                    if (board->matrix[i][j].bHasMine) {
                        bmpToDraw = mineTile;
                    }
                    else {
                        bmpToDraw = revealedTiles[board->matrix[i][j].adjacentMinesNum];
                    }
                    break;
            }

            if (bmpToDraw != nullptr) {
                al_draw_scaled_bitmap(bmpToDraw, 0, 0, srcW, srcH, currentX, currentY, TILE_SIZE, TILE_SIZE, 0);
            }
        }
    }

    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    float iconScale = 0.5f;

    float clockW = al_get_bitmap_width(clockIcon) * iconScale;
    float clockH = al_get_bitmap_height(clockIcon) * iconScale;
    float clockX = startX - clockW - 140.0f;
    float hudY = startY + boardHeightPx - clockH;
    al_draw_scaled_bitmap(clockIcon, 0, 0, al_get_bitmap_width(clockIcon), al_get_bitmap_height(clockIcon), clockX, hudY, clockW, clockH, 0);

    char timeStr[5];
    snprintf(timeStr, sizeof(timeStr), "%d", board->timeCounter);
    float textYOffset = (clockH - al_get_font_line_height(mcFont)) / 2.0f;
    drawTextWithOutline(mcFont, white, black, clockX + clockW + 20.0f, hudY + textYOffset, ALLEGRO_ALIGN_LEFT, timeStr);

    float tntW = al_get_bitmap_width(mineTile) * iconScale;
    float tntH = al_get_bitmap_height(mineTile) * iconScale;
    float tntX = startX + boardWidthPx + 140.0f;
    al_draw_scaled_bitmap(mineTile, 0, 0, al_get_bitmap_width(mineTile), al_get_bitmap_height(mineTile), tntX, hudY, tntW, tntH, 0);

    int remainingMines = board->minesNum - board->flagsPlacedNum;
    char minesStr[4];
    snprintf(minesStr, sizeof(minesStr), "%d", remainingMines);
    drawTextWithOutline(mcFont, white, black, tntX - 20.0f, hudY + textYOffset, ALLEGRO_ALIGN_RIGHT, minesStr);

    if (board->state == GameState::Won || board->state == GameState::Lost) {
        ALLEGRO_BITMAP* message = board->state == GameState::Won ? wonText : lostText;

        float textScale = 0.25f;
        float textWidth = al_get_bitmap_width(message) * textScale;
        float textHeight = al_get_bitmap_height(message) * textScale;
        float textCoordinateX = (SCREEN_WIDTH - textWidth) / 2.0f;
        float textCoordinateY = 0;

        al_draw_scaled_bitmap(message, 0, 0, al_get_bitmap_width(message), al_get_bitmap_height(message), textCoordinateX, textCoordinateY, textWidth, textHeight, 0);

        float subMessageScale = 0.3225f;
        float subMessageWidth = al_get_bitmap_width(subMessage) * subMessageScale;
        float subMessageHeight = al_get_bitmap_height(subMessage) * subMessageScale;
        float subMessageCoordinateX = (SCREEN_WIDTH - subMessageWidth) / 2.0f;
        float subMessageCoordinateY = 70.f;

        al_draw_scaled_bitmap(subMessage, 0, 0, al_get_bitmap_width(subMessage), al_get_bitmap_height(subMessage), subMessageCoordinateX, subMessageCoordinateY, subMessageWidth, subMessageHeight, 0);
    }
}

void handleBoardClick(struct board* board, int mouseButton, float mouseX, float mouseY, bool* bIsFirstClick) {
    float boardWidthPx = board->width * TILE_SIZE;
    float boardHeightPx = board->height * TILE_SIZE;

    float startX = (SCREEN_WIDTH - boardWidthPx) / 2.0f;
    float startY = SCREEN_HEIGHT - boardHeightPx - 20.0f;

    if (mouseX >= startX && mouseX < startX + boardWidthPx && mouseY >= startY && mouseY < startY + boardHeightPx) {

        int j = (mouseX - startX) / TILE_SIZE;
        int i = (mouseY - startY) / TILE_SIZE;

        switch (mouseButton) {
            case 1:
                if (*bIsFirstClick) {
                    initBoardAfterFirstClick(board, i, j);
                    *bIsFirstClick = false;
                }
                revealTile(board, i, j);
                break;

            case 2:
                switch (board->matrix[i][j].state) {
                    case TileState::Revealed:
                        break;

                    case TileState::Hidden:
                        board->matrix[i][j].state = TileState::Flagged;
                        board->flagsPlacedNum += 1;
                        break;

                    case TileState::Flagged:
                        board->matrix[i][j].state = TileState::QuestionMarked;
                        board->flagsPlacedNum -= 1;
                        break;

                    case TileState::QuestionMarked:
                        board->matrix[i][j].state = TileState::Hidden;
                        break;
                }
        }
    }
}
