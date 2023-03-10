/* structs.h: definições de objetos */

typedef struct Entity Entity;

/* Como o nome sugere, essa estrutura age como um encarregado 
 * para lidar com as funções de lógica e desenho
 * no loop principal do jogo */
typedef struct
{
    void (*logic)(void);
    void (*draw)(void);
} Delegate;

/* A estrutura Game contém as referências ao 
 * renderizador e à janela */
typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} Game;

/* a estrutura Entity contém as referências às 
 * entidades do jogo, como o jogador e os inimigos */
struct Entity
{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    SDL_Texture *texture;
    Entity *next;
};

typedef struct
{
    Entity fighterHead, *fighterTail;
    Entity powerHead, *powerTail;
} Level;
