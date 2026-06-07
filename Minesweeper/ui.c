#include "ui.h"
#include "io.h"
#include <stdio.h>

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

void drawBackground(ALLEGRO_BITMAP* backgroundImage) {
    al_draw_scaled_bitmap(backgroundImage, 0, 0, al_get_bitmap_width(backgroundImage), al_get_bitmap_height(backgroundImage), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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
    float xCoordinate = (SCREEN_WIDTH - targetWidth) / 2;

    return createButton(image, hoverImage, xCoordinate, yCoordinate, scale);
}

bool bIsMouseWithinButton(float mouseX, float mouseY, struct UIButton* button) {
    return (mouseX >= button->x && mouseX <= button->x + button->width && mouseY >= button->y && mouseY <= button->y + button->height);
}

void drawButton(struct UIButton* button, float mouseX, float mouseY) {

    bool bIsHovered = bIsMouseWithinButton(mouseX, mouseY, button);

    ALLEGRO_BITMAP* textureToDraw = bIsHovered ? button->hoverImage : button->image;

    al_draw_scaled_bitmap(textureToDraw, 0, 0, al_get_bitmap_width(textureToDraw), al_get_bitmap_height(textureToDraw), button->x, button->y, button->width, button->height, 0);
}

void drawMenu(struct board* board, ALLEGRO_BITMAP* backgroundImage, ALLEGRO_BITMAP* titleImage, struct UIButton* playButton, struct UIButton* quitButton) {
    drawBackground(backgroundImage);

    float titleScale = 0.5;
    float titleWidth = al_get_bitmap_width(titleImage) * titleScale;
    float titleHeight = al_get_bitmap_height(titleImage) * titleScale;
    float titleCoordinateX = (SCREEN_WIDTH - titleWidth) / 2;
    float titleCoordinateY = 15;

    al_draw_scaled_bitmap(titleImage, 0, 0, al_get_bitmap_width(titleImage), al_get_bitmap_height(titleImage), titleCoordinateX, titleCoordinateY, titleWidth, titleHeight, 0);

    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);

    drawButton(playButton, mouseState.x, mouseState.y);
    drawButton(quitButton, mouseState.x, mouseState.y);
}

void handleMenuClick(struct board* board, int mouseButton, float mouseX, float mouseY, struct UIButton* playButton, struct UIButton* quitButton, bool* bIsFirstClick) {
    if (mouseButton == 1 && bIsMouseWithinButton(mouseX, mouseY, playButton)) {
        writeLog("User pressed the play button.");
        if (loadGameFromJson(board)) {
            *bIsFirstClick = false;
        }
        board->state = Running;
    }
    else if (mouseButton == 1 && bIsMouseWithinButton(mouseX, mouseY, quitButton)) {
        writeLog("User pressed the quit button.");
        board->state = Exit;
    }
}

