#ifndef OBSTACLES_HPP_
#define OBSTACLES_HPP_

#include <glm/vec3.hpp>
#include "abcg.hpp"
#include <glm/vec3.hpp>
#include "boids.hpp"

class OpenGLWindow;
class Boids;

class Obstacles {
    public:
        void update(float deltaTime);

    private:
        friend OpenGLWindow;
        friend Boids;

        float scale{0.15f};
        float rotationAngle11{0.50f};
        float rotationAngle12{0.25f};

        float rotationAngle21{0.75f};
        float rotationAngle22{0.35f};

        float rotationAngle31{1.50f};
        float rotationAngle32{0.55f};

        int num_obstacles{12};
        std::array<glm::vec3, 12> obstaclePositionList;

        void setupObstacles();
};

#endif