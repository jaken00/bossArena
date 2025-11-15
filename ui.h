#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


typedef struct {
    SDL_Texture *texture;
    SDL_Rect src;
    SDL_Rect dst;
    int frame_width;
    int frame_height;
    int frame_count;
    int current_frame;
} Sprite;

Sprite spriteLoad(SDL_Renderer *renderer, const char *path, int frame_width, int frame_height, int frame_count);
void sprite_SetFrame(Sprite *s, int frame_index);
void sprite_render(SDL_Renderer *renderer, Sprite *s);
void render_sprite_per_health(SDL_Renderer *renderer, Sprite *s, int hp);