void drawBoard(struct board* board, ALLEGRO_BITMAP* backgroundImage, ALLEGRO_BITMAP* hiddenTile, ALLEGRO_BITMAP* revealedTiles[], ALLEGRO_BITMAP* questionTile, ALLEGRO_BITMAP* flaggedTile, ALLEGRO_BITMAP* mineTile, ALLEGRO_BITMAP* wonText, ALLEGRO_BITMAP* lostText, ALLEGRO_BITMAP* subMessage, ALLEGRO_BITMAP* clockTile, ALLEGRO_FONT* pixelFont) {
    drawBackground(backgroundImage);

    float boardWidthPx = board->width * TILE_SIZE;
    float boardHeightPx = board->height * TILE_SIZE;

    float startX = (SCREEN_WIDTH - boardWidthPx) / 2;
    float startY = SCREEN_HEIGHT - boardHeightPx - 20;

    float width = al_get_bitmap_width(hiddenTile);
    float height = al_get_bitmap_height(hiddenTile);

    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {

            float currentX = startX + (j * TILE_SIZE);
            float currentY = startY + (i * TILE_SIZE);

            ALLEGRO_BITMAP* textureToDraw = NULL;

            switch (board->matrix[i][j].state) {
                case Hidden:
                    textureToDraw = hiddenTile;
                    break;

                case Flagged:
                    textureToDraw = flaggedTile;
                    break;

                case QuestionMarked:
                    textureToDraw = questionTile;
                    break;

                case Revealed:
                    if (board->matrix[i][j].bHasMine) {
                        textureToDraw = mineTile;
                    }
                    else {
                        textureToDraw = revealedTiles[board->matrix[i][j].adjacentMinesNum];
                    }
                    break;
            }

            if (textureToDraw != NULL) {
                al_draw_scaled_bitmap(textureToDraw, 0, 0, width, height, currentX, currentY, TILE_SIZE, TILE_SIZE, 0);
            }
        }
    }

    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    float iconScale = 0.5;

    float clockWidth = al_get_bitmap_width(clockTile) * iconScale;
    float clockHeight = al_get_bitmap_height(clockTile) * iconScale;
    float clockCoordinateX = startX - clockWidth - 140;
    float coordinateY = startY + boardHeightPx - clockHeight;
    al_draw_scaled_bitmap(clockTile, 0, 0, al_get_bitmap_width(clockTile), al_get_bitmap_height(clockTile), clockCoordinateX, coordinateY, clockWidth, clockHeight, 0);

    char timeText[5];
    snprintf(timeText, sizeof(timeText), "%d", board->timeCounter);
    float textYOffset = (clockHeight - al_get_font_line_height(pixelFont)) / 2;
    drawTextWithOutline(pixelFont, white, black, clockCoordinateX + clockWidth + 20, coordinateY + textYOffset, ALLEGRO_ALIGN_LEFT, timeText);

    float mineWidth = al_get_bitmap_width(mineTile) * iconScale;
    float mineHeight = al_get_bitmap_height(mineTile) * iconScale;
    float mineCoordinateX = startX + boardWidthPx + 140;
    al_draw_scaled_bitmap(mineTile, 0, 0, al_get_bitmap_width(mineTile), al_get_bitmap_height(mineTile), mineCoordinateX, coordinateY, mineWidth, mineHeight, 0);

    int minesLeft = board->minesNum - board->flagsPlacedNum;
    char minesText[4];
    snprintf(minesText, sizeof(minesText), "%d", minesLeft);
    drawTextWithOutline(pixelFont, white, black, mineCoordinateX - 20, coordinateY + textYOffset, ALLEGRO_ALIGN_RIGHT, minesText);

    if (board->state == Won || board->state == Lost) {
        ALLEGRO_BITMAP* message = board->state == Won ? wonText : lostText;

        float textScale = 0.25;
        float textWidth = al_get_bitmap_width(message) * textScale;
        float textHeight = al_get_bitmap_height(message) * textScale;
        float textCoordinateX = (SCREEN_WIDTH - textWidth) / 2;
        float textCoordinateY = 0;

        al_draw_scaled_bitmap(message, 0, 0, al_get_bitmap_width(message), al_get_bitmap_height(message), textCoordinateX, textCoordinateY, textWidth, textHeight, 0);

        float subMessageScale = 0.3225;
        float subMessageWidth = al_get_bitmap_width(subMessage) * subMessageScale;
        float subMessageHeight = al_get_bitmap_height(subMessage) * subMessageScale;
        float subMessageCoordinateX = (SCREEN_WIDTH - subMessageWidth) / 2;
        float subMessageCoordinateY = 70;

        al_draw_scaled_bitmap(subMessage, 0, 0, al_get_bitmap_width(subMessage), al_get_bitmap_height(subMessage), subMessageCoordinateX, subMessageCoordinateY, subMessageWidth, subMessageHeight, 0);
    }
}

void handleBoardClick(struct board* board, int mouseButton, float mouseX, float mouseY, bool* bIsFirstClick) {
    float boardWidthPx = board->width * TILE_SIZE;
    float boardHeightPx = board->height * TILE_SIZE;

    float startX = (SCREEN_WIDTH - boardWidthPx) / 2;
    float startY = SCREEN_HEIGHT - boardHeightPx - 20;

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
                writeLog("Revealed tile(s).");
                break;

            case 2:
                switch (board->matrix[i][j].state) {
                    case Revealed:
                        break;

                    case Hidden:
                        board->matrix[i][j].state = Flagged;
                        board->flagsPlacedNum += 1;
                        writeLog("Used placed a flag on a tile.");
                        break;

                    case Flagged:
                        board->matrix[i][j].state = QuestionMarked;
                        board->flagsPlacedNum -= 1;
                        writeLog("Used question marked a tile.");
                        break;

                    case QuestionMarked:
                        board->matrix[i][j].state = Hidden;
                        writeLog("Used unquestion marked a tile.");
                        break;
                }
        }
    }
}
