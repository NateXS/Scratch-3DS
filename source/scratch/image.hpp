#pragma once
#include "miniz/miniz.h"
#include <string>
#include <vector>

class Image {
  public:
    /**
     * `3DS`: Takes every Image in a Scratch sb3 file and converts them to RGBA data.
     * `SDL`: Takes every image in a Scratch sb3 file and turns it into an 'SDL_Image' object.
     */
    static void loadImages(mz_zip_archive *zip);

    /**
     * `3DS`: Turns a single image from an unzipped Scratch project into RGBA data.
     * `SDL`: Loads a single `SDL_Image` from an unzipped filepath.
     */
    static void loadImageFromFile(std::string filePath);

    /**
     * `3DS`: Nothing yet yippie
     * `SDL`: Loads a single `SDL_Image` from a zip file.
     */
    static void loadImageFromSB3(mz_zip_archive *zip, const std::string &costumeId);

    /**
     * `3DS`: Frees a `C2D_Image` from memory.
     * `SDL`: Frees an `SDL_Image` from memory.
     */
    static void freeImage(const std::string &costumeId);

    /**
     * `3DS`: Queues a `C2D_Image` to be freed using `costumeId` to find it.
     *        The image will be freed once `FlushImages()` is called.
     * `SDL`: Currently does nothing 😁😁
     */
    static void queueFreeImage(const std::string &costumeId);

    /**
     * Checks every Image in memory to see if they can be freed.
     */
    static void FlushImages();
};