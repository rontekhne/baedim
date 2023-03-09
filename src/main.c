/* main.c: onde o jogo começa */

#include "common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

Game game;
Entity player;
Entity power;

int main(int argc, char *argv[])
{
    memset(&game, 0, sizeof(Game));
    memset(&player, 0, sizeof(Entity));
    memset(&power, 0, sizeof(Entity));

    initSDL();

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("gfx/player.png");

    power.texture = loadTexture("gfx/playerPower.png");
    atexit(cleanup);

    while (true) {
        prepareScene();
        doInput();

        player.x += player.dx;
        player.y += player.dy;

        if (game.up) { player.y -= 4; }
        if (game.down) { player.y += 4; }
        if (game.left) { player.x -= 4; }
        if (game.right) { player.x += 4; }

        if (game.power && power.health == 0) {
            power.x = player.x;
            power.y = player.y;
            power.dx = 16;
            power.dy = 0;
            power.health = 1;
        }

        power.x += power.dx;
        power.y += power.dy;

        if (power.x > SCREEN_WIDTH) {
            power.health = 0;
        }

        blit(player.texture, player.x, player.y);

        if (power.health > 0) {
            blit(power.texture, power.x, power.y);
        }

        presentScene();
        SDL_Delay(16); /* em torno de 62 quadros por segundo */
    }
}
