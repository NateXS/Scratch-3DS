#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

extern std::vector<struct Extension> extensions;

struct Extension {
    std::string name;
    nlohmann::json types;
    void *handle;
};
