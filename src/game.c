#include "game.h"




void drawProjectiles(SDL_Renderer* renderer, Enemy *enemy, Player *player){
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentEnemyProjectile = &enemy->projectiles[i];
        if(currentEnemyProjectile->active == true){
            SDL_RenderFillRect(renderer, &currentEnemyProjectile->projectileRect);

        }
    }
    for(int i = 0; i < player->projectile_count; i++){
        Projectile* currentPlayerProjectile = &player->projectiles[i];

        if(currentPlayerProjectile->active == true){
            SDL_RenderFillRect(renderer, &currentPlayerProjectile->projectileRect);
        }
    }
}

void enemyProjectileCollisionCall(Player *player, Enemy *enemy){

    const Uint32 COOLDOWN_DURATION = 500;
    Uint32 now = SDL_GetTicks();

    bool canCollidePlayer = (now - player->lastHitTime >= COOLDOWN_DURATION);

    if(!canCollidePlayer) return;

    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentProjectile = &enemy->projectiles[i];
        if(!currentProjectile->active){
            continue;
        }

        if(SDL_HasIntersection(&player->playerRect, &currentProjectile->projectileRect)){
            currentProjectile->active = false;
            takeDamage(&player->health, currentProjectile->damageValue);
            player->lastHitTime = now;
            break;
        }
    }
}

void playerProjectileCollisionCall(Player *player, Enemy *enemy) {
    const Uint32 COOLDOWN_DURATION = 500;
    Uint32 now = SDL_GetTicks();

    bool canCollideEnemy = (now - enemy->lastHitTime >= COOLDOWN_DURATION);

    if(!canCollideEnemy) return;

    for(int i = 0; i < player->projectile_count; i++){
        Projectile* currentProjectile = &player->projectiles[i];
        if(!currentProjectile->active){
            continue;
        }

        if(SDL_HasIntersection(&enemy->enemyRect, &currentProjectile->projectileRect)){
            currentProjectile->active = false;
            takeDamage(&enemy->health, currentProjectile->damageValue);
            printf("ENEMY HP: %d\n", enemy->health.hp);
            printf("ENEMY PHASE: %d\n", enemy->phase);
            enemy->lastHitTime = now;
            break;
        }
    }
}


void takeDamage(Health *health, int damage){
    health->hp -= damage;
}




void enemy_attack_timer(double deltaTime, double *attackTimer, Enemy *enemy, Player player){
    *attackTimer += deltaTime;
    if(*attackTimer >= ATTACK_INTERVAL){

        monitorEnemyPhase(enemy);
        fireProjectile(
            enemy,
            player.playerRect.x + player.playerRect.w / 2,
            player.playerRect.y + player.playerRect.h / 2
        );

        *attackTimer = 0;
    }
}

void move_enemy(Enemy *enemy, SDL_Rect *playerRect, double deltaTime){
    double movespeed = (enemy->movespeed/1000) * deltaTime;
    int diffx = enemy->enemyRect.x - playerRect->x;
    int diffy = enemy->enemyRect.y - playerRect->y;


    if(diffx > 0){
        enemy->enemyRect.x -= playerRect->x * movespeed;
    }
    if(diffx < 0){
        enemy->enemyRect.x += playerRect->x * movespeed;
    }
    if(diffy > 0){
        enemy->enemyRect.y -= playerRect->y * movespeed;
    }
    if(diffy < 0){
        enemy->enemyRect.y += playerRect->y * movespeed;
    }

}

