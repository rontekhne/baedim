/* input.c: gerencia a entrada */

#include "common.h"
#include "input.h"

void doInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
}
