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
    SDL_Rect abilityRect;
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

typedef struct {
    int health;
    int phase;
    double movespeed;    
    SDL_Rect enemyRect;
    Ability** abilities;
    size_t ability_count;
} Enemy;


Ability* createAbility(char name[6], int damage, int cooldown, SDL_Scancode hotkey, SDL_Rect abilityRect){
    Ability* ability = malloc(sizeof(Ability));

    strncpy(ability->name, name, sizeof ability->name);
    ability->damage = damage;
    ability->cooldown = cooldown;
    ability->hotkey = hotkey;
    ability->abilityRect = abilityRect;

    return ability;
}

Enemy createEnemy(){
    Enemy enemy;

    enemy.health = 50;
    enemy.phase = 1;
    enemy.movespeed = 200.0 / 1000.0; //conver to milies!
    SDL_Rect enemyRect = {SCREEN_WIDTH / 2, 50, 64, 64}; // NEED CHANGE THESE TO DEFINE CONSTS
    printf("Rect: x=%d, y=%d, w=%d, h=%d\n", 
       enemyRect.x, enemyRect.y, enemyRect.w, enemyRect.h);
    enemy.enemyRect = enemyRect;
    enemy.ability_count = 5;
    enemy.abilities = malloc(sizeof *enemy.abilities * enemy.ability_count);
    SDL_Rect placeHolderRect = {0,0,20,20};
    enemy.abilities[0] = createAbility("BOSS1", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[1] = createAbility("BOSS2", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[2] = createAbility("BOSS3", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[3] = createAbility("BOSS4", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
    enemy.abilities[4] = createAbility("BOSS5", 10, 10, SDL_SCANCODE_UNKNOWN, placeHolderRect);
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

    SDL_ShowCursor(true);
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

        int mouseX, mouseY;
        Uint32 button_pos = SDL_GetMouseState(&mouseX, &mouseY);
        printf("\nCURRENT MOUSE X`: %d\n", mouseX);
        printf("\nCURRENT MOUSE Y: %d\n", mouseY); //GET CURRENT PLAYER POS AND THEN DRAW LINE BETWEEN THE TWO

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
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // need to first drwa balck background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &player.playerRect);
        SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
        SDL_RenderDrawRect(renderer, &enemy.enemyRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    freePlayer(&player);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}