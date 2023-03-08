/* input.c: gerencia a entrada */

#include "common.h"
#include "input.h"

extern Game game;

/* mais scancodes em https://wiki.libsdl.org/SDL_Scancode */

/* gerencia as teclas que são apertadas */
void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            game.up = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            game.down = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            game.left = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            game.right = 1;
        }
    }
}

/* gerencia as teclas que são liberadas  */
void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0) {
        if (event->keysym.scancode == SDL_SCANCODE_UP) {
            game.up = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
            game.down = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
            game.left = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
            game.right = 0;
        }
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
