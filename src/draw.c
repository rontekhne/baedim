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

/* carrega a imagem e retorna a textura */
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION,
        SDL_LOG_PRIORITY_INFO,
        "Loading %s",
        filename);

    texture = IMG_LoadTexture(game.renderer, filename);

    return texture;
}

/* desenha a textura na tela nas coordenadas x e y */
void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}


