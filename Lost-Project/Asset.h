#pragma once
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>

#include "animation.h"

class Assets {
public:
    //static Assets& getInstance() {
    //    static Assets instance;
    //    return instance;
    //}
    //Assets(const Assets&) = delete;
    //Assets& operator=(const Assets&) = delete;

    void registerMesh(const std::string& name, uint8_t id) {
        modelDatas[name] = id;
    }

    uint8_t GetModelData(const std::string& name) {
        if (!modelDatas.count(name)) {
            std::cout << "Asset not found: " << name << std::endl;
            return 0;
        }
        return modelDatas[name];
    }

    bool HasModelData(const std::string& name) {
        return modelDatas.count(name) > 0;
    }

	void registerAnimation(const std::string& name, Animation* anim) {
        animations[name] = anim;
    }

    Animation* getAnimation(const std::string& name) {
        if (!animations.count(name)) return nullptr;
        return animations[name];
    }

private:
    //Assets() {}
    std::unordered_map<std::string, uint8_t> modelDatas;
    std::unordered_map<std::string, Animation*> animations;
};
