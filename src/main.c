/* main.c: onde o jogo começa */

#include "common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

Game game;

int main(int argc, char *argv[])
{
    memset(&game, 0, sizeof(Game));
    initSDL();
    atexit(cleanup);

    while (true) {
        prepareScene();
        doInput();
        presentScene();
        SDL_Delay(16); /* em torno de 62 quadros por segundo */
    }
}
