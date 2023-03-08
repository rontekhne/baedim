/* main.c: onde o jogo começa */

#include "common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

Game game;
Entity player;

int main(int argc, char *argv[])
{
    memset(&game, 0, sizeof(Game));
    memset(&player, 9, sizeof(Entity));
    initSDL();

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("gfx/player.png");

    atexit(cleanup);

    while (true) {
        prepareScene();
        doInput();

        if (game.up) { player.y -= 4; }
        if (game.down) { player.y += 4; }
        if (game.left) { player.x -= 4; }
        if (game.right) { player.x += 4; }

        blit(player.texture, player.x, player.y);

        presentScene();
        SDL_Delay(16); /* em torno de 62 quadros por segundo */
    }
}
