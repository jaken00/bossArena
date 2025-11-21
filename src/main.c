//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "ui.h"
#include "render.h"

int main(int argc, char* argv[]) {
    //INIT FUNCTIONS
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
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
    
    // Initialize render textures
    initRenderTextures(renderer);

    //PLAYER AND ENEMY
    Player player = createPlayer();
    Enemy enemy = createEnemy();

    //DELTA TIME
    bool running = true;
    int last = SDL_GetPerformanceCounter();

    Sprite hearts;
    
    const double animation_frame_time = 100.0;

    hearts = spriteLoad(renderer, "assets/hearty_strip6.png", 64, 64, 6);
    sprite_SetFrame(&hearts, 0);

    
    double animation_timer = 0.0;
    double attackTimer = 0.0;
    
    SDL_Rect enemyAbilityRect;
    SDL_ShowCursor(true);


    bool circleActive = false;
    int circleCx, circleCy, circleRadius;
    double circleTimer = 0.0;
    bool circleDamagePhase = false;
    const double CIRCLE_WARNING_TIME = 500.0; // 0.5 seconds in milliseconds
    const double CIRCLE_DAMAGE_TIME = 5000.0; // 5 seconds in milliseconds

    while(running){
        /*  DELTA TIME    */
        int now = SDL_GetPerformanceCounter();
        double deltaTime = 0;
        deltaTime = (double)((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
        last = now;

        animation_timer += deltaTime;
        SDL_Event e;
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
        }

        int mouseX, mouseY;
        Uint32 mouse_pos = SDL_GetMouseState(&mouseX, &mouseY);

        update_player_movement(&player, keystate, &e, deltaTime);

        if(keystate[SDL_SCANCODE_Q]){
            running = false;
        }
        if(keystate[SDL_SCANCODE_SPACE]){
            playerFire(&player, mouseX, mouseY);
        }

        
        enemy_attack_timer(deltaTime, &attackTimer, &enemy, player);
        enemyProjectileCollisionCall(&player, &enemy);
        playerProjectileCollisionCall(&player, &enemy);
        move_enemy(&enemy, &player.playerRect, deltaTime);


        // Render background
        renderBackground(renderer);
        
        // Render entities
        renderEntities(renderer, &player.playerRect, &enemy.enemyRect);
        updateProjectile(&enemy, deltaTime);
        playerProjUpdate(&player, deltaTime);
        renderProjectiles(renderer, &enemy, &player);
        if(enemyP2Attack(&enemy)){
            circleActive = true;
            circleTimer = 0.0;
            circleDamagePhase = false;
            circleRadius = randomRadius();
            circleCx = circleRadius + rand() % (SCREEN_WIDTH - 2 * circleRadius);
            circleCy = circleRadius + rand() % (SCREEN_HEIGHT - 2 * circleRadius);
        }

        if(circleActive){
            circleTimer += deltaTime;
            
            // Check if we should transition from warning to damage phase
            if(circleTimer >= CIRCLE_WARNING_TIME && !circleDamagePhase){
                circleDamagePhase = true;
            }
            
            if(circleTimer >= CIRCLE_WARNING_TIME + CIRCLE_DAMAGE_TIME){
                circleActive = false;
                circleDamagePhase = false;
            } else {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                if(circleDamagePhase){
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 120);
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 80);
                }
                draw_filled_circle(renderer, circleCx, circleCy, circleRadius);
                
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                
                if(circleDamagePhase){
                    int playerCenterX = player.playerRect.x + player.playerRect.w / 2;
                    int playerCenterY = player.playerRect.y + player.playerRect.h / 2;
                    
                    int dx = playerCenterX - circleCx;
                    int dy = playerCenterY - circleCy;
                    double distance = sqrt(dx * dx + dy * dy);
                    
                    if(distance <= circleRadius){
                        const Uint32 COOLDOWN_DURATION = 500;
                        Uint32 now = SDL_GetTicks();
                        bool canCollidePlayer = (now - player.lastHitTime >= COOLDOWN_DURATION);
                        
                        if(canCollidePlayer){
                            takeDamage(&player.health, 1); 
                            player.lastHitTime = now;
                        }
                    }
                }
            }
        }
        

        if(animation_timer >= animation_frame_time){
            animation_timer -= animation_frame_time;
            int next = hearts.current_frame += 1;

            if(next >= hearts.frame_count){
                next = 0;
            }
            sprite_SetFrame(&hearts, next);
        }

        render_sprite_per_health(renderer, &hearts, player.health.hp);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    freePlayer(&player);
    cleanupRenderTextures();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}