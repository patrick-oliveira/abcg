#ifndef LIGHTSOURCE_HPP_
#define LIGHTSOURCE_HPP_

#include <glm/vec3.hpp>
#include "abcg.hpp"
#include <glm/vec3.hpp>
#include "boids.hpp"

class OpenGLWindow;
class Boids;

class LightSource {
    public:
        void update(float deltaTime);

    private:
        friend OpenGLWindow;
        friend Boids;

        float scale{0.10f};
        float rotationAngle{1.00f};

        glm::vec3 lightSourcePosition;

        void setupLightSource();
};

#endif