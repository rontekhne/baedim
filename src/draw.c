/* draw.c: funções relacionadas ao desenho e renderização */

#include "common.h"
#include "draw.h"

extern Game game;

/* prepara o renderizador */
void prepareScene(void)
{
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
    SDL_RenderClear(game.renderer);
}

/* renderiza! */
void presentScene(void)
{
    SDL_RenderPresent(game.renderer);
}
