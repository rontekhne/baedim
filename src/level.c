/* level.c: responsável pela lógica do jogo */

#include "common.h"
#include "draw.h"
#include "level.h"
#include "util.h"

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
static void enemyChasePlayer(Entity *e); 
static void drawFighters(void);
static void drawPower(void);
static void spawnEnemies(void);
static int powerHitFighter(Entity *p);
static void doEnemies(void);
static void throwEnemyPower(Entity *e);
static void clipPlayer(void);
static void resetLevel(void);

static Entity *player;
static SDL_Texture *powerTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *enemyPowerTexture;
static SDL_Texture *playerTexture;
static int enemySpawnTimer;
static int levelResetTimer;

/* inicializa a fase */
void initLevel(void)
{
    game.delegate.logic = logic;
    game.delegate.draw = draw;

    memset(&level, 0, sizeof(Level));
    level.fighterTail = &level.fighterHead;
    level.powerTail = &level.powerHead;

    powerTexture = loadTexture("gfx/playerPower.png");
    enemyTexture = loadTexture("gfx/enemy.png");
    enemyPowerTexture = loadTexture("gfx/enemyPower.png");
    playerTexture = loadTexture("gfx/player.png");

    resetLevel();
}

/* reseta as linked lists e times do level */
static void resetLevel(void)
{
    Entity *e;

    while (level.fighterHead.next) {
        e = level.fighterHead.next;
        level.fighterHead.next = e->next;
        free(e);
    }

    while (level.powerHead.next) {
        e = level.powerHead.next;
        level.powerHead.next = e->next;
        free(e);
    }

    memset(&level, 0, sizeof(Level));
    level.fighterTail = &level.fighterHead;
    level.powerTail = &level.powerHead;

    initPlayer();
    enemySpawnTimer = 0;
    levelResetTimer = FPS * 2;
}

/* inicializa o jogador */
static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    level.fighterTail->next = player;
    level.fighterTail = player;

    player->health = 5;
    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    
    player->side = SIDE_PLAYER;
}

/* chama as funções de lógica */
static void logic(void)
{
    doPlayer();
    doEnemies();
    doFighters();
    doPower();
    spawnEnemies();
    clipPlayer();

    if (player == NULL && --levelResetTimer <= 0) {
        resetLevel();
    }
}

/* gerencia o comportamento do jogador */
static void doPlayer(void)
{
    if (player != NULL) {
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
    power->side = player->side; // ???
    SDL_QueryTexture(power->texture, NULL, NULL, &power->w, &power->h);

    power->y += (player->h / 2) - (power->h / 2);

    power->side = SIDE_PLAYER; // ???
                               
    player->reload = 16;
}

/* gerencia a vida do inimigo e sua habilidade ou power */
static void doEnemies(void)
{
    Entity *e;

    for (e = level.fighterHead.next; e != NULL; e = e->next) {
        if (e != player && player != NULL && --e->reload <= 0) {
            throwEnemyPower(e);
        }
    }
}

/* gerencia o power do enemy */
static void throwEnemyPower(Entity *e)
{
    Entity *power;

    power = malloc(sizeof(Entity));
    memset(power, 0, sizeof(Entity));
    level.powerTail->next = power;
    level.powerTail = power;

    power->x = e->x;
    power->y = e->y;
    power->health = 1;
    power->texture = enemyPowerTexture;
    power->side = e->side;
    SDL_QueryTexture(power->texture, NULL, NULL, &power->w, &power->h);

    power->x += (e->w / 2) - (power->w / 2);
    power->y += (e->h / 2) - (power->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &power->dx, &power->dy);

    power->dx *= ENEMY_POWER_SPEED;
    power->dy *= ENEMY_POWER_SPEED;

    power->side = SIDE_ENEMY;

    e->reload = (rand() % FPS * 2);
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
            e->health = 0;
        }

        if (e->health == 0) {
            if (e == player) {
                player = NULL;
            }

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

        if (powerHitFighter(p) || p->x < -p->w || p->y < -p->h || p->x > SCREEN_WIDTH || p->y > SCREEN_HEIGHT) {
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

/* gerencia a colisão entre a power do player e o enemy */
static int powerHitFighter(Entity *p)
{
    Entity *e;

    for (e = level.fighterHead.next; e != NULL; e = e->next) {
        if (e->side != p->side && collision(p->x, p->y, p->w, p->h, e->x, e->y, e->w, e->h)) {
            if (e == player) {
                --player->health;
            }else {
                p->health = 0; // p é de power, da função doPower
                e->health = 0;
            }

            return 1;
        }
    }

    return 0;
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
       
        //enemy->dx = -(2 + (rand() % 4)); // velocidade (speed) entre -2 e -5   
        enemy->side = SIDE_ENEMY;
        enemy->health = 1;
        enemy->reload = FPS * (1 + (rand() % 3));
        enemySpawnTimer = 30 + (rand() % FPS); // novo enemy aparece
    }
}

/* chama as funções de desenho */
static void draw(void)
{
    drawFighters();
    drawPower();
}

/* limita o player para não avançar além de onde deve */
static void clipPlayer(void)
{
    if (player != NULL) {
        if (player->x < 0) {
            player->x = 0;
        }
        if (player->y < 0) {
            player->y = 0;
        }
        if (player->x > SCREEN_WIDTH / 2) {
            player->x = SCREEN_WIDTH / 2;
        }
        if (player->y > SCREEN_HEIGHT - player->h) {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}

static void enemyChasePlayer(Entity *e)
{
    double hyp; // normaliza
   
    if (player != NULL) {
        // calcula a direção entre o player e o enemy
        e->dx = (double)(player->x - e->x);
        e->dy = (double)(player->y - e->y);
            
        // normaliza, isto é, divide os termos pela magnitude (hipotenusa)            
        hyp = sqrt(e->dx*e->dx + e->dy*e->dy);
        e->dx /= hyp;
        e->dy /= hyp;

        // adiciona a direção à posição do enemy (e multiplica) 
        // (ainda precisa definir a velocidade que será multiplicada pela direção)
        e->x += e->dx; // * speed
        e->y += e->dy; // * speed
    }
}

static void drawFighters(void)
{
    Entity *e;
    
    for (e = level.fighterHead.next; e != NULL; e = e->next) {
        // enemy anda no sentido do player até matá-lo ou ser morto
        if (e != player) {
            enemyChasePlayer(e);
        }  

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

