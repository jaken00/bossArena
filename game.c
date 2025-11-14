#include "game.h"

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
