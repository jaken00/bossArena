//C:\msys64\msys2_shell.cmd -defterm -here -no-start -mingw64
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct Ability{
    int cooldown;
    int damage;
    char* name[50];
    SDL_Scancode hotkey;
} Ability;


typedef struct {
    int health;
    int mana;
    SDL_Rect playerRect;
    Ability** abilities;
} Player;

Ability* createAbility(char* name[50], int damage, int cooldown, SDL_Scancode hotkey){
    Ability* ability = malloc(sizeof(Ability));

    strcpy(ability->name, name);
    ability->damage = damage;
    ability->cooldown = cooldown;
    ability->hotkey = hotkey;

    return ability;
}



Player createPlayer(){
    Player player;
    player.health = 8;
    player.mana = 10;

    SDL_Rect playerRect = {10, 10, 32, 32};
    player.playerRect = playerRect;
    player.abilities = malloc(sizeof(Ability*) * 3);
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
        printf('Unable to Create Renderer');
    }

    Player player = createPlayer();


    bool running = true;
    while(running){
        
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
        }


        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_Q]){
            running = false;
        }



        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    freePlayer(&player);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}