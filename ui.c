#include "ui.h"

Sprite spriteLoad(SDL_Renderer *renderer, const char *path, int frame_width, int frame_height, int frame_count){
    Sprite s = {0};

    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if(!texture) {
        SDL_Log("Img texture Failed: %s", IMG_GetError());
        return s;
    }

    s.texture = texture;
    s.frame_width = frame_width;
    s.frame_height = frame_height;
    s.frame_count = frame_count;
    s.current_frame = 0;

    s.src.x = 0;
    s.src.y = 0;
    s.src.w = frame_width;
    s.src.h = frame_height;


    //dest is where it goes on the screen
    s.dst.x = 100;
    s.dst.y = 50;
    s.dst.w = frame_width;
    s.dst.h = frame_height;

    return s;

}

void sprite_SetFrame(Sprite *s, int frame_index){
    if(frame_index < 0) frame_index = 0;

    if(frame_index >= s->frame_count) frame_index = s->frame_count -1;
    s->current_frame = frame_index;
    s->src.x = s->current_frame * s->frame_width;
    s->src.y = 0; // only 1 row easy
}

void sprite_render(SDL_Renderer *renderer, Sprite *s){
    SDL_RenderCopy(renderer, s->texture, &s->src, &s->dst);
}

void render_sprite_per_health(SDL_Renderer *renderer, Sprite *s, int hp) {
    if (hp <= 0) return;

    int base_x = s->dst.x;          // starting X position
    int spacing = s->dst.w + 4;     // 4px gap between hearts (tweak)

    for (int i = 0; i < hp; i++) {
        s->dst.x = base_x + i * spacing;
        sprite_render(renderer, s);
    }

    // restore original x so we don't mess up other uses of this sprite
    s->dst.x = base_x;
}
