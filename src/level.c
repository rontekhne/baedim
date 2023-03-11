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
static void doFighters(void);
static void doPower(void);
static void drawFighters(void);
static void drawPower(void);
static void spawnEnemies(void);

static Entity *player;
static SDL_Texture *powerTexture;
static SDL_Texture *enemyTexture;
static int enemySpawnTimer;

/* inicializa a fase */
void initLevel(void)
{
    game.delegate.logic = logic;
    game.delegate.draw = draw;

    memset(&level, 0, sizeof(Level));
    level.fighterTail = &level.fighterHead;
    level.powerTail = &level.powerHead;

    initPlayer();

    powerTexture = loadTexture("gfx/playerPower.png");
    enemyTexture = loadTexture("gfx/enemy.png");

    enemySpawnTimer = 0;
}

/* inicializa o jogador */
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

/* chama as funções de lógica */
static void logic(void)
{
    doPlayer();
    doFighters();
    doPower();
    spawnEnemies();
}

/* gerencia o comportamento do jogador */
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
}

/* configura o poder do jogador */
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

/* gerencia a vida das entidades, jogador e inimigo */
static void doFighters(void)
{
    Entity *e, *prev;

    prev = &level.fighterHead;

    for (e = level.fighterHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x < -e->w) {            
            if (e == level.fighterTail) {
                level.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}

/* gerencia a vida do poder do jogador */
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

/* configura a aparição do inimigo */
static void spawnEnemies(void)
{
    Entity *enemy;

    if (--enemySpawnTimer <= 0) {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        level.fighterTail->next = enemy;
        level.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
            
        
        enemy->dx = -(2 + (rand() % 4)); // velocidade (speed) entre -2 e -5   
        enemy->dy = player->dy / (PLAYER_SPEED * 2);

        //enemy->dy = player->dy;
        enemySpawnTimer = 30 + (rand() % 89); // novo enemy aparece entre 0.5 e 1.5 segundos ou 30 e 89 milisegundos
    }
}

/* chama as funções de desenho */
static void draw(void)
{
    drawFighters();
    drawPower();
}

static void drawFighters(void)
{
    Entity *e;
    
    for (e = level.fighterHead.next; e != NULL; e = e->next) {
        blit(e->texture, e->x, e->y);
    }
}

static void drawPower(void)
{
    Entity *p;

    for (p = level.powerHead.next; p != NULL; p = p->next) {
        blit(p->texture, p->x, p->y);
    }
}

