#include "scratch/blockExecutor.hpp"
#include "scratch/input.hpp"
#include "scratch/render.hpp"
#include "scratch/unzip.hpp"
#include <chrono>
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <unistd.h>
#endif

// arm-none-eabi-addr2line -e Scratch.elf xxx
// ^ for debug purposes

static void exitApp() {
    Render::deInit();
}

static bool initApp() {
    return Render::Init();
}

static void mainLoop() {
    // this is for the FPS
    static std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    static std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    // this is for frametime check
    static std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();
    static std::chrono::high_resolution_clock::time_point frameEndTime = std::chrono::high_resolution_clock::now();

    endTime = std::chrono::high_resolution_clock::now();
    if (endTime - startTime >= std::chrono::milliseconds(1000 / Scratch::FPS)) {
        startTime = std::chrono::high_resolution_clock::now();
        frameStartTime = std::chrono::high_resolution_clock::now();

        Input::getInput();
        BlockExecutor::runRepeatBlocks();
        Render::renderSprites();

        frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEndTime - frameStartTime;
        // std::cout << "\x1b[17;1HFrame time: " << frameDuration.count() << " ms" << std::endl;
        // std::cout << "\x1b[18;1HSprites: " << sprites.size() << std::endl;
    }
    if (toExit) {
        exitApp();
        exit(0);
    }
}

int main(int argc, char **argv) {
#ifdef __EMSCRIPTEN__
    chdir("romfs");
#endif

    if (!initApp()) {
        exitApp();
        return 1;
    }

    if (!Unzip::load()) {

        if (Unzip::projectOpened == -3) { // main menu

            MainMenu menu;
            bool isLoaded = false;
            while (!isLoaded) {

                menu.render();
                if ((!menu.hasProjects && menu.shouldExit) || !Render::appShouldRun()) {
                    exitApp();
                    return 0;
                }

                if (Unzip::filePath != "") {
                    menu.cleanup();
                    if (!Unzip::load()) {
                        exitApp();
                        return 0;
                    }
                    isLoaded = true;
                }
            }
            if (!Render::appShouldRun()) {
                menu.cleanup();
                exitApp();
                return 0;
            }

        } else {

            exitApp();
            return 0;
        }
    }

    BlockExecutor::runAllBlocksByOpcode(Block::EVENT_WHENFLAGCLICKED);
    BlockExecutor::timer = std::chrono::high_resolution_clock::now();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (Render::appShouldRun())
        mainLoop();
#endif
}
