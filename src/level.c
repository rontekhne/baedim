/* level.c: responsável pela lógica do jogo */

#include "common.h"
#include "draw.h"
#include "level.h"

extern Game game;
extern Level level;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void throwPower(void);
static void doPlayer(void);
static void doPower(void);
static void drawPlayer(void);
static void drawPower(void);

static Entity *player;
static SDL_Texture *powerTexture;

void initLevel(void)
{
    game.delegate.logic = logic;
    game.delegate.draw = draw;

    memset(&level, 0, sizeof(Level));
    level.fighterTail = &level.fighterHead;
    level.powerTail = &level.powerHead;

    initPlayer();

    powerTexture = loadTexture("gfx/playerPower.png");
}

static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    level.fighterTail->next = player;
    level.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("gfx/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void)
{
    doPlayer();
    doPower();
}

static void doPlayer(void)
{
    player->dx = player->dy = 0;

    if (player->reload > 0) {
        player->reload--;
    }

    if (game.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }
    if (game.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }
    if (game.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }
    if (game.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }
    if (game.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0) {
        throwPower();
    }
    player->x += player->dx;
    player->y += player->dy;
}

static void throwPower(void)
{
    Entity *power;

    power = malloc(sizeof(Entity));
    memset(power, 0, sizeof(Entity));
    level.powerTail->next = power;
    level.powerTail = power;

    power->x = player->x;
    power->y = player->y;
    power->dx = PLAYER_POWER_SPEED;
    power->health = 1;
    power->texture = powerTexture;
    SDL_QueryTexture(power->texture, NULL, NULL, &power->w, &power->h);

    power->y += (player->h / 2) - (power->h / 2);
    player->reload = 8;
}

static void doPower(void)
{
    Entity *p, *prev;

    prev = &level.powerHead;

    for (p = level.powerHead.next; p != NULL; p = p->next) {
        p->x += p->dx;
        p->y += p->dy;

        if (p->x > SCREEN_WIDTH) {
            if (p == level.powerTail) {
                level.powerTail = prev;
            }
            prev->next = p->next;
            free(p);
            p = prev;
        }
        prev = p;
    }
}

static void draw(void)
{
    drawPlayer();
    drawPower();
}

static void drawPlayer(void)
{
    blit(player->texture, player->x, player->y);
}

static void drawPower(void)
{
    Entity *p;

    for (p = level.powerHead.next; p != NULL; p = p->next) {
        blit(p->texture, p->x, p->y);
    }
}

