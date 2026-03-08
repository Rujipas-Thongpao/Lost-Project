#pragma once
#include <glm/glm.hpp>
#include <iostream>

class GLMUtils {
public:
    static void printVec3(const glm::vec3& v, const char* label = "") {
        std::cout << label << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }

    static void printVec4(const glm::vec4& v, const char* label = "") {
        std::cout << label << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
    }

    static void printMat4(const glm::mat4& m, const char* label = "") {
        std::cout << label << std::endl;
        for (int row = 0; row < 4; row++) {
            std::cout << "  [ ";
            for (int col = 0; col < 4; col++)
                std::cout << m[col][row] << "\t";
            std::cout << "]" << std::endl;
        }
    }

    static void printMat3(const glm::mat3& m, const char* label = "") {
        std::cout << label << std::endl;
        for (int row = 0; row < 3; row++) {
            std::cout << "  [ ";
            for (int col = 0; col < 3; col++)
                std::cout << m[col][row] << "\t";
            std::cout << "]" << std::endl;
        }
    }
};