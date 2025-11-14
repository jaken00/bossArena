//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "game.h"

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