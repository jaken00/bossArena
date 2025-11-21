#include "render.h"

void renderEntities(SDL_Renderer *renderer, SDL_Rect *player, SDL_Rect *enemy){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, player);
    SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
    SDL_RenderFillRect(renderer, enemy);
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