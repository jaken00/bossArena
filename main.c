//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

/*
1. Fix projectile spawns for Enemy
2. Copy Projectile for player off of Space button
3. Refactor abilities for enemy -> whole rework. 
    3.1 Rather than having abilties change we are going to just overlap so ever 3 seconds a projectile then 
    a cirlce will spawn under 64 hp then half the screen then 32 hp projectiles shot in cone then at 16hp frequence drops down to 2 seconds
4. Rework player abilties
5. get player dash working on E
6. Reflect working on Q
7. Textures added into game as well

*/

typedef struct Ability{
    int cooldown;
    int damage;
    char name[50];
    bool draw;
    SDL_Rect abilityRect;
    SDL_Scancode hotkey;
} Ability;

typedef struct Projectile{
    SDL_Rect projectileRect;
    double velocityX;
    double velocityY;
    bool active;
} Projectile;

typedef struct {
    int health;
    int mana;
    double movespeed;
    SDL_Rect playerRect;
    Ability** abilities;
    size_t ability_count;
} Player;

typedef struct {
    int health;
    int phase;
    double movespeed;    
    SDL_Rect enemyRect;
    Ability** abilities;
    size_t ability_count;
    
    Projectile* projectiles;
    size_t projectile_count;
    size_t projectile_capacity;
} Enemy;

void fireProjectile(Enemy *enemy, int targetX, int targetY){
    if (enemy->projectile_count >= enemy->projectile_capacity){
        enemy->projectile_count = 0; 
    }

    Projectile *proj = &enemy->projectiles[enemy->projectile_count];
    enemy->projectile_count++;
    SDL_Rect projectileRect = {enemy->enemyRect.x + (enemy->enemyRect.w /2), enemy->enemyRect.y + enemy->enemyRect.h, 8, 4};
    proj->projectileRect = projectileRect;
    proj->active = true;
    
    int speed = 100;

    int differenceX = targetX - enemy->enemyRect.x;
    int differenceY = targetY - enemy->enemyRect.y;

    double length = sqrt(differenceX * differenceX + differenceY * differenceY);

    proj->velocityX = (differenceX / length) * speed;
    proj->velocityY = (differenceY / length) * speed;


}

void updateProjectile(Enemy *enemy, double deltaTime){
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentProjectile = &enemy->projectiles[i];
        if(currentProjectile->active == true){
            currentProjectile->projectileRect.x += ((double)currentProjectile->velocityX / 1000) * deltaTime;
            currentProjectile->projectileRect.y += ((double)currentProjectile->velocityY / 1000) * deltaTime;
        }

        if(currentProjectile->projectileRect.x > SCREEN_WIDTH || currentProjectile->projectileRect.y > SCREEN_HEIGHT + currentProjectile->projectileRect.h){
            currentProjectile->active = false;
        }
    }
}

void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy){
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentProjectile = &enemy->projectiles[i];
        if(currentProjectile->active == true){
            SDL_RenderDrawRect(renderer, &currentProjectile->projectileRect);
        }
    }
}

void monitorEnemyPhase(Enemy *enemy){
    if(enemy->health <= 64){
    enemy->phase = 2;
    }
    else if (enemy->health <= 48)
    {
        enemy->phase = 3;
    }
    else if (enemy->health <= 32){
        enemy->phase = 4;
    }
    else{
        enemy->phase = 1;
    }
}

Ability* createAbility(char name[6], int damage, int cooldown ,SDL_Scancode hotkey, SDL_Rect abilityRect){
    Ability* ability = malloc(sizeof(Ability));

    strncpy(ability->name, name, sizeof ability->name);
    ability->damage = damage;
    ability->cooldown = cooldown;
    ability->hotkey = hotkey;
    ability->abilityRect = abilityRect;
    ability->draw = false;

    return ability;
}

