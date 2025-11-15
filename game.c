#include "game.h"

void fireProjectile(Enemy *enemy, int targetX, int targetY){
    if (enemy->projectile_count >= enemy->projectile_capacity){
        enemy->projectile_count = 0; 
    }

    Projectile *proj = &enemy->projectiles[enemy->projectile_count++];
    
    double enemyCx      = enemy->enemyRect.x + enemy->enemyRect.w / 2.0;
    double enemyBottomY = enemy->enemyRect.y + enemy->enemyRect.h;

    int projWidth = 10;
    int projHeight = 10;

    SDL_Rect projectileRect = {
        (int)(enemyCx - projWidth / 2.0),
        (int)(enemyBottomY - projHeight / 2.0),
        projWidth,
        projHeight
    };
    proj->projectileRect = projectileRect;
    proj->posX = projectileRect.x;
    proj->posY = projectileRect.y;
    proj->active = true;
    proj->damageValue = 1;

    double projCx = proj->projectileRect.x + proj->projectileRect.w / 2.0;
    double projCy = proj->projectileRect.y + proj->projectileRect.h / 2.0;

    double targetCx = (double)targetX;
    double targetCy = (double)targetY;

    double dx = targetCx - projCx;
    double dy = targetCy - projCy;

    double length = sqrt(dx * dx + dy * dy);
    if (length == 0.0) {
        proj->velocityX = 0.0;
        proj->velocityY = 0.0;
        return;
    }

    double speed = 300.0;
    proj->velocityX = (dx / length) * speed;
    proj->velocityY = (dy / length) * speed;

}


void updateProjectile(Enemy *enemy, double deltaTime){
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentProjectile = &enemy->projectiles[i];
        if(currentProjectile->active == true){
            double deltaSeconds = deltaTime / 1000.0;
            currentProjectile->posX += currentProjectile->velocityX * deltaSeconds;
            currentProjectile->posY += currentProjectile->velocityY * deltaSeconds;

            currentProjectile->projectileRect.x = (int)currentProjectile->posX;
            currentProjectile->projectileRect.y = (int)currentProjectile->posY;
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
            SDL_RenderFillRect(renderer, &currentProjectile->projectileRect);

        }
    }
}

void enemyProjectileCollisionCall(Player *player, Enemy *enemy){
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentProjectile = &enemy->projectiles[i];
        bool hit = projectileHit(&player->playerRect, &currentProjectile->projectileRect);
        if(hit){
            takeDamage(&player->health, currentProjectile->damageValue);
        }

    }
}

bool projectileHit(SDL_Rect *target, SDL_Rect *source){
    int targetLeft = target->x;
    int targetRight = target->x + target->w;
    int targetTop = target->y;
    int TargetBot = target->y + target->h;

    int sourceLeft = source->x;
    int sourceRight = source->x + source->w;
    int sourceTop = source->y;
    int sourceBot = source->y + source->h;

    if(sourceLeft >= targetRight){
        return false;
    }
    if(sourceRight <= targetLeft){
        return false;
    }
    if(sourceTop >= TargetBot){
        return false;
    }
    if(sourceBot <= targetTop){
        return false;
    }

    return true;

}

void takeDamage(Health *health, int damage){
    health->hp -= damage;
}


void monitorEnemyPhase(Enemy *enemy){
    if(enemy->health.hp <= 64){
    enemy->phase = 2;
    }
    else if (enemy->health.hp <= 48)
    {
        enemy->phase = 3;
    }
    else if (enemy->health.hp <= 32){
        enemy->phase = 4;
    }
    else{
        enemy->phase = 1;
    }
}






Enemy createEnemy(){
    Enemy enemy;
    Health enemyHP;
    enemyHP.hp = 80;
    enemyHP.maxHp = 80;

    enemy.health = enemyHP;
    enemy.phase = 1;
    enemy.movespeed = 200.0 / 1000.0; //conver to milies!
    SDL_Rect enemyRect = {SCREEN_WIDTH / 2, 50, 64, 64}; // NEED CHANGE THESE TO DEFINE CONSTS
    enemy.enemyRect = enemyRect;
    enemy.ability_count = 5;
    
    //projectile logic
    enemy.projectile_count = 0;
    enemy.projectile_capacity = 100;
    enemy.projectiles = malloc(sizeof(Projectile) * enemy.projectile_capacity);

    return enemy;
}

Player createPlayer(){
    Player player;
    Health playerHP;
    playerHP.hp = 5;
    playerHP.maxHp = 8;
    player.health = playerHP;

    player.mana = 10;
    player.movespeed = 500.0 / 1000.0; //deivde by 1k for milisecond conversion

    SDL_Rect playerRect = {10, 10, 32, 32};
    player.playerRect = playerRect;
    player.ability_count = 4;


    return player;
}

void freePlayer(Player *player){
    free(player);
}

void update_player_movement(Player *player, const Uint8 *keystate, double deltaTime){
        double movement = player->movespeed * deltaTime;

        if (keystate[SDL_SCANCODE_W] && player->playerRect.y > 0){
                player->playerRect.y -= movement;
            }

        if (keystate[SDL_SCANCODE_S] && player->playerRect.y < SCREEN_HEIGHT - player->playerRect.h){
                player->playerRect.y += movement;
        
            }

        if (keystate[SDL_SCANCODE_D] && player->playerRect.x < SCREEN_WIDTH - player->playerRect.w){
            player->playerRect.x += movement;
        } 
        if (keystate[SDL_SCANCODE_A] && player->playerRect.x > 0){
            player->playerRect.x -= movement;
        } 
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

