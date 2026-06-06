#include "logic.h"
#include "ui.h"
#include <stdlib.h>
#include <time.h>

int main() {
    initAllegro();

    srand(time(NULL));

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_BITMAP* icon = al_load_bitmap("assets/images/icon.png");
    ALLEGRO_BITMAP* background = al_load_bitmap("assets/images/background.png");
    ALLEGRO_BITMAP* title = al_load_bitmap("assets/images/title.png");
    ALLEGRO_BITMAP* wonText = al_load_bitmap("assets/images/won.png");
    ALLEGRO_BITMAP* lostText = al_load_bitmap("assets/images/lost.png");
    ALLEGRO_BITMAP* subMessage = al_load_bitmap("assets/images/subMessage.png");
    ALLEGRO_BITMAP* playButton = al_load_bitmap("assets/images/play.png");
    ALLEGRO_BITMAP* playButtonSelected = al_load_bitmap("assets/images/play_sel.png");
    ALLEGRO_BITMAP* quitButton = al_load_bitmap("assets/images/quit.png");
    ALLEGRO_BITMAP* quitButtonSelected = al_load_bitmap("assets/images/quit_sel.png");
    ALLEGRO_BITMAP* tileHidden = al_load_bitmap("assets/images/tiles/barrel_bottom_spruce.png");
    ALLEGRO_BITMAP* tileQM = al_load_bitmap("assets/images/tiles/barrel_bottom_spruce_qm.png");
    ALLEGRO_BITMAP* tileFlagged = al_load_bitmap("assets/images/tiles/barrel_bottom_spruce_flag.png");
    ALLEGRO_BITMAP* tileMine = al_load_bitmap("assets/images/tiles/tnt.png");
    ALLEGRO_BITMAP* clockIcon = al_load_bitmap("assets/images/clock.png");
    ALLEGRO_FONT* mcFont = al_load_ttf_font("assets/fonts/mine.otf", 36, 0);

    char filepath[] = "assets/images/tiles/barrel_bottom_oak_0.png";
    ALLEGRO_BITMAP* tileNumber[9];
    for (int i = 0; i <= 8; i++) {
        filepath[38] = '0' + i;
        tileNumber[i] = al_load_bitmap(filepath);
    }

    al_set_display_icon(display, icon);

    bool bIsFirstClick = true;

    struct board* gameBoard = (struct board*)malloc(sizeof(struct board));
    if (gameBoard == NULL) {
        return -1;
    }
    createBoard(gameBoard);

    float titleHeight = al_get_bitmap_height(title) * 0.5f;
    float playY = 15.0f + titleHeight;
    float quitY = playY + (al_get_bitmap_height(playButton) * 0.3f) + 30.0f;

    struct UIButton btnPlay = createCenteredButton(playButton, playButtonSelected, playY, 0.3f);
    struct UIButton btnQuit = createCenteredButton(quitButton, quitButtonSelected, quitY, 0.3f);

    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    while (gameBoard->state != GameState::Exit) {

        ALLEGRO_EVENT event;
        while (al_get_next_event(eventQueue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                gameBoard->state = GameState::Exit;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                switch (gameBoard->state) {
                    case GameState::Menu:
                        handleMenuClick(gameBoard, event.mouse.button, event.mouse.x, event.mouse.y, &btnPlay, &btnQuit);
                        break;

                    case GameState::Running:
                        handleBoardClick(gameBoard, event.mouse.button, event.mouse.x, event.mouse.y, &bIsFirstClick);
                        break;

                    case GameState::Won:
                    case GameState::Lost:
                        createBoard(gameBoard);
                        bIsFirstClick = true;
                        break;
                }
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (gameBoard->state == GameState::Won || gameBoard->state == GameState::Lost) {
                    createBoard(gameBoard);
                    bIsFirstClick = true;
                }
            }
        }

        if (gameBoard->timeCounter < 9999 && gameBoard->state == GameState::Running && !bIsFirstClick) {
            gameBoard->frameCounter += 1;
            if (gameBoard->frameCounter >= 24) {
                gameBoard->timeCounter += 1;
                gameBoard->frameCounter = 0;
            }
        }

        switch (gameBoard->state) {
            case GameState::Menu:
                drawMenu(gameBoard, background, title, &btnPlay, &btnQuit);
                break;

            case GameState::Running:
            case GameState::Won:
            case GameState::Lost:
                drawBoard(gameBoard, background, tileHidden, tileNumber, tileQM, tileFlagged, tileMine, wonText, lostText, subMessage, clockIcon, mcFont);
                break;
        }

        al_flip_display();
        al_rest(1.0 / 24.0);
    }

    al_destroy_event_queue(eventQueue);
    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);
    al_destroy_bitmap(title);
    al_destroy_bitmap(wonText);
    al_destroy_bitmap(lostText);
    al_destroy_bitmap(subMessage);
    al_destroy_bitmap(playButton);
    al_destroy_bitmap(playButtonSelected);
    al_destroy_bitmap(quitButton);
    al_destroy_bitmap(quitButtonSelected);
    al_destroy_bitmap(tileHidden);
    al_destroy_bitmap(tileQM);
    al_destroy_bitmap(tileFlagged);
    al_destroy_bitmap(tileMine);
    al_destroy_bitmap(clockIcon);
    for (int i = 0; i <= 8; i++) {
        al_destroy_bitmap(tileNumber[i]);
    }
    al_destroy_font(mcFont);
    al_destroy_display(display);

    free(gameBoard);

    return 0;
}
