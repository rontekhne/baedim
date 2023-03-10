/* init.c: funções que configuram a SDL */

#include "common.h"
#include "init.h"

extern Game game;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

void initSDL(void)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;//SDL_WINDOW_OPENGL | /*SDL_WINDOW_RESIZABLE |*/ SDL_WINDOW_MAXIMIZED; 

    /* inicia o modo de vídeo da SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* cria a janela */
    game.window = SDL_CreateWindow(
        "Baedim",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, //SCREEN_WIDTH,
        0, //SCREEN_HEIGHT,
        windowFlags);

    if (!game.window) {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* uma dica que especifica a qualidade do dimensionamento para linear */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    /* cria o renderizador */
    game.renderer = SDL_CreateRenderer(game.window, -1, rendererFlags);

    if (!game.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* tela cheia */
    SDL_SetWindowFullscreen(game.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    /* pega o tamanho da janela */
    SDL_GetRendererOutputSize(game.renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    printf("SCREEN_WIDTH = %d\nSCREEN_HEIGHT = %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    /* desabilita o cursor */
    SDL_ShowCursor(SDL_DISABLE);

    /* inicializa o suporte a imagens jpg e png */
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void cleanup(void)
{
    IMG_Quit();
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
}

