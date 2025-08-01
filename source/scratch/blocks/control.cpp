#include "control.hpp"

BlockResult ControlBlocks::If(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    Value conditionValue = Scratch::getInputValue(block, "CONDITION", sprite);
    bool condition = false;
    if (conditionValue.isNumeric()) {
        condition = conditionValue.asDouble() != 0.0;
    } else condition = !conditionValue.asString().empty();

    bool shouldStop = false;

    if (condition) {
        auto it = block.parsedInputs.find("SUBSTACK");
        if (it != block.parsedInputs.end()) {
            Block *subBlock = &sprite->blocks[it->second.blockId];
            if (subBlock) {
                bool isRepeating = false;

                // Run the block and store ran block IDs
                for (auto &ranBlock : executor.runBlock(*subBlock, sprite)) {
                    block.substackBlocksRan.push_back(ranBlock->id);
                    if (ranBlock->isRepeating) {
                        isRepeating = true;
                    }
                    if (ranBlock->shouldStop) {
                        shouldStop = true;
                    }
                }
                // If repeating, update waitingIfBlock and pause this block
                if (isRepeating) {
                    for (auto &stackBlock : block.substackBlocksRan) {
                        sprite->blocks[stackBlock].waitingIfBlock = block.id;
                    }
                    block.substackBlocksRan.clear();
                    return BlockResult::RETURN;
                }
            }
        }
    }
    block.substackBlocksRan.clear();

    if (shouldStop)
        return BlockResult::RETURN;

    return BlockResult::CONTINUE;
}

BlockResult ControlBlocks::ifElse(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    Value conditionValue = Scratch::getInputValue(block, "CONDITION", sprite);
    bool condition = false;
    bool shouldStop = false;
    if (conditionValue.isNumeric()) {
        condition = conditionValue.asDouble() != 0.0;
    } else {
        condition = !conditionValue.asString().empty();
    }

    // Select correct substack key
    std::string key = condition ? "SUBSTACK" : "SUBSTACK2";
    auto it = block.parsedInputs.find(key);
    if (it != block.parsedInputs.end()) {
        Block *subBlock = &sprite->blocks[it->second.blockId];
        if (subBlock) {
            bool isRepeating = false;

            // Run the block and store ran block IDs
            for (auto &ranBlock : executor.runBlock(*subBlock, sprite)) {
                block.substackBlocksRan.push_back(ranBlock->id);
                if (ranBlock->isRepeating) {
                    isRepeating = true;
                }
                if (ranBlock->shouldStop) {
                    shouldStop = true;
                }
            }

            // If repeating, update waitingIfBlock and pause this block
            if (isRepeating) {
                for (auto &stackBlock : block.substackBlocksRan) {
                    sprite->blocks[stackBlock].waitingIfBlock = block.id;
                }
                block.substackBlocksRan.clear();
                return BlockResult::RETURN;
            }
        }
    }

    if (shouldStop)
        return BlockResult::RETURN;

    return BlockResult::CONTINUE;
}

BlockResult ControlBlocks::createCloneOf(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    // std::cout << "Trying " << std::endl;

    Block *cloneOptions = nullptr;
    auto it = block.parsedInputs.find("CLONE_OPTION");
    cloneOptions = &sprite->blocks[it->second.literalValue.asString()];

    Sprite *spriteToClone = getAvailableSprite();
    if (!spriteToClone) return BlockResult::CONTINUE;
    if (cloneOptions->fields["CLONE_OPTION"][0] == "_myself_") {
        *spriteToClone = *sprite;
    } else {
        for (Sprite *currentSprite : sprites) {
            if (currentSprite->name == Math::removeQuotations(cloneOptions->fields["CLONE_OPTION"][0]) && !currentSprite->isClone) {
                *spriteToClone = *currentSprite;
            }
        }
    }
    spriteToClone->blockChains.clear();

    if (spriteToClone != nullptr && !spriteToClone->name.empty()) {
        spriteToClone->isClone = true;
        spriteToClone->isStage = false;
        spriteToClone->toDelete = false;
        spriteToClone->id = Math::generateRandomString(15);
        Log::log("Cloned " + sprite->name);
        // add clone to sprite list
        sprites.push_back(spriteToClone);
        Sprite *addedSprite = sprites.back();
        // Run "when I start as a clone" scripts for the clone
        for (Sprite *currentSprite : sprites) {
            if (currentSprite == addedSprite) {
                for (auto &[id, block] : currentSprite->blocks) {
                    if (block.opcode == block.CONTROL_START_AS_CLONE) {
                        // std::cout << "Running clone block " << block.id << std::endl;
                        executor.runBlock(block, currentSprite);
                    }
                }
            }
        }
    }
    return BlockResult::CONTINUE;
}
BlockResult ControlBlocks::deleteThisClone(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    if (sprite->isClone) {
        sprite->toDelete = true;
        Log::log("Deleted " + sprite->name + "'s clone.");
        return BlockResult::CONTINUE;
    }
    return BlockResult::CONTINUE;
}

