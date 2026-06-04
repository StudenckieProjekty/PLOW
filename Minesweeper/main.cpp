#include "logic.h"
#include "ui.h"
#include <stdlib.h>
#include <time.h>

int main() {
    initAllegro();

    srand(time(NULL));

    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    ALLEGRO_BITMAP* windowIcon = al_load_bitmap("assets/images/icon.png");
    ALLEGRO_BITMAP* bgImage = al_load_bitmap("assets/images/background.png");
    ALLEGRO_BITMAP* titleImage = al_load_bitmap("assets/images/title.png");
    ALLEGRO_BITMAP* playBtn = al_load_bitmap("assets/images/play.png");
    ALLEGRO_BITMAP* playSelBtn = al_load_bitmap("assets/images/play_sel.png");
    ALLEGRO_BITMAP* quitBtn = al_load_bitmap("assets/images/quit.png");
    ALLEGRO_BITMAP* quitSelBtn = al_load_bitmap("assets/images/quit_sel.png");
    ALLEGRO_BITMAP* hiddenTile = al_load_bitmap("assets/images/tiles/barrel_bottom_spruce.png");
    ALLEGRO_BITMAP* revealedTiles[9];
    revealedTiles[0] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak.png");
    revealedTiles[1] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_1.png");
    revealedTiles[2] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_2.png");
    revealedTiles[3] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_3.png");
    revealedTiles[4] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_4.png");
    revealedTiles[5] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_5.png");
    revealedTiles[6] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_6.png");
    revealedTiles[7] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_7.png");
    revealedTiles[8] = al_load_bitmap("assets/images/tiles/barrel_bottom_oak_8.png");
    ALLEGRO_BITMAP* mineTile = al_load_bitmap("assets/images/tiles/tnt.png");

    al_set_display_icon(display, windowIcon);

    bool bIsFirstClick = true;

    struct board* gameBoard = (struct board*)malloc(sizeof(struct board));
    if (gameBoard == NULL) {
        return -1;
    }
    createBoard(gameBoard);

    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    while (gameBoard->state != GameState::Exit) {

        ALLEGRO_EVENT event;
        while (al_get_next_event(eventQueue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                gameBoard->state = GameState::Exit;
            }
            else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.button == 1) {
                    if (gameBoard->state == GameState::Running) {
                        handleBoardClick(gameBoard, event.mouse.x, event.mouse.y, &bIsFirstClick);
                    }
                }
            }
        }

        switch (gameBoard->state) {
            case GameState::Menu:
                drawMenu(gameBoard, bgImage, titleImage, playBtn, playSelBtn, quitBtn, quitSelBtn);
                break;

            case GameState::Running:
                drawBoard(gameBoard, bgImage, hiddenTile, revealedTiles, mineTile);
                break;
        }

        al_flip_display();
        al_rest(1.0 / 24.0);
    }

    al_destroy_event_queue(eventQueue);
    al_destroy_bitmap(bgImage);
    al_destroy_bitmap(titleImage);
    al_destroy_bitmap(windowIcon);
    al_destroy_bitmap(hiddenTile);
    al_destroy_display(display);

    free(gameBoard);

    return 0;
}
