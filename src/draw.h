/* draw.h: arquivo de cabeçalho para draw.c */

void prepareScene(void);
void presentScene(void);
SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture *texture, int x, int y);
