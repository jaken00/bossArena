#include "player.h"

Player createPlayer(){
    Player player;
    Health playerHP;
    playerHP.hp = 5;
    playerHP.maxHp = 8;
    player.health = playerHP;

    player.lastHitTime = 0;
    player.lastShootTime = 0;
    player.lastDashTime = 0;

    player.mana = 10;
    player.movespeed = 500.0 / 1000.0; //deivde by 1k for milisecond conversion

    SDL_Rect playerRect = {(SCREEN_WIDTH / 2) - 32, SCREEN_HEIGHT - 48, 50, 50};
    player.playerRect = playerRect;
    player.ability_count = 4;

    player.projectile_count = 0;
    player.projectile_capacity = 100;
    player.projectiles = malloc(sizeof(Projectile) * player.projectile_capacity);

    return player;

}

void freePlayer(Player *player){
    free(player);
}

void update_player_movement(Player *player, const Uint8 *keystate, SDL_Event *event, double deltaTime){
    double movement = player->movespeed * deltaTime;
    int dashDirectionX = 0;
    int dashDirectionY = 0;

    const Uint32 COOLDOWN_DURATION = 5000; 
    Uint32 now = SDL_GetTicks();
    bool canDashPlayer = (now - player->lastDashTime >= COOLDOWN_DURATION);

    if (keystate[SDL_SCANCODE_W] && player->playerRect.y > 0){
        player->playerRect.y -= movement;
        dashDirectionY = -1;
    }

    if (keystate[SDL_SCANCODE_S] && player->playerRect.y < SCREEN_HEIGHT - player->playerRect.h){
        player->playerRect.y += movement;
        dashDirectionY = 1;
    }

    if (keystate[SDL_SCANCODE_D] && player->playerRect.x < SCREEN_WIDTH - player->playerRect.w){
        player->playerRect.x += movement;
        dashDirectionX = 1;
    } 
    if (keystate[SDL_SCANCODE_A] && player->playerRect.x > 0){
        player->playerRect.x -= movement;
        dashDirectionX = -1;
    } 

    bool hasDir = (dashDirectionX != 0 || dashDirectionY != 0);

    if (keystate[SDL_SCANCODE_E] && canDashPlayer && hasDir){
        int dashDistance = 80;

        player->playerRect.x += dashDirectionX * dashDistance;
        player->playerRect.y += dashDirectionY * dashDistance;

        player->lastDashTime = now;
    }
}


void playerFire(Player *player, int mouseX, int mouseY){
    const Uint32 COOLDOWN_DURATION = 500;
    Uint32 now = SDL_GetTicks();

    bool canShoot = (now - player->lastShootTime >= COOLDOWN_DURATION);

    if(!canShoot) return;

    if (player->projectile_count >= player->projectile_capacity){
        player->projectile_count = 0; 
    }

    Projectile *proj = &player->projectiles[player->projectile_count++];
    
    double playerCx      = player->playerRect.x + player->playerRect.w / 2.0;
    double playerTopy    = player->playerRect.y;
    double playerBottomY = player->playerRect.y + player->playerRect.h;

    int projWidth = 20;
    int projHeight = 20;
    
    double trueStart;

    if(player->playerRect.y > SCREEN_HEIGHT / 2) {
        trueStart = playerTopy;
    }
    else{
        trueStart = playerBottomY;
    }

    SDL_Rect projectileRect = {
        (int)(playerCx - projWidth / 2.0),
        (int)(trueStart - projHeight / 2.0),
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

    double targetCx = (double)mouseX;
    double targetCy = (double)mouseY;

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

    player->lastShootTime = now;
}
void playerProjUpdate(Player *player, double deltaTime){
    for(int i = 0; i < player->projectile_count; i++){
        Projectile* currentProjectile = &player->projectiles[i];
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