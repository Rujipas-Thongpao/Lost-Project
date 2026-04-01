#pragma once
// Random.h
#pragma once
#include <random>
#include <glm/glm.hpp>

namespace Engine {
    class Random {
    public:
        // float between min and max
        static float range(float min, float max) {
            return min + dist() * (max - min);
        }

        // int between min and max inclusive
        static int range(int min, int max) {
            return min + (int)(dist() * (max - min + 1));
        }

        // float between 0 and 1
        static float value() {
            return dist();
        }

        // random point inside unit sphere
        static glm::vec3 insideSphere() {
            return glm::vec3(
                range(-1.0f, 1.0f),
                range(-1.0f, 1.0f),
                range(-1.0f, 1.0f)
            );
        }

        // random point on unit sphere surface
        static glm::vec3 onSphere() {
            return glm::normalize(insideSphere());
        }

        // random vec3 between two vec3s
        static glm::vec3 range(glm::vec3 min, glm::vec3 max) {
            return glm::vec3(
                range(min.x, max.x),
                range(min.y, max.y),
                range(min.z, max.z)
            );
        }

        // random bool with probability 0-1
        static bool chance(float probability) {
            return dist() < probability;
        }

    private:
        // seeded once, reused forever
        static std::mt19937& getRng() {
            static std::mt19937 rng(std::random_device{}());
            return rng;
        }

        static float dist() {
            static std::uniform_real_distribution<float> d(0.0f, 1.0f);
            return d(getRng());
        }
    };
}
