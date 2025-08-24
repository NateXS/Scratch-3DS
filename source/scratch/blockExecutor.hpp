#pragma once
#include "interpret.hpp"
#include "sprite.hpp"
#include <chrono>
#include <functional>
#include <unordered_map>

extern size_t blocksRun;

enum class BlockResult {
    CONTINUE,
    RETURN,
    BREAK
};

class BlockExecutor {
  private:
    std::unordered_map<Block::opCode, std::function<BlockResult(Block &, Sprite *, Block **, bool *)>> handlers;
    std::unordered_map<Block::opCode, std::function<Value(Block &, Sprite *)>> valueHandlers;
    // std::unordered_map<Block::opCode, std::function<Value(Block&,Sprite*)>> conditionBlockHandlers;

  public:
    BlockExecutor();
    void runBlock(Block &block, Sprite *sprite, Block *waitingBlock = nullptr, bool *withoutScreenRefresh = nullptr);
    static std::vector<Block *> runAllBlocksByOpcode(Block::opCode opcodeToFind);
    static void runRepeatBlocks();
    static void runRepeatsWithoutRefresh(Sprite *sprite, std::string blockChainID);
    static std::vector<std::pair<Block *, Sprite *>> runBroadcasts();
    Value getBlockValue(Block &block, Sprite *sprite);
    Value runConditionalBlock(std::string blockId, Sprite *sprite);
    static void addToRepeatQueue(Sprite *sprite, Block *block);
    static bool hasActiveRepeats(Sprite *sprite, std::string blockChainID);

    static std::chrono::_V2::system_clock::time_point timer;

  private:
    void registerHandlers();
    BlockResult executeBlock(Block &block, Sprite *sprite, Block **waitingBlock, bool *withoutScreenRefresh);
};