Enemy createEnemy(){
    Enemy enemy;

    enemy.health = 80;
    enemy.phase = 1;
    enemy.movespeed = 200.0 / 1000.0; //conver to milies!
    SDL_Rect enemyRect = {SCREEN_WIDTH / 2, 50, 64, 64}; // NEED CHANGE THESE TO DEFINE CONSTS
    enemy.enemyRect = enemyRect;
    enemy.ability_count = 5;
    enemy.abilities = malloc(sizeof *enemy.abilities * enemy.ability_count);
    SDL_Rect placeHolderRect = {enemyRect.x,enemyRect.y+32+10,20,20};
    enemy.abilities[0] = createAbility("BOSS1", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[1] = createAbility("BOSS2", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[2] = createAbility("BOSS3", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[3] = createAbility("BOSS4", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[4] = createAbility("BOSS5", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);

    enemy.projectile_count = 0;
    enemy.projectile_capacity = 100;

    enemy.projectiles = malloc(sizeof(Projectile) * enemy.projectile_capacity);

    return enemy;
}

Player createPlayer(){
    Player player;
    player.health = 8;
    player.mana = 10;
    player.movespeed = 500.0 / 1000.0; //deivde by 1k for milisecond conversion

    SDL_Rect playerRect = {10, 10, 32, 32};
    player.playerRect = playerRect;
    player.ability_count = 4;

    player.abilities = malloc(sizeof *player.abilities * player.ability_count);
    SDL_Rect placeHolderRect = {0,0,20,20};
    player.abilities[0] = createAbility("TEST", 10, 10, SDL_SCANCODE_Q, placeHolderRect);
    player.abilities[1] = createAbility("TEST2", 10, 10, SDL_SCANCODE_E, placeHolderRect);
    player.abilities[2] = createAbility("TEST3", 10, 10, SDL_SCANCODE_F, placeHolderRect);
    SDL_Rect basic_shot_rect = {0,0,5,5};
    player.abilities[3] = createAbility("Basic Shot", 1, 0, SDL_BUTTON_LEFT, basic_shot_rect);

    return player;
}

void freePlayer(Player *player){
    for(int i = 0; i < 3; i++){
        free(player->abilities[i]);
    }

    free(player->abilities);
    free(player);
}

int main(int argc, char* argv[]) {
    //INIT FUNCTIONS
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow(
        "Boss Arena",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
    if(renderer == NULL){
        printf("Unable to Create Renderer");
    }

    //PLAYER AND ENEMY
    Player player = createPlayer();
    Enemy enemy = createEnemy();

    //DELTA TIME
    bool running = true;
    int last = SDL_GetPerformanceCounter();

    double attackTimer = 0.0;
    const double ATTACK_INTERVAL = 3000.0; //maybe turn into define? might not be needed


    SDL_Rect enemyAbilityRect;
    SDL_ShowCursor(true);
    while(running){
        /*  DELTA TIME    */
        int now = SDL_GetPerformanceCounter();
        double deltaTime = 0;
        deltaTime = (double)((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
        last = now;

        attackTimer += deltaTime;


        
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
        }

        int mouseX, mouseY;
        Uint32 button_pos = SDL_GetMouseState(&mouseX, &mouseY);

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_Q]){
            running = false;
        }
        if(keystate[SDL_SCANCODE_W]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.y -= movement_calcualtion;
        }
        if(keystate[SDL_SCANCODE_S]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.y += movement_calcualtion;
        }
        if(keystate[SDL_SCANCODE_D]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.x += movement_calcualtion;
        }
        if(keystate[SDL_SCANCODE_A]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.x -= movement_calcualtion;
        }

        if(attackTimer >= ATTACK_INTERVAL){

            monitorEnemyPhase(&enemy);
            fireProjectile(&enemy, player.playerRect.x + (player.playerRect.w /2), player.playerRect.y + (player.playerRect.h /2));
            printf("Player pos X: %d\n", player.playerRect.x);

            attackTimer = 0;
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // need to first drwa balck background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &player.playerRect);
        SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
        SDL_RenderDrawRect(renderer, &enemy.enemyRect);
        SDL_SetRenderDrawColor(renderer, 0,0,255,255);
        updateProjectile(&enemy, deltaTime);
        drawProjectiles(renderer, &enemy);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    freePlayer(&player);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}