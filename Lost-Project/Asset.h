#pragma once
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>

class Assets {
public:
    static Assets& getInstance() {
        static Assets instance;
        return instance;
    }
    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;

    void registerMesh(const std::string& name, uint8_t id) {
        meshes[name] = id;
    }

    uint8_t getMesh(const std::string& name) {
        if (!meshes.count(name)) {
            std::cout << "Asset not found: " << name << std::endl;
            return 0;
        }
        return meshes[name];
    }

    bool hasMesh(const std::string& name) {
        return meshes.count(name) > 0;
    }

private:
    Assets() {}
    std::unordered_map<std::string, uint8_t> meshes;
};
