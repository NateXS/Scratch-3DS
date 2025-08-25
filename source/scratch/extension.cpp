#include "extension.hpp"
#include "interpret.hpp"
#include "scratch-3ds.hpp"
#include "value.hpp"

std::vector<struct Extension> extensions;

ExtensionSprite convertSpriteToExtensionSprite(Sprite *sprite) {
    std::unordered_map<std::string, ExtensionSound> extensionSounds;
    for (auto &pair : sprite->sounds)
        extensionSounds.emplace(pair.first, convertSoundToExtensionSound(&pair.second));

    std::unordered_map<std::string, ExtensionComment> extensionComments;
    for (auto &pair : sprite->comments)
        extensionComments.emplace(pair.first, convertCommentToExtensionComment(&pair.second));

    std::unordered_map<std::string, ExtensionVariable> extensionVariables;
    for (auto &pair : sprite->variables)
        extensionVariables.emplace(pair.first, convertVariableToExtensionVariable(&pair.second));

    std::unordered_map<std::string, ExtensionList> extensionLists;
    for (auto &pair : sprite->lists)
        extensionLists.emplace(pair.first, convertListToExtensionList(&pair.second));

    std::vector<ExtensionCostume> extensionCostumes;
    for (auto &costume : sprite->costumes)
        extensionCostumes.push_back(convertCostumeToExtensionCostume(&costume));

    std::unordered_map<std::string, ExtensionBroadcast> extensionBroadcasts;
    for (auto &pair : sprite->broadcasts)
        extensionBroadcasts.emplace(pair.first, convertBroadcastToExtensionBroadcast(&pair.second));

    return {
        &sprite->name,
        &sprite->id,
        &sprite->isStage,
        &sprite->isClone,
        extensionSounds,
        extensionComments,
        &sprite->draggable,
        &sprite->visible,
        &sprite->currentCostume,
        &sprite->volume,
        &sprite->xPosition,
        &sprite->yPosition,
        &sprite->rotationCenterX,
        &sprite->rotationCenterY,
        &sprite->size,
        &sprite->rotation,
        &sprite->layer,
        &sprite->ghostEffect,
        &sprite->colorEffect,
        &sprite->spriteWidth,
        &sprite->spriteHeight,
        &sprite->collisionPoints,
        extensionVariables,
        [sprite](std::string id, std::any value) {
            sprite->variables[id].value = Value(value);
        },
        extensionLists,
        [sprite](std::string id, std::vector<std::any> values) {
            sprite->lists[id].items.clear();

            for (const auto &value : values)
                sprite->lists[id].items.push_back(Value(value));
        },
        [sprite](std::string id, int index, std::any value) {
            sprite->lists[id].items[index] = Value(value);
        },
        [sprite](std::string id, std::any value) {
            sprite->lists[id].items.push_back(Value(value));
        },
        extensionCostumes,
        [sprite](int costume) {
            sprite->currentCostume = costume;
        },
        extensionBroadcasts,
        [](std::string id) {
            broadcastQueue.push_back(id);
        },
        (ExtensionSprite::RotationStyle)sprite->rotationStyle,
        [sprite](ExtensionSprite::RotationStyle rotationStyle) {
            sprite->rotationStyle = (Sprite::RotationStyle)rotationStyle;
        }
    };
}
