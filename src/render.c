#include "render.h"

// Static texture storage
static SDL_Texture *backgroundTexture = NULL;
static SDL_Texture *playerTexture = NULL;
static SDL_Texture *enemyTexture = NULL;
static SDL_Texture *playerFireballTexture = NULL;
static SDL_Texture *enemyFireballTexture = NULL;

void initRenderTextures(SDL_Renderer *renderer) {
    // Load background texture
    backgroundTexture = IMG_LoadTexture(renderer, "assets/backgroundResized.png");
    if (!backgroundTexture) {
        SDL_Log("Failed to load background texture: %s", IMG_GetError());
        // Try original if resized doesn't exist
        backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");
        if (!backgroundTexture) {
            SDL_Log("Failed to load background texture (original): %s", IMG_GetError());
        }
    }
    
    // Load player texture
    playerTexture = IMG_LoadTexture(renderer, "assets/playerResized.png");
    if (!playerTexture) {
        SDL_Log("Failed to load player texture: %s", IMG_GetError());
        // Try original if resized doesn't exist
        playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
        if (!playerTexture) {
            SDL_Log("Failed to load player texture (original): %s", IMG_GetError());
        }
    }
    
    // Load enemy texture
    enemyTexture = IMG_LoadTexture(renderer, "assets/enemyResized.png");
    if (!enemyTexture) {
        SDL_Log("Failed to load enemy texture: %s", IMG_GetError());
        // Try original if resized doesn't exist
        enemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
        if (!enemyTexture) {
            SDL_Log("Failed to load enemy texture (original): %s", IMG_GetError());
        }
    }
    
    // Load player fireball texture (10x10)
    playerFireballTexture = IMG_LoadTexture(renderer, "assets/playerFireball.png");
    if (!playerFireballTexture) {
        SDL_Log("Failed to load player fireball texture: %s", IMG_GetError());
    }
    
    // Load enemy fireball texture (10x10)
    enemyFireballTexture = IMG_LoadTexture(renderer, "assets/enemyFireball.png");
    if (!enemyFireballTexture) {
        SDL_Log("Failed to load enemy fireball texture: %s", IMG_GetError());
    }
}

void cleanupRenderTextures() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }
    if (playerTexture) {
        SDL_DestroyTexture(playerTexture);
        playerTexture = NULL;
    }
    if (enemyTexture) {
        SDL_DestroyTexture(enemyTexture);
        enemyTexture = NULL;
    }
    if (playerFireballTexture) {
        SDL_DestroyTexture(playerFireballTexture);
        playerFireballTexture = NULL;
    }
    if (enemyFireballTexture) {
        SDL_DestroyTexture(enemyFireballTexture);
        enemyFireballTexture = NULL;
    }
}

void renderBackground(SDL_Renderer *renderer) {
    if (backgroundTexture) {
        // Get texture dimensions or use screen size
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);
    } else {
        // Fallback to black background if texture not loaded
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
}

void renderEntities(SDL_Renderer *renderer, SDL_Rect *player, SDL_Rect *enemy){
    // Render player texture
    if (playerTexture) {
        SDL_RenderCopy(renderer, playerTexture, NULL, player);
    } else {
        // Fallback to colored rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, player);
    }
    
    // Render enemy texture
    if (enemyTexture) {
        SDL_RenderCopy(renderer, enemyTexture, NULL, enemy);
    } else {
        // Fallback to colored rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, enemy);
    }
}


void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int x = radius, y = 0, err = 0;
    while (x >= y) {
        SDL_RenderDrawLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderDrawLine(renderer, cx - y, cy + x, cx + y, cy + x);
        SDL_RenderDrawLine(renderer, cx - y, cy - x, cx + y, cy - x);
        SDL_RenderDrawLine(renderer, cx - x, cy - y, cx + x, cy - y);
        y++;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x--;
            err += 1 - 2 * x;
        }
    }
}

void draw_circle_outline(SDL_Renderer *renderer, int cx, int cy, int radius) {
    // Draw only the outline of the circle using Bresenham's algorithm
    int x = radius, y = 0, err = 0;
    while (x >= y) {
        // Draw 8 symmetric points on the circle
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);
        
        y++;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x--;
            err += 1 - 2 * x;
        }
    }
}

void renderProjectiles(SDL_Renderer* renderer, Enemy *enemy, Player *player) {
    // Render enemy projectiles with fireball texture (10x10)
    for(int i = 0; i < enemy->projectile_count; i++){
        Projectile* currentEnemyProjectile = &enemy->projectiles[i];
        if(currentEnemyProjectile->active == true){
            // Ensure projectile is 20x20
            SDL_Rect projRect = currentEnemyProjectile->projectileRect;
            projRect.w = 20;
            projRect.h = 20;
            
            if (enemyFireballTexture) {
                SDL_RenderCopy(renderer, enemyFireballTexture, NULL, &projRect);
            } else {
                // Fallback to colored rectangle if texture not loaded
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &projRect);
            }
        }
    }
    
    // Render player projectiles with fireball texture (10x10)
    for(int i = 0; i < player->projectile_count; i++){
        Projectile* currentPlayerProjectile = &player->projectiles[i];
        if(currentPlayerProjectile->active == true){
            // Ensure projectile is 20x20
            SDL_Rect projRect = currentPlayerProjectile->projectileRect;
            projRect.w = 20;
            projRect.h = 20;
            
            if (playerFireballTexture) {
                SDL_RenderCopy(renderer, playerFireballTexture, NULL, &projRect);
            } else {
                // Fallback to colored rectangle if texture not loaded
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &projRect);
            }
        }
    }
}