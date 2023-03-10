/* input.c: gerencia a entrada */

#include "common.h"
#include "input.h"

extern Game game;

/* mais scancodes em https://wiki.libsdl.org/SDL_Scancode */

/* gerencia as teclas que são liberadas  */
void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        game.keyboard[event->keysym.scancode] = 0;
    }
}
/* gerencia as teclas que são apertadas */
void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        game.keyboard[event->keysym.scancode] = 1;
    }
}


void doInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    exit(EXIT_SUCCESS);
                }
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
            default:
                break;
        }
    }
}
