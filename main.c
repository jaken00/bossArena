//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct Ability{
    int cooldown;
    int damage;
    char name[50];
    SDL_Scancode hotkey;
} Ability;


typedef struct {
    int health;
    int mana;
    double movespeed;
    SDL_Rect playerRect;
    Ability** abilities;
    size_t ability_count;
} Player;

Ability* createAbility(char name[20], int damage, int cooldown, SDL_Scancode hotkey){
    Ability* ability = malloc(sizeof(Ability));

    strncpy(ability->name, name, sizeof ability->name);
    ability->damage = damage;
    ability->cooldown = cooldown;
    ability->hotkey = hotkey;

    return ability;
}



Player createPlayer(){
    Player player;
    player.health = 8;
    player.mana = 10;
    player.movespeed = 300.0 / 1000.0; //deivde by 1k for milisecond conversion

    SDL_Rect playerRect = {10, 10, 32, 32};
    player.playerRect = playerRect;
    player.ability_count = 3;

    player.abilities = malloc(sizeof *player.abilities * player.ability_count);
    player.abilities[0] = createAbility("TEST", 10, 10, SDL_SCANCODE_Q);
    player.abilities[1] = createAbility("TEST2", 10, 10, SDL_SCANCODE_E);
    player.abilities[2] = createAbility("TEST3", 10, 10, SDL_SCANCODE_F);

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

    Player player = createPlayer();

    bool running = true;
    int last = SDL_GetPerformanceCounter();
    while(running){
        /*  DELTA TIME    */
        int now = SDL_GetPerformanceCounter();
        double deltaTime = 0;
        deltaTime = (double)((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
        last = now;

        
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
        }


        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_Q]){
            running = false;
        }
        else if(keystate[SDL_SCANCODE_W]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.y -= movement_calcualtion;
        }
        else if(keystate[SDL_SCANCODE_S]){
            double movement_calcualtion = player.movespeed * deltaTime;
            player.playerRect.y += movement_calcualtion;
        }

        
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // need to first drwa balck background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
        SDL_RenderDrawRect(renderer, &player.playerRect);

        

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    freePlayer(&player);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}