BlockResult ControlBlocks::stop(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    block.shouldStop = false;
    std::string stopType = block.fields.at("STOP_OPTION")[0];
    if (stopType == "all") {
        toExit = true;
        return BlockResult::RETURN;
    }
    if (stopType == "this script") {
        for (std::string repeatID : sprite->blockChains[block.blockChainID].blocksToRepeat) {
            Block *repeatBlock = &sprite->blocks[repeatID];
            if (repeatBlock) {
                repeatBlock->repeatTimes = -1;
            }
        }

        for (auto &chainBlock : sprite->blockChains[block.blockChainID].blockChain) {
            chainBlock->waitingIfBlock = "";
        }

        sprite->blockChains[block.blockChainID].blocksToRepeat.clear();
        block.shouldStop = true;
        return BlockResult::RETURN;
    }

    if (stopType == "other scripts in sprite") {
        for (auto &[id, chain] : sprite->blockChains) {
            if (id == block.blockChainID) continue;
            for (std::string repeatID : chain.blocksToRepeat) {
                Block *repeatBlock = &sprite->blocks[repeatID];
                if (repeatBlock) {
                    repeatBlock->repeatTimes = -1;
                }
            }
            for (auto &chainBlock : chain.blockChain) {
                chainBlock->waitingIfBlock = "";
            }
            chain.blocksToRepeat.clear();
        }
        return BlockResult::CONTINUE;
    }
    return BlockResult::RETURN;
}

BlockResult ControlBlocks::startAsClone(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {
    return BlockResult::CONTINUE;
}

BlockResult ControlBlocks::wait(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {

    if (block.repeatTimes != -1 && !fromRepeat) {
        block.repeatTimes = -1;
    }

    if (block.repeatTimes == -1) {
        block.repeatTimes = -2;

        Value duration = Scratch::getInputValue(block, "DURATION", sprite);
        if (duration.isNumeric()) {
            block.waitDuration = duration.asDouble() * 1000; // convert to milliseconds
        } else {
            block.waitDuration = 0;
        }

        block.waitStartTime = std::chrono::high_resolution_clock::now();

        BlockExecutor::addToRepeatQueue(sprite, &block);
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - block.waitStartTime).count();
    block.repeatTimes -= 1;

    if (elapsedTime >= block.waitDuration && block.repeatTimes <= -4) {
        block.repeatTimes = -1;
        BlockExecutor::removeFromRepeatQueue(sprite, &block);
        return BlockResult::CONTINUE;
    }

    return BlockResult::RETURN;
}

BlockResult ControlBlocks::waitUntil(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {

    if (block.repeatTimes != -1 && !fromRepeat) {
        block.repeatTimes = -1;
    }

    if (block.repeatTimes == -1) {
        block.repeatTimes = -4;
        BlockExecutor::addToRepeatQueue(sprite, &block);
    }

    Value conditionValue = Scratch::getInputValue(block, "CONDITION", sprite);

    bool conditionMet = false;
    if (conditionValue.isNumeric()) {
        conditionMet = conditionValue.asDouble() != 0.0;
    } else {
        conditionMet = !conditionValue.asString().empty();
    }

    if (conditionMet) {
        block.repeatTimes = -1;
        BlockExecutor::removeFromRepeatQueue(sprite, &block);
        return BlockResult::CONTINUE;
    }

    return BlockResult::RETURN;
}

BlockResult ControlBlocks::repeat(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {

    if (block.repeatTimes != -1 && !fromRepeat) {
        block.repeatTimes = -1;
    }

    if (block.repeatTimes == -1) {
        block.repeatTimes = Scratch::getInputValue(block, "TIMES", sprite).asInt();
        BlockExecutor::addToRepeatQueue(sprite, &block);
    }

    if (block.repeatTimes > 0) {
        auto it = block.parsedInputs.find("SUBSTACK");
        if (it != block.parsedInputs.end()) {
            Block *subBlock = &sprite->blocks[it->second.blockId];
            if (subBlock) {
                executor.runBlock(*subBlock, sprite);
            }
        }

        // Countdown
        block.repeatTimes -= 1;
        return BlockResult::RETURN;
    } else {
        block.repeatTimes = -1;
    }
    // std::cout << "done with repeat " << block.id << std::endl;
    BlockExecutor::removeFromRepeatQueue(sprite, &block);
    return BlockResult::CONTINUE;
}

BlockResult ControlBlocks::repeatUntil(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {

    if (block.repeatTimes != -1 && !fromRepeat) {
        block.repeatTimes = -1;
    }

    if (block.repeatTimes == -1) {
        block.repeatTimes = -2;
        BlockExecutor::addToRepeatQueue(sprite, &block);
    }

    Value conditionValue = Scratch::getInputValue(block, "CONDITION", sprite);
    bool condition = false;
    if (conditionValue.isNumeric()) {
        condition = conditionValue.asDouble() != 0.0;
    } else condition = !conditionValue.asString().empty();

    if (condition) {
        block.repeatTimes = -1;
        BlockExecutor::removeFromRepeatQueue(sprite, &block);

        return BlockResult::CONTINUE;
    }

    auto it = block.parsedInputs.find("SUBSTACK");
    if (it != block.parsedInputs.end()) {
        const std::string &blockId = it->second.blockId;
        auto blockIt = sprite->blocks.find(blockId);
        if (blockIt != sprite->blocks.end()) {
            Block *subBlock = &blockIt->second;
            executor.runBlock(*subBlock, sprite);
        } else {
            std::cerr << "Invalid blockId: " << blockId << std::endl;
        }
    }

    // Continue the loop
    return BlockResult::RETURN;
}

BlockResult ControlBlocks::forever(Block &block, Sprite *sprite, bool *withoutScreenRefresh, bool fromRepeat) {

    if (block.repeatTimes != -1 && !fromRepeat) {
        block.repeatTimes = -1;
    }

    if (block.repeatTimes == -1) {
        block.repeatTimes = -3;
        BlockExecutor::addToRepeatQueue(sprite, &block);
    }

    auto it = block.parsedInputs.find("SUBSTACK");
    if (it != block.parsedInputs.end()) {
        Block *subBlock = &sprite->blocks[it->second.blockId];
        if (subBlock) {
            executor.runBlock(*subBlock, sprite);
        }
    }
    return BlockResult::RETURN;
}