#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>

int main() {
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(5);
    al_install_mouse();

    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);

    al_rest(3.0);
    al_destroy_display(display);

    return 0;
}
