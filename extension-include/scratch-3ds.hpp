#pragma once

#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef SDL_BUILD
#include <SDL2/SDL.h>
#elif defined(__3DS__)
#include <citro3d.h>
#endif

struct ExtensionVariable {
    const std::string name;
    const std::string id;
    const bool cloud;
    const std::any value;
};

struct ExtensionList {
    const std::string name;
    const std::string id;
    const std::vector<std::any> values;
};

struct ExtensionCostume {
    const std::string id;
    const std::string name;
    const std::string fullName;
    const std::string dataFormat;
    const int bitmapResolution;
    const double rotationCenterX;
    const double rotationCenterY;
};

struct ExtensionSound {
    const std::string id;
    const std::string name;
    const std::string dataFormat;
    const std::string fullName;
    const int sampleRate;
    const int sampleCount;
};

struct ExtensionComment {
    const std::string id;
    const std::string blockId;
    const std::string text;
    const bool minimized;
    const int x;
    const int y;
    const int width;
    const int height;
};

struct ExtensionBroadcast {
    const std::string id;
    const std::string name;
};

struct ExtensionSprite {
    enum RotationStyle {
        NONE,
        LEFT_RIGHT,
        ALL_AROUND
    };

    const std::string *name;
    const std::string *id;
    const bool *isStage;
    const bool *isClone;
    const std::unordered_map<std::string, ExtensionSound> sounds;
    const std::unordered_map<std::string, ExtensionComment> comments;
    bool *draggable;
    bool *visible;
    int *currentCostume;
    float *volume;
    double *xPosition;
    double *yPosition;
    int *rotationCenterX;
    int *rotationCenterY;
    double *size;
    double *rotation;
    int *layer;
    float *ghostEffect;
    double *colorEffect;
    int *width;
    int *height;
    std::vector<std::pair<double, double>> *collisionPoints;

    const std::unordered_map<std::string, ExtensionVariable> variables;
    std::function<void(std::string, std::any)> setVariable;

    const std::unordered_map<std::string, ExtensionList> lists;
    std::function<void(std::string, std::vector<std::any>)> setList;
    std::function<void(std::string, int, std::any)> setListItem;
    std::function<void(std::string, std::any)> pushList;

    const std::vector<ExtensionCostume> costumes;
    std::function<void(int)> switchCostumeTo;

    const std::unordered_map<std::string, ExtensionBroadcast> broadcasts;
    std::function<void(std::string)> broadcast;

    const RotationStyle rotationStyle;
    std::function<void(RotationStyle)> setRotationStyle;
};

struct ExtensionData {
    std::function<ExtensionSprite()> getSprite; // If this is not being ran in a sprite this will just be a nullptr
    std::function<std::vector<ExtensionSprite>()> getSprites;

    std::function<void(std::string)> logInfo;
    std::function<void(std::string)> logWarning;
    std::function<void(std::string)> logError;

#ifdef SDL_BUILD
    SDL_GameController *controller;
    SDL_Window *window;
    SDL_Renderer *renderer;
#elif defined(__3DS__)
    C3D_RenderTarget *topScreen;
    C3D_RenderTarget *bottomScreen;
#endif
};
