/* main.c: onde o jogo começa */

#include "common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "level.h"
#include "main.h"

Game game;
Level level;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
    long then;
    float remainder;

    memset(&game, 0, sizeof(Game));
    memset(&SCREEN_WIDTH, 0, sizeof(int));
    memset(&SCREEN_HEIGHT, 0, sizeof(int));

    initSDL();
    atexit(cleanup);
    initLevel();
    
    while (true) {
        prepareScene();
        doInput();
        game.delegate.logic();
        game.delegate.draw();
        presentScene();
        capFrameRate(&then, &remainder);
    }

    return 0;
}

static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;
    *remainder -= (int)*remainder;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;

    if (wait < 1) {
        wait = 1;
    }

    SDL_Delay(wait);
    *remainder += 0.667;
    *then = SDL_GetTicks();
}
