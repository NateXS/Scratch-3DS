#include "scratch/blockExecutor.hpp"
#include "scratch/input.hpp"
#include "scratch/render.hpp"
#include "scratch/unzip.hpp"
#include <chrono>

#ifdef ENABLE_CLOUDVARS
#include "scratch/os.hpp"
#include <mist/mist.hpp>
#include <random>
#include <sstream>
#endif

#if defined(__WIIU__) && defined(ENABLE_CLOUDVARS)
#include <whb/sdcard.h>
#endif

// arm-none-eabi-addr2line -e Scratch.elf xxx
// ^ for debug purposes

#ifdef ENABLE_CLOUDVARS
uint64_t projectHash;
std::unique_ptr<MistConnection> cloudConnection = nullptr;
#endif

static void exitApp() {
    Render::deInit();
}

static bool initApp() {
    return Render::Init();
}

#ifdef ENABLE_CLOUDVARS
void initMist() {
    // Username Stuff

    std::random_device rd;
    std::ostringstream usernameStream;
    usernameStream << "player" << std::setw(7) << std::setfill('0') << rd() % 10000000;
    std::string username = usernameStream.str();

    std::ostringstream projectID;
    projectID << "Scratch-3DS/hash-" << projectHash;
    cloudConnection = std::make_unique<MistConnection>(projectID.str(), username, "contact@grady.link");

    cloudConnection->onConnectionStatus([](bool connected, const std::string &message) {
        if (connected) {
            Log::log("Mist++ Connected:");
            Log::log(message);
            return;
        }
        Log::log("Mist++ Disconnected:");
        Log::log(message);
    });

    cloudConnection->onVariableUpdate(BlockExecutor::handleCloudVariableChange);

#ifdef __WIIU__
    cloudConnection->connect(false);
#else
    cloudConnection->connect();
#endif
}
#endif

int main(int argc, char **argv) {
    if (!initApp()) {
        exitApp();
        return 1;
    }

    // this is for the FPS
    std::chrono::_V2::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    std::chrono::_V2::system_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    // this is for frametime check
    std::chrono::_V2::system_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();
    std::chrono::_V2::system_clock::time_point frameEndTime = std::chrono::high_resolution_clock::now();

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

#ifdef ENABLE_CLOUDVARS
    initMist();
#endif

    BlockExecutor::runAllBlocksByOpcode(Block::EVENT_WHENFLAGCLICKED);
    BlockExecutor::timer = std::chrono::high_resolution_clock::now();

    while (Render::appShouldRun()) {
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
            break;
        }
    }

    exitApp();
    return 0;
}
