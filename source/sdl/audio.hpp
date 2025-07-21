#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
class SDL_Audio {
  public:
    Mix_Chunk *audioChunk;
    std::string audioId;

    SDL_Audio();
    ~SDL_Audio();
};

extern std::unordered_map<std::string, SDL_Audio *> SDL_Sounds;