/* structs.h: definições de objetos */

/* A estrutura Game contém as referências ao 
 * renderizador e à janela */
typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;
