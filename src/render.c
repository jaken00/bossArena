#include "render.h"

void renderEntities(SDL_Renderer *renderer, SDL_Rect *player, SDL_Rect *enemy){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, player);
    SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
    SDL_RenderFillRect(renderer, enemy);
}


void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    printf("RENDERING FUNCTION CALLED!\n");
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