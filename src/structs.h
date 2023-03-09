/* structs.h: definições de objetos */

/* A estrutura Game contém as referências ao 
 * renderizador e à janela */
typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
    int power;
} Game;

/* a estrutura Entity contém as referências às 
 * entidades do jogo, como o jogador e os inimigos */
typedef struct 
{
    int x;
    int y;
    int dx;
    int dy;
    int health;
    SDL_Texture *texture;
} Entity;
