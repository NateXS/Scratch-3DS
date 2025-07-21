#include "sound.hpp"
#include "../audio.hpp"
#include "../unzip.hpp"

Value SoundBlocks::volume(Block &block, Sprite *sprite) {
    return Value(sprite->volume);
}

BlockResult SoundBlocks::playSoundUntilDone(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {

    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::playSound(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    Value inputValue = Scratch::getInputValue(block, "SOUND_MENU", sprite);
    std::string inputString = inputValue.asString();

    // if no blocks are inside the input
    auto inputFind = block.parsedInputs.find("SOUND_MENU");
    if (inputFind != block.parsedInputs.end() && inputFind->second.inputType == ParsedInput::LITERAL) {
        Block *inputBlock = findBlock(inputValue.asString());
        if (inputBlock != nullptr) {
            inputString = inputBlock->fields["SOUND_MENU"][0].get<std::string>();
        }
    }
    std::cout << "finding sound.. " << inputString << std::endl;
    auto soundFind = sprite->sounds.find(inputString);
    if (soundFind != sprite->sounds.end()) {
        const Sound *sound = &soundFind->second;
        SoundPlayer::loadSoundFromSB3(sprite, &Unzip::zipArchive, sound->fullName);
        SoundPlayer::playSound(sound->fullName);
    }

    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::stopAllSounds(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::changeEffectBy(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::setEffectTo(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::clearSoundEffects(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::changeVolumeBy(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}

BlockResult SoundBlocks::setVolumeTo(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh) {
    return BlockResult::CONTINUE;
}
