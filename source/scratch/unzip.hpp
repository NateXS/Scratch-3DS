#include "interpret.hpp"
#include <filesystem>
#include <fstream>

#ifdef ENABLE_CLOUDVARS
extern size_t projectHash;
#endif

class Unzip {
  public:
    static volatile int projectOpened;
    static volatile bool threadFinished;
    static std::string filePath;
    static mz_zip_archive zipArchive;
    static std::vector<char> zipBuffer;

    static void openScratchProject(void *arg) {
        std::ifstream file;
        int isFileOpen = openFile(&file);
        if (isFileOpen == 0) {
            std::cerr << "Failed to open Scratch project." << std::endl;
            Unzip::projectOpened = -1;
            Unzip::threadFinished = true;
            return;
        } else if (isFileOpen == -1) {
            std::cout << "Main Menu Activated." << std::endl;
            Unzip::projectOpened = -3;
            Unzip::threadFinished = true;
            return;
        }
        nlohmann::json project_json = unzipProject(&file);
        if (project_json.empty()) {
            std::cerr << "Project.json is empty." << std::endl;
            Unzip::projectOpened = -2;
            Unzip::threadFinished = true;
            return;
        }
        loadSprites(project_json);
        Unzip::projectOpened = 1;
        Unzip::threadFinished = true;
        return;
    }

    static std::vector<std::string> getProjectFiles(const std::string directory) {

        std::vector<std::string> projectFiles;

        for (const auto &entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".sb3") {
                std::string fileName = entry.path().filename().string();
                projectFiles.push_back(fileName);
            }
        }
        return projectFiles;
    }

    static nlohmann::json unzipProject(std::ifstream *file) {

        nlohmann::json project_json;

        if (projectType != UNZIPPED) {
            // read the file
            std::cout << "Reading SB3..." << std::endl;
            std::streamsize size = file->tellg(); // gets the size of the file
            file->seekg(0, std::ios::beg);        // go to the beginning of the file
            zipBuffer.resize(size);
            if (!file->read(zipBuffer.data(), size)) {
                return project_json;
            }

            // open ZIP file from the thing that we just did
            std::cout << "Opening SB3 file..." << std::endl;
            memset(&zipArchive, 0, sizeof(zipArchive));
            if (!mz_zip_reader_init_mem(&zipArchive, zipBuffer.data(), zipBuffer.size(), 0)) {
                return project_json;
            }

            // extract project.json
            std::cout << "Extracting project.json..." << std::endl;
            int file_index = mz_zip_reader_locate_file(&zipArchive, "project.json", NULL, 0);
            if (file_index < 0) {
                return project_json;
            }

            size_t json_size;
            const char *json_data = static_cast<const char *>(mz_zip_reader_extract_to_heap(&zipArchive, file_index, &json_size, 0));

#ifdef ENABLE_CLOUDVARS
            // Get project hash for cloud variables
            std::hash<std::string> hash_func;
            projectHash = hash_func(std::string(json_data, json_size));
#endif

            // Parse JSON file
            std::cout << "Parsing project.json..." << std::endl;
            project_json = nlohmann::json::parse(std::string(json_data, json_size));
            mz_free((void *)json_data);

            // Image::loadImages(&zipArchive);
            // mz_zip_reader_end(&zipArchive);
        } else {
            // if project is unzipped
            file->clear();                 // Clear any EOF flags
            file->seekg(0, std::ios::beg); // Go to the start of the file
            (*file) >> project_json;
        }
        Image::loadImages(&zipArchive);
        return project_json;
    }

    static int openFile(std::ifstream *file);

    static bool load();
};
