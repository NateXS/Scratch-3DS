#include "../scratch/unzip.hpp"
#include "../scratch/menus/loading.hpp"
#include "render.hpp"
#include <3ds.h>

volatile int Unzip::projectOpened = 0;
volatile bool Unzip::threadFinished = false;
std::string Unzip::filePath = "";
mz_zip_archive Unzip::zipArchive;
std::vector<char> Unzip::zipBuffer;

int Unzip::openFile(std::ifstream *file) {
    Log::log("Unzipping Scratch Project...");

    // load Scratch project into memory
    Log::log("Loading SB3 into memory...");
    const char *filename = "project.sb3";
    const char *unzippedPath = "romfs:/project/project.json";

    // first try embedded unzipped project
    file->open(unzippedPath, std::ios::binary | std::ios::ate);
    projectType = UNZIPPED;
    if (!(*file)) {
        Log::log("No unzipped project, trying embedded.");

        // try embedded zipped sb3
        file->open("romfs:/" + std::string(filename), std::ios::binary | std::ios::ate); // loads file from romfs
        projectType = EMBEDDED;
        if (!(*file)) {
            Log::log("No embedded Scratch project, trying SD card");

            // load main menu if not already
            if (filePath == "") return -1;

            // if main menu was loaded, load the selected file from main menu
            file->open(filePath, std::ios::binary | std::ios::ate);
            projectType = UNEMBEDDED;
            if (!(*file)) {
                Log::logError("Couldnt find file. jinkies.");
                return 0;
            }
        } else {
            filePath = "romfs:/" + std::string(filename);
        }
    }
    return 1;
}

bool Unzip::load() {

    Unzip::threadFinished = false;
    Unzip::projectOpened = 0;

    s32 mainPrio = 0;
    svcGetThreadPriority(&mainPrio, CUR_THREAD_HANDLE);

    Thread projectThread = threadCreate(
        Unzip::openScratchProject,
        NULL,
        0x4000,
        mainPrio + 1,
        -1,
        false);

    if (!projectThread) {
        Unzip::threadFinished = true;
        Unzip::projectOpened = -3;
    }

    Loading loading;
    loading.init();

    while (!Unzip::threadFinished) {
        loading.render();
        gspWaitForVBlank();
    }
    threadJoin(projectThread, U64_MAX);
    threadFree(projectThread);
    if (Unzip::projectOpened != 1) {
        loading.cleanup();
        return false;
    }
    loading.cleanup();
    // disable new 3ds clock speeds for a bit cus it crashes for some reason otherwise????
    osSetSpeedupEnable(false);
    return true;
}
