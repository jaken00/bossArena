//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "game.h"
#include "ui.h"
#include "render.h"

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

    Sprite hearts;
    
    const double animation_frame_time = 100.0;

    hearts = spriteLoad(renderer, "assets/hearty_strip6.png", 64, 64, 6);
    sprite_SetFrame(&hearts, 0);

    
    double animation_timer = 0.0;
    double attackTimer = 0.0;
    
    SDL_Rect enemyAbilityRect;
    SDL_ShowCursor(true);
    while(running){
        /*  DELTA TIME    */
        int now = SDL_GetPerformanceCounter();
        double deltaTime = 0;
        deltaTime = (double)((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
        last = now;

        animation_timer += deltaTime;
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
        }

        int mouseX, mouseY;
        Uint32 mouse_pos = SDL_GetMouseState(&mouseX, &mouseY);

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_Q]){
            running = false;
        }
        if(keystate[SDL_SCANCODE_SPACE]){
            playerFire(&player, mouseX, mouseY);
        }

        update_player_movement(&player, keystate, deltaTime);
        enemy_attack_timer(deltaTime, &attackTimer, &enemy, player);
        enemyProjectileCollisionCall(&player, &enemy);
        move_enemy(&enemy, &player.playerRect, deltaTime);


        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // need to first drwa balck background
        SDL_RenderClear(renderer);
        renderEntities(renderer, &player.playerRect, &enemy.enemyRect);
        SDL_SetRenderDrawColor(renderer, 0,0,255,255);
        updateProjectile(&enemy, deltaTime);
        playerProjUpdate(&player, deltaTime);
        drawProjectiles(renderer, &enemy, &player);

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
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}