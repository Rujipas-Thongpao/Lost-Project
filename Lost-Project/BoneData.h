#pragma once

#include <stdio.h>
#include <map>
#include <string>
#include "animData.h"

class BoneData {
public:
    std::map<std::string, BoneInfo> m_BoneInfoMap; 
    int m_BoneCounter = 0;

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }
};
